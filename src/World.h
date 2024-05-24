//
// Created by jansensamosa on 5/4/24.
//

#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <cmath>
#include <random>

#include "ElementDefinitions.h"
#include "Engine.h"
#include "Rendering/Shader.h"

class World {
public:
    World();
    ~World();

    void Update();
    void brushElement(int x, int y, int radius, int ELEMENT_ID);

    uint8_t* getPixelBuffer();

private:
    props::element** world;
    uint8_t* buffer;

    uint8_t gameTime = 0; // how many ticks have elapsed since game started

    void MoveLikePowder(int x, int y);
    void MoveLikeLiquid(int x, int y);
    void MoveLikeGas(int x, int y);

    void UpdateAsSand(int x, int y);
    void UpdateAsWater(int x, int y);
    void UpdateAsOil(int x, int y);
    void UpdateAsFire(int x, int y);
    void UpdateAsBurningWood(int x, int y);
    void UpdateAsSteam(int x, int y);
    void UpdateAsInfiniteSource(int x, int y);
    void UpdateAsPlant(int x, int y);
    void UpdateAsAcid(int x, int y);
    void UpdateAsLava(int x, int y);
    void UpdateAsGunpowder(int x, int y);
    void UpdateAsSalt(int x, int y);
    void UpdateAsAcidCloud(int x, int y);
    void UpdateAsRock(int x, int y);

    void updateElementLifetime(int x, int y);

    void placeExplosion(int x, int y, int radius);

    bool hasNeighbor(int x, int y, int ELEMENT_ID_NEIGHBOR);
    uint8_t* getNeighborsIDs (int x, int y);

    void burnNeighbors(int x, int y);

    bool replaceNeighbors(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO);
    bool replaceAdjacentNeighbors(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO);

    bool replaceElementAt(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO);
    void placeElement(int x, int y, int ELEMENT_ID);
    void placeElementDestructively(int x, int y, int ELEMENT_ID);

    // helper functions
    bool canMoveTo(int x, int y, int x2, int y2);
    bool isEmpty(int x, int y);
    bool inBounds(int x, int y);

    void swapElement(int x1, int y1, int x2, int y2);
    bool tryMoveElement(int x, int y, int dx, int dy);

    void updatePixel(int x, int y);
};

#endif //GAME_H

