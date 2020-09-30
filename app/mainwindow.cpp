#define _UNICODE
#include <QFileDialog>
#include <iostream>
//#include <string>
#include <MediaInfo/MediaInfo.h>
//#include <libavcodec/avcodec.h>
//#include <FLAC/all.h>
//#include <FLAC++/all.h>
//#include <wavpack/wavpack.h>
//#include <lame/lame.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"
#include "progress.h"
#include "extracting.h"
#include "settings.h"
#include "smartsettings.h"
#include "taskcomplete.h"

using namespace MediaInfoLib;


QString _message = "";
QString input_file = "";
QString output_file = "";
QString folder_output = "";
QString fmt = "";
int sampling = 0;
int bit_depth = 0;
int bitrate = 0;
int dur_mod;
//int loop_start;
QString preset;
QString _combobox_23_value = "";
extern int _combobox_24_index;
extern int _combobox_25_index;
extern int _combobox_26_index;
extern bool flag_no_smart;
extern bool flag_no_encoding;
extern bool flag_no_extracting;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->resizeColumnToContents(1);
    ui->tableWidget->resizeColumnToContents(2);
    ui->tableWidget->resizeColumnToContents(3);
    ui->tableWidget->resizeColumnToContents(4);
    ui->tableWidget->resizeColumnToContents(5);
    ui->tableWidget->setRowCount(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_clicked()
{
    About about;
    about.setModal(true);
    about.exec();
}

void MainWindow::on_actionSettings_clicked()
{
    Settings settings;
    settings.setModal(true);
    settings.exec();
}

void MainWindow::on_pushButton_1_clicked()  // open_file
{
    QStringList fileName;
    try {
        fileName = QFileDialog::getOpenFileNames(this, tr("Open Files"), QDir::currentPath(), tr("Audio Files: *.mp3, *.flac, *.wav,"
                                                                                                            " *.ape, *.aac, *.ac3, *.dff, *.dts,"
                                                                                                            " *.ogg, *.m4a, *.mkv, *.iso (*.mp3"
                                                                                                            " *.flac *.wav *.ape *.aac *.ac3 *.dff"
                                                                                                            " *.dts *.ogg *.m4a *.mkv *.iso);;All files (*.*)"));
    }  catch (...) {
        _message = "One of files cannot be opened.";
        Taskcomplete taskcomplete;
        taskcomplete.setModal(true);
        taskcomplete.exec();
        return;
    };

    if (fileName.isEmpty())
    {
        return;

    } else {
        MediaInfo MI;
        int i = 1;
        int sep = 0;
        int count = fileName.size();
        while (i <= count) {
            int numRows = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(numRows + 1);
            QString file_qstr = fileName.at(i-1);
            std::wstring file_wstr = file_qstr.toStdWString();
            sep = file_wstr.rfind('/');
            std::wstring folder_input_str = file_wstr.substr(0, sep);
            std::wstring file_input_str = file_wstr.substr(sep+1);
            QString folder_input = QString::fromStdWString(folder_input_str);
            QString file_input = QString::fromStdWString(file_input_str);
            QTableWidgetItem *newItem_file = new QTableWidgetItem(file_input);
            ui->tableWidget->setItem(numRows, 0, newItem_file);
            QTableWidgetItem *newItem_folder = new QTableWidgetItem(folder_input);
            ui->tableWidget->setItem(numRows, 5, newItem_folder);
            MI.Open(file_wstr);
            std::wstring fmt_wstr = MI.Get(Stream_General, 0, L"Format");
            std::wstring sampling_wstr = MI.Get(Stream_Audio, 0, L"SamplingRate");
            std::wstring bit_depth_wstr = MI.Get(Stream_Audio, 0, L"BitDepth");
            std::wstring bitrate_wstr = MI.Get(Stream_Audio, 0, L"BitRate");
            QString fmt_ = QString::fromStdWString(fmt_wstr);
            QString sampling_ = QString::fromStdWString(sampling_wstr);
            QString bit_depth_ = QString::fromStdWString(bit_depth_wstr);
            int bitrate_int = 0;
            try {
                bitrate_int = std::stoi(bitrate_wstr);
            }  catch (...) {
                std::cout << "No bitrate info!" << std::endl;
            }
            QString bitrate_ = QString::number(bitrate_int/1000);
            QTableWidgetItem *newItem_fmt = new QTableWidgetItem(fmt_);
            ui->tableWidget->setItem(numRows, 1, newItem_fmt);
            QTableWidgetItem *newItem_sampling = new QTableWidgetItem(sampling_);
            ui->tableWidget->setItem(numRows, 2, newItem_sampling);
            QTableWidgetItem *newItem_bit_depth = new QTableWidgetItem(bit_depth_);
            ui->tableWidget->setItem(numRows, 3, newItem_bit_depth);
            QTableWidgetItem *newItem_bitrate = new QTableWidgetItem(bitrate_ + " kb/s");
            ui->tableWidget->setItem(numRows, 4, newItem_bitrate);
            i++;
        }
        MI.Close();
        ui->tableWidget->selectRow(0);
    };
}

void MainWindow::on_pushButton_2_clicked()  // save_file
{
    ui->lineEdit_2->clear();
    QString folder_save = QFileDialog::getExistingDirectory(this, tr("Select output folder"), QDir::currentPath());
    ui->lineEdit_2->setText(folder_save);
    ui->lineEdit_2->end(false);
}

void MainWindow::on_pushButton_3_clicked() // smart_detect
{
    QString file = "";
    QString file_without_ext = "";
    QString file_ext = "";
    QString prefix = "";
    QString cur_format = "";
    bool comp_in;
    bool comp_out;
    int row = ui->tableWidget->currentRow();
    if (row != -1) {
        input_file = ui->tableWidget->item(row, 5)->text() + "/" + ui->tableWidget->item(row, 0)->text();
    };
    folder_output = ui->lineEdit_2->text();
    if ((input_file == "") or (folder_output == "")) {
        _message = "Select first input and output files!";
        Taskcomplete taskcomplete;
        taskcomplete.setModal(true);
        taskcomplete.exec();
        return;
    };
    prefix = ui->comboBox_23->currentText().toLower();
    file = ui->tableWidget->item(row, 0)->text();
    std::wstring file_wstr = file.toStdWString();
    int sep = file_wstr.rfind('.');
    std::wstring file_without_ext_wstr = file_wstr.substr(0, sep);
    std::wstring file_ext_wstr = file_wstr.substr(sep + 1);
    file_without_ext = QString::fromStdWString(file_without_ext_wstr);
    file_ext = QString::fromStdWString(file_ext_wstr);
    output_file = folder_output + "/" + file_without_ext + "_optimized." + prefix;
    std::cout << "Output file: " << output_file.toStdString() << std::endl;         // Output file debug information //
    cur_format = ui->comboBox_23->currentText();
    comp_out = (cur_format == "FLAC") or (cur_format == "WAV");
    comp_in = (file_ext != "flac") && (file_ext != "wav") && (file_ext != "dff");
    if (comp_out && comp_in) {
        _message = "Only lossless files are supported for converting to FLAC or WAV format.";
        Taskcomplete taskcomplete;
        taskcomplete.setModal(true);
        taskcomplete.exec();
        return;
    };
    if ((ui->comboBox_24->currentText() == "Smart mode") or (ui->comboBox_25->currentText() == "Smart mode")
            or (ui->comboBox_26->currentText() == "Smart mode"))
    {
        fmt = ui->tableWidget->item(row, 1)->text();
        QString sampling_ = ui->tableWidget->item(row, 2)->text();
        sampling = sampling_.toInt();
        QString bit_depth_ = ui->tableWidget->item(row, 3)->text();
        bit_depth = bit_depth_.toInt();
        QString bitrate_ = ui->tableWidget->item(row, 4)->text().remove(" kb/s");
        bitrate = bitrate_.toInt();
        // Debugging information
        std::cout << "Format: " << fmt.toStdString() << std::endl;
        std::cout << "Sampling: " << sampling << std::endl;
        std::cout << "Bit depth: " << bit_depth << std::endl;
        std::cout << "Bitrate: " << bitrate << std::endl;
        //
        if ((fmt == "") or (sampling == 0) or (bit_depth == 0))
        {
            _message = "It is impossible to determine the settings for the Smart mode."
                        " Select the settings manually.";
            Taskcomplete taskcomplete;
            taskcomplete.setModal(true);
            taskcomplete.exec();
            return;
        } else {
            _combobox_23_value = ui->comboBox_23->currentText();
            SmartSettings smartsettings;
            smartsettings.setModal(true);
            smartsettings.exec();                                   // Calling smartsettings and wait for return //
            if (flag_no_smart == false) {
                return;
            };
            std::cout << "_combobox_24_index: " << _combobox_24_index << std::endl;         // Debug information //
            std::cout << "_combobox_25_index: " << _combobox_25_index << std::endl;
            std::cout << "_combobox_26_index: " << _combobox_26_index << std::endl;
            ui->comboBox_24->setCurrentIndex(_combobox_24_index);   // Continue on return from smartsettings //
            ui->comboBox_26->setCurrentIndex(_combobox_26_index);
            ui->comboBox_25->setCurrentIndex(_combobox_25_index);
            make_preset();
        };
    } else {
        make_preset();
    };
}

void MainWindow::on_pushButton_6_clicked() // remove_file
{
    int row = ui->tableWidget->currentRow();
    if (row != -1) {
        ui->tableWidget->removeRow(row);
    };
}

void MainWindow::on_comboBox_23_currentTextChanged() // settings_menu
{
    ui->comboBox_26->clear();
    ui->comboBox_25->clear();
    ui->comboBox_24->clear();
    ui->label_credits_13->clear();
    ui->comboBox_24->setEnabled(true);
    ui->comboBox_25->setEnabled(true);
    ui->comboBox_26->setEnabled(true);

    if (ui->comboBox_23->currentText() == "FLAC") {
        ui->comboBox_26->addItems({"Smart mode", "16 bits", "24 bits"});
        ui->comboBox_26->setCurrentIndex(0);
        ui->comboBox_25->addItems({"Smart mode", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1", "0"});
        ui->comboBox_25->setCurrentIndex(0);
        ui->comboBox_24->addItems({"Smart mode", "8000", "11025", "16000", "22050", "24000", "32000", "44100", "48000", "88200", "96000", "176400", "192000"});
        ui->comboBox_24->setCurrentIndex(0);
        ui->label_credits_13->setText("Compress level:");
    };
    if (ui->comboBox_23->currentText() == "MP3") {
        ui->comboBox_26->addItems({"Auto"});
        ui->comboBox_26->setCurrentIndex(0);
        ui->comboBox_25->addItems({"320k", "256k", "224k", "192k", "160k", "128k", "112k", "96k", "80k", "64k", "48k", "40k", "32k", "24k", "16k", "8k"});
        ui->comboBox_25->setCurrentIndex(3);
        ui->comboBox_24->addItems({"8000", "11025", "12000", "16000", "22050", "24000", "32000", "44100", "48000"});
        ui->comboBox_24->setCurrentIndex(7);
        ui->label_credits_13->setText("Bitrate:");
        ui->comboBox_26->setEnabled(false);
    };
    if (ui->comboBox_23->currentText() == "WAV") {
        ui->comboBox_26->addItems({"Smart mode", "16 bits", "24 bits"});
        ui->comboBox_26->setCurrentIndex(0);
        ui->comboBox_25->addItems({"Auto"});
        ui->comboBox_25->setCurrentIndex(0);
        ui->comboBox_24->addItems({"Smart mode", "8000", "11025", "16000", "22050", "24000", "32000", "44100", "48000", "88200", "96000", "176400", "192000"});
        ui->comboBox_24->setCurrentIndex(0);
        ui->label_credits_13->setText("Bitrate:");
        ui->comboBox_25->setEnabled(false);
    };
    if (ui->comboBox_23->currentText() == "DFF") {
        ui->comboBox_26->addItems({"1 bit"});
        ui->comboBox_26->setCurrentIndex(0);
        ui->comboBox_25->addItems({"2 ch", "5.1 ch"});
        ui->comboBox_25->setCurrentIndex(0);
        ui->comboBox_24->addItems({"2822400"});
        ui->comboBox_24->setCurrentIndex(0);
        ui->label_credits_13->setText("Channels:");
        ui->comboBox_24->setEnabled(false);
        ui->comboBox_26->setEnabled(false);
    };
}

void MainWindow::make_preset() // make_preset
{
    QString sampling_rate = ui->comboBox_24->currentText();
    QString abitrate = ui->comboBox_25->currentText();
    QString compression_level = ui->comboBox_25->currentText();
    QString channels = ui->comboBox_25->currentText();
    QString sample_fmt = "";
    QString method = "aresample";
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
                    method = "aresample";
                };
                if (a.find("[soxr]") != std::string::npos) {
                    method = "soxr";
                };
                if (a.find("[dither_triangular_hp]") != std::string::npos) {
                    method = "dither triangular hp";
                };
            };
        };
        if (j == 0) {
            std::cout << "Settings file is empty!" << std::endl; // Debug information //
        };
        f.close();
    };

    if (ui->comboBox_26->currentText() == "16 bits") {
        sample_fmt = "s16";
    };
    if (ui->comboBox_26->currentText() == "24 bits") {
        sample_fmt = "s24";
    };
    if ((ui->comboBox_26->currentText() == "24 bits") && (ui->comboBox_23->currentText() == "FLAC")) {
        sample_fmt = "s32";
    };

    if ((ui->comboBox_23->currentText() == "FLAC") && (method == "aresample")) {
        preset = QString("-map 0:a:0 -vn -c:a flac -compression_level %1 -af aresample=out_sample_fmt=%2:out_sample_rate=%3").arg(compression_level).arg(sample_fmt).arg(sampling_rate);
        encode_file();
    };
    if ((ui->comboBox_23->currentText() == "FLAC") && (method == "soxr")) {
        preset = QString("-map 0:a:0 -vn -c:a flac -compression_level %1 -resampler soxr -sample_fmt %2 -ar %3").arg(compression_level).arg(sample_fmt).arg(sampling_rate);
        encode_file();
    };
    if ((ui->comboBox_23->currentText() == "FLAC") && (method == "dither triangular hp")) {
        preset = QString("-map 0:a:0 -vn -c:a flac -compression_level %1 -dither_method triangular_hp -sample_fmt %2 -ar %3").arg(compression_level).arg(sample_fmt).arg(sampling_rate);
        encode_file();
    };
    if (ui->comboBox_23->currentText() == "MP3") {
        preset = QString("-map 0:a:0 -vn -c:a libmp3lame -ar %1 -b:a %2").arg(sampling_rate).arg(abitrate);
        encode_file();
    };
    if ((ui->comboBox_23->currentText() == "WAV") && (sample_fmt == "s16")) {
        preset = QString("-map 0:a:0 -vn -bitexact -c:a pcm_s16le -ar %1").arg(sampling_rate);
        encode_file();
    };
    if ((ui->comboBox_23->currentText() == "WAV") && (sample_fmt == "s24")) {
        preset = QString("-map 0:a:0 -vn -bitexact -c:a pcm_s24le -ar %1").arg(sampling_rate);
        encode_file();
    };
    if ((ui->comboBox_23->currentText() == "DFF") && (channels == "2 ch")) {
        preset = "sacd_extract -2 -p -c -i";
        extract_file();
    };
    if ((ui->comboBox_23->currentText() == "DFF") && (channels == "5.1 ch")) {
        preset = "sacd_extract -m -p -c -i";
        extract_file();
    };
}

