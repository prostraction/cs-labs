#include "mainwindow.h"
#include "ui_mainwindow.h"

/// test only
#include "watermarkimage.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    EncodingImageView   = nullptr;
    DecodingImageView   = nullptr;
    DecodedPixmapBits   = nullptr;
    DisplayedDecode     = nullptr;

    BitsX = 64;
    BitsY = 64;

    Table = new UI_TableModel;
    ui->tableView->setModel(Table);

    WillBeDecodedView = new QPixmap;

    TableVerticalHeader = ui->tableView->verticalHeader();
    TableVerticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    TableVerticalHeader->setMinimumSectionSize(18);

    TableVerticalHeader->setMaximumSectionSize(18);
    TableVerticalHeader->setDefaultSectionSize(18);

    status = new QProgressBar;
    ui->statusbar->insertPermanentWidget(0, status);

    ForcedExit = false;
    Stopped = false;
}

MainWindow::~MainWindow() {
    if (EncodingImageView != nullptr)   delete EncodingImageView;
    if (DecodingImageView != nullptr)   delete DecodingImageView;
    if (DecodedPixmapBits != nullptr)   delete DecodedPixmapBits;
    if (WillBeDecodedView != nullptr)   delete WillBeDecodedView;
    if (DisplayedDecode != nullptr)   delete DisplayedDecode;
    delete status;
    delete TableVerticalHeader;
    delete Table;
    delete ui;
}

// test
void MainWindow::on_pushButton_released() {
    Watermark::Test(" ", std::string("aa bb cc dd "), ui->DCT_Pass->text().toStdString(), 9999, 9999);
}

void MainWindow::Encode(int32_t &err,
                        const std::string &FilePath,
                        const std::string &OutputPath,
                        const std::string &WatermarkText,
                        const uchar* TransferBits,
                        const uint32_t &BitsX,
                        const uint32_t &BitsY) {
    err =  Watermark::Encode(FilePath,
                      OutputPath,
                      WatermarkText,
                      TransferBits,
                      BitsX,
                      BitsY,
                      70,
                      "asdoykoprtyjto,pe[ryjopertyjope[rt,yjtope[ryjoprt,yjope[rtyjro,pe[t,yjope[rtyjerj458036-yun45890tjkls;dfgvc");
}


void MainWindow::on_Encode_released() {
    uchar *TransferBits = nullptr;
    if (EncodingImageView != nullptr) {

        QImage temp = EncodingImageView->toImage();
        TransferBits = new uchar[EncodingImageView->width() * EncodingImageView->height()];
        for (int32_t i = 0, j = 0; j < EncodingImageView->width() * EncodingImageView->height(); i+=4, j++) {
            TransferBits[j] = temp.bits()[i];
        }

        WatermarkText = "";
        BitsX = EncodingImageView->height();
        BitsY = EncodingImageView->width();
    }
    else if (ui->EncodingText->toPlainText().length() > 0 ){
        WatermarkText = ui->EncodingText->toPlainText();
    }
    else {
        return;
    }

    int32_t thread_count = std::thread::hardware_concurrency();
    std::vector<std::thread> workers(thread_count);
    int32_t *err = new int32_t[thread_count];
    memset(err, 0, thread_count * sizeof(int32_t));

    for (int32_t element = 0; element < Table->rowCount(); element += thread_count) {
        for (int32_t i = 0; i < thread_count; i++) {
            if (element + i < Table->rowCount()) {
                QString InPath = QString::fromStdString(Table->fullFilePathAt(element+i));
                QString OutPath;

                if (ui->CustomDirectory->isChecked())
                    OutPath = OutputDirectory + InPath.right(InPath.length() - InPath.lastIndexOf('/'));
                else
                    OutPath = InPath.left(InPath.lastIndexOf('.')) + ui->postfix->text() + InPath.right(InPath.length() - InPath.lastIndexOf('.'));

                QFutureWatcher<int> t;
                /*
                 * t.setFuture(QtConcurrent::map(Encode,
                                                          err[i],
                                                          Table->fullFilePathAt(element + i),
                                                          OutPath.toStdString(),
                                                          WatermarkText.toStdString(),
                                                          TransferBits,
                                                          200,
                                                          200));
                                                          */

                workers[i] = std::thread(Encode,
                                         std::ref(err[i]),
                                         Table->fullFilePathAt(element + i),
                                         OutPath.toStdString(),
                                         WatermarkText.toStdString(),
                                         TransferBits,
                                         16,
                                         16);
                events.processEvents(QEventLoop::AllEvents);
            }

            if (ForcedExit) {
                for (int32_t i = 0; i < thread_count; i++) {
                    if (workers[i].joinable())
                        workers[i].join();
                }
                events.exit(0);
                delete[] TransferBits;
                delete[] err;
                return;
            }
            status->setValue(int(double(element + i) / double(Table->rowCount()) * 100.));
        }
        for (int32_t i = 0; i < thread_count; i++) {
            if (workers[i].joinable())
                workers[i].join();
            qDebug() << err[i];

            switch (err[i]) {
                case -1: {
                    Table->insertWatermarkTextAt(element+i, "Водяной знак не был предоставлен.");
                    Table->insertStatusAt(element+i, "Ошибка.");
                    break;
                }
                case -2: {
                    Table->insertWatermarkTextAt(element+i, "Размеры водяного знака не указаны.");
                    Table->insertStatusAt(element+i, "Ошибка.");
                    break;
                }
                case -3: {
                    Table->insertWatermarkTextAt(element+i, "Ошибка при прочтении файла.");
                    Table->insertStatusAt(element+i, "Ошибка.");
                    break;
                }
                case -4: {
                    Table->insertWatermarkTextAt(element+i, "Водяной знак слишком большой для данного изображения.");
                    Table->insertStatusAt(element+i, "Ошибка.");
                    break;
                }
            }
        }
    }

    delete[] err;
    delete[] TransferBits;

}


