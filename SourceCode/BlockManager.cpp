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
void BlockManager::update(Dog* dog) {
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
        int rem = 3;
        if(dog -> num_dogs < 10) rem = 3;
        else if(dog -> num_dogs < 20) rem = 6;
        else if(dog -> num_dogs < 50) rem = 8;
        else rem = 10;

        int val = (rand() % rem) + 1;

        if(dog -> num_dogs >= 25 ){
            if(t == BLOCKTYPE::SUB) val *= (dog -> num_dogs / 2);
        }

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

void BlockManager::checkCollision(Dog* dog) {
    for (auto &b : blocks) {
        // 1. 取得 block 和 dog 的矩形
        int bx = b.getX();
        int by = b.getY();
        int bw = b.getWidth();
        int bh = b.getHeight();

        float dx = dog->getX();
        float dy = dog->getY();
        float dw = dog->getWidth();
        float dh = dog->getHeight();

        // 2. 簡單 AABB 碰撞
        bool collided = !(dx + dw < bx || dx > bx + bw || dy + dh < by || dy > by + bh);

        if (collided) {
            // 3. 根據 type 做對應動作
            switch (b.getType()) {
                case BLOCKTYPE::ADD:
                    dog->num_dogs += b.getValue();
                    break;
                case BLOCKTYPE::SUB:
                    dog->num_dogs -= b.getValue();
                    if(dog->num_dogs < 1) dog->num_dogs = 1;
                    break;
                case BLOCKTYPE::MUL:
                    dog->num_dogs *= b.getValue();
                    break;
            }

            // 4. 碰撞後把 block 移出畫面（避免重複觸發）
            // 也可以設置一個狀態如 "COLLIDED" 之類
            b.init(-b.getWidth(), by, bw, bh, b.getValue(), b.getType(), b.getValue());
        }
    }
}




void BlockManager::draw() {
    for (auto &b : blocks)
        b.draw();
}