void MainWindow::extract_file() // extract_file
{
    std::cout << "Extract file ..." << std::endl;
    std::cout << "Preset: " << preset.toStdString() << std::endl;
    ui->pushButton_1->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    Extracting extracting;
    extracting.setModal(true);
    extracting.exec();                    // Call progress and wait for return //
    ui->pushButton_1->setEnabled(true); // Continue on return from progress //
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    int row = ui->tableWidget->currentRow();
    int numRows = ui->tableWidget->rowCount();
    if (numRows > (row + 1)) {
        ui->tableWidget->selectRow(row + 1);
    };
    if (flag_no_extracting == false) {
      _message = "Extracting aborted!";
    };
    Taskcomplete taskcomplete;
    taskcomplete.setModal(true);
    taskcomplete.exec();
}

void MainWindow::encode_file() // encode_file
{
    std::cout << "Encode file ..." << std::endl;
    std::cout << "Preset: " << preset.toStdString() << std::endl;
    int dur;
    ui->pushButton_1->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    MediaInfo MI;
    std::wstring input_file_wstr = input_file.toStdWString();
    MI.Open(input_file_wstr);
    std::wstring dur_wstr = MI.Get(Stream_General, 0, L"Duration");
    MI.Close();
    try {
        dur = std::stoi(dur_wstr);    
    }  catch (...) {
        std::cout << "No duration info!" << std::endl;
        ui->pushButton_1->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        _message = "Select correct input file!";
        Taskcomplete taskcomplete;
        taskcomplete.setModal(true);
        taskcomplete.exec();
        return;
    };
    dur_mod = dur/1000;
    std::cout << "Duration: " << dur_mod << std::endl;
    Progress progress;
    progress.setModal(true);
    progress.exec();                    // Call progress and wait for return //
    ui->pushButton_1->setEnabled(true); // Continue on return from progress //
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    int row = ui->tableWidget->currentRow();
    int numRows = ui->tableWidget->rowCount();
    if (numRows > (row + 1)) {
        ui->tableWidget->selectRow(row + 1);
    };
    if (ui->comboBox_23->currentText() == "FLAC") {
        ui->comboBox_24->setCurrentIndex(0);
        ui->comboBox_26->setCurrentIndex(0);
        ui->comboBox_25->setCurrentIndex(0);
    };
    if (ui->comboBox_23->currentText() == "WAV") {
        ui->comboBox_24->setCurrentIndex(0);
        ui->comboBox_26->setCurrentIndex(0);
    };
    if (flag_no_encoding == false) {
      _message = "Encoding aborted!";
    };
    Taskcomplete taskcomplete;
    taskcomplete.setModal(true);
    taskcomplete.exec();
}

