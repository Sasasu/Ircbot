#include "miaowbot.h"
MiaowBot::MiaowBot(std::string service, int point) : bot(service, point) {}

void MiaowBot::join(std::string str) { bot.raw_send("JOIN " + str); }

void MiaowBot::setuser(std::string str) {
    bot.raw_send("USER " + str + " 0 * :miaow");
}

void MiaowBot::setnick(std::string str) { bot.raw_send("NICK " + str); }

void MiaowBot::send_msg(std::string str, std::string to){
    bot.raw_send("PRIVMSG " + to + " :" + str);
}

void MiaowBot::start() {
    bot.start();
    bot.async_read(boost::bind(&callback, this, _1));
}

void MiaowBot::run() { bot.run(); }

void MiaowBot::callback(MiaowBot *bot, std::string str) {
    //:Sasasu!~li@180.212.140.146 PRIVMSG #TJPU_LUG_ :测试文字
    IrcMessage message(str);
    if (message.message_type == IrcMessage::PRIVMSG) {
        std::string text = message.text;
        if (message.username == "miaowbot")
            return;

        // text has a space at the back
        if (text[text.length() - 2] == 'z' &&
            message.channle == "#linuxba" && rand() % 3 == 0) {

            if (text.length() >= 3 && text[text.length() - 3] == 'r') {
                return; // Orz <- do net send 'z'
            }
            bot->send_msg("z", message.channle);
            return;
        }

        std::regex r("(卖个萌|賣個萌|[qpbd][wm][qpbd])");
        std::sregex_iterator sregex(text.begin(), text.end(), r);
        std::regex dont_r("(http|shadow)");
        std::sregex_iterator dont_r_s(text.begin(), text.end(), dont_r);
        if (sregex != std::sregex_iterator() &&
            dont_r_s == std::sregex_iterator()) {
            bot->send_msg(switchstr(sregex->str()), message.channle);
            return;
        }
#ifdef DEBUG
        std::cout << message.text << std::endl;
        if (message.channle == "#archlinux-cn-bot") {
#else
        if (message.channle == "#archlinux-cn-offtopic") {
#endif
            static std::map<std::string, int> hualao;
            hualao[message.username]++;
            std::cout << "[INFO] increase " << message.username
                      << std::endl;
            if (text.substr(0, 7) == "'hualao") {
                std::vector<std::pair<std::string, int>> rank(
                    hualao.begin(), hualao.end());
                sort(rank.begin(), rank.end(),
                     [](std::pair<std::string, int> &a,
                        std::pair<std::string, int> &b) {
                         return a.second > b.second;
                     });
                for (size_t i = 0; i < 3 && i < rank.size(); i++) {
                    bot->send_msg(rank[i].first + "  " +
                                      std::to_string(rank[i].second),
                                  message.channle);
                }
                return;
            }
            if (text.substr(0, 6) == "'reset") {
                hualao.clear();
                bot->send_msg("清空了~", message.channle);
                return;
            }
            if (text.substr(0, 5) == "'miaostop") {
                bot->send_msg("窝滚了~", message.channle);
                exit(0);
                return;
            }
        }
    }
}
std::string MiaowBot::switchstr(std::string str) {
    if (str == "卖个萌") {
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