uchar* MainWindow::Decode(const std::string    &FilePathIn,
                        std::string          &DecodedWord,       // Если ЦВЗ это слово
                        const uchar          *WImageBits,
                        const uint32_t       &WImageBitsX,
                        const uint32_t       &WImageBitsY,
                        const uchar          *WImageBitsOriginal,
                        const uint32_t       &Q,
                        const std::string    &DCT_Pass) {

return Watermark::Decode(FilePathIn,
                           DecodedWord,
                           WImageBits,
                           WImageBitsX,
                           WImageBitsY,
                           WImageBitsOriginal,
                           Q,
                           DCT_Pass);
}

void MainWindow::on_Decode_released() {
    uchar *TransferBits = nullptr;
    if (DecodingImageView != nullptr) {
        QImage temp = DecodingImageView->toImage();
        TransferBits = new uchar[DecodingImageView->width() * DecodingImageView->height()];
        memcpy(TransferBits, temp.bits(), DecodingImageView->width() * DecodingImageView->height());
    }

    int32_t thread_count = std::thread::hardware_concurrency();
    std::vector<std::future<uchar*>> workers(thread_count);
    std::vector<std::string> DecodedWord(thread_count);
    uchar** WatermarkImageBits = new uchar*[thread_count];
    for (int32_t i = 0; i < thread_count; i++)
        WatermarkImageBits[i] = new uchar[BitsX * BitsY];

    std::string temp = "asdoykoprtyjto,pe[ryjopertyjope[rt,yjtope[ryjoprt,yjope[rtyjro,pe[t,yjope[rtyjerj458036-yun45890tjkls;dfgvc";

    for (int32_t element = 0; element < Table->rowCount(); element += thread_count) {
        for (int32_t i = 0; i < thread_count; i++) {
            if (element + i < Table->rowCount()) {
                QString InPath = QString::fromStdString(Table->fullFilePathAt(element+i));

                /*
                QFuture<uchar*> t = QtConcurrent::run(Decode, Table->fullFilePathAt(element + i),
                                                      std::ref(DecodedWord[i]),
                                                      WatermarkImageBits[i],
                                                      64,
                                                      64,
                                                      TransferBits,
                                                      150,
                                                      temp)
                                                      */

                workers[i] = std::async(Decode,
                                         Table->fullFilePathAt(element + i),
                                         std::ref(DecodedWord[i]),
                                         WatermarkImageBits[i],
                                         16,
                                         16,
                                         TransferBits,
                                         70,
                                         temp);

                /*
                Decode( Table->fullFilePathAt(element + i),
                                                         std::ref(DecodedWord[i]),
                                                         WatermarkImageBits[i],
                                                         64,
                                                         64,
                                                         TransferBits,
                                                         150,
                                                         temp);
                                                         */

                events.processEvents(QEventLoop::AllEvents);
                if (ForcedExit) {
                    for (int32_t i = 0; i < thread_count; i++) {
                        if (workers[i].valid())
                            workers[i].wait();
                    }
                    //for (uint32_t i = 0; i < thread_count; i++)
                    //    delete[] WatermarkImageBits[i];
                    //delete[] WatermarkImageBits;
                    //delete[] TransferBits;
                    //WatermarkImageBits = nullptr;
                    //TransferBits = nullptr;
                    qDebug() << "thread: " << Stopped;
                    Stopped = true;
                    events.exit(0);
                    break;
                    //return;
                }
            }
            status->setValue(int(double(element + i) / double(Table->rowCount()) * 100.));
        }
        for (int32_t i = 0; i < thread_count; i++) {
            if (workers[i].valid()) {
                workers[i].wait();
                WatermarkImageBits[i] = workers[i].get();
                Table->insertWatermarkAt(element+i, WatermarkImageBits[i], 64, 64);
                Table->insertWatermarkTextAt(element+i, QString::fromStdString(DecodedWord[i]));
            }

            events.processEvents(QEventLoop::AllEvents);
            if (ForcedExit) {
                for (int32_t i = 0; i < thread_count; i++) {
                    if (workers[i].valid())
                        workers[i].wait();
                }
                //for (uint32_t i = 0; i < thread_count; i++)
                //    delete[] WatermarkImageBits[i];
                //delete[] WatermarkImageBits;
                //delete[] TransferBits;
                //WatermarkImageBits = nullptr;
                //TransferBits = nullptr;
                Stopped = true;
                events.exit(0);
                break;
                //return;
            }

            //qDebug() << QString::fromStdString(Table->fullFilePathAt(element + i));
            //qDebug() << i << QString::fromStdString(DecodedWord[i]);

            DecodedWord[i].clear();
        }
    }

    //if (WatermarkImageBits != nullptr) {
      //  for (uint32_t i = 0; i < thread_count; i++) {
        //    if (WatermarkImageBits[i])
          //      delete[] WatermarkImageBits[i];
        //}
        //delete[] WatermarkImageBits;
    //}

    if (TransferBits != nullptr)
        delete[] TransferBits;
}


