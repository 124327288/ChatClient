#ifndef PDFLISTVIEW_H
#define PDFLISTVIEW_H
#include "stdafx.h"
#include <QListView>
#include "Model/pdflistmodel.h"
#include "Delegate/pdflistitemdelegate.h"
class PdfListView : public QListView
{
    Q_OBJECT
public:
    explicit PdfListView(QWidget *parent = nullptr);

signals:

public slots:

private:

};

#endif // PDFLISTVIEW_H