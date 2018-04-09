#ifndef PDFWINDOW_H
#define PDFWINDOW_H
#include "stdafx.h"
#include <QMainWindow>
#include <mupdf/fitz.h>
#include <mupdf/pdf.h>
namespace Ui {
class PDFWindow;
}

class PDFWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PDFWindow(QWidget *parent = 0);
    ~PDFWindow();
//    void LoadDocument
    void LoadPdf(const QString &fileName, float scaleX, float scaleY, float rotateDegree, std::vector<fz_pixmap *> *fzPixmapList);
    void ShowPdf(const QString &fileName);
private slots:
    void on_actionOpen_triggered();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::PDFWindow *ui;

};

#endif // PDFWINDOW_H
