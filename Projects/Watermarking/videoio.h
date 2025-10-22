#ifndef VIDEOIO_H
#define VIDEOIO_H

#include <iostream>
#include <fstream>

#include "watermarkimage.h"
#include "watermarkembedding.h"
#include "fileio.h"

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libavutil/imgutils.h"
    #include "time.h"
}


class VideoIO {
public:
    VideoIO(const std::string &InPath);
    VideoIO(const std::string &InPath, const std::string &OutPath);
    ~VideoIO(); // only not ffmpeg elements

    void Init();            // avlib
    void EncodeProcess();   // frame: transcoding with(out) watermark / other: remuxing
    void DecodeProcess();   // decoding watermark
    void Finish();          // avlib

    /*  Watermarks  */
    void SetWImageEncoding(const WatermarkImage *_WatermarkImage);
    void SetWImageDecoding(const WatermarkImage *_WatermarkImage);
    void SetWImageProvided(const WatermarkImage *_WatermarkImage);
    void SetQ               (const int &_Q);
    void SetDCTPassword     (const std::string &_Password);
    void SetMethod          (const int &_Method);
private:
    /* Video parameters */
    std::string InFilePath = "";
    std::string OutFilePath = "";
    uint32_t Width   = 0;
    uint32_t Height  = 0;

    /*  Watermark   */
    enum ContainerType {none, Bitmap_BGR888, jpg};
    enum MethodType {LSB, DCT, DWT_Sanghavi, DWT_Soheili};

    uint32_t WImageX = 0;
    uint32_t WImageY = 0;
    WatermarkImage *WImageEncoding = nullptr;
    WatermarkImage *WImageDecoding = nullptr;
    WatermarkImage *WImageProvided = nullptr;
    std::string DCT_Password = "aaaaa";
    int Q = 150;
    int Method = DCT;

    /*  Decode */
    AVFormatContext     *ifmt_ctx     = nullptr;   // Video/Audio Input
    AVPacket            *pkt          = nullptr;   // Video/Audio I/O-remuxing
    AVStream            *in_stream    = nullptr;   // Current stream
    AVCodecParameters   *in_codecpar  = nullptr;   // Current codec
    int                 *stream_mapping = nullptr; // Stream list (input)
    int                  stream_mapping_size = 0;  // Stream list (input)
    void DecodeFrame(AVFrame *frame);

    /*  Remuxing (rename) */
    AVCodec         *pCodec     = nullptr;
    AVCodecContext  *pCodecCtx  = nullptr;

    /*  Encode (video) */
    AVOutputFormat  *oformat        = nullptr;  // Video/Audio Output ref
    AVFormatContext *ofmt_ctx       = nullptr;  // Video/Audio Output
    AVCodec         *codec_encode   = nullptr;  // Video Output
    AVCodecContext  *context_encode = nullptr;  // Video Output
    AVFrame         *frame_encode   = nullptr;  // Video Output-transcoding
    AVPacket        *pkt_encode     = nullptr;  // Video Output-transcoding
    void InitEncode(int index);
    int  FrameEncode(AVFrame *frame, const int &stream_index);
    void FinishEncode();

    /*  Formats */
    struct SwsContext *to_BGR24     = nullptr;
    struct SwsContext *to_original  = nullptr;

    uint64_t    StartSecond = 0;
    uint64_t    FinishSecond = -1;
    bool Finished = false;
};

#endif // VIDEOIO_H
