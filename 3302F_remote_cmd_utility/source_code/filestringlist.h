#ifndef FILESTRINGLIST_H
#define FILESTRINGLIST_H

#include <QString>
#include <QStringList>

class FileStringList : public QStringList
{
public:
    FileStringList();

    // load strings from a text file
    void loadFromFile(QString filename);

    // save strings into a text file
    void saveToFile(QString filename);

};

#endif // FILESTRINGLIST_H
