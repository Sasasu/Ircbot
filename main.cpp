#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <regex>
#include <iostream>
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

class MiaowBot{
public:
    MiaowBot(std::string service,int point):bot(service,point){}
    void join(std::string str){
        bot.raw_send("JOIN " + str);
    }
    void setuser(std::string str){
        bot.raw_send("USER " + str + " 0 * :miaow");
    }
    void setnick(std::string str){
        bot.raw_send("NICK " + str);
    }
    void send_msg(std::string str, std::string to) {
        bot.raw_send("PRIVMSG " + to + " :" + str);
    }
    void start(){
        bot.start();
        bot.async_read(boost::bind(&callback,this,_1));
    }
    void run(){
        bot.run();
    }

private:
    static void callback(MiaowBot *bot,std::string str){
        std::regex r(".*?卖个萌.*?");
        std::sregex_iterator it(str.begin(), str.end(), r);     //end是尾后迭代器，regex_iterator是regex_iterator的string类型的版本
        if(it!=std::sregex_iterator()){
            std::regex rr("#[^: ]*");
            std::sregex_iterator ii(str.begin(),str.end(),rr);
            bot->send_msg("喵~",ii->str());
        }

    }

    IrcBot bot;
};

int main() {
    MiaowBot bot("irc.freenode.net", 6667);
    bot.start();
    bot.setnick("miaowbot");
    bot.setuser("miaowbot");
    bot.join("#TJPU_LUG");
    bot.join("#archlinux-cn");
    bot.join("##ana");
    bot.join("#avplayer");
    bot.run();
    return 0;
}
