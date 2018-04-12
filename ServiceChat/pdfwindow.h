#ifndef PDFWINDOW_H
#define PDFWINDOW_H
#include "stdafx.h"
#include <QMainWindow>
#include <memory>
#include "FzPdf/fz_header.h"
#include "Model/pdflistmodel.h"
#include "Delegate/pdflistitemdelegate.h"
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
//    void LoadDocument();
//    std::shared_ptr<FzPixmap> LoadPixmap(int number, FzMatrix *mat);
//    fz_pixmap *LoadPixmap(int number, fz_matrix *mat);
    void ShowPdf();
    void ShowPdf(const QString &fileName);
    QString &FileName();
    void ClosePdf();
private slots:
    void on_actionOpen_triggered();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::PDFWindow *ui;
//    MuPdfUtil::Document *document = nullptr;
//    MuPdfUtil::RDocument *document = nullptr;
    QString m_fileName;
    PdfListModel *pdfListModel;
    PdfListItemDelegate *pdfListItemDelegate;
};

#endif // PDFWINDOW_H
