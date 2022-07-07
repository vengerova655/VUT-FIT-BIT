#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mqttcontroller.h"
#include "mqttentry.h"
#include "mqtttopicbook.h"

#include <QMainWindow>
#include <QHash>
#include <QPushButton>
#include <QTreeWidgetItem>
#include "mqtt/async_client.h"
#include "mqtt/topic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(MqttController *controller, QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void createEntry();
    void deleteEntry();
    void editEntry();
    void saveEntry();
    void discardEntry();
    void resetEntry();
    void showTree();
    void showList();
    void subscribe();
    void addsubscribe();

private:
    Ui::MainWindow *ui;
    MqttController * m_controller;
    QHash<QListWidgetItem*,MqttEntry*> m_entryMap;
    void setupConnections();
    QTreeWidgetItem childroot(QTreeWidgetItem *root, QTreeWidgetItem *child);
//    void connectSubscribe();
};
#endif // MAINWINDOW_H
