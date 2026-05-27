#ifndef ZOORK_DOOR_H
#define ZOORK_DOOR_H

#include "Passage.h"

class Door : public Passage {
public:
    Door(const std::string &, const std::string &, Room*, Room*, bool locked = true);

    void lock();

    void unlock();

    bool isLocked() const;

    void enter() override;

private:
    bool locked;
};

#endif //ZOORK_DOOR_H
