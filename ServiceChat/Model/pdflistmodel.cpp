#include "pdflistmodel.h"

PdfListModel::~PdfListModel()
{
    for(auto &p : pixMap)
    {
        fz_drop_pixmap(m_doc.ctx(), p.second);
        p.second = nullptr;
    }
}

PdfListModel::PdfListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

PdfListModel::PdfListModel(const QString &fileName, QObject *parent):PdfListModel()
{
    m_fileName = fileName;
    m_doc.Open(m_fileName);
}

void PdfListModel::LoadDocument(const QString &fileName)
{
    if(m_fileName == fileName)
        return;
    m_fileName = fileName;
    m_doc.Open(fileName);
}

fz_pixmap *PdfListModel::LoadPixmap(int i) const
{
    return m_doc.LoadPixmap(i, &m_ctm);
}

void PdfListModel::Update()
{
    beginResetModel();
    endResetModel();
}

int PdfListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    if(m_fileName.isEmpty())
        return 0;
    return m_doc.GetPageCount();
}

QVariant PdfListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(m_fileName.isEmpty())
        return QVariant();
    if(role == Qt::DisplayRole)
    {
        int i = index.row();
        fz_pixmap *pix = nullptr;
        if(pixMap.find(i) != pixMap.end() || pixMap[i])
        {
            pix = pixMap[i];
        }
        else
        {
            pix = LoadPixmap(i);
            pixMap[i] = pix;
        }
        int w = fz_pixmap_width(m_doc.ctx(), pix);
        int h = fz_pixmap_height(m_doc.ctx(), pix);

        QImage img = QImage(pix->samples, w, h, QImage::Format_RGB888);
        return img;
    }
    return QVariant();
}

QString PdfListModel::fileName() const
{
    return m_fileName;
}

fz_matrix PdfListModel::ctm() const
{
    return m_ctm;
}

void PdfListModel::setCtm(const fz_matrix &ctm)
{
    m_ctm = ctm;
}

MuPdfUtil::Document PdfListModel::doc() const
{
    return m_doc;
}

void PdfListModel::setDoc(const MuPdfUtil::Document &doc)
{
    m_doc = doc;
}


