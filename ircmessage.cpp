#include "ircmessage.h"
#include <regex>
#include <iostream>
IrcMessage::IrcMessage()
{

}

IrcMessage::IrcMessage(std::__cxx11::string str){
    std::regex self_regex("PRIVMSG");
    if (std::regex_search(str, self_regex)) {
        std::regex reg("\\S+");
        std::sregex_iterator text=std::sregex_iterator(str.begin(),str.end(),reg);
        //:Sasasu!~li@180.212.140.146 PRIVMSG #TJPU_LUG_ :测试文字

        std::string tmp = text->str();
        size_t len = tmp.length();
        size_t i =1;
        for(;i<len;i++){
            if(tmp[i] == '!')
                break;
            this->username += tmp[i];
        }
        i+=2;
        for(;i<len;i++){
            if(tmp[i] == '@')
                break;
            this->host += tmp[i];
        }
        i+=1;
        for(;i<len;i++){
            this->ip += tmp[i];
        }
        text++;
        //手撕用户名和主机和ip

        tmp = text->str();
        if(tmp == "PRIVMSG")
            this->message_type = PRIVMSG;
        else
            this->message_type = OTHER;
        //手撕消息类型

        text++;
        this->channle = text->str();
        //手撕频道名

        text++;
        tmp = text->str();
        for(int i=1;i<tmp.length();i++){
            this->text += tmp[i];
        }
        //手撕用户名

    }else{
        this->message_type = OTHER;
    }
}
