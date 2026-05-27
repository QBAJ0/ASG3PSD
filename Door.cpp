#include "Door.h"

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
    Passage::enter();
}
