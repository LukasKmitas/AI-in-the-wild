#include "Droid.h"
#include "MoveTo.h"
#include <iostream>
#include <vector>

class Patrol : public Routine 
{
public:
    std::vector<std::pair<int, int>> patrolPoints;
    size_t currentPointIndex;
    MoveTo* moveTo;

    Patrol(Grid& grid, const std::vector<std::pair<int, int>>& points) : Routine(), patrolPoints(points), currentPointIndex(0) 
    {
        if (!patrolPoints.empty()) 
        {
            moveTo = new MoveTo(patrolPoints[0].first, patrolPoints[0].second, grid);
        }
        this->routineType = "Patrol";
        this->routineGrid = &grid;
    }

    void start(string msg) override 
    {
        if (patrolPoints.empty()) return;
        std::cout << ">>> Starting routine " << routineType << msg << std::endl;
        state = RoutineState::Running;
        moveTo->start(" from a Patrol node.");
    }

    void reset(string msg) override 
    {
        if (patrolPoints.empty()) return;
        std::cout << ">>> Resetting routine " << routineType << msg << std::endl;
        currentPointIndex = 0; // Reset to the first point
        moveTo = new MoveTo(patrolPoints[0].first, patrolPoints[0].second, *routineGrid);
        moveTo->reset(" from a Patrol node");
        state = RoutineState::None;
    }

    void act(Droid* droid, Grid& grid) override
    {
        if (patrolPoints.empty() || !moveTo->isRunning())
        {
            return;
        }

        moveTo->act(droid, grid);
 
        if (moveTo->isRunning())
        {
            int x = droid->getCurrentX();
            int y = droid->getCurrentY();
            grid.changeTileColor(x - 1, y - 1, sf::Color::Red, sf::seconds(3));
        }

        if (moveTo->isSuccess())
        {
            currentPointIndex = (currentPointIndex + 1) % patrolPoints.size();
            int nextX = patrolPoints[currentPointIndex].first;
            int nextY = patrolPoints[currentPointIndex].second;
            moveTo = new MoveTo(nextX, nextY, grid);
            moveTo->start(" moving to next patrol point.");
        }
        else if (moveTo->isFailure()) 
        {
            fail();
        }
    }
};
