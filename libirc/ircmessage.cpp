#include "ircmessage.h"
#include <boost/regex.hpp>
#include <iostream>
#include <string>

static const std::set<std::string> BOTNAME = {"teleboto", "teleboto_", "xmppbot", "tg2bot", "tg2offtopic"
#ifdef DEBUG
                                              ,
                                              "Sasasu"
#endif
};

void DEBUGOUT(const std::string& s) {
#ifdef DEBUG
    std::cerr << "[DEBUG] " << s << std::endl;
#endif
}

IrcMessage::IrcMessage() {}

IrcMessage::IrcMessage(std::string str) {
    boost::smatch what;
    if (boost::regex_match(
            str, what,
            boost::regex(":([^!]+)!([^ ]+) PRIVMSG ([^ ]+) :(.*)[\\r\\n]*"))) {
        this->username = what[1];
        this->host = what[2];
        this->channle = what[3];
        this->text = what[4];
        this->message_type = PRIVMSG;

        DEBUGOUT("get message " + text);

        if (BOTNAME.find(this->username) != BOTNAME.end()) {
            boost::smatch messages;
            boost::regex_match(this->text, messages, boost::regex("\\[(.*)\\](.*)"));
            this->username = messages[1];
            this->text = messages[2];
        }
        text.erase(remove_if(text.begin(), text.end(), [](const char& ch) { return ch == ' '; }), text.end());
        DEBUGOUT("real message " + text);
    } else {
        this->message_type = OTHER;
    }
}
