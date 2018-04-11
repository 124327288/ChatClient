#ifndef PDFLISTMODEL_H
#define PDFLISTMODEL_H

#include <QAbstractListModel>
#include "FzPdf/fz_header.h"
class PdfListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ~PdfListModel();
    explicit PdfListModel(QObject *parent = nullptr);
    explicit PdfListModel(const QString &fileName, QObject *parent = nullptr);
    void LoadDocument(const QString &fileName);
//    std::shared_ptr<FzPixmap> LoadPixmap(int i) const;
    fz_pixmap *LoadPixmap(int i) const;
    void Update();
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    FzMatrix mat() const;
    void setMat(const FzMatrix &mat);

    QString fileName() const;
//    void setFileName(const QString &fileName);

//    MuPdfUtil::Document *getDocument() const;
//    void setDocument(MuPdfUtil::Document *value);

    fz_matrix getCtm() const;
    void setCtm(const fz_matrix &ctm);

private:
    QString m_fileName;
//    mutable FzMatrix m_mat;
//    MuPdfUtil::Document *document = nullptr;
    mutable fz_matrix m_ctm;
    MuPdfUtil::RDocument rdoc;
};

#endif // PDFLISTMODEL_H
