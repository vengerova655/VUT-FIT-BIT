#include "mqtttopicbook.h"

MqttTopicBook::MqttTopicBook(QObject *parent) : QObject(parent)
{

}

MqttTopicBook::Entries MqttTopicBook::entries() const
{
    return m_entries;
}

MqttEntry *MqttTopicBook::createEntry()
{
    auto result = new MqttEntry(this);
    if(result != nullptr){
        m_entries.append(result);
        emit entryAdded(result);
    }
    return result;
}

bool MqttTopicBook::deleteEntry(MqttEntry *entry)
{
    if(m_entries.contains((entry))){
        emit entryRemoved(entry);
        m_entries.removeOne(entry);
        delete entry;
        return true;
    }
    return false;
}

