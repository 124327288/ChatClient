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
    ui->comboBox->setCurrentIndex(2);
}

PDFWindow::~PDFWindow()
{
    delete ui;
    delete document;
}

float PDFWindow::GetScale() const
{
    bool ok;
    QString txt = ui->comboBox->currentText();
    float s = txt.left(txt.count() - 1).toFloat(&ok);
    s = ok ? s : 100;
    return s / 100;
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

void PDFWindow::ShowPdf(const QString &fileName)
{
    m_fileName = fileName;
    setWindowTitle(tr("%1 - Mini PDF Reader").arg(fileName));
    if(document)
        delete document;
    document = nullptr;
    LoadDocument(fileName);
    int cnt = document->GetPageCount();
    DEBUG_VAR(cnt);
//    cnt = std::min(cnt, 10);
    float scale = GetScale();
    FzMatrix mat;
    mat.Scale(scale, scale);
//    mat.PreRotate(rotateDegree);
    QTimer *timer = new QTimer(this);
    timer->setInterval(200);
    connect(timer, &QTimer::timeout, [=]{
        static int i = 0;
        static FzMatrix ctm(mat);
        if(i >= cnt)
        {
            timer->stop();
            return;
        }
        std::shared_ptr<FzPixmap> pix = LoadPixmap(i, &ctm);
        unsigned char *samples = pix->getPixmap()->samples;
        int width = pix->getWidth();
        int height = pix->getHeight();

        QImage image(samples, width, height, QImage::Format_RGB888);

        QLabel *label = new QLabel;
        label->setPixmap(QPixmap::fromImage(image));
        ui->verticalLayout->addWidget(label);
        ui->verticalLayout->setAlignment(label, Qt::AlignHCenter);
        ++i;
    });
    timer->start();

//    setGeometry(geometry().x(), geometry().y(), max_width  + 100, 800);
}

void PDFWindow::on_actionOpen_triggered()
{
    m_fileName = QFileDialog::getOpenFileName(this, tr("Open PDF file"), "D:/", "*.pdf");
    if(!m_fileName.isNull())
    {
        while(QLayoutItem *item = ui->verticalLayout->takeAt(0))
            delete item;
        ShowPdf(m_fileName);
    }

}

void PDFWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    float scale = GetScale();
    DEBUG_VAR(scale);
    if(!m_fileName.isNull())
    {
        while(QLayoutItem *item = ui->verticalLayout->takeAt(0))
            delete item;
        ShowPdf(m_fileName);
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
