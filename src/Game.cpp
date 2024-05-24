//
// Created by jansensamosa on 5/4/24.
//

#include "Game.h"

using namespace props;

int Game::BRUSH_SIZE = 5;
int Game::cursor_pos_x = 10;
int Game::cursor_pos_y = 10;
int Game::cursor_pos_prev_x = 10;
int Game::cursor_pos_prev_y = 10;

Game::Game(Shader &prog) : renderer(WorldRenderer(&prog)) {
    world = new element *[WIDTH];
    buffer = new uint8_t[WIDTH * HEIGHT * 3];

    for (int i = 0; i < WIDTH; i++) {
        world[i] = new element[HEIGHT];
    }

    set_brush_size(5);
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            world[x][y] = ELEMENTS_TEMPLATES[AIR_ID];
        }
    }

    renderer.Render(getPixelBuffer());
}

Game::~Game() {
    for (int i = 0; i < WIDTH; ++i) {
        delete[] world[i];
    }
    delete[] world;
    delete buffer;
}

void Game::MoveLikePowder(int x, int y) {
    int dir = rand() % 2 - 1;
    if (dir == 0) dir = 1;

    int vel = world[x][y].verticalVelocity - 100;

    uint8_t maxVel = gameTime % 1 == 0 ? world[x][y].verticalVelocity + 1 : world[x][y].verticalVelocity;
    //uint8_t maxVel = min(world[x][y].verticalVelocity + 1, 20);

    //if (gameTime % 4 == 0 && (hasNeighbor(x, y, WATER_ID) || hasNeighbor(x, y, SALTWATER_ID))) {
    //    maxVel -= 2;
    //}

    world[x][y].verticalVelocity = 0;
    bool stopped = false;

    while (!stopped && world[x][y].verticalVelocity < maxVel) {
        world[x][y].verticalVelocity++;

        if (tryMoveElement(x, y, 0, -1)) {
            y--;
        } else if ( tryMoveElement(x, y, dir, -1)) {
            y--;
            x += dir;
        } else {
            stopped = true;
        }
    }

    //tryMoveElement(x, y, 0, -1);
    //tryMoveElement(x, y, 1, -1);
}

void Game::MoveLikeLiquid(int x, int y) {
    int dir = rand() % 2 - 1;
    if (dir == 0) dir = 1;


    uint8_t maxVel = gameTime % 1 == 0 ? world[x][y].verticalVelocity + 1 : world[x][y].verticalVelocity;
    //uint8_t maxVel = min(world[x][y].verticalVelocity + 1, 20);

    world[x][y].verticalVelocity = 0;
    bool stopped = false;
    int moved = 0;

    while (!stopped && world[x][y].verticalVelocity < maxVel) {
        world[x][y].verticalVelocity++;

        if (tryMoveElement(x, y, 0, -1)) {
            y--;
            moved++;
        } else if (tryMoveElement(x, y, dir, -1)) {
            y--;
            x += dir;
            moved++;
        } else {
            stopped = true;
        }
    }
    if (moved > 4) return;

    // spread logic
    uint8_t viscosity = ELEMENT_VISCOSITIES[world[x][y].elementID];
    int spreadDir = 0;
    int spreadAmount = 0;
    for (int i = 1; i < viscosity * 1; i++) {
        if (canMoveTo(x, y, x + i * dir, y) ) {
            spreadDir = 1 * dir;
            spreadAmount = i;
        } else if (canMoveTo(x, y, x - i * dir, y) ) {
            spreadDir = -1 * dir;
            spreadAmount = i;
        } else {
            break;
        }
    }

    tryMoveElement(x, y, spreadDir * spreadAmount, 0);
}

void Game::MoveLikeGas(int x, int y) {
    int dir = rand() % 2 - 1;
    if (dir == 0) dir = 1;

    if (tryMoveElement(x, y, 0, 1)) return;
    if (tryMoveElement(x, y, -dir, 1)) return;
    if (tryMoveElement(x, y, dir, 1)) return;

    for (int i = 5; i >= 1; i--) {
        if (tryMoveElement(x + dir * i, y, -dir * i, 0)) return;
    }
}

void Game::UpdateAsSand(int x, int y) {
    MoveLikePowder(x, y);
}

void Game::UpdateAsWater(int x, int y) {
    MoveLikeLiquid(x, y);
}

void Game::UpdateAsOil(int x, int y) {
    MoveLikeLiquid(x, y);
}

