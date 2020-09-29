#include <QProcess>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <math.h>
#include "extracting.h"
#include "ui_extracting.h"

QProcess *procedure_1 = new QProcess;
extern QString _message;
extern QString input_file;
extern QString folder_output;
extern QString preset;
bool calling_pr_1;
bool flag_no_extracting;
extern time_t loop_start;

Extracting::Extracting(QWidget *parent) :
    QDialog(parent),
    ui_extract(new Ui::Extracting)
{
    ui_extract->setupUi(this);
    flag_no_extracting = false;
    ui_extract->progressBar->setValue(0);
    try {
        disconnect(procedure_1, SIGNAL(finished(int)), this, SLOT(error_1()));
    }  catch (...) {
        std::cout << "Disconnect procedure_1 from error_1 false!" << std::endl;
    }
    try {
        disconnect(procedure_1, SIGNAL(finished(int)), this, SLOT(complete_extract()));
    }  catch (...) {
        std::cout << "Disconnect procedure_1 from complete_extract false!" << std::endl;
    }
    try {
        disconnect(procedure_1, SIGNAL(readyReadStandardOutput()), this, SLOT(progress_1()));
    }  catch (...) {
        std::cout << "Disconnect procedure_1 from progress_1 false!" << std::endl;
    }
    procedure_1->setProcessChannelMode(QProcess::MergedChannels);
    connect(procedure_1, SIGNAL(readyReadStandardOutput()), this, SLOT(progress_1()));
    calling_pr_1 = true;
    extracting();
}

Extracting::~Extracting()
{
    delete ui_extract;
}


void Extracting::extracting()
{
    connect(procedure_1, SIGNAL(finished(int)), this, SLOT(error_1()));
    loop_start = time (NULL);
    QStringList arguments;
    arguments << preset.split(" ") << input_file;
    procedure_1->setWorkingDirectory(folder_output);
    procedure_1->start("sacd_extract", arguments);
    if (!procedure_1->waitForStarted()) {
        std::cout << "cmd command not found!!!" << std::endl;
        disconnect(procedure_1, SIGNAL(finished(int)), this, SLOT(error_1()));
        _message = "An unknown error occurred!\n Possible 'sacd_extract' not installed.\n";
        flag_no_extracting = true;
        this->close();                                    // Exit on error from extracting to mainwinow //
    };
}

void Extracting::progress_1()
{
    QString line = procedure_1->readAllStandardOutput();
    QString line_mod = line.replace(": ", ":");
    int pos_st = line_mod.indexOf("Total:");
    if (pos_st != -1) {
          QString data = line_mod.split("Total:")[1];
          QString data_mod_str = data.split("%")[0];
          int data_mod = data_mod_str.toInt();
          time_t iter_start;
          iter_start = time (NULL);
          int timer = iter_start - loop_start;
          float full_time = (timer * 100) / (data_mod + 0.001);
          float rem_time = full_time - timer;
          if (rem_time < 0) {
              rem_time = 0;
          };
          int h = trunc(rem_time / 3600);
          int m = trunc((rem_time - (h * 3600)) / 60);
          int s = trunc(rem_time - (h * 3600) - (m * 60));
          int percent = data_mod;
          if (percent > 100) {
              percent = 100;
          };
          ui_extract->progressBar->setValue(percent);
          QString hrs = QString::number(h);
          QString min = QString::number(m);
          QString sec = QString::number(s);
          std::ostringstream sstr;
          sstr << std::setw(2) << std::setfill('0') << hrs.toStdString() << ":" << std::setw(2) << std::setfill('0') << min.toStdString() << ":" << std::setw(2) << std::setfill('0') << sec.toStdString();
          std::string tm = sstr.str();
          ui_extract->label_remaining->setText(QString::fromStdString(tm));
          if ((percent >= 50) && (calling_pr_1 == true)) {
              disconnect(procedure_1, SIGNAL(finished(int)), this, SLOT(error_1()));
              connect(procedure_1, SIGNAL(finished(int)), this, SLOT(complete_extract()));
              calling_pr_1 = false;
          };
    };
}

void Extracting::complete_extract()
{
    std::cout << "Extracting complete ..." << std::endl;
    disconnect(procedure_1, SIGNAL(finished(int)), this, SLOT(complete_extract()));
    _message = "Extracting completed!\n";
    flag_no_extracting = true;
    close();
}

void Extracting::error_1()
{
    std::cout << "Extracting error ..." << std::endl;
    disconnect(procedure_1, SIGNAL(finished(int)), this, SLOT(error_1()));
    _message = "An error occured!\nPossible reasons:\n - incorrect settings or error in the input file,\n - no enough space.";
    flag_no_extracting = true;
    close();
}
