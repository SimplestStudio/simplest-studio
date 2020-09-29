#include "mainwindow.h"
#include "smartsettings.h"
#include "ui_smartsettings.h"
#include <iostream>

extern QString _combobox_23_value;
extern int sampling;
extern int bit_depth;
bool flag_no_smart;
int _combobox_24_index = 0;
int _combobox_25_index = 0;
int _combobox_26_index = 0;

SmartSettings::SmartSettings(QWidget *parent) :
    QDialog(parent),
    ui_smartsettings(new Ui::SmartSettings)
{
    ui_smartsettings->setupUi(this);
    flag_no_smart = false;
    if (_combobox_23_value == "FLAC")
    {
        ui_smartsettings->label_credits_16->show();
        ui_smartsettings->comboBox_26->show();
    } else {
        ui_smartsettings->label_credits_16->hide();
        ui_smartsettings->comboBox_26->hide();
    };
    change_optimize_for();
}


SmartSettings::~SmartSettings()
{
    delete ui_smartsettings;
}

void SmartSettings::on_pushButton_11_clicked() // close
{
    close();
}

void SmartSettings::on_pushButton_10_clicked() // execute_make_preset
{
    int content_type = ui_smartsettings->comboBox_24->currentIndex();
    int bit_depth_output = ui_smartsettings->comboBox_25->currentIndex();
    if (_combobox_23_value == "FLAC") {
        int compression = ui_smartsettings->comboBox_26->currentIndex();
        _combobox_24_index = content_type + 1;
        _combobox_26_index = bit_depth_output + 1;
        _combobox_25_index = compression + 1;
    };
    if (_combobox_23_value == "WAV") {
        _combobox_24_index = content_type + 1;
        _combobox_26_index = bit_depth_output + 1;
    };
    flag_no_smart = true;
    close();
}

void SmartSettings::on_comboBox_27_currentTextChanged() // change_optimize_for
{
    change_optimize_for();
}

void SmartSettings::on_checkBox_stateChanged() // change_optimize_for
{
    change_optimize_for();
}

void SmartSettings::change_optimize_for()
{
    QString optimize_for = ui_smartsettings->comboBox_27->currentText();
    if (sampling <= 8000) {
        ui_smartsettings->comboBox_24->setCurrentIndex(0);
    };
    if ((sampling <= 11025) && (sampling > 8000)) {
        ui_smartsettings->comboBox_24->setCurrentIndex(1);
    };
    if ((sampling <= 16000) && (sampling > 11025)) {
        ui_smartsettings->comboBox_24->setCurrentIndex(2);
    };
    if ((sampling <= 22050) && (sampling > 16000)) {
        ui_smartsettings->comboBox_24->setCurrentIndex(3);
    };
    if ((sampling <= 24000) && (sampling > 22050)) {
        ui_smartsettings->comboBox_24->setCurrentIndex(4);
    };
    if ((sampling <= 32000) && (sampling > 24000)) {
        ui_smartsettings->comboBox_24->setCurrentIndex(5);
    };
    if ((sampling <= 44100) && (sampling > 32000)) {
        ui_smartsettings->comboBox_24->setCurrentIndex(6);
    };
    if (optimize_for == "Listening in the car on high-quality equipment") {
        if (sampling > 44100) {
            ui_smartsettings->comboBox_24->setCurrentIndex(6);
        };
        ui_smartsettings->comboBox_25->setCurrentIndex(0);
    };
    if (optimize_for == "Listening at home on standard equipment") {
        if (sampling > 44100) {
            if (ui_smartsettings->checkBox->isChecked()) {
                ui_smartsettings->comboBox_24->setCurrentIndex(7);
            } else {
                ui_smartsettings->comboBox_24->setCurrentIndex(6);
            };
        };
        ui_smartsettings->comboBox_25->setCurrentIndex(0);
    };
    if (optimize_for == "Listening at home on high-quality equipment") {
        if (sampling > 44100) {
            ui_smartsettings->comboBox_24->setCurrentIndex(7);
            if (ui_smartsettings->checkBox->isChecked()) {
                if (bit_depth == 16) {
                    ui_smartsettings->comboBox_25->setCurrentIndex(0);
                };
                if (bit_depth >= 24) {
                    ui_smartsettings->comboBox_25->setCurrentIndex(1);
                };
            } else {
                ui_smartsettings->comboBox_25->setCurrentIndex(0);
            };
        };
    };
    if (optimize_for == "Studio quality") {
        if ((sampling <= 48000) && (sampling > 44100)) {
            ui_smartsettings->comboBox_24->setCurrentIndex(7);
        };
        if ((sampling <= 88200) && (sampling > 48000)) {
            ui_smartsettings->comboBox_24->setCurrentIndex(8);
        };
        if ((sampling <= 96000) && (sampling > 88200)) {
            ui_smartsettings->comboBox_24->setCurrentIndex(9);
        };
        if ((sampling <= 176400) && (sampling > 96000)) {
            ui_smartsettings->comboBox_24->setCurrentIndex(10);
        };
        if ((sampling <= 192000) && (sampling > 176400)) {
            ui_smartsettings->comboBox_24->setCurrentIndex(11);
        };
        if (bit_depth == 16) {
            ui_smartsettings->comboBox_25->setCurrentIndex(0);
        };
        if (bit_depth >= 24) {
            ui_smartsettings->comboBox_25->setCurrentIndex(1);
        };
    };
}


