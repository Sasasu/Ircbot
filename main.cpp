#include "ircbot.h"
#include "ircmessage.h"
#include "miaowbot.h"
using namespace boost::asio;
int main() {
    MiaowBot bot("irc.freenode.net", 8001);
    bot.start();
#ifdef RELEASE
    bot.setnick("miaowbot");
    bot.setuser("miaowbot");
    bot.join("#linuxba");
    bot.join("##Orz");
    bot.join("#archlinux-cn-offtopic");
    bot.join("##ana");
    bot.join("#avplayer");
    bot.join("#TJPU_LUG");
#endif
#ifdef DEBUG
    bot.setnick("miaowbot_test");
    bot.setuser("miaowbot");
    bot.join("##Orz");
#endif
    bot.run();
    return 0;
}
