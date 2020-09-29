#include <QFile>
#include <QDir>
#include <iostream>
#include "settings.h"
#include "ui_settings.h"
#include "taskcomplete.h"

extern QString _message;


Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui_settings(new Ui::Settings)
{
    ui_settings->setupUi(this);
    QString Home_Path = QDir::homePath();
    QFile f(Home_Path + "/.fs_settings"); // Read settings from file
    if (f.exists() && f.open(QIODevice::ReadOnly))
    {
        int j = 0;
        QStringList line;
        while(!f.atEnd())
        {
            j ++;
            line << f.readLine();
            if (j == 1) {
                std::string a = line[0].toStdString();
                if (a.find("[aresample]") != std::string::npos) {
                    ui_settings->comboBox_26->setCurrentIndex(0);
                };
                if (a.find("[soxr]") != std::string::npos) {
                    ui_settings->comboBox_26->setCurrentIndex(1);
                };
                if (a.find("[dither_triangular_hp]") != std::string::npos) {
                    ui_settings->comboBox_26->setCurrentIndex(2);
                };
            };
        };
        if (j == 0) {
            std::cout << "Settings file is empty!" << std::endl;
        };
        f.close();
    };
}

Settings::~Settings()
{
    delete ui_settings;
}

void Settings::on_pushButton_7_clicked() // Close settings window
{
    close();
}

void Settings::on_pushButton_8_clicked() // Reset settings
{
    ui_settings->comboBox_26->setCurrentIndex(0);
}

void Settings::on_pushButton_6_clicked() // Save settings
{
    int method_name = ui_settings->comboBox_26->currentIndex();
    QString Home_Path = QDir::homePath();
    QFile f(Home_Path + "/.fs_settings"); // Writing settings to file
    if (f.open(QIODevice::WriteOnly))
    {
        if (method_name == 0) {
            f.write("[aresample]\n");
        };
        if (method_name == 1) {
            f.write("[soxr]\n");
        };
        if (method_name == 2) {
            f.write("[dither_triangular_hp]\n");
        };
        f.close();
        close();
    } else {
        _message = "Settings file not found!\n";
        Taskcomplete taskcomplete;
        taskcomplete.setModal(true);
        taskcomplete.exec();
    };
}
