#include "ircmessage.h"
#include <boost/regex.hpp>
#include <iostream>
#include <string>
static const std::set<char> FORMATCONTROL =
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
     0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
     0x02, 0x03, 0x1d, 0x1f, 0x16, 0x0f};

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

        if (this->username == "teleboto" || this->username == "teleboto_" ||
            this->username == "xmppbot" || this->username == "tg2bot" ||
            this->username == "tg2offtopic") {
            size_t start = 0, end = 0;
            for (size_t i = 0; i < this->text.length(); i++) {
                if (this->text[i] == '[')
                    start = i;
                if (this->text[i] == ']')
                    end = i;
                if (start != 0 && end != 0)
                    break;
            }
            std::string username;
            std::string text;
            for (size_t i = start + 1; i < end; i++) {
                if (FORMATCONTROL.find(this->text[i]) == FORMATCONTROL.end()) {
                    username.push_back(this->text[i]);
                }
            }
            for (size_t i = end; i < this->text.length(); i++) {
                if (FORMATCONTROL.find(this->text[i]) == FORMATCONTROL.end()) {
                    text.push_back(this->text[i]);
                }
            }
            this->username = username;
            this->text = text;
        }
    } else {
        this->message_type = OTHER;
    }
}
