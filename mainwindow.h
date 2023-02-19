#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "pattern_matching.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_detect_clicked();
    void showView();
    void freshView();

    void on_load_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    pattern_matching pm;
    QPixmap pixmap_show_src;
    QPixmap pixmap_show_dst;
    std::mutex img_lock;
    QString output_path = "/Users/wangjb/Documents/Project/pattern_matching/qfn/qfn-res";
};
#endif // MAINWINDOW_H