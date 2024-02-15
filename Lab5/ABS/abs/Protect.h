#ifndef PROTECT_H
#define PROTECT_H

#include "Droid.h"
#include "Routine.h"
#include <iostream>
#include <cmath>

class Protect : public Routine
{

public:

    int destX;
    int destY;
    sf::Vector2f target;
    int droidA;
    int droidB;

    Protect(int droidA, int droidB, Grid& g) : Routine()
    {
        this->destX = 1;
        this->destY = 1;
        this->target = g.getGridLocation(destX, destY);
        this->routineType = "Protect";
        this->routineGrid = &g;

        this->droidA = droidA;
        this->droidB = droidB;
        if (droidA != -1) this->droidA = droidA - 1;
        if (droidB != -1) this->droidB = droidB - 1;
    }

    void reset(string msg)
    {
        std::cout << ">>> Resetting routine " << routineType << msg << std::endl;
        state = RoutineState::None;
    }

    void act(Droid* droid, Grid& grid)
    {
        if (isRunning())
        {
            if (!droid->isAlive() || droid->alarmHasBeenRaised)
            {
                fail();
                return;
            }

            sf::Vector2f protectPoint = getProtectPoint(grid);
            destX = round(protectPoint.x);
            destY = round(protectPoint.y);

            if (destX < 1) destX = 1;
            if (destY < 1) destY = 1;
            if (destX > grid.gridSize) destX = grid.gridSize;
            if (destY > grid.gridSize) destY = grid.gridSize;

            droid->target = grid.getGridLocation(destX, destY);
            if (!isDroidAtDestination(droid, grid)) 
            {
                moveDroid(droid, grid);
            }
            else 
            {
                succeed("Protect for " + droid->name);
            }
        }
    }
    sf::Vector2f getProtectPoint(Grid& grid)
    {
        sf::Vector2f A = grid.getGridLocation(grid.m_gridDroids[droidA]->x, grid.m_gridDroids[droidA]->y);
        sf::Vector2f B = grid.getGridLocation(grid.m_gridDroids[droidB]->x, grid.m_gridDroids[droidB]->y);

        sf::Vector2f R = grid.getGridLocation(this->destX, this->destY);

        sf::Vector2f AB = B - A;
        sf::Vector2f AR = R - A;

        float magnitudeAB = std::sqrt(AB.x * AB.x + AB.y * AB.y);
        float dotProduct = (AR.x * AB.x + AR.y * AB.y);
        //float projection = dotProduct / (magnitudeAB * magnitudeAB);

        //sf::Vector2f closestPoint = A + AB * std::max(0.0f, std::min(projection, 1.0f));
        sf::Vector2f interesctionPoint;

        return interesctionPoint;
    }

    void moveDroid(Droid* droid, Grid& grid)
    {
        std::cout << ">>> Droid " << droid->name << " moving to " << destX << ", " << destY << std::endl;

        if (destX < 1 || destX > grid.gridSize || destY < 1 || destY > grid.gridSize)
            return;

        sf::Vector2f direction = droid->target - droid->position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 0) 
        {
            const float movementSpeed = 1.0;
            sf::Vector2f normalizedDirection = direction / distance;
            sf::Vector2f movement = normalizedDirection * movementSpeed;

            droid->position += movement;
        }

        if (isDroidAtDestination(droid, grid))
        {
            succeed("MoveTo for " + droid->name);
        }
    }

    bool isDroidAtDestination(Droid* droid, Grid& grid)
    {
        sf::Vector2f direction = droid->target - droid->position;
        return ((int)grid.length(direction) == 0);
    }
};
#endif
