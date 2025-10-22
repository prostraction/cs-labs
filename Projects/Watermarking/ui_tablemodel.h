#ifndef UI_TABLEMODEL_H
#define UI_TABLEMODEL_H

#include <QAbstractTableModel>
#include <QKeyEvent>
#include <qdebug.h>

class UI_TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit UI_TableModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    std::string fullFilePathAt(const int row);
    std::string at(const int column, const int row);

    void insertFilepath (const QString &FilePath);
    void insertWatermarkAt(const int &row, uchar *Watermark, const int &WatermarkX, const int &WatermarkY);
    void insertWatermarkTextAt(const int &row, const QString &WatermarkText);
    void insertStatusAt(const int &row, const QString &statusText);

    void remove(int row);
    void clean();

    uchar *getWatermarkAt(const QModelIndex &index) const;
    const int32_t getWidth(const QModelIndex &index) const;
    const int32_t getHeight(const QModelIndex &index) const;

    struct DataModel {
        // Общее состояние
        QString FilePath;
        QString FilePathVisible;
        QString ErrorTextMessage;
        QString StatusTextMessage;

        // Знак
        uchar* Watermark;
        QString WatermarkText;
        int32_t Watermark_X, Watermark_Y;

        QString status;
        int32_t error;
        int32_t jpeg_quality_factor;

        DataModel() {
            FilePath = "";
            FilePathVisible = "";
            ErrorTextMessage = "";
            StatusTextMessage = "";
            WatermarkText = "";
            Watermark_X = 0; Watermark_Y = 0;
            status = ""; error = 0; jpeg_quality_factor = 100;
            Watermark = nullptr;
        }

        DataModel(const QString &FilePath) {
            this->FilePath = FilePath;
            QString customLastIndex = FilePath.left(FilePath.lastIndexOf('/') - 1);
            if (customLastIndex.lastIndexOf('/') == -1) {
                customLastIndex = FilePath.left(FilePath.lastIndexOf('/'));
            }
            FilePathVisible = FilePath.right(FilePath.length() - customLastIndex.lastIndexOf('/'));
            ErrorTextMessage = "";
            StatusTextMessage = "";
            Watermark = nullptr;
            WatermarkText = "";
            Watermark_X = 0; Watermark_Y = 0;
            status = ""; error = 0; jpeg_quality_factor = 100;
        }

        ~DataModel() {
            //if (Watermark != nullptr)
                //delete[] Watermark;
            //WatermarkText.clear();
        }
    };

private:
    QList<DataModel> m_data;
    uint32_t RowCount;
    uint32_t ColumnCount;
    int32_t LastID;

};

#endif // UI_TABLEMODEL_H
