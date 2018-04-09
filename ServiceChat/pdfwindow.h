#ifndef PDFWINDOW_H
#define PDFWINDOW_H
#include "stdafx.h"
#include <QMainWindow>
#include <memory>
#include "FzPdf/fz_header.h"
namespace Ui {
class PDFWindow;
}

class PDFWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PDFWindow(QWidget *parent = 0);
    ~PDFWindow();
    void LoadDocument(const QString &fileName);
    std::shared_ptr<FzPixmap> LoadPixmap(int number, FzMatrix *mat);
    void LoadPdf(const QString &fileName, float scaleX, float scaleY, float rotateDegree);
    void ShowPdf(const QString &fileName);
private slots:
    void on_actionOpen_triggered();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::PDFWindow *ui;
    MuPdfUtil::Document *document = nullptr;
    struct PixmapNum
    {
        int number;
        std::shared_ptr<FzPixmap> pPixmap;
    };
    QList<PixmapNum> pixmapList;
};

#endif // PDFWINDOW_H
