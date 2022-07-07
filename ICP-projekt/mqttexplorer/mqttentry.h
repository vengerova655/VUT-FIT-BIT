#ifndef MQTTENTRY_H
#define MQTTENTRY_H

#include <QObject>
#include <QString>
#include <QDateTime>

class MqttEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString Msq_name READ Msq_name WRITE setMsq_name NOTIFY Msq_nameChanged)
    Q_PROPERTY(QString Msq_topic READ Msq_topic WRITE setMsq_topic NOTIFY Msq_topicChanged)
    Q_PROPERTY(QDateTime Msq_time READ Msq_time WRITE setMsq_time NOTIFY Msq_timeChanged)
    Q_PROPERTY(QByteArray Msq_payload READ Msq_payload WRITE setMsq_payload NOTIFY Msq_payloadChanged)
public:
    explicit MqttEntry(QObject *parent = nullptr);
//    ~MqttEntry();

    QString Msq_name() const;
    void setMsq_name(const QString &value);

    QString Msq_topic() const;
    void setMsq_topic(const QString &value);

    QDateTime Msq_time() const;
    void setMsq_time(const QDateTime &value);

    QByteArray Msq_payload() const;
    void setMsq_payload(const QByteArray &value);

signals:
    void Msq_nameChanged();
    void Msq_topicChanged();
    void Msq_timeChanged();
    void Msq_payloadChanged();

public slots:

private:
    QString msq_name;
    QString msq_topic;
    QDateTime msq_time;
    QByteArray msq_payload;


};

#endif // MQTTENTRY_H