void MainWindow::on_AddFiles_released() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.jpg *.jpeg *.bmp)"));
    dialog.setViewMode(QFileDialog::List);

    QStringList FileNames;
    if (dialog.exec()) {
        FileNames = dialog.selectedFiles();
        for (uint32_t i = 0; i < (unsigned)FileNames.count(); i++)
            Table->insertFilepath(FileNames[i]);
        ui->tableView->resizeColumnsToContents();
    }
}


void MainWindow::on_AddDirectory_released() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::List);

    QStringList DirectoryNames;
    if (dialog.exec()) {
        DirectoryNames = dialog.selectedFiles();
        for (uint32_t i = 0; i < (unsigned)DirectoryNames.count(); i++) {
            QDir directory(DirectoryNames[i]);
            QStringList FileNames = directory.entryList(QDir::Files);
            for (uint32_t j = 0; j < (unsigned)FileNames.count(); j++) {
                if (FileNames[j].contains(".jpg")  ||
                    FileNames[j].contains(".jpeg") ||
                    FileNames[j].contains(".bmp"))
                Table->insertFilepath(DirectoryNames[i] + "/" + FileNames[j]);
            }
            FileNames.clear();
        }
        ui->tableView->resizeColumnsToContents();
    }
    DirectoryNames.clear();
}





void MainWindow::on_LoadImageButton_released() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images Grayscale (*.png *.bmp)"));
    dialog.setViewMode(QFileDialog::List);

    QStringList FileNames;
    if (dialog.exec()) {
        FileNames = dialog.selectedFiles();

        //QImage temp;
        //temp.load(FileNames[0], QImage::Format_Indexed8);
        //temp.load(FileNames[0]);

        EncodingImageView = new QPixmap;
        EncodingImageView->load(FileNames[0]);
        //EncodingImageView->fromImage(temp);
        ui->EncodingImageLabel->setPixmap(*EncodingImageView);

        BitsX = EncodingImageView->height();
        BitsY = EncodingImageView->width();
        ui->ImageRadio->click();
    }
}

