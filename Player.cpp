//
// Created by Richard Skarbez on 5/7/23.
//

#include "Player.h"

Player *Player::playerInstance = nullptr;

void Player::setCurrentRoom(Room* room) {
    currentRoom = room;
}

Room* Player::getCurrentRoom() const {
    return currentRoom;
}

void Player::addItem(std::shared_ptr<Item> item) {
    inventory.push_back(std::move(item));
}

std::shared_ptr<Item> Player::getItem(const std::string &name) const {
    for (const auto &item : inventory) {
        if (item->getName() == name) {
            return item;
        }
    }
    return nullptr;
}

std::shared_ptr<Item> Player::removeItem(const std::string &name) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if ((*it)->getName() == name) {
            std::shared_ptr<Item> item = *it;
            inventory.erase(it);
            return item;
        }
    }
    return nullptr;
}
