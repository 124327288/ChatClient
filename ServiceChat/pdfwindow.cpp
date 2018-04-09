#include "pdfwindow.h"
#include "ui_pdfwindow.h"

#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>
#include <string>
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
}

#define FZ_CATCH(ctx, err)                                                          \
    fz_catch(ctx)                                                                   \
    {                                                                               \
        qDebug() << QString("cannot %1 : %2").arg(err).arg(fz_caught_message(ctx)); \
        fz_drop_context(ctx);                                                       \
        return;                                                                     \
    }

#define FZ_CATCH_EX(ctx, doc, err)                                                  \
    fz_catch(ctx)                                                                   \
    {                                                                               \
        qDebug() << QString("cannot %1 : %2").arg(err).arg(fz_caught_message(ctx)); \
        fz_drop_document(ctx, doc);                                                 \
        fz_drop_context(ctx);                                                       \
        return;                                                                     \
    };

void PDFWindow::LoadPdf(const QString &fileName, float scaleX, float scaleY, float rotateDegree, vector<fz_pixmap*> *fzPixmapList)
{
    string filename = fileName.toStdString();
    fz_context *ctx = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
    if(!ctx)
    {
        qDebug() << QString("cannot create mupdf context");
        return;
    }

    fz_try(ctx)
        fz_register_document_handlers(ctx);
    FZ_CATCH(ctx, "fz_register_document_handlers")

    fz_document *doc;
    fz_try(ctx)
        doc = fz_open_document(ctx, filename.data());
    FZ_CATCH_EX(ctx, doc, "fz_open_document")

    int page_count;
    fz_try(ctx)
        page_count = fz_count_pages(ctx, doc);
    FZ_CATCH_EX(ctx, doc, "fz_count_pages")

    fz_matrix ctm;
    fz_scale(&ctm, scaleX, scaleY);
    fz_pre_rotate(&ctm, rotateDegree);

    for(int i = 0; i < page_count; ++i)
    {
        fz_pixmap *pix;
        fz_try(ctx)
            pix = fz_new_pixmap_from_page_number(ctx, doc, i, &ctm, fz_device_rgb(ctx), 0);
        FZ_CATCH_EX(ctx, doc, "fz_new_pixmap_from_page_number")
        fzPixmapList->push_back(pix);
        unsigned char *samples = pix->samples;
//        int width = fz_pixmap_width(ctx, pix);
//        int height = fz_pixmap_height(ctx, pix);

//        max_width = max_width > width ? max_width : width;

//        QImage image(samples, width, height, QImage::Format_RGB888);

//        QLabel *label = new QLabel;
//        label->setPixmap(QPixmap::fromImage(image));
//        ui->verticalLayout->addWidget(label);
//        ui->verticalLayout->setAlignment(label, Qt::AlignHCenter);
//        fz_drop_pixmap(ctx, pix);
    }

    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
}


void PDFWindow::ShowPdf(const QString &fileName)
{
    QByteArray fileBytes = fileName.toUtf8();
    char *file = const_cast<char*>(fileBytes.data());





    int max_width = 0;

    for(int i = 0; i < page_count; ++i)
    {
        fz_pixmap *pix;
        fz_try(ctx)
            pix = fz_new_pixmap_from_page_number(ctx, doc, i, &ctm, fz_device_rgb(ctx), 0);
        FZ_CATCH_EX(ctx, doc, "fz_new_pixmap_from_page_number")

        unsigned char *samples = pix->samples;
        int width = fz_pixmap_width(ctx, pix);
        int height = fz_pixmap_height(ctx, pix);

        max_width = max_width > width ? max_width : width;

        QImage image(samples, width, height, QImage::Format_RGB888);
        //        image = image.scaledToWidth(2);

        QLabel *label = new QLabel;
        label->setPixmap(QPixmap::fromImage(image));
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->setAlignment(label, Qt::AlignHCenter);
        fz_drop_pixmap(ctx, pix);
    }

    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);

    setGeometry(geometry().x(), geometry().y(), max_width  + 100, 800);
}

void PDFWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDF file"), "D:/", "*.pdf");
    if(!fileName.isNull())
    {
        while(QLayoutItem *item = ui->verticalLayout->takeAt(0))
            delete item;
        ShowPDF(fileName);
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
