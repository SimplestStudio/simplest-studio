#ifndef EXTRACTING_H
#define EXTRACTING_H

#include <QDialog>

namespace Ui {
class Extracting;
}

class Extracting : public QDialog
{
    Q_OBJECT

public:
    explicit Extracting(QWidget *parent = nullptr);
    ~Extracting();

private slots:
    void extracting();

    void progress_1();

    void complete_extract();

    void error_1();

private:
    Ui::Extracting *ui_extract;
};

#endif // EXTRACTING_H
