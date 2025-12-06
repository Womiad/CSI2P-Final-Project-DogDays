#include "BlockManager.h"
#include "Block.h"
#include "data/DataCenter.h"
#include <allegro5/allegro_primitives.h>
#include <algorithm>   // ✅ 加上這行

void BlockManager::init() {
    blocks.clear();
    
    spawnFrame = 0;
    spawnFrameInterval = 120; // 60 FPS → 30 frame = 0.5 秒

    // spawnTimer = 0;
    // spawnInterval = 0.5f; // 0.5 秒產生一個

    midY = 250;
    highY = 50;
}
void BlockManager::update() {
    spawnFrame++;

    if (spawnFrame >= spawnFrameInterval) {
        spawnFrame = 0;

        Block b;
        int startX = 1280;
        int y = (rand() % 2 == 0 ? midY : highY);

        // 【隨機 type】
        BLOCKTYPE t;
        int r = rand() % 3;
        if (r == 0) t = BLOCKTYPE::ADD;
        else if (r == 1) t = BLOCKTYPE::SUB;
        else t = BLOCKTYPE::MUL;

        // 【隨機數字】
        int val = (rand() % 9) + 1;

        b.init(startX, y, 100, 225, 8, t, val);
        blocks.push_back(b);
    }


    for (auto &b : blocks)
        b.update();

    blocks.erase(
    std::remove_if(blocks.begin(), blocks.end(),
                   [](const Block& b) { return b.getX() + b.getWidth() < 0; }),
    blocks.end()
);
}



void BlockManager::draw() {
    for (auto &b : blocks)
        b.draw();
}