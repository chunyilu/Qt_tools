#include "batchDialog.h"
#include "ui_batchDialog.h"

BatchDialog::BatchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    execTimer = new QTimer(this);
    // connect the slot when timeout
    connect(execTimer, SIGNAL(timeout()), this, SLOT(on_execTimer_TimeOut()));

    // widget for display message
    displayTextEdit = NULL;
}

BatchDialog::~BatchDialog()
{
    delete ui;
}

void BatchDialog::setCmdInputWidgets(QList<CmdInputWidget*> *asciiList,
                           QList<CmdInputWidget*> *binList)
{
    asciiCmdInputWidgetList = asciiList;
    binCmdInputWidgetList = binList;
}

void BatchDialog::on_startPushButton_clicked()
{
    int ms = ui->intervalLineEdit->text().toInt();

    // init timer parameters
    execIndex = 0;
    execLoop = 0;
    execTimer->start(ms);

    // toggle buttons
    ui->startPushButton->setEnabled(false);
    ui->stopPushButton->setEnabled(true);

    // show message
    if(displayTextEdit!=NULL)
    {
        displayTextEdit->appendPlainText("");
        displayTextEdit->appendPlainText("Script start...");

        displayTextEdit->appendPlainText("");
        displayTextEdit->appendPlainText("Loop " + QString::number(execLoop));
    }
}

void BatchDialog::on_stopPushButton_clicked()
{
    // stop timer
    execTimer->stop();

    // toggle buttons
    ui->startPushButton->setEnabled(true);
    ui->stopPushButton->setEnabled(false);

    // show message
    if(displayTextEdit!=NULL)
    {
        displayTextEdit->appendPlainText("");
        displayTextEdit->appendPlainText("Script stop!");
    }
}

void BatchDialog::on_execTimer_TimeOut()
{
    int cmdNum = 0, loopNum = 0;

    // get the command and loop parameters
    cmdNum = ui->cmdNumLineEdit->text().toInt();
    loopNum = ui->loopLineEdit->text().toInt();

    // click the button to execute current command
    if(!(*asciiCmdInputWidgetList)[0]->lineEdit->text().isEmpty())
    {
       (*asciiCmdInputWidgetList)[execIndex]->button->click();
    }
    else
    {
       (*binCmdInputWidgetList)[execIndex]->button->click();
    }

    execIndex++;

    // check to start next loop
    if(execIndex == cmdNum)
    {
       execIndex=0;
       execLoop++;
       if( execLoop!= loopNum)
       {
         displayTextEdit->appendPlainText("");
         displayTextEdit->appendPlainText("Loop " + QString::number(execLoop));
       }
    }

    // 顯示目前進度
    ui->loopInfoLlineEdit->setText("Loop " + QString::number(execLoop) +
                                   "-" + QString::number(execIndex));
    // finish all the loops
    if(execLoop == loopNum)
    {
      execTimer->stop();
      displayTextEdit->appendPlainText("");
      displayTextEdit->appendPlainText("Script End !");

      // toggle buttons
      ui->startPushButton->setEnabled(true);
      ui->stopPushButton->setEnabled(false);

      ui->loopInfoLlineEdit->setText("Loop N/A");
    }
}

//void BatchDialog::setDisplay(QTextEdit *dsplEdit)
void BatchDialog::setDisplay(QPlainTextEdit *dsplEdit)
{
    displayTextEdit = dsplEdit;
}