void Game::UpdateAsFire(int x, int y) {
    if (hasNeighbor(x, y, WATER_ID)) {
        burnNeighbors(x, y);
        placeElement(x, y, AIR_ID);
    } else {
        burnNeighbors(x, y);

    }

    if (rand() % 10 < 3 && gameTime % ELEMENT_DECAY_RATES[FIRE_ID] == 0) {
        int dirX = rand() % 2 - 1;
        if (dirX == 0) dirX = 1;
        int dist = rand() % 5 + 2;
        int distY = rand() % 10 +  4;

        if(tryMoveElement(x, y, dirX * dist, distY)) {
            if (rand() % 10 < 3 && world[x + dirX *  dist][y + distY].lifeTime >= 2)  {
                updateElementLifetime(x + dirX * dist, y + distY);
            }
            updateElementLifetime(x + dirX * dist, y + distY);
        } else {
            updateElementLifetime(x, y);
        }
    }
}

void Game::UpdateAsBurningWood(int x, int y) {
    burnNeighbors(x, y);
    replaceNeighbors(x, y, AIR_ID, FIRE_ID);

    updateElementLifetime(x, y);
}

void Game::UpdateAsSteam(int x, int y) {
    updateElementLifetime(x, y);

    if (world[x][y].lifeTime == 0) {
        replaceElementAt(x, y, STEAM_ID, WATER_ID);
        return;
    }

    MoveLikeGas(x, y);
}

void Game::UpdateAsAcidCloud(int x, int y) {
    updateElementLifetime(x, y);

    if (world[x][y].lifeTime == 0) {
        replaceElementAt(x, y, ACIDCLOUD_ID, ACID_ID);
        return;
    }

    MoveLikeGas(x, y);
}

void Game::UpdateAsInfiniteSource(int x, int y) {
    uint8_t elementID = world[x][y].customData; // element to create

    if (elementID == 0) {
        uint8_t *neighbors = getNeighborsIDs(x, y);

        for (int i = 0; i < 8; i++) {
            if (neighbors[i] != AIR_ID && neighbors[i] != INFINITESOURCE_ID) {
                world[x][y].customData = neighbors[i];
                return;
            }
        }
        return;
    }
    replaceAdjacentNeighbors(x, y, AIR_ID, world[x][y].customData);
}

void Game::UpdateAsPlant(int x, int y) {
    if (rand() % 2 == 0) {
        replaceNeighbors(x, y, WATER_ID, PLANT_ID);
        replaceNeighbors(x, y, STEAM_ID, PLANT_ID);
    } else {
        replaceNeighbors(x, y, WATER_ID, AIR_ID);
        replaceNeighbors(x, y, STEAM_ID, AIR_ID);
    }
}

void Game::UpdateAsAcid(int x, int y) {
    if (rand() % 20 < 3) {
        for (int i = 0; i < SIZE_ALWAYS_AT_END; i++) {
            if (i != AIR_ID && i != ACID_ID && i != FIRE_ID && i != INFINITESOURCE_ID && i != ACIDCLOUD_ID) {
                if (replaceNeighbors(x, y, i, AIR_ID)) {
                    placeElement(x, y, AIR_ID);
                    return;
                };
            }
        }
    }
    MoveLikeLiquid(x, y);
}

void Game::UpdateAsLava(int x, int y) {
    if (rand() % 25 <= 1 && hasNeighbor(x, y, WATER_ID)) {
        placeElement(x, y, ROCK_ID);
        return;
    }

    burnNeighbors(x, y);

    if (rand() % 100 <= 1) {
        replaceNeighbors(x, y, AIR_ID, FIRE_ID);
    }

   if (rand() % 200 <= 1) {
       replaceAdjacentNeighbors(x, y, ROCK_ID, LAVA_ID);
   }
   if (rand() % 400 <= 1) {
       replaceNeighbors(x, y, ROCK_ID, LAVA_ID);
   }

    MoveLikeLiquid(x, y);
}

void Game::UpdateAsGunpowder(int x, int y) {
    if (rand() % 75 <= 1) {
        if (hasNeighbor(x, y, FIRE_ID) || hasNeighbor(x, y, LAVA_ID) || hasNeighbor(x, y, WOODBURNING_ID)) {
            placeExplosion(x, y, rand() % 5 + 10);
        }
    }

    MoveLikePowder(x, y);
}

void Game::UpdateAsSalt(int x, int y) {
    if (rand() % 1 <= 1 && replaceNeighbors(x, y, WATER_ID, SALTWATER_ID)) {
        placeElement(x, y, AIR_ID);
        return;
    }

    MoveLikePowder(x, y);
}

void Game::UpdateAsRock(int x, int y) {
    return;
}


