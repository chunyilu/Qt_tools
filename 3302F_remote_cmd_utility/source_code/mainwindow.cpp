#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmdinputwidget.h"
#include "filestringlist.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QSerialPortInfo>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QTextBrowser>
#include <QDateTime>


bool MainWindow::CheckRS232Port(void)
{
    int index = 0;
    bool ret = false;

    index = ui->comComboBox->currentIndex();

    if(index == 0)
    {
        ui->plainTextEdit_ascii->appendPlainText("PLEASE OPEN RS232 PORT!!!!");

        ret = false;
    }
    else if(serialPort->isOpen() == false)
    {
        ui->plainTextEdit_ascii->appendPlainText("OPEN RS232 PORT FAIL!!!!");

        ret = false;
    }
    else
    {
        ui->plainTextEdit_ascii->appendPlainText("Send command:");

        ret = true;
    }

    return ret;
}

void MainWindow::PrintSystemTime(void)
{
    int hour, min, sec, msec;
    QDateTime dt = QDateTime::currentDateTime();

    hour = dt.time().hour();
    min = dt.time().minute();
    sec = dt.time().second();
    msec = dt.time().msec();

    ui->plainTextEdit_ascii->appendPlainText(QString::number(hour)+':'+QString::number(min)+':'+QString::number(sec)+':'+QString::number(msec));
}

//QCOM_Joe_0003, add readAll() to read all bytes from UART
// occured when comport ready to be read
void MainWindow::handleReadyRead()
{
    char buf[16];
    QString text;
    char c;

    //Must check this for respond command!!!
    QByteArray data = serialPort->readAll();

    int len = data.size();

    //20150601, move cursor to the end and print data
    ui->plainTextEdit_ascii->moveCursor(QTextCursor::End);
    ui->plainTextEdit_ascii->appendPlainText(data);
    ui->plainTextEdit_ascii->moveCursor(QTextCursor::End);

    text+=QString("0x");

    snprintf(buf, 16, "%08x: ", Hex_Num);

    text+=buf;

    // put bytes into inputstream
    for(int i=0;i<len;i++)
    {
        c = data.at(i);

        sprintf(buf,"%02x ", c&0xFF);
        buf[4]='\0';
        text+=buf;
    }

    Hex_Num = Hex_Num+16;

    text.clear();

    //print system time when user check this function
    if(ui->checkBox_EnableTimeLog->isChecked())
    {
        PrintSystemTime();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Hex_Num = 0;//Initial variable

    // prepare a layout to manage the cmdInputWidgets of ascii types
    QVBoxLayout *vAsciiLayout = new QVBoxLayout(ui->asciiScrollAreaWidgetContents);
    vAsciiLayout->setSpacing(1);
    vAsciiLayout->setContentsMargins(1, 1, 1,1);
    vAsciiLayout->setSpacing(0);
    vAsciiLayout->setContentsMargins(0, 0, 0, 0);

    // Prepare all the cmdInputWidgets
    for(int i=0;i<COMMAND_NUM;i++)
    {
        QString s="Enter ";
        CmdInputWidget *cmdIptWidget = new CmdInputWidget(ui->asciiScrollAreaWidgetContents);

        s = s + QString::number(i+1);
        cmdIptWidget->button->setText(s);

        // set the background color of line edit
        if((i%2)==0)
        {
            cmdIptWidget->setLineEditBgColor(cmdIptWidget->lightColor);
        }
        else
        {
           cmdIptWidget->setLineEditBgColor(cmdIptWidget->warmColor);
        }


        vAsciiLayout->addWidget(cmdIptWidget);

        // connect the button clicked signal
        connect(cmdIptWidget->button, SIGNAL(clicked()), this, SLOT(on_asciiButton_Clicked()));

        // backup the widgets into a list
        asciiCmdInputWidgetList.append(cmdIptWidget);
    }

    // list all the comport avalible
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->comComboBox->addItem(serialPortInfo.portName());
    }

    // create comport object
    serialPort = new QSerialPort();

    // Dialog for batch execution
    batchDialog = new BatchDialog(this);
    batchDialog->setDisplay(ui->plainTextEdit_ascii);

    execTimer = new QTimer(this);
    // connect the slot when timeout
    connect(execTimer, SIGNAL(timeout()), this, SLOT(on_execTimer_TimeOut()));

}

MainWindow::~MainWindow()
{
    if(serialPort !=NULL)
    {
        serialPort->close();
        delete serialPort;
    }

    delete ui;
}

// The [clean] function in menu is clicked
void MainWindow::on_actionClean_triggered()
{
    ui->plainTextEdit_ascii->clear();
}

