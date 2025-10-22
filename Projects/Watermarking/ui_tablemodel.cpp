#include "ui_tablemodel.h"

UI_TableModel::UI_TableModel(QObject *parent)
    : QAbstractTableModel(parent) {
    RowCount = 2;
    ColumnCount = 0;
    LastID = 0;
}

QVariant UI_TableModel::headerData(int column, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        if (column == 0)
            return tr("File");
        if (column == 1)
            return tr("Error");
        if (column == 2)
            return tr("Status");
    }
    return QVariant();
}

int UI_TableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_data.count();
}

int UI_TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 3;
}

QVariant UI_TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.column() == 0)
        return m_data.at(index.row()).FilePathVisible;
    else if (index.column() == 1) {
        //if (m_data.at(index.row()).Watermark == nullptr) {
        //    return QString("Nullptr");
        //}
        return m_data.at(index.row()).ErrorTextMessage;
    }
    else if (index.column() == 2)
        return m_data.at(index.row()).StatusTextMessage;
    else
        return "";

    return QVariant();
}

std::string UI_TableModel::fullFilePathAt(const int row) {
    return m_data.at(row).FilePath.toStdString();
}

std::string UI_TableModel::at(const int column, const int row) {
    if (column == 0)
        return m_data.at(row).FilePathVisible.toStdString();
    else if (column == 1)
        return m_data.at(row).ErrorTextMessage.toStdString();
    else if (column == 2)
        return m_data.at(row).StatusTextMessage.toStdString();
    else
        return "";
}

void UI_TableModel::insertFilepath(const QString &FilePath) {
    int row = m_data.count();
    beginInsertRows(QModelIndex(), row, row);
    m_data.insert(row, DataModel(FilePath));
    endInsertRows();
}

void UI_TableModel::insertWatermarkAt(const int &row, uchar *Watermark, const int &WatermarkX, const int &WatermarkY) {
    if (row > m_data.count() || row < 0)
        return;
    if (WatermarkX * WatermarkY <= 0)
        return;

    if (m_data[row].Watermark != nullptr)
        delete[] m_data[row].Watermark;
    m_data[row].Watermark = Watermark;

}

void UI_TableModel::insertWatermarkTextAt(const int &row, const QString &WatermarkText) {
    if (row >= m_data.count() || row < 0)
        return;
    m_data[row].ErrorTextMessage = WatermarkText;
}

void UI_TableModel::insertStatusAt(const int &row, const QString &statusText) {
    if (row >= m_data.count() || row < 0)
        return;
    m_data[row].StatusTextMessage = statusText;
}

void UI_TableModel::remove(int row) {
    if (row < 0 || row > m_data.count())
            return;
    if (m_data.at(row).Watermark) {
        delete[] m_data.at(row).Watermark;
    }
    m_data.removeAt(row);
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
}

void UI_TableModel::clean() {
    for (int i = 0; i < m_data.count(); i++) {
        if (m_data.at(i).Watermark)
            delete[] m_data.at(i).Watermark;
    }
    beginResetModel();
    //qDeleteAll(m_data.begin(), m_data.end());
    m_data.clear();
    endResetModel();
}



uchar *UI_TableModel::getWatermarkAt(const QModelIndex &index) const {
    if (!index.isValid())
        return nullptr;
    if (index.row() > m_data.count() || index.row() < 0)
        return nullptr;
    return m_data.at(index.row()).Watermark;
}

const int32_t UI_TableModel::getHeight(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;
    if (index.row() > m_data.count() || index.row() < 0)
        return 0;
    return m_data.at(index.row()).Watermark_X;
}

const int32_t UI_TableModel::getWidth(const QModelIndex &index) const {
    if (!index.isValid())
        return 0;
    if (index.row() > m_data.count() || index.row() < 0)
        return 0;
    return m_data.at(index.row()).Watermark_Y;
}


