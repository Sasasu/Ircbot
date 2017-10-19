#include "miaowbot.h"
static const boost::regex MAINREGEX(
    "(卖个萌)|"
    "(賣個萌)|"
    "^([qpbd]+[wm]+[qpbd]+).*|"
    ".*([qpbd]+[wm]+[qpbd]+\\r)|"
    "(z+)\\r");
MiaowBot::MiaowBot(std::string service, int point) : bot(service, point) {}

void MiaowBot::join(std::string str) { bot.raw_send("JOIN " + str); }

void MiaowBot::setuser(std::string str) {
    bot.raw_send("USER " + str + " 0 * :miaow");
}

void MiaowBot::setnick(std::string str) { bot.raw_send("NICK " + str); }

void MiaowBot::send_msg(std::string str, std::string to) {
    bot.raw_send("PRIVMSG " + to + " :" + str);
}

void MiaowBot::start() {
    bot.start();
    bot.async_read(boost::bind(&callback, this, _1));
}

void MiaowBot::run() { bot.run(); }

void MiaowBot::callback(MiaowBot* bot, std::string str) {
    IrcMessage message(str);
    if (message.message_type == IrcMessage::PRIVMSG) {
        std::string text = message.text;
        if (message.username.find("bot") != std::string::npos)
            return;

        boost::smatch what;
        if (boost::regex_match(text, what, MAINREGEX)) {
            for (size_t i = 1; i < what.size(); i++) {
                std::string tmp_str = switchstr(what[i]);
                if (tmp_str != "") {
                    bot->send_msg(tmp_str, message.channle);
                    return;
                }
            }
        }
    }
}
std::string MiaowBot::switchstr(const std::string& str) {
    if (str.empty())
        return "";
    else if (str[0] == 'z') {
        return rand() % 3 == 0 ? "z" : "";
    } else if (str == "卖个萌") {
        return rand() % 3 == 0 ? "你才卖萌,你全家都卖萌" : "喵~";
    } else if (str == "賣個萌") {
        return rand() % 3 == 0 ? "你才賣萌,你全家都賣萌" : "喵~";
    } else {
        std::string tmp = str;
        for (size_t i = 0; i < tmp.length(); i++) {
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
