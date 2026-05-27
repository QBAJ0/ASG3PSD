#include "Door.h"
#include "Player.h"

Door::Door(const std::string &n, const std::string &d, Room* from, Room* to, bool isLocked)
        : Passage(n, d, from, to), locked(isLocked) {}

void Door::lock() {
    locked = true;
}

void Door::unlock() {
    locked = false;
}

bool Door::isLocked() const {
    return locked;
}

void Door::enter() {
    if (locked) {
        std::cout << "The door is locked.\n\n";
        return;
    }

    Player *player = Player::instance();
    if (getTo()->getName() == "attic" && !player->getItem("lantern")) {
        std::cout << "It is too dark upstairs to see.\n\n";
        return;
    }

    Passage::enter();

    if (getTo()->getName() == "attic" && player->getItem("key") && player->getItem("lantern")) {
        std::cout << "The future shall hold more quests for you.\n\n";
    }
}
