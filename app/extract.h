#ifndef EXTRACT_H
#define EXTRACT_H

#include <QDialog>

namespace Ui {
class extract;
}

class extract : public QDialog
{
    Q_OBJECT

public:
    explicit extract(QWidget *parent = nullptr);
    ~extract();

private:
    Ui::extract *ui;
};

#endif // EXTRACT_H
