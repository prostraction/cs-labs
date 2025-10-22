#include "videoio.h"

VideoIO::VideoIO(const std::string &InPath) : InFilePath(InPath){
    OutFilePath = "";
}

VideoIO::VideoIO(const std::string &InPath, const std::string &OutPath) : InFilePath(InPath), OutFilePath(OutPath){

}

VideoIO::~VideoIO() {
    if (WImageEncoding)
        delete WImageEncoding;
    if (WImageDecoding)
        delete WImageDecoding;
    if (WImageProvided)
        delete WImageProvided;
    DCT_Password.clear();
}

void VideoIO::Init() {
    int stream_index = 0;
    pkt = av_packet_alloc();
    if (!pkt) {
        fprintf(stderr, "Could not allocate AVPacket\n");
        return;
    }

    int error;
    error = avformat_open_input(&ifmt_ctx, InFilePath.c_str(), 0, 0);
    if (error < 0) { fprintf(stderr, "Could not open input file '%s'\n", InFilePath.c_str()); return; }
    error = avformat_find_stream_info(ifmt_ctx, 0);
    if (error < 0) {fprintf(stderr, "Failed to retrieve input stream information\n"); return; }
    av_dump_format(ifmt_ctx, 0, InFilePath.c_str(), 0);

    if (OutFilePath.length() > 0) {
        avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, OutFilePath.c_str());
        if (!ofmt_ctx) { fprintf(stderr, "Could not create output context\n"); return; }
    }


    stream_mapping_size = ifmt_ctx->nb_streams;
    stream_mapping = (int*)av_calloc(stream_mapping_size, sizeof(*stream_mapping));
    if (!stream_mapping) { fprintf(stderr, "(int*)av_calloc failed\n"); return; }
    if (OutFilePath.length() > 0)
        oformat = ofmt_ctx->oformat;

    for (uint32_t i = 0; i < ifmt_ctx->nb_streams; i++) {
        pCodec = nullptr;
        pCodecCtx = nullptr;
        pCodecCtx = ifmt_ctx->streams[i]->codec;

        in_stream = ifmt_ctx->streams[i];
        in_codecpar = in_stream->codecpar;

        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            stream_mapping[i] = -1;
            continue;
        }

        if (in_codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            Width = pCodecCtx->width;
            Height = pCodecCtx->height;
            to_BGR24 = sws_getContext(Width,
                Height,
                pCodecCtx->pix_fmt,
                Width,
                Height,
                AV_PIX_FMT_BGR24,
                SWS_BILINEAR,
                NULL,
                NULL,
                NULL
             );
            if (!to_BGR24) {  fprintf(stderr, "Error: Fail to sws_getContext to_BGR24.\n"); return; }
        }

        stream_mapping[i] = stream_index++;
        if (OutFilePath.length() > 0) {
            if (in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO) {
                ofmt_ctx->streams[i] = avformat_new_stream(ofmt_ctx, NULL);
                if (!ofmt_ctx->streams[i]) {fprintf(stderr, "Failed allocating output stream\n"); return; }

                error = avcodec_parameters_copy(ofmt_ctx->streams[i]->codecpar, in_codecpar);
                if (error < 0) {fprintf(stderr, "Failed to copy codec parameters\n"); return; }
                ofmt_ctx->streams[i]->codecpar->codec_tag = 0;
            }

            if (in_codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                 InitEncode(i);
            }
        }
    }
    if (OutFilePath.length() > 0) {
        av_dump_format(ofmt_ctx, 0, OutFilePath.c_str(), 1);
        if (oformat->flags & AVFMT_GLOBALHEADER) {
            ofmt_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
        if (!(oformat->flags & AVFMT_NOFILE)) {
            error = avio_open2(&ofmt_ctx->pb, OutFilePath.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr);
            if (error < 0) { fprintf(stderr, "Could not open output file '%s'", OutFilePath.c_str()); return;}
        }
        error = avformat_write_header(ofmt_ctx, NULL);
        if (error < 0) { fprintf(stderr, "Error occurred when opening output file\n"); }
    }
    // test
    //WImageEncoding = new WatermarkImage(23, 23, "aaaaaaaadfsdfgsdgsty4634aaaaaaaadfsdfgsdgsty4634aaaaaaaadfsdfgsdgsty4634", false);
}

