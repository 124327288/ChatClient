#ifndef PDFWINDOW_H
#define PDFWINDOW_H

#include <QMainWindow>

namespace Ui {
class PDFWindow;
}

class PDFWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PDFWindow(QWidget *parent = 0);
    ~PDFWindow();
    void ShowPDF(const QString &fileName);
private slots:
    void on_actionOpen_triggered();

private:
    Ui::PDFWindow *ui;
};

#endif // PDFWINDOW_H
