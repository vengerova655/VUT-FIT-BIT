#ifndef MQTTCONTROLLER_H
#define MQTTCONTROLLER_H

#include <QObject>
#include "mqtttopicbook.h"
//#include "mqttentry.h"

class MqttController : public QObject
{
    Q_OBJECT
public:
    explicit MqttController(MqttTopicBook *topicbook, QObject *parent = nullptr);

    MqttEntry *createEntry(QString *name);
    bool deleteEnntry( MqttEntry *entry);
signals:

private:
    MqttTopicBook *m_topicBook;
};

#endif // MQTTCONTROLLER_H