// Public methods
void Game::Update() {
    gameTime = (gameTime + 1) % 255;

    // update elements
    for (int y = 0; y < HEIGHT; y++) {
        bool e = y % 2 == 0;
        for (int x = e ? 0 : WIDTH - 1; e ? x < WIDTH : x >= 0; e ? x++ : x--) {
            element& element = world[x][y];
            if (element.updated) continue;

            uint8_t curr = element.elementID;

            if (element.lifeTime <= 0) {
                placeElement(x, y, AIR_ID);
            }

            switch (curr) {
                case SAND_ID:
                    element.updated = true;
                    UpdateAsSand(x, y);
                break;
                case WATER_ID:
                    element.updated = true;
                    UpdateAsWater(x, y);
                break;case OIL_ID:
                    element.updated = true;
                    UpdateAsOil(x, y);
                break;
                case DIRT_ID:
                    element.updated = true;
                    UpdateAsSand(x, y);
                break;
                case FIRE_ID:
                    element.updated = true;
                    UpdateAsFire(x, y);
                    break;
                case WOODBURNING_ID:
                    element.updated = true;
                    UpdateAsBurningWood(x, y);
                    break;
                case STEAM_ID:
                    element.updated = true;
                    UpdateAsSteam(x, y);
                    break;
                case INFINITESOURCE_ID:
                    element.updated = true;
                    UpdateAsInfiniteSource(x, y);
                    break;
                case PLANT_ID:
                    element.updated = true;
                    UpdateAsPlant(x, y);
                    break;
                case ACID_ID:
                    element.updated = true;
                    UpdateAsAcid(x, y);
                    break;;
                case LAVA_ID:
                    element.updated = true;
                    UpdateAsLava(x, y);
                    break;
                case GUNPOWDER_ID:
                    element.updated = true;
                    UpdateAsGunpowder(x, y);
                    break;
                case SALT_ID:
                    element.updated = true;
                    UpdateAsSalt(x, y);
                    break;
                case SALTWATER_ID:
                    element.updated = true;
                    UpdateAsWater(x, y);
                    break;
                case ACIDCLOUD_ID:
                    element.updated = true;
                    UpdateAsAcidCloud(x, y);
                    break;
                case ROCK_ID:
                    element.updated = true;
                    UpdateAsRock(x, y);
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::updateElementLifetime(int x, int y) {
    int element_id = world[x][y].elementID;

    if (gameTime % ELEMENT_DECAY_RATES[element_id] == 0) {
        world[x][y].lifeTime--;
    }
}

void Game::placeExplosion(int x, int y, int radius) {
    for (int i = -radius + x; i <= radius + x; i++) {
        for (int j = -radius + y; j <= radius + y; j++) {
            bool withinRadius = sqrt((x - i)*(x - i) + (y - j)*(y - j)) <= radius;

            if(withinRadius && inBounds(i, j) && world[i][j].elementID != INFINITESOURCE_ID) {
                placeElement(i, j, FIRE_ID);
            }
        }
    }

}

void Game::brushElement(int x, int y, int radius, int ELEMENT_ID) {
    int prevX = cursor_pos_prev_x;
    int prevY = cursor_pos_prev_y;

    double xStep = (x - prevX) / ((radius + 1));
    double yStep = (y - prevY) / ((radius + 1));

    int steps = xStep != 0 ? abs((x - prevX) / xStep) : 1;
    steps += yStep != 0 ? abs((y - prevY) / yStep) : 1;
    for (int k = 0; k < steps + 3; k++) {
        int curX = prevX + xStep * k;
        int curY = prevY + yStep * k;

        for (int i = -radius + curX; i <= radius + curX; i++) {
            for (int j = -radius + curY; j <= radius + curY; j++) {
                bool withinRadius = sqrt((curX - i)*(curX - i) + (curY - j)*(curY - j)) <= radius;

                if(withinRadius && inBounds(i, j) && (rand() % 20 <= 1 || ELEMENT_ID == AIR_ID ||ELEMENT_ID == ROCK_ID || ELEMENT_ID == WOOD_ID)) {
                        placeElementDestructively(i, j, ELEMENT_ID);
                }
            }
        }
    }
}

bool Game::hasNeighbor(int x, int y, int ELEMENT_ID_NEIGHBOR) {
    return (inBounds(x + 1, y    ) && world[x + 1][y].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x - 1, y    ) && world[x - 1][ y    ].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x, y + 1    ) && world[x][ y + 1    ].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x, y - 1    ) && world[x][ y - 1    ].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x + 1, y - 1) && world[x + 1][ y - 1].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x - 1, y - 1) && world[x - 1][ y - 1].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x + 1, y + 1) && world[x + 1][ y + 1].elementID == ELEMENT_ID_NEIGHBOR) ||
    (inBounds(x - 1, y + 1) && world[x - 1][ y + 1].elementID == ELEMENT_ID_NEIGHBOR);
}