void MainWindow::on_comComboBox_currentIndexChanged(int index)
{
    QString s = ui->comComboBox->currentText();
    int buadrateIndex = 0;
    QString baudrateString = "";

    // the index is designed mapping to enum QSerialPort::BaudRate
    buadrateIndex = ui->baudrateComboBox->currentIndex();
    baudrateString = ui->baudrateComboBox->currentText();

    //171214, Chunyi add other property index
    int flow_ctl_index = ui->comboBox_flow_ctl->currentIndex();
    int stop_bits_index = ui->comboBox_stop_bits->currentIndex();
    int parity_index = ui->comboBox_parity->currentIndex();
    int data_bits_index = ui->comboBox_data_bits->currentIndex();

    if(index !=0)
    {
        // open com port
        // 1.open port
        serialPort->setPortName(s);
        serialPort->open(QIODevice::ReadWrite);
        if (serialPort->isOpen())
        {
            // 2.  set properties
            switch(buadrateIndex)
            {
                case 0:
                    serialPort->setBaudRate(QSerialPort::Baud1200);
                break;
                case 1:
                    serialPort->setBaudRate(QSerialPort::Baud2400);
                break;
                case 2:
                    serialPort->setBaudRate(QSerialPort::Baud4800);
                break;
                case 3:
                    serialPort->setBaudRate(QSerialPort::Baud9600);
                break;
                case 4:
                    serialPort->setBaudRate(QSerialPort::Baud19200);
                break;
                case 5:
                    serialPort->setBaudRate(QSerialPort::Baud38400);
                break;
                case 6:
                    serialPort->setBaudRate(QSerialPort::Baud57600);
                break;
                case 7:
                    serialPort->setBaudRate(QSerialPort::Baud115200);//3302F
                break;
                default:
                    //do nothing
                break;
            }
//            serialPort->setStopBits(QSerialPort::OneStop);
            switch (stop_bits_index)
            {
                case 0:
                    serialPort->setStopBits(QSerialPort::OneStop);//3302F
                    break;
                case 1:
                    serialPort->setStopBits(QSerialPort::OneAndHalfStop);
                    break;
                case 2:
                    serialPort->setStopBits(QSerialPort::TwoStop);
                    break;
                default:
                    break;
            }
//            serialPort->setParity(QSerialPort::NoParity);
            switch (parity_index)
            {
                case 0:
                    serialPort->setParity(QSerialPort::NoParity);//3302F
                    break;
                case 1:
                    serialPort->setParity(QSerialPort::EvenParity);
                    break;
                case 2:
                    serialPort->setParity(QSerialPort::OddParity);
                    break;
                case 3:
                    serialPort->setParity(QSerialPort::SpaceParity);
                    break;
                case 4:
                    serialPort->setParity(QSerialPort::MarkParity);
                    break;
                default:
                    break;
            }
//            serialPort->setDataBits(QSerialPort::Data8);
            switch (data_bits_index)
            {
                case 0:
                    serialPort->setDataBits(QSerialPort::Data5);
                    break;
                case 1:
                    serialPort->setDataBits(QSerialPort::Data6);
                    break;
                case 2:
                    serialPort->setDataBits(QSerialPort::Data7);
                    break;
                case 3:
                    serialPort->setDataBits(QSerialPort::Data8);//3302F
                    break;
                default:
                    break;
            }

            switch (flow_ctl_index)
            {
                case 0:
                    serialPort->setFlowControl(QSerialPort::NoFlowControl);
                    break;
                case 1:
                    serialPort->setFlowControl(QSerialPort::HardwareControl); //3302F
                    break;
                case 2:
                    serialPort->setFlowControl(QSerialPort::SoftwareControl);
                    break;
                default:
                    //do nothing
                    break;
            }

            connect(serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));

            ui->plainTextEdit_ascii->appendPlainText(s + ", " + baudrateString + " opened.");
        }
        else
        {
            ui->plainTextEdit_ascii->appendPlainText(s + ", " + baudrateString + "could not be opened.");
        }
    }
    else
    {
        // close com port
        ui->plainTextEdit_ascii->appendPlainText("RS232 closed !");

        serialPort->close();
    }
}



void MainWindow::on_actionOpen_Command_List_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open command list"),
                                                    ".", tr("*.txt"));

    if(filename.isEmpty())
    {
        return;
    }

    FileStringList fsl;
    fsl.loadFromFile(filename);

    // extract ascii cmd
    int childIndex = 0;
    for(int i=0;i<fsl.size();i++)
    {
        QString cmdStr = fsl[i];
        if(cmdStr.startsWith("cmd="))
        {
            CmdInputWidget *childCIW;

            childCIW = asciiCmdInputWidgetList[childIndex];
            cmdStr = cmdStr.right(cmdStr.size() - 4);
            childCIW->lineEdit->setText(cmdStr);
            childIndex++;
        }
    }
}

