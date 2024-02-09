#include "Droid.h"
#include "MoveTo.h"
#include "HealthPill.h"
#include <iostream>
#include <vector>

class SearchForHealthPills : public Routine 
{
private:
    MoveTo* moveTo;
    bool itemFound;

public:
    SearchForHealthPills(Grid& grid) : Routine(), moveTo(nullptr), itemFound(false) 
    {
        this->routineType = "SearchForHealthPills";
        this->routineGrid = &grid;
    }

    void start(string msg) override 
    {
        std::cout << ">>> Starting routine " << routineType << msg << std::endl;
        state = RoutineState::Running;
        itemFound = false;
        searchForNextHealthPill();
    }

    void searchForNextHealthPill() 
    {
        for (HealthPill* pill : routineGrid->m_gridHealthPills)
        {
            if (pill->visible)
            {
                moveTo = new MoveTo(pill->x, pill->y, *routineGrid);
                moveTo->start(" to pick up HealthPill.");
                return;
            }
        }
        itemFound = false;
    }

    void act(Droid* droid, Grid& grid) override
    {
        if (moveTo == nullptr)
        {
            fail();
            return;
        }

        if (!itemFound)
        {
            moveTo->act(droid, grid);
        }

        if (moveTo->isSuccess())
        {
            HealthPill* pill = grid.getHealthPillAt(droid->getCurrentX(), droid->getCurrentY());
            if (pill != nullptr && pill->visible)
            {
                pill->HasBeenPickedUp();
                std::cout << "Collected HealthPill at " + std::to_string(pill->x) + ", " + std::to_string(pill->y) + " by " + droid->name << std::endl;
                itemFound = true;
            }

            itemFound = false;
            searchForNextHealthPill();

            if (moveTo == nullptr)
            {
                succeed("Collected all HealthPills.");
            }
        }
        else if (moveTo->isFailure())
        {
            fail();
        }
    }

    void reset(string msg) override 
    {
        std::cout << ">>> Resetting routine " << routineType << msg << std::endl;
        moveTo = nullptr;
        itemFound = false;
        state = RoutineState::Running;
    }
};
