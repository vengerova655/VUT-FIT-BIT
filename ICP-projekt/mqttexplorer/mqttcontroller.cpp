#include "mqttcontroller.h"

MqttController::MqttController(
        MqttTopicBook *topicbook, QObject *parent) :
    QObject(parent),
    m_topicBook(topicbook)
{
    Q_ASSERT(topicbook!= nullptr);
}

MqttEntry *MqttController::createEntry(QString *name)
{
    auto result = m_topicBook->createEntry();
    if(result!=nullptr){
        result->setMsq_name(*name);
    }
    return result;
}

bool MqttController::deleteEnntry(MqttEntry *entry)
{
    return m_topicBook->deleteEntry(entry);
}