void VideoIO::EncodeProcess() {
    int error;
    int64_t i = 0;

    if (!WImageEncoding) {
        fprintf(stderr, "Nothing to EncodeProcess: Watermark was not provided! Exiting.\n");
        Finish();
        Finished = true;
        return;
    }
    else {
        fprintf(stderr, "[Watermark] Value: %s\n", WImageEncoding->TextData().c_str());
    }

    while (true) {
        error = av_read_frame(ifmt_ctx, pkt);
        if (error < 0) break;

        in_stream = ifmt_ctx->streams[pkt->stream_index];
        if (pkt->stream_index >= stream_mapping_size ||
            stream_mapping[pkt->stream_index] < 0) {
            av_packet_unref(pkt);
            continue;
        }
        pkt->stream_index = stream_mapping[pkt->stream_index];

        pCodec = NULL;
        pCodecCtx = NULL;
        pCodecCtx = ifmt_ctx->streams[pkt->stream_index]->codec;
        pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
        if (pCodec == NULL) { fprintf(stderr, "Unsupported codec!\n"); return;}

        error = avcodec_open2(pCodecCtx, pCodec, nullptr);
        if (error < 0) {fprintf(stderr, "Unable to open Input Codec for Encoding!\n"); return; }

        if (pkt->stream_index == AVMEDIA_TYPE_VIDEO) {
            AVFrame *frame = nullptr;
            frame=av_frame_alloc();

            uint8_t *buffer = nullptr;
            buffer = (uint8_t *)av_malloc(Width * Height * 3 * sizeof(uint8_t));

            AVFrame *frameRGB = nullptr;
            frameRGB = av_frame_alloc();

            frameRGB->width = Width;
            frameRGB->height = Height;
            frameRGB->format = static_cast<int>(AV_PIX_FMT_BGR24);
            av_frame_get_buffer(frameRGB, 32);

            int frameFinished;
            //DecodeFrame(frame);
            avcodec_decode_video2(pCodecCtx, frame, &frameFinished, pkt);

            ///*
            // frame->pict_type == 1
            fprintf(stderr, "[%lld]\n", i);

            if (false){//i > 3){// && (i % 10 > 0 && i % 10 < 6)) {
                // Convert the image from its native format to RGB
                sws_scale(to_BGR24, (uint8_t const * const *)frame->data,
                          frame->linesize, 0, Height,
                          frameRGB->data, frameRGB->linesize);

                if (Method == DCT) {
                    WatermarkEmbedding::EncodeBitmapBenham((char*)frameRGB->data[0],
                            WImageEncoding, DCT_Password,
                            Width, Height, Q, -Q, Bitmap_BGR888);
                }
                else if (Method == DWT_Soheili) {
                    WatermarkEmbedding::EncodeBitmapSoheili((char*)frameRGB->data[0],
                            WImageEncoding, Width, Height, Q, Bitmap_BGR888);
                }
                sws_scale(to_original, (uint8_t const * const *)frameRGB->data,
                      frameRGB->linesize, 0, Height,
                      frame_encode->data, frame_encode->linesize);
                frame_encode->pts = i;
                FrameEncode(frame_encode, pkt->stream_index);
                i++;
            }
            else {
                frame->pts = i;
                FrameEncode(frame, pkt->stream_index);

                // testing: different timebases
                if (false){// < 0) {
                    if (pkt->pts != AV_NOPTS_VALUE)
                        pkt->pts = av_rescale_q(i, context_encode->time_base, ofmt_ctx->streams[pkt->stream_index]->time_base);
                    if (pkt->dts != AV_NOPTS_VALUE)
                        pkt->dts = av_rescale_q(i, context_encode->time_base, ofmt_ctx->streams[pkt->stream_index]->time_base);

                    //pkt->pts = pkt->dts = i;
                    //av_packet_rescale_ts(pkt, context_encode->time_base, streams[pkt->stream_index]->time_base);
                    av_interleaved_write_frame(ofmt_ctx, pkt);
                }
                i++;
            }
            //*/
            //fprintf(stderr, "%lld\n", i);
            av_free(buffer);
            av_frame_free(&frameRGB);
            av_frame_free(&frame);
            av_packet_unref(pkt_encode);
        }


        else {
            av_packet_rescale_ts(pkt, in_stream->time_base, ofmt_ctx->streams[pkt->stream_index]->time_base);
            error = av_interleaved_write_frame(ofmt_ctx, pkt);
        }

        av_packet_unref(pkt);


        //pkt_encode->pts = av_rescale_q(pkt_encode->pts, context_encode->time_base, out_stream->time_base);
        //pkt_encode->dts = av_rescale_q(pkt_encode->dts, context_encode->time_base, out_stream->time_base);



        /* pkt is now blank (av_interleaved_write_frame() takes ownership of
         * its contents and resets pkt), so that no unreferencing is necessary.
         * This would be different if one used av_write_frame(). */


        //if (error < 0) {  fprintf(stderr, "Error muxing packet\n"); break; }
        //qDebug() << "i: " << i;

        //av_free(frame);
        //av_free(frameRGB);

        //delete[] bmp_file;

        //avcodec_close(pCodecCtx);
        //avcodec_close(pCodecCtxOrig);
    }
    //EncodeFrame(nullptr);
    //av_write_trailer(ofmt_ctx);

}

