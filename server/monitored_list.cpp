#include "monitored_list.h"


MonitoredList::MonitoredList() {}

void MonitoredList::push_back(Player* player) {
    std::unique_lock<std::mutex> lck(mtx);
    list.push_back(player);
}

void MonitoredList::broadcast(const Command& announce) {
    std::unique_lock<std::mutex> lck(mtx);
    for (auto& player: list) {
        player->add_message_to_queue(announce);
    }
}

void MonitoredList::clean_up() {
    std::unique_lock<std::mutex> lck(mtx);
    list.remove_if([](Player* player) {
        if (not player->is_connected()) {
            player->disconnect();
            delete player;
            return true;
        }
        return false;
    });
}

void MonitoredList::clear() {
    std::unique_lock<std::mutex> lck(mtx);
    for (Player* player: list) {
        player->disconnect();
        delete player;
    }
}
