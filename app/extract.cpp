#include <QProcess>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <math.h>

#include "extract.h"
#include "ui_extract.h"

QProcess *procedure_4 = new QProcess;
extern QString _message;
extern QString input_file;
extern QString output_file;
extern int dur_mod;
extern QString preset;
bool calling_pr_4;
bool flag_no_encoding;
//bool flag_no_extracting;
time_t loop_start;

extract::extract(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::extract)
{
    ui->setupUi(this);
}

extract::~extract()
{
    delete ui;
}