void VideoIO::Finish() {
    if (!Finished) {
        sws_freeContext(to_BGR24);

        av_freep(&stream_mapping);
        av_packet_free(&pkt);
        avformat_close_input(&ifmt_ctx);
        avcodec_close(pCodecCtx);

        if (OutFilePath.length() > 0) {
            /* close output */
            FinishEncode();
        }
    }
}

void VideoIO::FinishEncode() {
    sws_freeContext(to_original);

    // write trailer and close file
    av_write_trailer(ofmt_ctx);
    av_packet_free(&pkt_encode);
    av_frame_free(&frame_encode);
    avcodec_close(context_encode);
    avcodec_free_context(&context_encode);
    avio_close(ofmt_ctx->pb);
}

void VideoIO::SetWImageEncoding(const WatermarkImage *_WatermarkImage) {
    if (WImageEncoding != nullptr) delete WImageEncoding;
    WImageEncoding = new WatermarkImage(_WatermarkImage);
    WImageEncoding->SetData(_WatermarkImage->Data());
}

void VideoIO::SetWImageDecoding(const WatermarkImage *_WatermarkImage) {
    if (WImageDecoding != nullptr) delete WImageDecoding;
    WImageDecoding = new WatermarkImage(_WatermarkImage);
}

void VideoIO::SetWImageProvided(const WatermarkImage *_WatermarkImage) {
    if (WImageProvided != nullptr) delete WImageProvided;
    WImageProvided = new WatermarkImage(_WatermarkImage);
    WImageProvided->SetData(_WatermarkImage->Data());
    WImageProvided->ConvertToText();
}

void VideoIO::SetQ(const int &_Q) {
    Q = _Q;
}

void VideoIO::SetDCTPassword(const std::string &_Password) {
    DCT_Password = _Password;
}

void VideoIO::SetMethod(const int &_Method) {
    Method = _Method;
}


