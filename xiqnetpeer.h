#ifndef PROTOPEER_H
#define PROTOPEER_H

#include "xiqnet_global.h"

#include <QObject>
#include <QString>
#include <QAbstractSocket>

class XiQNetPeerPrivate;
class XiQNetWrapper;
namespace google
{
  namespace protobuf
  {
    class Message;
  }
}

class QTcpSocket;

/**
 * @brief Custom QTcpSocket based implementation of a TCP network peer
 */
class XIQNET_QTSHARED_EXPORT XiQNetPeer : public QObject
{
  Q_OBJECT
public:
  explicit XiQNetPeer(QObject *t_parent = 0);
  explicit XiQNetPeer(qintptr t_socketDescriptor, QObject *t_parent = 0);

  ~XiQNetPeer();

  QString getIpAddress() const;
  quint16 getPort() const;
  bool isConnected() const;


  int getPeerId() const;
  void setPeerId(int t_peerId);

  /**
   * @brief Allows access to the QTcpSocket in case of non protobuf transmission
   * @return QTcpSocket used by this instance
   * @todo refactor, as this violates the OOP principle "separation of concerns"
   */
  QTcpSocket *getTcpSocket() const;

  QString getErrorString() const;

signals:
  /**
   * @brief Emitted if successful connected
   */
  void sigConnectionEstablished();
  /**
   * @brief Emitted when the connection is interrupted
   */
  void sigConnectionClosed();
  /**
   * @brief Emitted when incoming messages arrive
   * @param t_Message
   * @bug Do not use the t_Message parameter with Qt::QueuedConnection as it may be deleted before the slot is called
   */
  void sigMessageReceived(QByteArray t_Message);
  /**
   * @brief Emitted on socket failure
   * @param t_socketError
   */
  void sigSocketError(QAbstractSocket::SocketError t_socketError);

public slots:
  /**
   * @brief Transmits the message
   * @param t_message
   */
  void sendMessage(QByteArray t_message) const;
  /**
   * @brief Starts the connection in case of a dedicated host connection
   *
   * Called after Signal/Slot setup
   * @param t_ipAddress
   * @param t_port
   */
  void startConnection(QString t_ipAddress, quint16 t_port);
  /**
   * @brief Closes the connection
   */
  void stopConnection();

protected slots:
  /**
   * @brief Translates incoming messages to google::protobuf::Message via the XiQNetWrapper
   */
  void onReadyRead();

private:
  /**
   * @brief PIMPL pointer
   */
  XiQNetPeerPrivate *d_ptr = 0;
};

#endif // PROTOPEER_H
