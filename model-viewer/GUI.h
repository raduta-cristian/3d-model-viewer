#pragma once
#include "qwidget.h"
#include "qtableview.h"
#include <qlayout.h>
#include <qlabel.h>
#include <qslider.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <string>
#include <sstream>
#include <qframe.h>

using std::string;

class GUI :
    public QWidget
{
private:

    QLabel* selectText = new QLabel();
    QLabel* slider_val = new QLabel();
    QPushButton* viewBtn = new QPushButton("View");
    QPushButton* selectBtn = new QPushButton("Select File");
    QLabel* selectedFileLabel = new QLabel("                  ");

    QString filename;

    QString getFileName(string path) const;

public:
    bool toClose = true;

    GUI() : QWidget() {
        buildUI();
        connectUI();
    }

    void buildUI();
    void connectUI();

    string getFilePath() {
        return filename.toStdString();
    }
};

