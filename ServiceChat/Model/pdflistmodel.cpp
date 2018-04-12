#include "pdflistmodel.h"

PdfListModel::~PdfListModel()
{
//    delete document;
    for(auto &p : pixMap)
    {
        fz_drop_pixmap(rdoc.getContext(), p.second);
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
//    document = new MuPdfUtil::Document(m_fileName);
    rdoc.Open(m_fileName);
}

void PdfListModel::LoadDocument(const QString &fileName)
{
    if(m_fileName == fileName)
        return;
    m_fileName = fileName;
//    if(document)
//        delete document;
//    document = new MuPdfUtil::Document(m_fileName);
    rdoc.Open(fileName);
}

fz_pixmap *PdfListModel::LoadPixmap(int i) const
{
    return rdoc.LoadPixmap(i, &m_ctm);
}

//std::shared_ptr<FzPixmap> PdfListModel::LoadPixmap(int i) const
//{
//    if(!document)
//        return nullptr;
//    std::shared_ptr<FzPixmap> pix =
//            std::make_shared<FzPixmap>(document->getContext(),
//            document->getDocument(),
//            i, &m_mat,
//            FzColorspace::FzDeviceRgb(document->getContext()));
//    return pix;
//}

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
//    if(!document)
//        return 0;
//    return document->GetPageCount();
    return rdoc.GetPageCount();
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
        QImage img;
        if(pixMap.find(i) != pixMap.end())
        {
            pix = pixMap[i];
        }
        else
        {
            pix = LoadPixmap(i);
            pixMap[i] = pix;
            img = QImage(pix->samples, pix->w, pix->h, QImage::Format_RGB888);
            DEBUG_VAR(img.save(QString("123321IMG%1.jpg").arg(i)));
        }
        int w = fz_pixmap_width(rdoc.getContext(), pix);
        int h = fz_pixmap_height(rdoc.getContext(), pix);

//        DEBUG_VAR(width);
//        DEBUG_VAR(height);
//        DEBUG_VAR(pix->w);
//        DEBUG_VAR(pix->h);
//        qDebug() << "";
        img = QImage(pix->samples, w, h, QImage::Format_RGB888);

//        std::shared_ptr<FzPixmap> fzpix = LoadPixmap(i);
//        unsigned char *samples = fzpix->getPixmap()->samples;

//        fz_pixmap *pix = LoadPixmap(i);
//        unsigned char *samples = pix->samples;
//        int width = fzpix->getWidth();
//        int height = fzpix->getHeight();
//        int width = fz_pixmap_height(rdoc.getContext(), pix);
//        int height = fz_pixmap_width(rdoc.getContext(), pix);
//        auto lambda = [=](void *data){
//            DEBUG_VAR(this);
//            if(fz_pixmap *pix = static_cast<fz_pixmap*>(data))
//            {
//                fz_drop_pixmap(rdoc.getContext(), pix);
//            }
//        };
//        QPixmap pixmap()
//        int size = pix->w * pix->h * pix->n;
//        QVector<unsigned char> sampleList;
////
//        for(int i = 0; i < size; ++i)
//        {
//            if((i - 2) % 3 == 0)
//                sampleList.push_back(0);
//            sampleList.push_back(samples[i]);
//        }
//        unsigned char *new_samples = new unsigned char[sampleList.count()];
//        for(int i = 0; i < sampleList.count(); ++i)
//            new_samples[i] = sampleList[i];
//        QImage image(new_samples, width, height, QImage::Format_RGB888, [](void *data){
//            if(unsigned char *samples = static_cast<unsigned char*>(data))
//            {
//                delete[] samples;
//            }
//        });
//        fz_drop_pixmap(rdoc.getContext(), pix);
        return img;
    }
    return QVariant();
}

//FzMatrix PdfListModel::mat() const
//{
//    return m_mat;
//}

//void PdfListModel::setMat(const FzMatrix &mat)
//{
//    m_mat = mat;
//}

QString PdfListModel::fileName() const
{
    return m_fileName;
}

//void PdfListModel::setFileName(const QString &fileName)
//{
//    m_fileName = fileName;
//}

//MuPdfUtil::Document *PdfListModel::getDocument() const
//{
//    return document;
//}

//void PdfListModel::setDocument(MuPdfUtil::Document *value)
//{
//    document = value;
//}

fz_matrix PdfListModel::getCtm() const
{
    return m_ctm;
}

void PdfListModel::setCtm(const fz_matrix &ctm)
{
    m_ctm = ctm;
}
