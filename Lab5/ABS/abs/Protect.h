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
    int droidP;

    Protect(int droidA, int droidB, int droidP, Grid& g) : Routine()
    {
        this->destX = 1;
        this->destY = 1;
        this->target = g.getGridLocation(destX, destY);
        this->routineType = "Protect";
        this->routineGrid = &g;

        this->droidA = droidA;
        this->droidB = droidB;
        this->droidP = droidP;
        if (droidA != -1) this->droidA = droidA - 1;
        if (droidB != -1) this->droidB = droidB - 1;
        if (droidP != -1) this->droidP = droidP - 1;
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
        // Calculate the midpoint between droid A and droid B
        // This works 
        sf::Vector2f protectPoint;

        int droidAx = grid.m_gridDroids[droidA]->x;
        int droidAy = grid.m_gridDroids[droidA]->y;
        int droidBx = grid.m_gridDroids[droidB]->x;
        int droidBy = grid.m_gridDroids[droidB]->y;

        protectPoint.x = (droidAx + droidBx) / 2.0f;
        protectPoint.y = (droidAy + droidBy) / 2.0f;

        return protectPoint;
        // End 

       // sf::Vector2f A = grid.getGridLocation(grid.m_gridDroids[droidA]->x, grid.m_gridDroids[droidA]->y);
       // sf::Vector2f B = grid.getGridLocation(grid.m_gridDroids[droidB]->x, grid.m_gridDroids[droidB]->y);
       // //sf::Vector2f R = grid.getGridLocation(this->destX, this->destY);
       // sf::Vector2f P = grid.getGridLocation(grid.m_gridDroids[droidP]->x, grid.m_gridDroids[droidP]->y);

       // sf::Vector2f AB = B - A;
       // sf::Vector2f AP = P - A;

       // float magnitudeAB = std::sqrt(AB.x * AB.x + AB.y * AB.y);
       // sf::Vector2f normalizedAB = AB / magnitudeAB;
       // //float normalisedX = AB.x / magnitudeAB;
       // //float normalisedY = AB.y / magnitudeAB;
       // //float dotProduct = (P.x * P.x + A.y * A.y) * (normalisedX / normalisedY);
       // float dotProduct = P.x - A.x * normalizedAB.x + P.y - A.y * normalizedAB.y;
       // sf::Vector2f projection = P + normalizedAB * dotProduct;

       ///* float interesctionX = A.x + (normalisedX * dotProduct);
       // float interesctionY = A.y + (normalisedY * dotProduct);
       // sf::Vector2f interesctionPoint = sf::Vector2f(interesctionX, interesctionY);*/

       // std::cout << "A: " << A.x << ", " << A.y << std::endl;
       // std::cout << "B: " << B.x << ", " << B.y << std::endl;
       // std::cout << "P: " << P.x << ", " << P.y << std::endl;
       // std::cout << "Closest Point: " << projection.x << ", " << projection.y << std::endl;

       // return projection;
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
