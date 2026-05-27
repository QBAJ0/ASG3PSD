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
    Player *player = Player::instance();

    if (locked) {
        player->setCurrentRoom(getFrom());
        std::cout << "The door is locked.\n\n";
        return;
    }

    if (getTo()->getName() == "attic" && !player->isAtticLanternUsed()) {
        std::cout << "It is too dark upstairs to see.\n\n";
        return;
    }

    Passage::enter();
}
