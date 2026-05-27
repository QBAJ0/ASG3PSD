#include "Item.h"
#include "RoomDefaultEnterCommand.h"
#include "Passage.h"
#include "Player.h"
#include "Room.h"
#include "ZOOrkEngine.h"
#include <memory>


int main() {
    std::shared_ptr<Room> start = std::make_shared<Room>("start-room",
                           "You are standing in an open field west of a white house, with a boarded front door.\n");

    std::shared_ptr<Room> south_of_house = std::make_shared<Room>("south-of-house",
                                    "You are facing the south side of a white house.  There is no door here, and all the windows are barred.\n");

    std::shared_ptr<Room> behind_house = std::make_shared<Room>("behind-house",
                                  "You are behind the white house. A path leads into the forest to the east. In one corner of the house there is a small window which is slightly ajar.\n");

    std::shared_ptr<Room> front_path = std::make_shared<Room>("front-path",
                                  "A narrow dirt path curves around the front of the house. The field is quiet.\n");

    std::shared_ptr<Room> west_road = std::make_shared<Room>("west-road",
                                 "An old road disappears into tall grass. The house is barely visible to the east.\n");

    std::shared_ptr<Room> side_yard = std::make_shared<Room>("side-yard",
                                 "You are in a cramped side yard between the house and a leaning fence.\n");

    std::shared_ptr<Room> forest_edge = std::make_shared<Room>("forest-edge",
                                   "Trees crowd close together here. A darker trail continues east.\n");

    std::shared_ptr<Room> deep_forest = std::make_shared<Room>("deep-forest",
                                   "The forest canopy blocks most of the light. A large dead old tree looms over the clearing.\n");

    std::shared_ptr<Room> living_room = std::make_shared<Room>("living-room",
                                   "A dusty living room with faded wallpaper and a staircase leading up.\n");

    std::shared_ptr<Room> attic = std::make_shared<Room>("attic",
                             "You are in a cramped attic filled with old trunks and cobwebs.\n");

    Passage::createBasicPassage(start.get(), south_of_house.get(), "south", true);
    Passage::createBasicPassage(south_of_house.get(), behind_house.get(), "east", true);
    Passage::createBasicPassage(start.get(), front_path.get(), "north", true);
    Passage::createBasicPassage(start.get(), west_road.get(), "west", true);
    Passage::createBasicPassage(south_of_house.get(), side_yard.get(), "west", true);
    Passage::createBasicPassage(behind_house.get(), forest_edge.get(), "east", true);
    Passage::createBasicPassage(forest_edge.get(), deep_forest.get(), "east", true);
    Passage::createBasicPassage(behind_house.get(), living_room.get(), "in", true);
    Passage::createBasicPassage(living_room.get(), attic.get(), "up", true);

    auto leaflet = std::make_shared<Item>("leaflet",
        "The darkness upstairs fears what lies buried beneath the old tree.");
    start->addItem(leaflet);

    auto shovel = std::make_shared<Item>("shovel",
        "A rusty shovel leans against the fence.");
    side_yard->addItem(shovel);

    auto lantern = std::make_shared<Item>("lantern",
        "A battery-powered brass lantern sits on a table.");
    living_room->addItem(lantern);

    auto stick = std::make_shared<Item>("stick",
        "A sturdy fallen branch lies on the ground.");
    deep_forest->addItem(stick);

    ZOOrkEngine zoork(start);

    zoork.run();

    return 0;
}