void VideoIO::InitEncode(int index) {
    int ret = 0;
    fprintf(stderr, "Encode Init started.\n");

    // open output format context
    ofmt_ctx->streams[index] = avformat_new_stream(ofmt_ctx, NULL);
    if (!ofmt_ctx->streams[index]) {fprintf(stderr, "Failed allocating output stream\n"); return; }

    ret = avcodec_parameters_copy(ofmt_ctx->streams[index]->codecpar, in_codecpar);
    if (ret < 0) {fprintf(stderr, "Failed to copy codec parameters\n"); return; }
    ofmt_ctx->streams[index]->codecpar->codec_tag = 0;


    // create new video stream
    codec_encode  = avcodec_find_encoder(ofmt_ctx->streams[index]->codecpar->codec_id); // format_encode->oformat->video_codec

    // open video encoder
    context_encode = avcodec_alloc_context3(codec_encode);
    context_encode->width       = Width;//width;
    context_encode->height      = Height;//height;
    context_encode->pix_fmt     = codec_encode->pix_fmts[0];

    //context_encode->time_base   = av_inv_q(in_stream->r_frame_rate);
    //context_encode->time_base   = in_stream->codec->time_base;
    //context_encode->sample_aspect_ratio = in_stream->codec->sample_aspect_ratio;
    //out_stream->time_base = in_stream->time_base;
    //out_stream->avg_frame_rate = in_stream->avg_frame_rate;
    //context_encode->sample_rate = in_stream->codec->sample_rate;
    //out_stream->r_frame_rate = in_stream->r_frame_rate;
    //context_encode->sample_aspect_ratio = in_stream->codec->sample_aspect_ratio;

    context_encode->time_base   = av_inv_q(in_stream->r_frame_rate);
    ofmt_ctx->streams[index]->time_base       = context_encode->time_base;;//context_encode->time_base;
    ofmt_ctx->streams[index]->r_frame_rate    = in_stream->r_frame_rate;

    //fprintf(stderr, "FPS input: (%d/%d)\n", in_stream->r_frame_rate.num, in_stream->r_frame_rate.den);
    //fprintf(stderr, "FPS output: (%d/%d)\n", streams[index]->r_frame_rate.num, streams[index]->r_frame_rate.den);
    //fprintf(stderr, "context_decode->time_base (%d / %d)\n", ifmt_ctx->streams[0]->codec->time_base.num, ifmt_ctx->streams[0]->codec->time_base.den);
    //fprintf(stderr, "context_encode->time_base (%d / %d)\n", context_encode->time_base.num, context_encode->time_base.den);
    //fprintf(stderr, "in_stream->time_base (%d / %d)\n", in_stream->time_base.num, in_stream->time_base.den);
    //fprintf(stderr, "in_stream->codec->time_base (%d / %d)\n", in_stream->codec->time_base.num, in_stream->codec->time_base.den);
    //fprintf(stderr, "out_stream->time_base (%d / %d)\n", streams[index]->time_base.num, streams[index]->time_base.den);
    //fprintf(stderr, "out_stream->codec->time_base (%d / %d)\n", streams[index]->codec->time_base.num, streams[index]->codec->time_base.den);

    // context_encode->framerate   = in_stream->r_frame_rate;
    //context_encode->thread_count = 1;

    if (ofmt_ctx->streams[index]->codecpar->codec_id == AV_CODEC_ID_H264) {
        AVDictionary * codec_options( 0 );
        av_dict_set( &codec_options, "preset", "veryfast", 0 );
        // av_dict_set( &codec_options, "AnyCodecParameter", "Value", 0 );
        ret = avcodec_open2(context_encode, codec_encode, &codec_options);
    }
    else {
        ret = avcodec_open2(context_encode, codec_encode, nullptr);
    }
    if (ret < 0) {
        fprintf(stderr, "Error: Fail to avcodec_open2: ret = %d.\n", ret);
        return;
    }


    //if (out_stream->codecpar->codec_id == AV_CODEC_ID_H264)
      //      av_opt_set(context_encode->priv_data, "preset", "slow", 0);

    //context_encode->max_qdiff   = 1;
    //context_encode->qmax        = 20;
    //context_encode->qmin        = 10;
    //context_encode->max_b_frames = 0;
    //context_encode->gop_size = 24;
    //context_encode->max_b_frames = 3;
    //context_encode->me_cmp = 1;
    //context_encode->me_range = 16;

    to_original = sws_getContext(
        Width,
        Height,
        AV_PIX_FMT_BGR24,
        Width,
        Height,
        context_encode->pix_fmt,
        SWS_BILINEAR,
        NULL,
        NULL,
        NULL
    );

    if (!to_original) {
        fprintf(stderr, "Error: Fail to sws_getContext to_original.\n");
        return;
    }

    // allocate frame buffer for encoding
    frame_encode = av_frame_alloc();
    frame_encode->width = Width;
    frame_encode->height = Height;
    frame_encode->format = static_cast<int>(context_encode->pix_fmt);
    ret = av_frame_get_buffer(frame_encode, 32);
    if (ret < 0) {
        fprintf(stderr, "Error: Fail to av_frame_get_buffer: ret = %d\n", ret);
        return;
    }

    // allocate packet to retrive encoded frame
    pkt_encode = av_packet_alloc();
    fprintf(stderr, "Encode Init finished.\n");
}


int VideoIO::FrameEncode(AVFrame *frame, const int &stream_index) {
    char str[AV_ERROR_MAX_STRING_SIZE];
    int ret = 0;

    ret = avcodec_send_frame(context_encode, frame);
    av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, ret);
    if (ret != 0) {
        fprintf(stderr, "Frame sent:\t %s.\n", str);
        return -1;
    }

    while ((ret = avcodec_receive_packet(context_encode, pkt_encode)) >= 0) {
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
              break;
        av_packet_rescale_ts(pkt_encode, context_encode->time_base, ofmt_ctx->streams[stream_index]->time_base);
        ret = av_interleaved_write_frame(ofmt_ctx, pkt_encode);
        av_packet_unref(pkt_encode);

        av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, ret);
        if (ret != 0) {
            fprintf(stderr, "Frame coded:\t %s.\n", str);
            return -1;
        }

    }
    return 0;
}

