#include "pdfwindow.h"
#include "ui_pdfwindow.h"

#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <string>
#include "FzPdf/fz_header.h"
using namespace std;
PDFWindow::PDFWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PDFWindow)
{
    ui->setupUi(this);
}

PDFWindow::~PDFWindow()
{
    delete ui;
    delete document;
}

void PDFWindow::LoadDocument(const QString &fileName)
{
    if(!document)
        document = new MuPdfUtil::Document(fileName);
}

std::shared_ptr<FzPixmap> PDFWindow::LoadPixmap(int number, FzMatrix *mat)
{
    if(!document)
        return nullptr;
    std::shared_ptr<FzPixmap> pix = std::make_shared<FzPixmap>(document->getContext(),
                                                               document->getDocument(),
                                                               number, mat,
                                                               FzColorspace::FzDeviceRgb(document->getContext()));
    return pix;
}

void PDFWindow::LoadPdf(const QString &fileName, float scaleX, float scaleY, float rotateDegree)
{
//    NewContext();
//    NewDocument(fileName);
//    context->RegisterDocumentHandlers();
//    pageCount = document->CountPages();
//    FzMatrix mat;
//    mat.Scale(scaleX, scaleY);
//    mat.PreRotate(rotateDegree);

//    for(int i = 0; i < page_count; ++i)
//    {
//        fz_pixmap *pix;
//        fz_try(ctx)
//            pix = fz_new_pixmap_from_page_number(ctx, doc, i, &ctm, fz_device_rgb(ctx), 0);
//        FZ_CATCH_EX(ctx, doc, "fz_new_pixmap_from_page_number")
//        fzPixmapList->push_back(pix);
//        unsigned char *samples = pix->samples;
////        int width = fz_pixmap_width(ctx, pix);
////        int height = fz_pixmap_height(ctx, pix);

////        max_width = max_width > width ? max_width : width;

////        QImage image(samples, width, height, QImage::Format_RGB888);

////        QLabel *label = new QLabel;
////        label->setPixmap(QPixmap::fromImage(image));
////        ui->verticalLayout->addWidget(label);
////        ui->verticalLayout->setAlignment(label, Qt::AlignHCenter);
////        fz_drop_pixmap(ctx, pix);
//    }

//    fz_drop_document(ctx, doc);
//    fz_drop_context(ctx);
}


void PDFWindow::ShowPdf(const QString &fileName)
{
    if(document)
        delete document;
    document = nullptr;
    LoadDocument(fileName);
    int cnt = document->GetPageCount();
    cnt = std::min(cnt, 10);
    FzMatrix mat;
    mat.Scale(scaleX, scaleY);
    mat.PreRotate(rotateDegree);
    for(int i = 0; i < cnt; ++i)
    {
        std::shared_ptr<FzPixmap> pix = LoadPixmap(fileName, &mat);
        unsigned char *samples = pix->getPixmap()->samples;
        int width = pix->getWidth();
        int height = pix->getHeight();

        QImage image(samples, width, height, QImage::Format_RGB888);

        QLabel *label = new QLabel;
        label->setPixmap(QPixmap::fromImage(image));
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->setAlignment(label, Qt::AlignHCenter);
    }

//    QByteArray fileBytes = fileName.toUtf8();
//    char *file = const_cast<char*>(fileBytes.data());





//    int max_width = 0;

//    for(int i = 0; i < page_count; ++i)
//    {
//        fz_pixmap *pix;
//        fz_try(ctx)
//            pix = fz_new_pixmap_from_page_number(ctx, doc, i, &ctm, fz_device_rgb(ctx), 0);
//        FZ_CATCH_EX(ctx, doc, "fz_new_pixmap_from_page_number")

//        unsigned char *samples = pix->samples;
//        int width = fz_pixmap_width(ctx, pix);
//        int height = fz_pixmap_height(ctx, pix);

//        max_width = max_width > width ? max_width : width;

//        QImage image(samples, width, height, QImage::Format_RGB888);
//        //        image = image.scaledToWidth(2);

//        QLabel *label = new QLabel;
//        label->setPixmap(QPixmap::fromImage(image));
//        ui->verticalLayout->addWidget(label);
//        ui->verticalLayout->setAlignment(label, Qt::AlignHCenter);
//        fz_drop_pixmap(ctx, pix);
//    }

//    fz_drop_document(ctx, doc);
//    fz_drop_context(ctx);

//    setGeometry(geometry().x(), geometry().y(), max_width  + 100, 800);
}

void PDFWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDF file"), "D:/", "*.pdf");
    if(!fileName.isNull())
    {
        while(QLayoutItem *item = ui->verticalLayout->takeAt(0))
            delete item;
//        ShowPDF(fileName);
    }

}

void PDFWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    float scale = arg1.left(arg1.count() - 1).toFloat() / 100;
    DEBUG_VAR(scale);
    for(int i = 0; i < ui->verticalLayout->count();  ++i)
    {
        QWidget *widget = ui->verticalLayout->itemAt(0)->widget();
        if(QLabel *label = dynamic_cast<QLabel*>(widget))
        {
            const QPixmap *pixmap = label->pixmap();
            DEBUG_VAR(pixmap);
            if(!pixmap)
                continue;
            QSize size = pixmap->size();
            label->setPixmap(pixmap->scaled(size * scale));
        }
    }
}
