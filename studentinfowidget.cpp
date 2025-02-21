#include "studentinfowidget.h"
#include "ui_studentinfowidget.h"

StudentInfoWidget::StudentInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentInfoWidget)
{
    ui->setupUi(this);
}

StudentInfoWidget::~StudentInfoWidget()
{
    delete ui;
}
