#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MqttTopicBook topic_book;
    MqttController controller(&topic_book);
    MainWindow w(&controller);
    w.show();
    return a.exec();
}
