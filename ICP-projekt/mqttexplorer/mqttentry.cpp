#include "mqttentry.h"

MqttEntry::MqttEntry(QObject *parent) : QObject(parent)
{

}

QString MqttEntry::Msq_name() const
{
    return msq_name;
}

void MqttEntry::setMsq_name(const QString &value)
{
    if(msq_name!=value){
        msq_name = value;
        emit Msq_nameChanged();
    }
}

QString MqttEntry::Msq_topic() const
{
    return msq_topic;
}

void MqttEntry::setMsq_topic(const QString &value)
{
    if(msq_topic!=value){
        msq_topic = value;
        emit Msq_topicChanged();
    }
}

QDateTime MqttEntry::Msq_time() const
{
    return msq_time;
}

void MqttEntry::setMsq_time(const QDateTime &value)
{
    if (msq_time != value){
        msq_time = value;
        emit Msq_timeChanged();
    }
}

QByteArray MqttEntry::Msq_payload() const
{
    return msq_payload;
}

void MqttEntry::setMsq_payload(const QByteArray &value)
{
    if(msq_payload!=value){
        msq_payload = value;
        emit Msq_payloadChanged();
    }
}
