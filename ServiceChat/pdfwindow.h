#ifndef PDFWINDOW_H
#define PDFWINDOW_H
#include "stdafx.h"
#include <QMainWindow>
#include <memory>
#include "FzPdf/fz_header.h"
#include <View/pdflistview.h>
namespace Ui {
class PDFWindow;
}

class PDFWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PDFWindow(QWidget *parent = 0);
    ~PDFWindow();
    float GetScale() const;
    void ShowPdf();
    void ShowPdf(const QString &fileName);
    void ClosePdf();

    PdfListView *pdfListView() const;
    void setPdfListView(PdfListView *pdfListView);

    QString fileName() const;
    void setFileName(const QString &fileName);

    PdfListModel *pdfListModel() const;
    void setPdfListModel(PdfListModel *pdfListModel);

    PdfListItemDelegate *pdfListItemDelegate() const;
    void setPdfListItemDelegate(PdfListItemDelegate *pdfListItemDelegate);

private slots:
    void on_actionOpen_triggered();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::PDFWindow *ui;
    QString m_fileName;
    PdfListView *m_pdfListView;
    PdfListModel *m_pdfListModel;
    PdfListItemDelegate *m_pdfListItemDelegate;
};

#endif // PDFWINDOW_H