void MainWindow::on_CustomDirectory_released() {
    ui->ChooseOutputDirectoryButton->setEnabled(true);
}


void MainWindow::on_NoCustomDirectory_released() {
    ui->ChooseOutputDirectoryButton->setEnabled(false);
    ui->LabelOutputDirectory->setEnabled(false);
}


void MainWindow::on_ChooseOutputDirectoryButton_released() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::List);
    QStringList Dirs;

    if (dialog.exec()) {
        Dirs = dialog.selectedFiles();
        OutputDirectory = Dirs[0];
        ui->LabelOutputDirectory->setText(OutputDirectory);
        ui->LabelOutputDirectory->setEnabled(true);
    }
}


void MainWindow::on_CustomWSize_released() {
    if (ui->CustomWImageX->isEnabled()) {
        ui->CustomWImageX->setEnabled(0);
    }
    else if (!ui->CustomWImageX->isEnabled()) {
        ui->CustomWImageX->setEnabled(1);
    }
    if (ui->CustomWImageY->isEnabled()) {
        ui->CustomWImageY->setEnabled(0);
    }
    else if (!ui->CustomWImageY->isEnabled()) {
        ui->CustomWImageY->setEnabled(1);
    }
}


void MainWindow::on_TextRadio_released() {
    if (ui->ImageRadio->isChecked()) {
        ui->ImageRadio->setChecked(0);
    }
    if (!ui->TextRadio->isChecked()) {
        ui->TextRadio->setChecked(1);
    }
}


void MainWindow::on_ImageRadio_released() {
    if (ui->TextRadio->isChecked()) {
        ui->TextRadio->setChecked(0);
    }
    if (!ui->ImageRadio->isChecked()) {
        ui->ImageRadio->setChecked(1);
    }
}

void MainWindow::UpdateDecodingInfo(const QModelIndex &index) {
    if (DisplayedDecode != nullptr)
        delete[] DisplayedDecode;
    DisplayedDecode = new uchar[64*64*4];
    memset(DisplayedDecode, 0, 64*64*4);

    uchar* one_bit_img = new uchar[64*64];
    memcpy(one_bit_img, Table->getWatermarkAt(index), 64*64*sizeof(uchar));
            //Table->getWatermarkAt(index);



    for (int i = 0, j = 0; i < 64*64; i++, j+=4) {


        DisplayedDecode[j]      = one_bit_img[i] * 255;
        DisplayedDecode[j+1]    = one_bit_img[i] * 255;
        DisplayedDecode[j+2]    = one_bit_img[i] * 255;
        DisplayedDecode[j+3]    = 0xFF;

        qDebug() << one_bit_img[i] << DisplayedDecode[j] << DisplayedDecode[j+1] << DisplayedDecode[j+2];
    }
    qDebug() << Table->getWatermarkAt(index);
    QImage temp_img(one_bit_img, 64, 64, 64 * sizeof(uchar) * 4, QImage::Format_RGB32);

    QImage image((const unsigned char*)DisplayedDecode, 64, 64, QImage::Format_RGB32);
    image.save("____dddddd________.png", nullptr, 100);


    temp_img.save("temp.png");
    //ui->DecodingImageLabel->
    //WillBeDecodedView->loadFromData(Table->getWatermarkAt(index), 64*64*4);
    WillBeDecodedView->fromImage(image);
    ui->DecodingImageLabel->setPixmap(*WillBeDecodedView);
}


void MainWindow::on_tableView_activated(const QModelIndex &index) {

}

void MainWindow::on_tableView_pressed(const QModelIndex &index) {
    UpdateDecodingInfo(index);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    ForcedExit = true;
}


void MainWindow::on_CleanTable_released() {
    ForcedExit = true;
    int counter = 0;
    while (!Stopped) {
        qDebug() << "clean: " << Stopped;
        _sleep(500);
        counter++;
    }
    Table->clean();
    status->setValue(100);
    ForcedExit = false;
    Stopped = false;
}


void MainWindow::on_pushButton_2_released()
{
    Watermark::Video();
}

