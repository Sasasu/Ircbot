#include "ircmessage.h"
#include <boost/regex.hpp>
#include <iostream>
#include <string>
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

        if (this->username == "teleboto" || this->username == "teleboto_" || this->username == "xmppbot") {
            int start = 0, end = 0;
            for (int i = 0; i < this->text.length(); i++) {
                if (this->text[i] == '[')
                    start = i;
                if (this->text[i] == ']')
                    end = i;
                if (start != 0 && end != 0)
                    break;
            }
            std::string username;
            std::string text;
            for (int i = start; i < end; i++) {
                username.push_back(this->text[i]);
            }
            for (int i = end; i < this->text.length(); i++) {
                text.push_back(this->text[i]);
            }
            this->username = username;
            this->text = text;
        }

    } else {
        this->message_type = OTHER;
    }
}
