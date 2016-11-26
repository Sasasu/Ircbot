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
        //不会正则就是菜啊.等考完期中再来改
        std::regex tmp(".*?PRIVMSG.*?");
        std::sregex_iterator tmptmp(str.begin(), str.end(), tmp);
        //^.*?:(.*?)!.*?PRIVMSG #(.*?) :(.*?)$
        if(tmptmp!=std::sregex_iterator()){
            std::regex r(".*?卖个萌.*?");
            std::sregex_iterator rt(str.begin(), str.end(), r);
            if(rt!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                ii.operator ++();
                bot->send_msg(rand()%2==0?"喵~":"你才卖萌,你全家都卖萌",ii->str());
                return;
            }
            std::regex qwq(".*?qwq.*?");
            std::sregex_iterator rqwq(str.begin(), str.end(), qwq);
            if(rqwq!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                bot->send_msg("pwp",ii->str());
                return;
            }
            std::regex pwp(".*?pwp.*?");
            std::sregex_iterator rpwp(str.begin(), str.end(), pwp);
            if(rpwp!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                bot->send_msg("qwq",ii->str());
                return;
            }
            std::regex qwp(".*?qwp.*?");
            std::sregex_iterator rqwp(str.begin(), str.end(), qwp);
            if(rqwp!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                bot->send_msg("pwq",ii->str());
                return;
            }
            std::regex pwq(".*?pwq.*?");
            std::sregex_iterator rpwq(str.begin(), str.end(), pwq);
            if(rpwq!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                bot->send_msg("qwp",ii->str());
                return;
            }
            std::regex bwb(".*?bwb.*?");
            std::sregex_iterator rbwb(str.begin(), str.end(), bwb);
            if(rbwb!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                bot->send_msg("dwd",ii->str());
                return;
            }
            std::regex dwd(".*?dwd.*?");
            std::sregex_iterator rdwd(str.begin(), str.end(), dwd);
            if(rdwd!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                bot->send_msg("bwb",ii->str());
                return;
            }
            std::regex dwb(".*?dwb.*?");
            std::sregex_iterator rdwb(str.begin(), str.end(), dwb);
            if(rdwb!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                bot->send_msg("bwd",ii->str());
                return;
            }
            std::regex bwd(".*?bwd.*?");
            std::sregex_iterator rbwd(str.begin(), str.end(), bwd);
            if(rbwd!=std::sregex_iterator()){
                std::regex d("#[^: ]*");
                std::sregex_iterator ii(str.begin(),str.end(),d);
                bot->send_msg("dwb",ii->str());
                return;
            }
        }

    }

    IrcBot bot;
};

int main() {
    MiaowBot bot("irc.freenode.net", 6667);
    bot.start();
    bot.setnick("miaowbot");
    bot.setuser("miaowbot");
    bot.join("#linuxba");
    bot.join("##Orz");
    bot.join("#archlinux-cn");
    bot.join("##ana");
    bot.join("#avplayer");
    bot.join("#TJPU_LUG_");
    bot.run();
    return 0;
}
