//
// Created by jansensamosa on 5/4/24.
//

#include "Game.h"

#include <cmath>


Game::Game(Shader &prog) : shaderProgram(prog), world(World()) {
    float vertices[] = {
        // positions   // texture coords
        -1.0f,  1.0f,  0.0f, 1.0f, // top left
        -1.0f, -1.0f,  0.0f, 0.0f, // bottom left
         1.0f, -1.0f,  1.0f, 0.0f, // bottom right
         1.0f,  1.0f,  1.0f, 1.0f  // top right
    };
    unsigned int indices[] = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    //Vertices
    glGenVertexArrays(1, &renderPlaneID);
    unsigned int EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(renderPlaneID);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture
    glGenTextures(1, &gameTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gameTextureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, World::WIDTH, World::HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, world.getWorldPixelBuffer());
}

void Game::MoveLikePowder(int x, int y) {
    int dir = rand() % 2 - 1;
    if (dir == 0) dir = 1;

    int vel = world.get(x,y)->verticalVelocity - 100;

    std::uint8_t maxVel = gameTime % 1 == 0 ? world.get(x,y)->verticalVelocity + 1 : world.get(x,y)->verticalVelocity;
    //std::uint8_t maxVel = min(world.get(x,y)->verticalVelocity + 1, 20);

    //if (gameTime % 4 == 0 && (world.hasNeighbor(x, y, WATER_ID) || world.hasNeighbor(x, y, SALTWATER_ID))) {
    //    maxVel -= 2;
    //}

    world.get(x,y)->verticalVelocity = 0;
    bool stopped = false;

    while (!stopped && world.get(x,y)->verticalVelocity < maxVel) {
        world.get(x,y)->verticalVelocity++;

        if (world.tryMoveElement(x, y, 0, -1)) {
            y--;
        } else if ( world.tryMoveElement(x, y, dir, -1)) {
            y--;
            x += dir;
        } else {
            stopped = true;
        }
    }

    //world.tryMoveElement(x, y, 0, -1);
    //world.tryMoveElement(x, y, 1, -1);
}

void Game::MoveLikeLiquid(int x, int y) {
    int dir = rand() % 2 - 1;
    if (dir == 0) dir = 1;


    std::uint8_t maxVel = gameTime % 1 == 0 ? world.get(x,y)->verticalVelocity + 1 : world.get(x,y)->verticalVelocity;
    //std::uint8_t maxVel = min(world.get(x,y)->verticalVelocity + 1, 20);

    world.get(x,y)->verticalVelocity = 0;
    bool stopped = false;
    int moved = 0;

    while (!stopped && world.get(x,y)->verticalVelocity < maxVel) {
        world.get(x,y)->verticalVelocity++;

        if (world.tryMoveElement(x, y, 0, -1)) {
            y--;
            moved++;
        } else if (world.tryMoveElement(x, y, dir, -1)) {
            y--;
            x += dir;
            moved++;
        } else {
            stopped = true;
        }
    }
    if (moved > 4) return;

    // spread logic
    std::uint8_t viscosity = ELEMENT_VISCOSITIES[world.get(x,y)->elementID];
    int spreadDir = 0;
    int spreadAmount = 0;
    for (int i = 1; i < viscosity * 1; i++) {
        if (world.canMoveTo(x, y, x + i * dir, y) ) {
            spreadDir = 1 * dir;
            spreadAmount = i;
        } else if (world.canMoveTo(x, y, x - i * dir, y) ) {
            spreadDir = -1 * dir;
            spreadAmount = i;
        } else {
            break;
        }
    }

    world.tryMoveElement(x, y, spreadDir * spreadAmount, 0);
}

