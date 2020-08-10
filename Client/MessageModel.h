#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <vector>
#include <QDateTime>

class MessageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct Message {
        QDateTime m_time;
        QString m_message;
        bool post; // true - own message

        Message() = default;
        Message(QDateTime time, QString messageText, bool post);
        Message(QString messageText, bool post);
    };

    static void registerModel(const std::string& moduleName);

    void add(const Message& mess);
public:
    MessageModel();
    ~MessageModel();

    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;


private:
    std::vector<Message> m_messages;

    enum MessagesRoles {
        TIME = Qt::UserRole + 1,
        MESSAGE,
        POST
    };

    void mockData();

    };

#endif // MESSAGEMODEL_H
