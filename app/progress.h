#ifndef PROGRESS_H
#define PROGRESS_H

#include <QDialog>

namespace Ui {
class Progress;
}

class Progress : public QDialog
{
    Q_OBJECT

public:
    explicit Progress(QWidget *parent = nullptr);
    ~Progress();

private slots:
    void encoding();

    void progress_4();

    void complete();

    void error_2();

private:
    Ui::Progress *ui_progress;
};


#endif // PROGRESS_H
