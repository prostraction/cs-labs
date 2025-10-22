#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImageReader>
#include <QFileDialog>
#include <QScrollArea>
#include <QBuffer>
#include <QLabel>
#include <QHeaderView>
#include <QDir>
#include <QProgressBar>

#include <QtConcurrent>
#include <QFuture>
#include <future>


#include "fileio.h"
#include "watermark.h"

#include "ui_tablemodel.h"

#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    UI_TableModel* Table;
    QHeaderView *TableVerticalHeader;

    // переименовать позже
    QPixmap* EncodingImageView;
    QPixmap* DecodingImageView;
    QPixmap* WillBeDecodedView;

    uchar *DecodedPixmapBits;
    uchar *DisplayedDecode;
    QString WatermarkText;

    QString OutputDirectory;
    uint32_t BitsLength, BitsX, BitsY;
    uint32_t JPEGForced;

    QProgressBar *status;
    bool ForcedExit;
    bool Stopped;
    QEventLoop events;
    void closeEvent(QCloseEvent *event);

    void UpdateDecodingInfo(const QModelIndex &index);

    static void Encode(int32_t &err,
                       const std::string &FilePath,
                       const std::string &OutputPath,
                       const std::string &WatermarkText,
                       const uchar* TransferBits,
                       const uint32_t &BitsX,
                       const uint32_t &BitsY);

    static uchar* Decode(const std::string &FilePathIn,
                       std::string &DecodedWord,
                       const uchar *WImageBits,
                       const uint32_t &WImageBitsX,
                       const uint32_t &WImageBitsY,
                       const uchar *WImageBitsOriginal,
                       const uint32_t &Q,
                       const std::string &DCT_Pass);

private slots:
    void on_Encode_released();
    void on_Decode_released();
    void on_AddFiles_released();
    void on_AddDirectory_released();


    // test
    void on_pushButton_released();
    void on_LoadImageButton_released();
    void on_CustomDirectory_released();
    void on_NoCustomDirectory_released();
    void on_ChooseOutputDirectoryButton_released();
    void on_CustomWSize_released();
    void on_TextRadio_released();
    void on_ImageRadio_released();

    void on_tableView_activated(const QModelIndex &index);
    void on_tableView_pressed(const QModelIndex &index);

    void on_CleanTable_released();

    void on_pushButton_2_released();

signals:
    void exitRequired();
};
#endif // MAINWINDOW_H
