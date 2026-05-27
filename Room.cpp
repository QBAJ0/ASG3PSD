//
// Created by Richard Skarbez on 5/7/23.
//

#include "NullPassage.h"
#include "Room.h"

#include <utility>


Room::Room(const std::string &n, const std::string &d) : Location(n, d) {
    enterCommand = std::make_shared<RoomDefaultEnterCommand>(this);
}

Room::Room(const std::string &n, const std::string &d, std::shared_ptr<Command> c) : Location(n, d, std::move(c)) {}

void Room::addItem(std::shared_ptr<Item> item) {
    items.push_back(std::move(item));
}

std::shared_ptr<Item> Room::getItem(const std::string &name) const {
    for (const auto &item : items) {
        if (item->getName() == name) {
            return item;
        }
    }
    return nullptr;
}

std::shared_ptr<Item> Room::removeItem(const std::string &name) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it)->getName() == name) {
            std::shared_ptr<Item> item = *it;
            items.erase(it);
            return item;
        }
    }
    return nullptr;
}

void Room::addPassage(const std::string &direction, std::shared_ptr<Passage> p) {
    passageMap[direction] = std::move(p);
}

void Room::removePassage(const std::string &direction) {
    if (passageMap.contains(direction)) {
        passageMap.erase(direction);
    }
}

std::shared_ptr<Passage> Room::getPassage(const std::string &direction) {
    if (passageMap.contains(direction)) {
        return passageMap[direction];
    } else {
        std::cout << "It is impossible to go " << direction << "!\n";
        return std::make_shared<NullPassage>(this);
    }
}