void VideoIO::DecodeProcess() {
    int error;
    int64_t frame_counter = 0;

    if (WImageDecoding == nullptr) {
        fprintf(stderr, "Nothing to DecodeProcess: Watermark was not provided! Exiting.\n");
        Finish();
        Finished = true;
        return;
    }

    while (true) {
        error = av_read_frame(ifmt_ctx, pkt);
        if (error < 0) break;

        in_stream  = ifmt_ctx->streams[pkt->stream_index];
        if (pkt->stream_index >= stream_mapping_size ||
            stream_mapping[pkt->stream_index] < 0) {
            av_packet_unref(pkt);
            continue;
        }
        pkt->stream_index = stream_mapping[pkt->stream_index];

        pCodec = NULL;
        pCodecCtx = NULL;
        pCodecCtx = ifmt_ctx->streams[0]->codec;
        pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
        if (pCodec == NULL) { fprintf(stderr, "Unsupported codec!\n"); return;}

        error = avcodec_open2(pCodecCtx, pCodec, nullptr);
        if (error < 0) {fprintf(stderr, "Unable to open input Codec for Decoding\n"); return; }

        if (pkt->stream_index == AVMEDIA_TYPE_VIDEO) {
            AVFrame *frame = NULL;
            frame=av_frame_alloc();

            uint8_t *buffer = NULL;
            buffer = (uint8_t *)av_malloc(Width * Height * 3 * sizeof(uint8_t));

            AVFrame *frameRGB;
            frameRGB = av_frame_alloc();

            frameRGB->width = Width;
            frameRGB->height = Height;
            frameRGB->format = static_cast<int>(AV_PIX_FMT_BGR24);
            av_frame_get_buffer(frameRGB, 32);

            int frameFinished;
            avcodec_decode_video2(pCodecCtx, frame, &frameFinished, pkt);
            WatermarkImage *temp = new WatermarkImage(16,16,false);

            if (frame_counter > 3){// && (frame_counter % 10 > 0 && frame_counter % 10 < 6)) {//frame->pict_type == 1) {
                // Convert the image from its native format to RGB
                sws_scale(to_BGR24, (uint8_t const * const *)frame->data,
                          frame->linesize, 0, pCodecCtx->height,
                          frameRGB->data, frameRGB->linesize);

                WatermarkImage* WImageDecoded = nullptr;
                if (Method == DCT) {
                    if (WImageProvided == nullptr) {
                        WImageDecoded = WatermarkEmbedding::DecodeBitmapBenham((char*)frameRGB->data[0],
                                WImageDecoding, DCT_Password, Width, Height, Bitmap_BGR888);
                    }
                    else {
                        WImageDecoded = WatermarkEmbedding::DecodeBitmapBenham((char*)frameRGB->data[0],
                                WImageProvided, DCT_Password, Width, Height, Bitmap_BGR888);
                    }
                }
                else if (Method == DWT_Soheili) {
                    if (WImageProvided == nullptr) {
                        WImageDecoded = WatermarkEmbedding::DecodeBitmapSoheili((char*)frameRGB->data[0],
                                WImageDecoding, Width, Height, Q, Bitmap_BGR888);
                    }
                    else {
                        //WatermarkImage *test = new WatermarkImage(16,16,"qwertyuiopasdfghjklzxcvbnm1234567890",false);
                        WImageDecoded = WatermarkEmbedding::DecodeBitmapSoheili((char*)frameRGB->data[0],
                                WImageProvided, Width, Height, Q, Bitmap_BGR888);
                        //delete test;
                    }
                }

                if (WImageDecoded != nullptr) {
                    WImageDecoded->ConvertToText();
                    fprintf(stderr, "Decoding (video) [%lld]: \"%s\"\n", frame_counter, WImageDecoded->TextData().c_str());
                    delete WImageDecoded;
                }
                else {
                    fprintf(stderr, "Decoding (video) [%lld]: %s", frame_counter, "failed!\n");
                }
            }
            delete temp;
            frame_counter++;
            av_free(buffer);
            av_frame_free(&frameRGB);
            av_frame_free(&frame);
        }
        av_packet_unref(pkt);
    }
}

void VideoIO::DecodeFrame(AVFrame *frame) {
    int error;
    /* send the packet with the compressed data to the decoder */
    error = avcodec_send_packet(pCodecCtx, pkt);
    if (error < 0) {
        fprintf(stderr, "Error submitting the packet to the decoder\n");
        exit(1);
    }
    /* read all the output frames (in general there may be any number of them */
    while (error >= 0) {
        error = avcodec_receive_frame(pCodecCtx, frame);
        if (error == AVERROR(EAGAIN) || error == AVERROR_EOF)
            return;
        else if (error < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }
    }
}

