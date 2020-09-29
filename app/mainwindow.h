#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


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
    void on_actionSettings_clicked();

    void on_actionAbout_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_23_currentTextChanged();

    void make_preset();

    void encode_file();

    void extract_file();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
