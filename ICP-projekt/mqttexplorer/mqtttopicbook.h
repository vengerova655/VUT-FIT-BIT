#ifndef MQTTTOPICBOOK_H
#define MQTTTOPICBOOK_H

#include <QObject>
#include <QList>
#include "mqttentry.h"

class MqttTopicBook : public QObject
{
    Q_OBJECT
public:
    typedef QList<MqttEntry*> Entries;
    explicit MqttTopicBook(QObject *parent = nullptr);

    Entries entries() const;
    MqttEntry* createEntry();
    bool deleteEntry(MqttEntry *entry);



signals:

    void entryAdded(MqttEntry *entry);
    void entryRemoved(MqttEntry *entry);


public slots:

private:
    Entries m_entries;

};

#endif // MQTTTOPICBOOK_H
