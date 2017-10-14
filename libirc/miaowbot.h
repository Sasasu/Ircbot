#ifndef MIAOWBOT_H
#define MIAOWBOT_H
#include <boost/bind.hpp>
#include <regex>
#include <string>
#include "ircbot.h"
#include "ircmessage.h"

class MiaowBot {
   public:
    MiaowBot(std::string service, int point);
    void join(std::string str);
    void setuser(std::string str);
    void setnick(std::string str);
    void send_msg(std::string str, std::string to);
    void start();
    void run();

   private:
    static void callback(MiaowBot *bot, std::string str);

    static std::string switchstr(std::string str);

    IrcBot bot;
};

#endif  // MIAOWBOT_H
