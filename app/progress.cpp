#include <QProcess>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <math.h>
#include "progress.h"
#include "ui_progress.h"


QProcess *procedure_4 = new QProcess;
extern QString _message;
extern QString input_file;
extern QString output_file;
extern int dur_mod;
extern QString preset;
bool calling_pr_4;
bool flag_no_encoding;
time_t loop_start;

Progress::Progress(QWidget *parent) :
    QDialog(parent),
    ui_progress(new Ui::Progress)
{
    ui_progress->setupUi(this);
    flag_no_encoding = false;
    ui_progress->progressBar->setValue(0);
    try {
        disconnect(procedure_4, SIGNAL(finished(int)), this, SLOT(error_2()));
    }  catch (...) {
        std::cout << "Disconnect procedure_4 from error_2 false!" << std::endl;
    }
    try {
        disconnect(procedure_4, SIGNAL(finished(int)), this, SLOT(complete()));
    }  catch (...) {
        std::cout << "Disconnect procedure_4 from complete false!" << std::endl;
    }
    try {
        disconnect(procedure_4, SIGNAL(readyReadStandardOutput()), this, SLOT(progress_4()));
    }  catch (...) {
        std::cout << "Disconnect procedure_4 from progress_4 false!" << std::endl;
    }
    procedure_4->setProcessChannelMode(QProcess::MergedChannels);
    connect(procedure_4, SIGNAL(readyReadStandardOutput()), this, SLOT(progress_4()));
    calling_pr_4 = true;
    encoding();
}

Progress::~Progress()
{
    delete ui_progress;
}


void Progress::encoding()
{
    connect(procedure_4, SIGNAL(finished(int)), this, SLOT(error_2()));
    loop_start = time (NULL);
    QStringList arguments;
    arguments << "-hide_banner" << "-i" << input_file << preset.split(" ") << "-y" << output_file;
    procedure_4->start("ffmpeg", arguments);
    if (!procedure_4->waitForStarted()) {
        std::cout << "cmd command not found!!!" << std::endl;
        disconnect(procedure_4, SIGNAL(finished(int)), this, SLOT(error_2()));
        _message = "An unknown error occurred!\n Possible FFMPEG not installed.\n";
        flag_no_encoding = true;
        this->close();                                    // Exit on error from progress to mainwinow //
    };
}

void Progress::progress_4()
{
    QString line = procedure_4->readAllStandardOutput();
    QString line_mod = line.replace("   ", " ").replace("  ", " ").replace("  ", " ").replace("= ", "=");
    int pos_st = line_mod.indexOf("time=");
    if (pos_st != -1) {
          QString data = line_mod.split("time=")[1];
          QString data_mod = data.split(' ')[0];
          QStringList data_mod_2 = data_mod.split(':');
          int h_cur = data_mod_2[0].toInt();
          int m_cur = data_mod_2[1].toInt();
          int s_cur = data_mod_2[2].toFloat();
          float dur = h_cur * 3600 + m_cur * 60 + s_cur;
          time_t iter_start;
          iter_start = time (NULL);
          int timer = iter_start - loop_start;
          float full_time = (timer * dur_mod) / dur;
          float rem_time = full_time - timer;
          if (rem_time < 0) {
              rem_time = 0;
          };
          int h = trunc(rem_time / 3600);
          int m = trunc((rem_time - (h * 3600)) / 60);
          int s = trunc(rem_time - (h * 3600) - (m * 60));
          int percent = (dur * 100) / dur_mod;
          if (percent > 100) {
              percent = 100;
          };
          ui_progress->progressBar->setValue(percent);
          QString hrs = QString::number(h);
          QString min = QString::number(m);
          QString sec = QString::number(s);
          std::ostringstream sstr;
          sstr << std::setw(2) << std::setfill('0') << hrs.toStdString() << ":" << std::setw(2) << std::setfill('0') << min.toStdString() << ":" << std::setw(2) << std::setfill('0') << sec.toStdString();
          std::string tm = sstr.str();
          ui_progress->label_remaining->setText(QString::fromStdString(tm));
          if ((percent >= 50) && (calling_pr_4 == true)) {
              disconnect(procedure_4, SIGNAL(finished(int)), this, SLOT(error_2()));
              connect(procedure_4, SIGNAL(finished(int)), this, SLOT(complete()));
              calling_pr_4 = false;
          };
    };
}

void Progress::complete()
{
    std::cout << "Encoding complete ..." << std::endl;
    disconnect(procedure_4, SIGNAL(finished(int)), this, SLOT(complete()));
    _message = "Encoding completed!\n";
    flag_no_encoding = true;
    close();
}

void Progress::error_2()
{
    std::cout << "Encoding error ..." << std::endl;
    disconnect(procedure_4, SIGNAL(finished(int)), this, SLOT(error_2()));
    _message = "An error occured!\nPossible reasons:\n - incorrect settings or error in the input file,\n - no enough space.";
    flag_no_encoding = true;
    close();
}

