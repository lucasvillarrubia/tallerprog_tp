// #include "monitored_list.h"
//
//
// template <typename T>
// MonitoredList<T>::MonitoredList() {}
//
// template <typename T>
// void MonitoredList<T>::push_back(T player) {
//     std::unique_lock<std::mutex> lck(mtx);
//     list.push_back(player);
// }
//
// template <typename T>
// void MonitoredList<T>::broadcast(const Gamestate& announce) {
//     std::unique_lock<std::mutex> lck(mtx);
//     for (auto& player: list) {
//         player->add_message_to_queue(announce);
//     }
// }
//
// template <typename T>
// MonitoredList<T> MonitoredList<T>::select_if()
// {
// }
//
// template <typename T>
// void MonitoredList<T>::clean_up() {
//     std::unique_lock<std::mutex> lck(mtx);
//     list.remove_if([](Player* player) {
//         if (not player->is_connected()) {
//             player->disconnect();
//             delete player;
//             return true;
//         }
//         return false;
//     });
// }
//
// template <typename T>
// void MonitoredList<T>::clear() {
//     std::unique_lock<std::mutex> lck(mtx);
//     for (Player* player: list) {
//         player->disconnect();
//         delete player;
//     }
// }
