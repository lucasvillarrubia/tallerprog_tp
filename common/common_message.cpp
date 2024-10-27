#include <iostream>
#include <sstream>
#include <string>

#include "message.h"


#define BOX_APPEARED_MESSAGE "A new box has appeared"


Message::Message(): content(BOX_APPEARED_MESSAGE) {}

Message::Message(std::vector<char>& player_name, int reward_id) {
    std::stringstream msg_content;
    msg_content << player_name.data();
    msg_content << " picked up a ";
    msg_content << rewards_by_id.at(reward_id);
    content = msg_content.str();
}

void Message::print() { std::cout << content << "\n"; }

Message::~Message() {}
