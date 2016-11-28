#include "ircmessage.h"
#include <boost/regex.hpp>
#include <iostream>
IrcMessage::IrcMessage(){}

IrcMessage::IrcMessage(std::__cxx11::string str){
    boost::smatch what;
    if (boost::regex_match(str, what,
                           boost::regex(":([^!]+)!([^ ]+) PRIVMSG ([^ ]+) :(.*)[\\r\\n]*"))){
        this->username = what[1];
        this->host = what[2];
        this->channle = what[3];
        this->text = what[4];
        this->message_type = PRIVMSG;
    }else{
        this->message_type = OTHER;
    }
}
