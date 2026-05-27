//
// Created by Richard Skarbez on 5/7/23.
//

#include "ZOOrkEngine.h"
#include "Door.h"

#include <algorithm>
#include <cctype>
#include <utility>

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());
    player->getCurrentRoom()->enter();
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = tokenizeString(input);
        if (words.empty()) {
            continue;
        }
        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (command == "go") {
            handleGoCommand(arguments);
        } else if ((command == "look") || (command == "inspect")) {
            handleLookCommand(arguments);
        } else if ((command == "take") || (command == "get")) {
            handleTakeCommand(arguments);
        } else if (command == "drop") {
            handleDropCommand(arguments);
        } else if (command == "inventory") {
            handleInventoryCommand();
        } else if (command == "dig") {
            handleDigCommand();
        } else if (command == "use") {
            handleUseCommand(arguments);
        } else if (command == "quit") {
            handleQuitCommand(arguments);
        } else {
            std::cout << "I don't understand that command.\n";
        }
    }
}

void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    std::string direction;
    if (arguments[0] == "n" || arguments[0] == "north") {
        direction = "north";
    } else if (arguments[0] == "s" || arguments[0] == "south") {
        direction = "south";
    } else if (arguments[0] == "e" || arguments[0] == "east") {
        direction = "east";
    } else if (arguments[0] == "w" || arguments[0] == "west") {
        direction = "west";
    } else if (arguments[0] == "u" || arguments[0] == "up") {
        direction = "up";
    } else if (arguments[0] == "d" || arguments[0] == "down") {
        direction = "down";
    } else {
        direction = arguments[0];
    }

    Room* currentRoom = player->getCurrentRoom();
    auto passage = currentRoom->getPassage(direction);
    player->setCurrentRoom(passage->getTo());
    passage->enter();
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << player->getCurrentRoom()->getDescription() << "\n\n";
        return;
    }
    std::string itemName = arguments[0];

    std::shared_ptr<Item> item = player->getCurrentRoom()->getItem(itemName);
    if (!item) {
        item = player->getItem(itemName);
    }

    if (item) {
        std::cout << item->getDescription() << "\n\n";
    } else {
        std::cout << "You do not see that item here.\n\n";
    }
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Take what?\n\n";
        return;
    }

    std::string itemName = arguments[0];
    std::shared_ptr<Item> item = player->getCurrentRoom()->removeItem(itemName);

    if (!item) {
        std::cout << "You do not see that item here.\n\n";
        return;
    }

    player->addItem(item);
    std::cout << "Taken.\n\n";
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Drop what?\n\n";
        return;
    }

    std::string itemName = arguments[0];
    std::shared_ptr<Item> item = player->removeItem(itemName);
    if (!item) {
        std::cout << "You are not carrying that item.\n\n";
        return;
    }

    player->getCurrentRoom()->addItem(item);
    std::cout << "Dropped.\n\n";
}

void ZOOrkEngine::handleInventoryCommand() {
    const std::vector<std::shared_ptr<Item>> &items = player->getInventory();

    if (items.empty()) {
        std::cout << "You are carrying nothing.\n\n";
        return;
    }

    std::cout << "You are carrying:\n";
    for (const auto &item : items) {
        std::cout << "- " << item->getName() << "\n";
    }
    std::cout << "\n";
}

void ZOOrkEngine::handleDigCommand() {
    Room* currentRoom = player->getCurrentRoom();
    if (currentRoom->getName() != "deep-forest") {
        std::cout << "There is nothing useful to dig here.\n\n";
        return;
    }

    if (!player->getItem("shovel")) {
        std::cout << "You need something to dig with.\n\n";
        return;
    }

    if (rustyKeyRevealed) {
        std::cout << "You already dug up everything useful here.\n\n";
        return;
    }

    auto key = std::make_shared<Item>("key",
                                      "An old rusty key caked in dirt.");
    currentRoom->addItem(key);
    rustyKeyRevealed = true;
    std::cout << "You dig beneath the old tree and uncover a key.\n\n";
}

void ZOOrkEngine::handleUseCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Use what?\n\n";
        return;
    }

    std::string itemName = arguments[0];
    if (itemName != "key") {
        std::cout << "You can't use that here.\n\n";
        return;
    }

    if (!player->getItem("key")) {
        std::cout << "You do not have a key.\n\n";
        return;
    }

    Room* currentRoom = player->getCurrentRoom();
    if (currentRoom->getName() != "living-room") {
        std::cout << "There is nothing here to unlock with the key.\n\n";
        return;
    }

    std::shared_ptr<Passage> upPassage = currentRoom->getPassage("up");
    std::shared_ptr<Door> atticDoor = std::dynamic_pointer_cast<Door>(upPassage);
    if (!atticDoor) {
        std::cout << "There is nothing here to unlock with the key.\n\n";
        return;
    }

    if (!atticDoor->isLocked()) {
        std::cout << "The attic door is already unlocked.\n\n";
        return;
    }

    atticDoor->unlock();
    std::cout << "You unlock the attic door.\n\n";
}

void ZOOrkEngine::handleQuitCommand(std::vector<std::string> arguments) {
    std::string input;
    std::cout << "Are you sure you want to QUIT?\n> ";
    std::cin >> input;
    std::string quitStr = makeLowercase(input);

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    }
}

std::vector<std::string> ZOOrkEngine::tokenizeString(const std::string &input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, ' ')) {
        std::string normalized = makeLowercase(token);
        normalized.erase(std::remove(normalized.begin(), normalized.end(), '\r'), normalized.end());
        if (!normalized.empty()) {
            tokens.push_back(normalized);
        }
    }

    return tokens;
}

std::string ZOOrkEngine::makeLowercase(std::string input) {
    std::string output = std::move(input);
    std::transform(output.begin(), output.end(), output.begin(), ::tolower);

    return output;
}
