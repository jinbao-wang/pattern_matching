#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTextEdit>
#include <QDebug>
#include <QDir>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showView()));
    timer->start(1000 / 60.0f);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_load_clicked()
{
    cout << "info:start ------------------- " << endl;
    cout << pm.src_path << endl;
    cout << pm.dst_path << endl;
    cout << pm.out_path << endl;
    cout << pm.m_iMaxPos << endl;
    cout << pm.m_dMaxOverlap << endl;
    cout << pm.m_dScore << endl;
    cout << pm.m_dToleranceAngle << endl;
    cout << pm.m_iMinReduceArea << endl;
    cout << pm.k_size << endl;
    cout << pm.pixel_threshold << endl;

    pm.LoadSrc();
    pm.LoadDst();

    freshView();
}


void MainWindow::on_detect_clicked()
{
    // 创建输出文件夹，存检测cell图
    QString output_path = QString::fromStdString(pm.out_path);
    QDir dir;
    if (!dir.exists(output_path)) {
        dir.mkpath(output_path);
    }

    pm.LearnPattern();
    pm.Match();
    pm.SaveRes();
    freshView();

    cout << "info:save result, start calculate anomlay map ..." << endl;

    pm.CalculateAnomalyMap();

    cout << "info:end ------------------- " << endl;
}

void MainWindow::showView()
{
    if (!pixmap_show_src.isNull()){
        ui->label->setPixmap(pixmap_show_src.scaled(ui->label->size(), Qt::KeepAspectRatio));
        ui->label->show();
    }

    if (!pixmap_show_dst.isNull()){
        ui->label_10->setPixmap(pixmap_show_dst.scaled(ui->label_10->size(), Qt::KeepAspectRatio));
        ui->label_10->show();
    }

    pm.src_path = ui->plainTextEdit_2->toPlainText().toStdString();
    pm.dst_path = ui->plainTextEdit_3->toPlainText().toStdString();
    pm.out_path = ui->plainTextEdit_4->toPlainText().toStdString();
    pm.m_iMaxPos = ui->plainTextEdit->toPlainText().toInt();
    pm.m_dMaxOverlap = ui->plainTextEdit_5->toPlainText().toDouble();
    pm.m_dScore = ui->plainTextEdit_6->toPlainText().toDouble();
    pm.m_dToleranceAngle = ui->plainTextEdit_7->toPlainText().toDouble();
    pm.m_iMinReduceArea = ui->plainTextEdit_8->toPlainText().toInt();
    pm.k_size = ui->plainTextEdit_9->toPlainText().toInt();
    pm.pixel_threshold = ui->plainTextEdit_10->toPlainText().toInt();
}

void MainWindow::freshView()
{
    if (!pm.matSrc_bgr.empty()){
        img_lock.lock();
        pixmap_show_src = QPixmap::fromImage(QImage((unsigned char*)pm.matSrc_bgr.data, pm.matSrc_bgr.cols, pm.matSrc_bgr.rows,
                                                pm.matSrc_bgr.cols*pm.matSrc_bgr.channels(), QImage::Format_BGR888));
        img_lock.unlock();
    }

    if (!pm.matDst_bgr.empty()){
        img_lock.lock();
        pixmap_show_dst = QPixmap::fromImage(QImage((unsigned char*)pm.matDst_bgr.data, pm.matDst_bgr.cols, pm.matDst_bgr.rows,
                                                    pm.matDst_bgr.cols*pm.matDst_bgr.channels(), QImage::Format_BGR888));
        img_lock.unlock();
    }
}