void MainWindow::on_actionSave_Command_List_2_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save command list"),
                                                    ".", tr("*.txt"));

    FileStringList fsl;

    // save ascii commands
    for(int i=0;i<asciiCmdInputWidgetList.size();i++)
    {
        fsl.append("cmd=" + asciiCmdInputWidgetList[i]->lineEdit->text());
    }

    // save command file
    fsl.saveToFile(filename);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_log_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save ASCII log File"),
                                                    "d://ASCII_Log.txt",
                                                    tr("Files (*.txt)"));

    QFile file_node(filename);
    QStringList Line;
    QTextStream stream(&file_node);

    file_node.open(QIODevice::WriteOnly);

    QString str = ui->plainTextEdit_ascii->toPlainText();

    Line = QStringList(str);

    for ( QStringList::iterator it = Line.begin(); it != Line.end(); ++it )
    {
        stream << *it << "\n";
    }

    file_node.close();
}

// QCOM_Joe_0002, 20150126, add send command
// Send RS232 command in ascii string
void MainWindow::sendAsciiCommand(QString asciiCmd)
{
    unsigned char cmdKey[asciiCmd.length()];

    for(int i=0; i<asciiCmd.length(); i++)
    {
        cmdKey[i] = asciiCmd[i].toLatin1();
    }

    ui->plainTextEdit_ascii->appendPlainText(asciiCmd);

    // send command
    serialPort->write((char*)cmdKey, asciiCmd.length());

    //wait for the response bytes pushed into queue externally
//    sleep_ms(1000);
}

// slot for ascii command button clicked
void MainWindow::on_asciiButton_Clicked()
{

    if (false == CheckRS232Port())
    {
        return;
    }

    // get the button that send signal to this slot
    QPushButton *btn = (QPushButton*)this->sender();

    // get the related lineEdit and get it's text
    QLineEdit *edit = ((CmdInputWidget*)btn->parent())->lineEdit;

    sendAsciiCommand(edit->text());
}

// batch execute the commands
void MainWindow::on_actionBatchExec_triggered()
{
    batchDialog->show();
}

void MainWindow::on_toolButton_clicked()
{
    ui->plainTextEdit_ascii->clear();
}

//170920, chunyi, the auto save function is not ready
void MainWindow::on_actionAuto_save_log_file_triggered(bool checked)
{
    if(checked)
    {
        myDateTime = new QDateTime(QDateTime::currentDateTime());
        //get current start time
//        qDebug()<<"date = "<<myDateTime->date().toString("yy:MM:dd");
//        qDebug()<<"time = "<<myDateTime->time().toString("hh:mm:ss");

        ui->actionAuto_save_log_file->setIcon(QIcon(QPixmap(":/new/prefix1/checkmark-20.png")));
//        qDebug()<<"on_actionAuto_save_log_file_triggered checked == true";
        ui->plainTextEdit_ascii->appendPlainText("log to C://QtComShell_log//ASCII_Log_date_time.txt every 5 secs");

        execTimer->start(5000);
    }
    else
    {
        ui->actionAuto_save_log_file->setIcon(QIcon());
//        qDebug()<<"on_actionAuto_save_log_file_triggered checked == false";

        execTimer->stop();
    }
}

//exec this timer to auto save log file
void MainWindow::on_execTimer_TimeOut()
{
//    qDebug()<<"exec timer";
//    ui->plainTextEdit_ascii->appendPlainText("exec timer");

    //create a folder
    QDir().mkdir("C://QtComShell_log");

    //create a file
    QFile file("C://QtComShell//ASCII_Log_" + myDateTime->date().toString("yy_MM_dd_") + myDateTime->time().toString("hh_mm_ss") + ".txt");
    file.open(QIODevice::WriteOnly);

    QTextStream stream(&file);

    QString str = ui->plainTextEdit_ascii->toPlainText();

    QStringList Line = QStringList(str);

    for ( QStringList::iterator it = Line.begin(); it != Line.end(); ++it )
    {
        stream << *it << "\n";
    }

    file.close();
}

//20150601, add refresh function
void MainWindow::on_toolButton_refresh_clicked()
{
    ui->plainTextEdit_ascii->appendPlainText("Refresh com ports.");

    ui->comComboBox->blockSignals(true); //do not send signals from ui->comComboBox

    ui->comComboBox->clear();

    ui->comComboBox->addItem("CLOSE");

    // list all the comport avalible
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->comComboBox->addItem(serialPortInfo.portName());
    }

    ui->comComboBox->blockSignals(false); //enable ui->comComboBox send signal
}


