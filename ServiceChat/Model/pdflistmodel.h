#ifndef PDFLISTMODEL_H
#define PDFLISTMODEL_H

#include <QAbstractListModel>
#include "FzPdf/fz_header.h"
class PdfListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ~PdfListModel();
    explicit PdfListModel(const QString &fileName, QObject *parent = nullptr);
    std::shared_ptr<FzPixmap> LoadPixmap(int i) const;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    FzMatrix mat() const;
    void setMat(const FzMatrix &mat);

private:
    QString m_fileName;
    mutable FzMatrix m_mat;
    MuPdfUtil::Document *document = nullptr;
};

#endif // PDFLISTMODEL_H
