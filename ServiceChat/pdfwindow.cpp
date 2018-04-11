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
    setWindowTitle(tr("Mini PDF Reader"));
    ui->comboBox->setCurrentIndex(2);
    pdfListModel = new PdfListModel;
    pdfListItemDelegate = new PdfListItemDelegate;
    ui->listView->setModel(pdfListModel);
    ui->listView->setItemDelegate(pdfListItemDelegate);
    connect(ui->actionCloseDoc, &QAction::triggered, this, &PDFWindow::ClosePdf);
}

PDFWindow::~PDFWindow()
{
    delete ui;
//    delete document;
}

float PDFWindow::GetScale() const
{
    bool ok;
    QString txt = ui->comboBox->currentText();
    float s = txt.left(txt.count() - 1).toFloat(&ok);
    s = ok ? s : 100;
    return s / 100;
}

//void PDFWindow::LoadDocument()
//{
//    if(!document)
//        document = new MuPdfUtil::RDocument(m_fileName);
//}

//std::shared_ptr<FzPixmap> PDFWindow::LoadPixmap(int number, FzMatrix *mat)
//{
//    if(!document)
//        return nullptr;
//    std::shared_ptr<FzPixmap> pix = std::make_shared<FzPixmap>(document->getContext(),
//                                                               document->getDocument(),
//                                                               number, mat,
//                                                               FzColorspace::FzDeviceRgb(document->getContext()));
//    return pix;
//}

//fz_pixmap *PDFWindow::LoadPixmap(int number, fz_matrix *mat)
//{
//    return document->LoadPixmap(number, mat);
//}

void PDFWindow::ShowPdf()
{
    setWindowTitle(tr("%1 - Mini PDF Reader").arg(m_fileName));
    float scale = GetScale();
    fz_matrix ctm;
    fz_scale(&ctm, scale, scale);
//    FzMatrix mat;
//    mat.Scale(scale, scale);
//    mat.PreRotate(0);
    pdfListModel->LoadDocument(m_fileName);
//    pdfListModel->setMat(mat);
    pdfListModel->setCtm(ctm);
    pdfListModel->Update();
//    if(document)
//        delete document;
//    document = nullptr;
//    LoadDocument();
//    int cnt = document->GetPageCount();
////    cnt = std::min(cnt, 10);
//    float scale = GetScale();
//    FzMatrix mat;
//    mat.Scale(scale, scale);
//    mat.PreRotate(1);
//    QTimer *timer = new QTimer(this);
//    timer->setInterval(200);
//    int i = 0;
//    connect(timer, &QTimer::timeout, [=]() mutable {
//        if(i >= cnt)
//        {
//            timer->stop();
//            return;
//        }
//        std::shared_ptr<FzPixmap> fzpix = LoadPixmap(i, &mat);
//        unsigned char *samples = fzpix->getPixmap()->samples;
//        int width = fzpix->getWidth();
//        int height = fzpix->getHeight();

//        QImage image(samples, width, height, QImage::Format_RGB888);
//        QPixmap pix = QPixmap::fromImage(image);
//        QLabel *label = new QLabel;
//        label->setPixmap(pix);
//        ui->verticalLayout->addWidget(label);
////        DEBUG_VAR(ui->verticalLayout->count());
//        ui->verticalLayout->setAlignment(label, Qt::AlignHCenter);
//        ++i;
//    });
//    timer->start();

    //    setGeometry(geometry().x(), geometry().y(), max_width  + 100, 800);
}

void PDFWindow::ShowPdf(const QString &fileName)
{
    m_fileName = fileName;

    ShowPdf();
}

QString &PDFWindow::FileName()
{
    return m_fileName;
}

void PDFWindow::ClosePdf()
{
    setWindowTitle(tr("Mini PDF Reader"));
    for(int i = 0; i < ui->verticalLayout->count(); ++i)
    {
        QWidget *widget = ui->verticalLayout->itemAt(i)->widget();
        if(QLabel *label = dynamic_cast<QLabel*>(widget))
        {
            label->clear();
        }
    }
    while(QLayoutItem *item = ui->verticalLayout->takeAt(0))
        delete item;
}

void PDFWindow::on_actionOpen_triggered()
{
    m_fileName = QFileDialog::getOpenFileName(this, tr("Open PDF file"), "D:/", "*.pdf");
    if(!m_fileName.isNull())
    {
//        ClosePdf();
        ShowPdf();
    }

}

void PDFWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    float scale = GetScale();
    DEBUG_VAR(scale);
    if(!m_fileName.isNull())
    {
        ClosePdf();
        ShowPdf();
    }
//    for(int i = 0; i < ui->verticalLayout->count();  ++i)
//    {
//        QWidget *widget = ui->verticalLayout->itemAt(0)->widget();
//        if(QLabel *label = dynamic_cast<QLabel*>(widget))
//        {
//            const QPixmap *pixmap = label->pixmap();
//            DEBUG_VAR(pixmap);
//            if(!pixmap)
//                continue;
//            QSize size = pixmap->size();
//            label->setPixmap(pixmap->scaled(size * scale));
//        }
//    }
}
