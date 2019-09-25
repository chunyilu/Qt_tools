#include <QHBoxLayout>
#include "cmdinputwidget.h"

CmdInputWidget::CmdInputWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0, 0, 0, 0);

    //prepare a button at the right of this widget
    button= new QPushButton("Enter", this);
    //prepare a line edit at the left of this widget
    lineEdit = new QLineEdit(this);

    hLayout->addWidget(lineEdit);
    hLayout->addWidget(button);

    // init the internal color parameter
    lightColor.setRgb(255,255,241);
    warmColor.setRgb(255,255, 202);
}

void CmdInputWidget::setLineEditBgColor(QColor qColor)
{
    QPalette pal = lineEdit->palette();

    pal.setColor(QPalette::Base, qColor);
    lineEdit->setPalette(pal);
}



