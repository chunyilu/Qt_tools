#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QDialog>
#include <QList>
#include <QTimer>
#include <QTextEdit>
#include <QPlainTextEdit> //QCOM_Joe_0004
#include "cmdinputwidget.h"

namespace Ui {
class Dialog;
}

class BatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatchDialog(QWidget *parent = 0);
    ~BatchDialog();

    // mount the ascii and bin widgets of CmdInputWidget
    void setCmdInputWidgets(QList<CmdInputWidget*> *asciiList,
                               QList<CmdInputWidget*> *binList);

    // set a display widget for showing message of this dialog
    void setDisplay(QPlainTextEdit *dsplEdit);

private slots:
    void on_startPushButton_clicked();

    void on_stopPushButton_clicked();

    void on_execTimer_TimeOut();

private:
    Ui::Dialog *ui;

    // an textedit to display any message of this dialog
    QPlainTextEdit *displayTextEdit;

    // List to backup the pointer of CmdInputWidget objects.
    QList<CmdInputWidget*> *asciiCmdInputWidgetList;
    QList<CmdInputWidget*> *binCmdInputWidgetList;

    // a timer for batch execution
    QTimer *execTimer;

    // the index that command executed in a loop
    int execIndex;

    // the loop num that the command executed
    int execLoop;
};

#endif // BATCHDIALOG_H
