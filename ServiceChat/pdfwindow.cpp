#include "pdfwindow.h"
#include "ui_pdfwindow.h"

#include <QScrollBar>
#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <string>
#include "FzPdf/fz_header.h"
#include "pdfprinttool.h"
using namespace std;
PDFWindow::PDFWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PDFWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Mini PDF Reader"));
    m_pdfListView = new PdfListView;
    m_pdfListModel = new PdfListModel(m_pdfListView);
    m_pdfListItemDelegate = new PdfListItemDelegate(m_pdfListView);
    m_pdfListView->setModel(m_pdfListModel);
    m_pdfListView->setItemDelegate(m_pdfListItemDelegate);
    ui->verticalLayout->addWidget(m_pdfListView);
    ui->comboBox->setCurrentIndex(2);
    connect(ui->actionCloseDoc, &QAction::triggered, this, &PDFWindow::ClosePdf);
    connect(ui->actionPrint, &QAction::triggered, this, &PDFWindow::PrintPdf);
}

PDFWindow::~PDFWindow()
{
    delete ui;
}

float PDFWindow::GetScale() const
{
    bool ok;
    QString txt = ui->comboBox->currentText();
    float s = txt.left(txt.count() - 1).toFloat(&ok);
    s = ok ? s : 100;
    return s / 100;
}

void PDFWindow::ShowPdf()
{
    setWindowTitle(tr("%1 - Mini PDF Reader").arg(m_fileName));
    float scale = GetScale();
    fz_matrix ctm;
    fz_scale(&ctm, scale, scale);
    m_pdfListModel->LoadDocument(m_fileName);
    m_pdfListModel->setCtm(ctm);
    m_pdfListModel->Update();
    QTimer::singleShot(1000,[=]{
        DEBUG_VAR(m_pdfListItemDelegate->size());
        if(m_pdfListItemDelegate->size() != QSize())
        {
            m_pdfListView->resize(m_pdfListItemDelegate->size().width(), m_pdfListView->height());
        }
    });


//    m_pdfListView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //    setGeometry(geometry().x(), geometry().y(), max_width  + 100, 800);
}

void PDFWindow::ShowPdf(const QString &fileName)
{
    m_fileName = fileName;

    ShowPdf();
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

void PDFWindow::PrintPdf()
{
    if(fileName().isEmpty())
        return;
    DEBUG_FUNC;
    DEBUG_VAR(fileName());
    QString cmd = QString("pdfprint.exe -prompt \"%1\"").arg(fileName());
    DEBUG_VAR(PdfPrintTool::Instance().PrintW(cmd));
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
}

PdfListItemDelegate *PDFWindow::pdfListItemDelegate() const
{
    return m_pdfListItemDelegate;
}

void PDFWindow::setPdfListItemDelegate(PdfListItemDelegate *pdfListItemDelegate)
{
    m_pdfListItemDelegate = pdfListItemDelegate;
}

PdfListModel *PDFWindow::pdfListModel() const
{
    return m_pdfListModel;
}

void PDFWindow::setPdfListModel(PdfListModel *pdfListModel)
{
    m_pdfListModel = pdfListModel;
}

QString PDFWindow::fileName() const
{
    return m_fileName;
}

void PDFWindow::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

PdfListView *PDFWindow::pdfListView() const
{
    return m_pdfListView;
}

void PDFWindow::setPdfListView(PdfListView *pdfListView)
{
    m_pdfListView = pdfListView;
}
