#include "MessageModel.h"

#include <QDateTime>
#include <mutex>
#include <QQmlEngine>


MessageModel::Message::Message(QDateTime time,
                               QString messageText,
                               bool post)
    : m_time {time}
    , m_message {messageText}
    , post {post}
{}
MessageModel::Message::Message(QString messageText,
                               bool post)
    : m_time {QDateTime::currentDateTime()}
    , m_message {messageText}
    , post {post}
{}


/*void MessageModel::registerModel(const std::string& moduleName)
{
    static std::once_flag flag;
    std::call_once(flag, [&](){
        qmlRegisterType<MessageModel>(moduleName.c_str(), 1, 0, "CoreMessageModel");
    });
}*/

void MessageModel::add(const MessageModel::Message &mess)
{
   const std::size_t currentPos {m_messages.size()};

    emit beginInsertRows(QModelIndex(), currentPos, currentPos);
    m_messages.push_back(mess);
    emit endInsertRows();
}

MessageModel::MessageModel()
{

    mockData();
}

MessageModel::~MessageModel()
{

}

int MessageModel::rowCount(const QModelIndex &parent) const
{

    Q_UNUSED(parent)
    return static_cast<int>(m_messages.size());
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{

    if(!index.isValid() || index.row() > rowCount(index))
        return {};

    const Message& m {m_messages.at(index.row())};

    switch(role)
    {
    case MessagesRoles::TIME:
        return {m.m_time.toString("yy.MM.dd hh:mm:ss").toLatin1()};
    case MessagesRoles::MESSAGE:
        return {m.m_message};
    case MessagesRoles::POST:
        return {m.post};
    default:
        return {};
    }
}

#include <QTimer>
void MessageModel::mockData()
{

    m_messages = {
        Message("hello", false)  ,
        Message(QDateTime::currentDateTime(),"TooManyTextWithoutSpacesTooManyTextWithoutSpacesTooManyTextWithoutSpacesTooManyTextWithoutSpacesTooManyTextWithoutSpacesTooManyTextWithoutSpacesTooManyTextWithoutSpacesTooManyTextWithoutSpaces", true)  ,
        Message("Too many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many text", true)  ,
        Message(QDateTime::currentDateTime(),"Too many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many text on other side", false)  ,

    };

    QTimer::singleShot(5000, [&](){
        add( Message(QDateTime::currentDateTime(),"This message added dinamicly", false));
    });
}


QHash<int, QByteArray> MessageModel::roleNames() const
{

    return {
        {MessagesRoles::TIME,       "time"},
        {MessagesRoles::MESSAGE,    "message"},
        {MessagesRoles::POST,       "post"}
    };
}


bool MessageModel::insertRows(int row, int count, const QModelIndex &parent)
{

    emit beginInsertRows(parent, row, row + count);
    m_messages.reserve(m_messages.size() + count);
    m_messages.insert(std::next(m_messages.begin(), row), count, Message());
    emit endInsertRows();

    return true;
}