uint8_t * Game::getNeighborsIDs(int x, int y) {
    static uint8_t ids[8];

    ids[0] = inBounds(x - 1, y) ? world[x - 1][y].elementID : AIR_ID;
    ids[1] = inBounds(x + 1, y) ? world[x + 1][y].elementID : AIR_ID;
    ids[2] = inBounds(x, y + 1) ? world[x][y + 1].elementID : AIR_ID;
    ids[3] = inBounds(x, y - 1) ? world[x][y - 1].elementID : AIR_ID;
    ids[4] = inBounds(x - 1, y - 1) ? world[x - 1][y - 1].elementID : AIR_ID;
    ids[5] = inBounds(x + 1, y - 1) ? world[x + 1][y - 1].elementID : AIR_ID;
    ids[6] = inBounds(x + 1, y + 1) ? world[x + 1][y + 1].elementID : AIR_ID;
    ids[7] = inBounds(x - 1, y + 1) ? world[x - 1][y + 1].elementID : AIR_ID;

    return ids;
}

void Game::placeElement(int x, int y, int ELEMENT_ID = AIR_ID) {
    if (!inBounds(x, y)) {
        return;
    }
    world[x][y] = ELEMENTS_TEMPLATES[ELEMENT_ID];
    world[x][y].updated = true;

    if (ELEMENT_ID == WOODBURNING_ID) {
        world[x][y].lifeTime += rand() % 50;
    }
}

void Game::placeElementDestructively(int x, int y, int ELEMENT_ID) {
    placeElement(x, y, ELEMENT_ID);
    world[x][y].updated = false;
}

bool Game::replaceElementAt(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO) {
    if (inBounds(x, y) && world[x][y].elementID == ELEMENT_ID_FROM) {
        placeElement(x, y, ELEMENT_ID_TO);
        return true;
    }
    return false;
}

bool Game::replaceNeighbors(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO) {
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

bool Game::replaceAdjacentNeighbors(int x, int y, int ELEMENT_ID_FROM, int ELEMENT_ID_TO) {
    bool replaced = false;
    replaced = replaceElementAt(x + 1, y, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x - 1, y, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x, y + 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;
    replaced = replaceElementAt(x, y - 1, ELEMENT_ID_FROM, ELEMENT_ID_TO) || replaced;

    return replaced;
}

void Game::burnNeighbors(int x, int y) {
    if(rand() % 5 < 2) return;
    replaceNeighbors(x, y, WOOD_ID, WOODBURNING_ID);
    replaceNeighbors(x, y, WATER_ID, STEAM_ID);
    replaceNeighbors(x, y, SALTWATER_ID, STEAM_ID);
    replaceNeighbors(x, y, PLANT_ID, FIRE_ID);
    replaceNeighbors(x, y, OIL_ID, FIRE_ID);
    replaceNeighbors(x, y, ACID_ID, ACIDCLOUD_ID);
}

bool Game::canMoveTo(int x, int y, int x2, int y2) {
    if (!inBounds(x,y) || !inBounds(x2, y2)) return false;

    uint8_t density1 = ELEMENT_DENSITIES[world[x][y].elementID];
    uint8_t density2 = ELEMENT_DENSITIES[world[x2][y2].elementID];

    if (density1 == 255 || density2 == 255) {
        return false;
    }

    if (y >= y2) {
        return  density1 > density2;
    }
    return  density1 < density2;
}

bool Game::isEmpty(int x, int y) {
    return world[x][y].elementID == AIR_ID;
}

bool Game::inBounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

void Game::swapElement(int x1, int y1, int x2, int y2) {
    if (!inBounds(x1, y1) || !inBounds(x2, y2)) {
        cout << "not in bounds!" << endl;
        return;
    }
    element temp = world[x1][y1];
    world[x1][y1] = world[x2][y2];
    world[x2][y2] = temp;
}

// returns false if cannot move, returns true if successfully does
bool Game::tryMoveElement(int x, int y, int dx, int dy) {
    if (canMoveTo(x, y, x + dx, y + dy)) {
        swapElement(x, y, x + dx, y + dy);

        return true;
    }
    return false;
}

void Game::Render() {
    renderer.Render(getPixelBuffer());
}

uint8_t * Game::getPixelBuffer() {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = HEIGHT - 1; y >= 0; y--) {
            updatePixel(x, y);
        }
    }

    return buffer;
}

void Game::updatePixel(int x, int y) {
    world[x][y].updated = false; // for the next timestep

    Color color = ELEMENTS_COLORS[world[x][y].elementID];
    if (world[x][y].elementID == FIRE_ID) {
        color.g += (20* world[x][y].lifeTime);

    }

    int mouseDist = sqrt((cursor_pos_x - x)*(cursor_pos_x - x) + (cursor_pos_y - y) * (cursor_pos_y - y));
    if (mouseDist == BRUSH_SIZE || mouseDist == 0) {
        color.r = min(color.r + 100,255);
        color.g = min(color.g + 100,255);
        color.b = min(color.b + 100,255);
    }


    int index = y * WIDTH * 3 + x * 3;

    buffer[index] = color.r;
    buffer[index + 1] = color.g;
    buffer[index + 2] = color.b;
}
