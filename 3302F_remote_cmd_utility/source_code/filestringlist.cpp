#include <QFile>
#include <QTextStream>

#include "filestringlist.h"

FileStringList::FileStringList()
{
}

// load strings from a text file
void FileStringList::loadFromFile(QString filename)
{
    QFile file(filename);

    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine(); // the 'r' and '\n' are removed.
        this->append(line);
    }

    file.close();
}

// save strings into a text file
void FileStringList::saveToFile(QString filename)
{
    QFile file(filename);

    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);

    while(this->size()!=0)
    {
        QString line = this->takeAt(0);//take and remove the item 0
        out << line << "\r\n";
    }

    file.close();
}
