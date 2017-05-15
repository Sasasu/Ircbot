#ifndef IRCBOT_H
#define IRCBOT_H
#include <boost/asio.hpp>
#include <iostream>
class IrcBot {
  public:
    IrcBot(std::string service, std::string point);
    IrcBot(std::string service, int point);
    void start();

    void async_read(std::function<void(std::string)> callback);
    void async_write(const char *some, size_t size);
    void async_write(std::string str);

    void raw_send(std::string str);

    void run();

  private:
    void connect();
    void disconnect();

    boost::asio::io_service ioservice_;
    std::string addr_, point_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buff_;
};

#endif // IRCBOT_H
