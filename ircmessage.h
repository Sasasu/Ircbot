#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H
#include <string>

class IrcMessage
{
public:
    IrcMessage();
    IrcMessage(std::string);
    std::string username = "";
    std::string host = "";
    std::string ip = "";
    int message_type;
    std::string channle = "";
    std::string text = "";
    enum MessageType_enum{
        PRIVMSG,
        OTHER
    };
private:
    //:Sasasu!~li@180.212.140.146 PRIVMSG #TJPU_LUG_ :测试文字
};

#endif // IRCMESSAGE_H
