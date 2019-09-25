#ifndef CMDINPUTWIDGET_H
#define CMDINPUTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>


class CmdInputWidget : public QWidget
{
    Q_OBJECT
public:

    QColor lightColor; // a color near white

    QColor warmColor; // a color near yellow

    // button to Enter
    QPushButton *button;

    // editor for input
    QLineEdit *lineEdit;

    // set the bakcground color of line edit
    void setLineEditBgColor(QColor qColor);

    explicit CmdInputWidget(QWidget *parent = 0);

signals:

public slots:



};

#endif // CMDINPUTWIDGET_H
