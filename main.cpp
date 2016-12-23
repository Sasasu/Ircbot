#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <regex>
#include <string>

#include "ircmessage.h"

using namespace boost::asio;
class IrcBot {
public:
    IrcBot(std::string service, std::string point)
        : addr_(service), point_(point), socket_(ioservice_) {}
    IrcBot(std::string service, int point)
        : IrcBot(std::move(service), std::to_string(point)) {}
    void start() { connect(); }

    void async_read(std::function<void(std::string)> callback) {
        async_read_until(
                    socket_, buff_, "\r\n",
                    [this, callback](boost::system::error_code ec, size_t size) {
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
                std::cout << "[ERR] connect close by server" << std::endl;
                exit(1);
            }
        });
    }
    void async_write(const char *some, size_t size) {
        boost::asio::async_write(socket_, boost::asio::buffer(some, size),
                                 [](boost::system::error_code e, size_t s) {
            std::cout << "[INFO] write " << e.message()
                      << std::endl;
        });
    }
    void async_write(std::string str) {
        this->async_write(str.c_str(), str.length());
    }

    void raw_send(std::string str) {
        std::cout << "[INFO] write " << str << std::endl;
        async_write(str + "\r\n");
    }

    void run() { ioservice_.run(); }

private:
    void connect() {
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
    void disconnect() { socket_.close(); }

    io_service ioservice_;
    ip::tcp::socket socket_;
    std::string addr_, point_;
    streambuf buff_;
};

class MiaowBot {
public:
    MiaowBot(std::string service, int point) : bot(service, point) {}
    void join(std::string str) { bot.raw_send("JOIN " + str); }
    void setuser(std::string str) { bot.raw_send("USER " + str + " 0 * :miaow"); }
    void setnick(std::string str) { bot.raw_send("NICK " + str); }
    void send_msg(std::string str, std::string to) {
        bot.raw_send("PRIVMSG " + to + " :" + str);
    }
    void start() {
        bot.start();
        bot.async_read(boost::bind(&callback, this, _1));
    }
    void run() { bot.run(); }

private:
    static void callback(MiaowBot *bot, std::string str) {
        //:Sasasu!~li@180.212.140.146 PRIVMSG #TJPU_LUG_ :测试文字
        IrcMessage message(str);
        if (message.message_type == IrcMessage::PRIVMSG) {
            std::string text = message.text;

#ifdef DEBUG
            std::cout << "DEBUG " << text << std::endl;
            for (char a : text) {
                std::cout << "DEBUG " << a << std::endl;
            }

            std::cout << "DEBUG " << message.channle << std::endl;
#endif
            // text has a space at the back
            if (text[text.length() - 2] == 'z' &&
                    (message.channle == "#linuxba" || message.channle == "#TJPU_LUG") &&
                    rand() % 3 == 0) {

                if (text.length() >= 3 && text[text.length() - 3] == 'r') {
                    return; // Orz <- do net send 'z'
                }

                bot->send_msg("z", message.channle);
                return;
            }

            std::regex r("(卖个萌|[qpbd][wm][qpbd])");
            std::sregex_iterator sregex(text.begin(), text.end(), r);
            if (sregex != std::sregex_iterator()) {
                bot->send_msg(switchstr(sregex->str()), message.channle);
                return;
            }
        }
    }

    static std::string switchstr(std::string str) {
        if (str == "卖个萌") {
            return rand() % 3 == 0 ? "你才卖萌,你全家都卖萌" : "喵~";

        } else {
            std::string tmp = str;
            for (unsigned int i = 0; i < tmp.length(); i++) {
                if (tmp[i] == 'q') {
                    tmp[i] = 'p';
                    continue;
                }

                if (tmp[i] == 'p') {
                    tmp[i] = 'q';
                    continue;
                }

                if (tmp[i] == 'b') {
                    tmp[i] = 'd';
                    continue;
                }

                if (tmp[i] == 'd') {
                    tmp[i] = 'b';
                    continue;
                }
            }
            return tmp;
        }
    }

    IrcBot bot;
};

int main() {
    MiaowBot bot("irc.freenode.net", 6667);
    bot.start();
#ifdef RELEASE
    bot.setnick("miaowbot");
    bot.setuser("miaowbot");
    bot.join("#linuxba");
    bot.join("##Orz");
    bot.join("#archlinux-cn");
    bot.join("##ana");
    bot.join("#avplayer");
    bot.join("#TJPU_LUG");
#endif
#ifdef DEBUG
    bot.setnick("miaowbot_test");
    bot.setuser("miaowbot");
    bot.join("#TJPU_LUG_");
#endif
    bot.run();
    return 0;
}
