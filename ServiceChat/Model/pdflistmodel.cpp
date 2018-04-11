#include "pdflistmodel.h"

PdfListModel::~PdfListModel()
{
    delete document;
}

PdfListModel::PdfListModel(const QString &fileName, QObject *parent)
    : QAbstractListModel(parent)
{
    m_fileName = fileName;
    document = new MuPdfUtil::Document(m_fileName);
}

std::shared_ptr<FzPixmap> PdfListModel::LoadPixmap(int i) const
{
    if(!document)
        return nullptr;
    std::shared_ptr<FzPixmap> pix =
            std::make_shared<FzPixmap>(document->getContext(),
            document->getDocument(),
            i, &m_mat,
            FzColorspace::FzDeviceRgb(document->getContext()));
    return pix;
}

int PdfListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return document->GetPageCount();
}

QVariant PdfListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole)
    {
        int i = index.row();
        std::shared_ptr<FzPixmap> fzpix = LoadPixmap(i);
        unsigned char *samples = fzpix->getPixmap()->samples;
        int width = fzpix->getWidth();
        int height = fzpix->getHeight();
        QImage image(samples, width, height, QImage::Format_RGB888);
        return image;
    }
    return QVariant();
}

FzMatrix PdfListModel::mat() const
{
    return m_mat;
}

void PdfListModel::setMat(const FzMatrix &mat)
{
    m_mat = mat;
}
