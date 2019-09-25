#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QList>
#include <QTime>
#include <QCoreApplication>

#include "cmdinputwidget.h"
#include "batchDialog.h"

namespace Ui {
class MainWindow;
class Search_Window;
}

#define COMMAND_NUM     256  // define the number of command lines


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSerialPort *serialPort; // a comport for working

    unsigned int Hex_Num;

private slots:
    void on_execTimer_TimeOut(); //170920

    void on_actionClean_triggered();

    void on_comComboBox_currentIndexChanged(int index);

    void on_actionOpen_Command_List_triggered();

    void on_actionSave_Command_List_2_triggered();

    void on_actionExit_triggered();

    void on_action_log_triggered();

    // slot for ascii command button clicked
    void on_asciiButton_Clicked();

    void on_actionBatchExec_triggered();

    void handleReadyRead();

    void PrintSystemTime();

    void on_toolButton_clicked();

    void on_actionAuto_save_log_file_triggered(bool checked);

    bool CheckRS232Port();

    void on_toolButton_refresh_clicked();


private:
    QDateTime *myDateTime;
    // a timer for batch execution
    QTimer *execTimer;

    QFile file_node;

    Ui::MainWindow *ui;

    // pointer of dioloag for batch execution
    BatchDialog *batchDialog;

    // List to backup the pointer of CmdInputWidget objects.
    QList<CmdInputWidget*> asciiCmdInputWidgetList;

    // Send RS232 command in ascii string
    void sendAsciiCommand(QString asciiCmd);

    // sleep in ms
    void sleep_ms(int ms)
    {
        QTime t;
        t.start();
        while(t.elapsed()<ms)
        {
            QCoreApplication::processEvents(); // other process can run thread.
        }
    }

    QDateTime AutoSaveStartTime;

    //--------------
    //ASCII table
    //Decimal  Char
    //48       0
    //49       1
    //50       2
    //51       3
    //52       4
    //53       5
    //54       6
    //55       7
    //56       8
    //57       9
    //65       A
    //66       B
    //67       C
    //68       D
    //69       E
    //70       F
    //--------------
    //para:
    //first_char = decimal number for first character in ASCII table
    //second_char = decimal number for second character in ASCII table
    unsigned char StrToHex(int first_char, int second_char)
    {
        int n1,n2;
        unsigned char temp_byte;

        if(first_char>=48 && first_char<=57)//0 ~ 9
        {
            n1 = first_char-48;
        }
        else//A ~ F
        {
            n1 = first_char-55;
        }

        if(second_char>=48 && second_char<=57)//0 ~ 9
        {
            n2 = second_char-48;
        }
        else//A ~ F
        {
            n2 = second_char-55;
        }

        temp_byte = n1*16+n2;

        return temp_byte;
    }

};

#endif // MAINWINDOW_H