void Game::MoveLikeGas(int x, int y) {
    int dir = rand() % 2 - 1;
    if (dir == 0) dir = 1;

    if (world.tryMoveElement(x, y, 0, 1)) return;
    if (world.tryMoveElement(x, y, -dir, 1)) return;
    if (world.tryMoveElement(x, y, dir, 1)) return;

    for (int i = 5; i >= 1; i--) {
        if (world.tryMoveElement(x + dir * i, y, -dir * i, 0)) return;
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
    if (world.hasNeighbor(x, y, WATER_ID)) {
        burnNeighbors(x, y);
        world.placeElement(x, y, AIR_ID);
    } else {
        burnNeighbors(x, y);

    }

    if (rand() % 10 < 3 && gameTime % ELEMENT_DECAY_RATES[FIRE_ID] == 0) {
        int dirX = rand() % 2 - 1;
        if (dirX == 0) dirX = 1;
        int dist = rand() % 5 + 2;
        int distY = rand() % 10 +  4;

        if(world.tryMoveElement(x, y, dirX * dist, distY)) {
            if (rand() % 10 < 3 && world.get(x + dirX *  dist,y + distY)->lifeTime >= 2)  {
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
    world.replaceNeighbors(x, y, AIR_ID, FIRE_ID);

    updateElementLifetime(x, y);
}

void Game::UpdateAsSteam(int x, int y) {
    updateElementLifetime(x, y);

    if (world.get(x,y)->lifeTime == 0) {
        world.replaceElementAt(x, y, STEAM_ID, WATER_ID);
        return;
    }

    MoveLikeGas(x, y);
}

void Game::UpdateAsAcidCloud(int x, int y) {
    updateElementLifetime(x, y);

    if (world.get(x,y)->lifeTime == 0) {
        world.replaceElementAt(x, y, ACIDCLOUD_ID, ACID_ID);
        return;
    }

    MoveLikeGas(x, y);
}

void Game::UpdateAsInfiniteSource(int x, int y) {
    std::uint8_t elementID = world.get(x,y)->customData; // element to create

    if (elementID == 0) {
        std::uint8_t *neighbors = world.getNeighborsIDs(x, y);

        for (int i = 0; i < 8; i++) {
            if (neighbors[i] != AIR_ID && neighbors[i] != INFINITESOURCE_ID) {
                world.get(x,y)->customData = neighbors[i];
                return;
            }
        }
        return;
    }
    world.replaceAdjacentNeighbors(x, y, AIR_ID, world.get(x,y)->customData);
}

void Game::UpdateAsPlant(int x, int y) {
    if (rand() % 2 == 0) {
        world.replaceNeighbors(x, y, WATER_ID, PLANT_ID);
        world.replaceNeighbors(x, y, STEAM_ID, PLANT_ID);
    } else {
        world.replaceNeighbors(x, y, WATER_ID, AIR_ID);
        world.replaceNeighbors(x, y, STEAM_ID, AIR_ID);
    }
}

void Game::UpdateAsAcid(int x, int y) {
    if (rand() % 20 < 3) {
        for (int i = 0; i < SIZE_ALWAYS_AT_END; i++) {
            if (i != AIR_ID && i != ACID_ID && i != FIRE_ID && i != INFINITESOURCE_ID && i != ACIDCLOUD_ID) {
                if (world.replaceNeighbors(x, y, i, AIR_ID)) {
                    world.placeElement(x, y, AIR_ID);
                    return;
                };
            }
        }
    }
    MoveLikeLiquid(x, y);
}

void Game::UpdateAsLava(int x, int y) {
    if (rand() % 25 <= 1 && world.hasNeighbor(x, y, WATER_ID)) {
        world.placeElement(x, y, ROCK_ID);
        return;
    }

    burnNeighbors(x, y);

    if (rand() % 100 <= 1) {
        world.replaceNeighbors(x, y, AIR_ID, FIRE_ID);
    }

   if (rand() % 200 <= 1) {
       world.replaceAdjacentNeighbors(x, y, ROCK_ID, LAVA_ID);
   }
   if (rand() % 400 <= 1) {
       world.replaceNeighbors(x, y, ROCK_ID, LAVA_ID);
   }

    MoveLikeLiquid(x, y);
}

void Game::UpdateAsGunpowder(int x, int y) {
    if (rand() % 75 <= 1) {
        if (world.hasNeighbor(x, y, FIRE_ID) || world.hasNeighbor(x, y, LAVA_ID) || world.hasNeighbor(x, y, WOODBURNING_ID)) {
            placeExplosion(x, y, rand() % 5 + 10);
        }
    }

    MoveLikePowder(x, y);
}

void Game::UpdateAsSalt(int x, int y) {
    if (rand() % 1 <= 1 && world.replaceNeighbors(x, y, WATER_ID, SALTWATER_ID)) {
        world.placeElement(x, y, AIR_ID);
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
    for (int y = 0; y < World::HEIGHT; y++) {
        bool e = y % 2 == 0;
        for (int x = e ? 0 : World::WIDTH - 1; e ? x < World::WIDTH : x >= 0; e ? x++ : x--) {
            if (world.get(x,y)->updated) continue;

            std::uint8_t curr = world.get(x,y)->elementID;

            if (world.get(x,y)->lifeTime <= 0) {
                world.placeElement(x, y, AIR_ID);
            }

            switch (curr) {
                case SAND_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsSand(x, y);
                break;
                case WATER_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsWater(x, y);
                break;case OIL_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsOil(x, y);
                break;
                case DIRT_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsSand(x, y);
                break;
                case FIRE_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsFire(x, y);
                    break;
                case WOODBURNING_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsBurningWood(x, y);
                    break;
                case STEAM_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsSteam(x, y);
                    break;
                case INFINITESOURCE_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsInfiniteSource(x, y);
                    break;
                case PLANT_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsPlant(x, y);
                    break;
                case ACID_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsAcid(x, y);
                    break;;
                case LAVA_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsLava(x, y);
                    break;
                case GUNPOWDER_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsGunpowder(x, y);
                    break;
                case SALT_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsSalt(x, y);
                    break;
                case SALTWATER_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsWater(x, y);
                    break;
                case ACIDCLOUD_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsAcidCloud(x, y);
                    break;
                case ROCK_ID:
                    world.get(x,y)->updated = true;
                    UpdateAsRock(x, y);
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::updateElementLifetime(int x, int y) {
    int element_id = world.get(x,y)->elementID;

    if (gameTime % ELEMENT_DECAY_RATES[element_id] == 0) {
        world.get(x,y)->lifeTime--;
    }
}

void Game::placeExplosion(int x, int y, int radius) {
    for (int i = -radius + x; i <= radius + x; i++) {
        for (int j = -radius + y; j <= radius + y; j++) {
            bool withinRadius = sqrt((x - i)*(x - i) + (y - j)*(y - j)) <= radius;

            if(withinRadius && world.inBounds(i, j) && world.get(i,j)->elementID != INFINITESOURCE_ID) {
                world.placeElement(i, j, FIRE_ID);
            }
        }
    }

}

void Game::burnNeighbors(int x, int y) {
    if(rand() % 5 < 2) return;
    world.replaceNeighbors(x, y, WOOD_ID, WOODBURNING_ID);
    world.replaceNeighbors(x, y, WATER_ID, STEAM_ID);
    world.replaceNeighbors(x, y, SALTWATER_ID, STEAM_ID);
    world.replaceNeighbors(x, y, PLANT_ID, FIRE_ID);
    world.replaceNeighbors(x, y, OIL_ID, FIRE_ID);
    world.replaceNeighbors(x, y, ACID_ID, ACIDCLOUD_ID);
}

void Game::brushElement(int x, int y, int radius, int ELEMENT_ID) {
    int prevX = World::cursor_pos_prev_x;
    int prevY = World::cursor_pos_prev_y;

    double xStep = (x - prevX) / ((radius + 1));
    double yStep = (y - prevY) / ((radius + 1));

    int steps = xStep != 0 ? abs((x - prevX) / xStep) : 1;
    steps += yStep != 0 ? abs((y - prevY) / yStep) : 1;
    for (int k = 0; k < steps + 3; k++) {
        int curX = prevX + xStep * k;
        int curY = prevY + yStep * k;

        for (int i = -radius + curX; i <= radius + curX; i++) {
            for (int j = -radius + curY; j <= radius + curY; j++) {
                bool withinRadius = std::sqrt((curX - i)*(curX - i) + (curY - j)*(curY - j)) <= radius;

                if(withinRadius && world.inBounds(i, j) && (rand() % 20 <= 1 || ELEMENT_ID == AIR_ID ||ELEMENT_ID == ROCK_ID || ELEMENT_ID == WOOD_ID)) {
                        world.placeElementDestructively(i, j, ELEMENT_ID);
                }
            }
        }
    }
}

void Game::Render() {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, World::WIDTH, World::HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, world.getWorldPixelBuffer());
    shaderProgram.use();
    glBindVertexArray(renderPlaneID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
