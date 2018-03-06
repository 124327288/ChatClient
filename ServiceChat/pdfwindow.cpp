#include "pdfwindow.h"
#include "ui_pdfwindow.h"

#include <QFileDialog>
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>
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

void PDFWindow::ShowPDF(const QString &fileName)
{
    QByteArray fileBytes = fileName.toUtf8();
    char *file = const_cast<char*>(fileBytes.data());

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
        doc = fz_open_document(ctx, file);
    FZ_CATCH_EX(ctx, doc, "fz_open_document")

    int page_count;

    fz_try(ctx)
        page_count = fz_count_pages(ctx, doc);
    FZ_CATCH_EX(ctx, doc, "fz_count_pages")

    fz_matrix ctm;
    fz_scale(&ctm, 1, 1);
    fz_pre_rotate(&ctm, 0);

    fz_pixmap *pix;
    int page_number = 0;
    fz_try(ctx)
        pix = fz_new_pixmap_from_page_number(ctx, doc, page_number, &ctm, fz_device_rgb(ctx), 0);
    FZ_CATCH_EX(ctx, doc, "fz_new_pixmap_from_page_number")

    unsigned char *samples = pix->samples;
    int width = fz_pixmap_width(ctx, pix);
    int height = fz_pixmap_height(ctx, pix);

    QImage image(samples, width, height, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(image));

    fz_drop_pixmap(ctx, pix);
    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
}

void PDFWindow::on_actionOpen_triggered()
{
   QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDF file"), QString(), "*.pdf");
   ShowPDF(fileName);
}
