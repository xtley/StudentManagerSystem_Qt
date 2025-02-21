#ifndef STUDENTINFOWIDGET_H
#define STUDENTINFOWIDGET_H

#include <QWidget>

namespace Ui {
class StudentInfoWidget;
}

class StudentInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StudentInfoWidget(QWidget *parent = nullptr);
    ~StudentInfoWidget();

private:
    Ui::StudentInfoWidget *ui;
};

#endif // STUDENTINFOWIDGET_H
