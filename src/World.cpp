//
// Created by jansensamosa on 5/23/24.
//

#include "World.h"

int World::BRUSH_SIZE = 5;
int World::cursor_pos_x = 10;
int World::cursor_pos_y = 10;
int World::cursor_pos_prev_x = 10;
int World::cursor_pos_prev_y = 10;

World::World() {
    world = new element[WIDTH * HEIGHT];
    worldPixelBuffer = new uint8_t[WIDTH * HEIGHT * 3];

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            world[worldIndex(x, y)] = ELEMENTS_TEMPLATES[AIR_ID];
        }
    }
}

World::~World() {
    delete world;
    delete worldPixelBuffer;
}

element * World::get(int x, int y) const {
    return world + (y * WIDTH + x);
}

bool World::hasNeighbor(int x, int y, int ELEMENT_ID_NEIGHBOR) {
    return (inBounds(x + 1, y    ) && world[worldIndex(x + 1, y)].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x - 1, y    ) && world[worldIndex(x - 1, y)].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x, y + 1    ) && world[worldIndex(x, y + 1)].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x, y - 1    ) && world[worldIndex(x, y - 1)].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x + 1, y - 1) && world[worldIndex(x + 1, y - 1)].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x - 1, y - 1) && world[worldIndex(x - 1, y - 1)].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x + 1, y + 1) && world[worldIndex(x + 1, y + 1)].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x - 1, y + 1) && world[worldIndex(x - 1, y + 1)].elementID == ELEMENT_ID_NEIGHBOR);
}

std::uint8_t * World::getNeighborsIDs(int x, int y) {
    static std::uint8_t ids[8];

    ids[0] = inBounds(x - 1, y) ? world[worldIndex(x - 1, y)].elementID : AIR_ID;
    ids[1] = inBounds(x + 1, y) ? world[worldIndex(x + 1, y)].elementID : AIR_ID;
    ids[2] = inBounds(x, y + 1) ? world[worldIndex(x, y + 1)].elementID : AIR_ID;
    ids[3] = inBounds(x, y - 1) ? world[worldIndex(x, y - 1)].elementID : AIR_ID;
    ids[4] = inBounds(x - 1, y - 1) ? world[worldIndex(x - 1,y - 1)].elementID : AIR_ID;
    ids[5] = inBounds(x + 1, y - 1) ? world[worldIndex(x + 1,y - 1)].elementID : AIR_ID;
    ids[6] = inBounds(x + 1, y + 1) ? world[worldIndex(x + 1,y + 1)].elementID : AIR_ID;
    ids[7] = inBounds(x - 1, y + 1) ? world[worldIndex(x - 1,y + 1)].elementID : AIR_ID;

    return ids;
}

void World::placeElement(int x, int y, int ELEMENT_ID = AIR_ID) {
    if (!inBounds(x, y)) {
        return;
    }
    world[worldIndex(x, y)] = ELEMENTS_TEMPLATES[ELEMENT_ID];
    world[worldIndex(x, y)].updated = true;

    if (ELEMENT_ID == WOODBURNING_ID) {
        world[worldIndex(x, y)].lifeTime += rand() % 50;
    }
}

void World::placeElementDestructively(int x, int y, int ELEMENT_ID) {
    placeElement(x, y, ELEMENT_ID);
    world[worldIndex(x, y)].updated = false;
}

bool World::replaceElementAt(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO) {
    if (inBounds(x, y) && world[worldIndex(x, y)].elementID == ELEMENT_ID_FROM) {
        placeElement(x, y, ELEMENT_ID_TO);
        return true;
    }
    return false;
}

bool World::replaceNeighbors(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO) {
    bool replaced = false;

    replaced = replaceElementAt(x + 1, y, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x - 1, y, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x, y + 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x, y - 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x + 1, y - 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x - 1, y - 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x + 1, y + 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x - 1, y + 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;

    return replaced;
}

bool World::replaceAdjacentNeighbors(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO) {
    bool replaced = false;
    replaced = replaceElementAt(x + 1, y, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x - 1, y, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x, y + 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x, y - 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;

    return replaced;
}


bool World::canMoveTo(int x, int y, int x2, int y2) {
    if (!inBounds(x,y) || !inBounds(x2, y2)) return false;

    std::uint8_t density1 = ELEMENT_DENSITIES[world[worldIndex(x, y)].elementID];
    std::uint8_t density2 = ELEMENT_DENSITIES[world[worldIndex(x2, y2)].elementID];

    if (density1 == 255 || density2 == 255) {
        return false;
    }

    if (y >= y2) {
        return  density1 > density2;
    }
    return  density1 < density2;
}

bool World::isEmpty(int x, int y) {
    return world[worldIndex(x, y)].elementID == AIR_ID;
}

bool World::inBounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

void World::swapElement(int x1, int y1, int x2, int y2) {
    if (!inBounds(x1, y1) || !inBounds(x2, y2)) {
        std::cout << "not in bounds!" << std::endl;
        return;
    }
    element temp = world[worldIndex(x1, y1)];
    world[worldIndex(x1, y1)] = world[worldIndex(x2, y2)];
    world[worldIndex(x2, y2)] = temp;
}

// returns false if cannot move, returns true if successfully does
bool World::tryMoveElement(int x, int y, int dx, int dy) {
    if (canMoveTo(x, y, x + dx, y + dy)) {
        swapElement(x, y, x + dx, y + dy);

        return true;
    }
    return false;
}

int World::worldIndex(int x, int y) {
    return y * WIDTH + x;
}


std::uint8_t * World::getWorldPixelBuffer() {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = HEIGHT - 1; y >= 0; y--) {
            world[worldIndex(x, y)].updated = false; // for the next timestep

            Color color = ELEMENTS_COLORS[world[worldIndex(x, y)].elementID];
            if (world[worldIndex(x, y)].elementID == FIRE_ID) {
                color.g += (20* world[worldIndex(x, y)].lifeTime);

            }

            int mouseDist = sqrt((cursor_pos_x - x)*(cursor_pos_x - x) + (cursor_pos_y - y) * (cursor_pos_y - y));
            if (mouseDist == BRUSH_SIZE || mouseDist == 0) {
                color.r = std::min(color.r + 100,255);
                color.g = std::min(color.g + 100,255);
                color.b = std::min(color.b + 100,255);
            }


            int index = y * WIDTH * 3 + x * 3;

            worldPixelBuffer[index] = color.r;
            worldPixelBuffer[index + 1] = color.g;
            worldPixelBuffer[index + 2] = color.b;
        }
    }

    return worldPixelBuffer;
}