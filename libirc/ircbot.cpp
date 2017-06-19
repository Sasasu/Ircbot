#include "ircbot.h"

IrcBot::IrcBot(std::string service, std::string point)
    : addr_(service), point_(point), socket_(ioservice_) {}

IrcBot::IrcBot(std::string service, int point)
    : IrcBot(std::move(service), std::to_string(point)) {}

void IrcBot::start() { this->connect(); }

void IrcBot::async_read(std::function<void(std::string)> callback) {
    async_read_until(socket_, buff_, "\r\n",
                     [this, callback](boost::system::error_code ec, size_t) {
                         if (!ec) {
                             std::istream istream(&buff_);
                             std::string line;
                             std::getline(istream, line);
                             std::cout << "[SERV] " << line << std::endl;
                             if (line.compare(0, 4, "PING") == 0) {
                                 raw_send("PONG" + line.substr(4));
                             } else {
                                 callback(line);
                             }
                             async_read(callback);
                         } else {
                             std::cout << "[ERR] connect close by server"
                                       << std::endl;
                             exit(1);
                         }
                     });
}

void IrcBot::async_write(const char *some, size_t size) {
    boost::asio::async_write(socket_, boost::asio::buffer(some, size),
                             [](boost::system::error_code e, size_t) {
                                 std::cout << "[INFO] write " << e.message()
                                           << std::endl;
                             });
}

void IrcBot::async_write(std::string str) {
    this->async_write(str.c_str(), str.length());
}

void IrcBot::raw_send(std::string str) {
    std::cout << "[INFO] write " << str << std::endl;
    async_write(str + "\r\n");
}

void IrcBot::run() { ioservice_.run(); }

void IrcBot::connect() {
    using namespace boost::asio;
    ip::tcp::resolver resolver(ioservice_);
    ip::tcp::resolver::query query(addr_, point_);
    ip::tcp::resolver::iterator iterator = resolver.resolve(query);
    ip::tcp::endpoint endpoint = *iterator;
    try {
        socket_.connect(endpoint);
        std::cout << "[INFO] connect to " << socket_.remote_endpoint()
                  << std::endl;
    } catch (std::exception e) {
        std::cerr << "[ERR] connct failure" << std::endl;
    }
}

void IrcBot::disconnect() { socket_.close(); }
