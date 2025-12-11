#include "BlockManager.h"
#include "Block.h"
#include "data/DataCenter.h"
#include <allegro5/allegro_primitives.h>
#include "data/ImageCenter.h"
#include <algorithm>   // ✅ 加上這行

#include <iostream>

constexpr char weapon2_img_path[] = "./assets/image/weapon/weapon_lv2.png";
constexpr char weapon3_img_path[] = "./assets/image/weapon/weapon_lv3.png";
constexpr char weapon4_img_path[] = "./assets/image/weapon/weapon_lv4.png";
constexpr char weapon5_img_path[] = "./assets/image/weapon/weapon_lv5.png";
constexpr char weapon6_img_path[] = "./assets/image/weapon/weapon_lv6.png";
constexpr char weapon7_img_path[] = "./assets/image/weapon/weapon_lv7.png";

void BlockManager::init() {

    ImageCenter* IC = ImageCenter::get_instance();

    blocks.clear();
    
    spawnFrame = 0;
    spawnFrameInterval = 120; // 60 FPS → 30 frame = 0.5 秒

    midY = 250;
    highY = 50;

    Block::setupBitmapResource(
        IC -> get(weapon2_img_path),
        IC -> get(weapon3_img_path),
        IC -> get(weapon4_img_path),
        IC -> get(weapon5_img_path),
        IC -> get(weapon6_img_path),
        IC -> get(weapon7_img_path)
    );
}
void BlockManager::update(Dog* dog)
{
    spawnFrame++;

    DataCenter* DC = DataCenter::get_instance();

    int currentWeaponLevel = DC->weaponLevel;
    int idelWeaponLevel    = DC->idelWeaponLevel;

    // ⭐ 檢查目前是否已有武器升級方塊
    bool hasWeaponBlock = false;
    for (auto &b : blocks) {
        if (b.getType() == BLOCKTYPE::WEAPON_LEVEL_UP) {
            hasWeaponBlock = true;
            break;
        }
    }

    if (spawnFrame >= spawnFrameInterval) {

        spawnFrame = 0;

        Block b;

        int startX = 1280;
        int y = (rand() % 2 == 0 ? midY : highY);

        // ⭐ 建立 type pool
        std::vector<BLOCKTYPE> pool = {
            BLOCKTYPE::ADD,
            BLOCKTYPE::SUB,
            BLOCKTYPE::MUL
        };

        // ⭐ 把武器升級加入 pool（只有兩個條件都成立時）
        if (currentWeaponLevel < idelWeaponLevel && !hasWeaponBlock) {
            pool.push_back(BLOCKTYPE::WEAPON_LEVEL_UP);
        }

        // ⭐ 從 pool 隨機取一個
        BLOCKTYPE t = pool[rand() % pool.size()];


        // ======= 隨機數字 =======
        int rem = 3;
        if (dog->num_dogs < 10) rem = 3;
        else if (dog->num_dogs < 20) rem = 6;
        else if (dog->num_dogs < 50) rem = 8;
        else rem = 10;

        int val = (rand() % rem) + 1;

        if (dog->num_dogs >= 25) {
            if (t == BLOCKTYPE::SUB) val *= (dog->num_dogs / 2);
        }


        // ⭐ 武器升級 block 的 value = 下一個要顯示的武器等級
        if (t == BLOCKTYPE::WEAPON_LEVEL_UP) {
            val = currentWeaponLevel + 1;  // 例如 2→3→4...
            if (val > idelWeaponLevel)
                val = idelWeaponLevel;
        }

        // 建立 block
        b.init(startX, y, 100, 225, 8, t, val);
        blocks.push_back(b);
    }

    // block 更新
    for (auto &b : blocks)
        b.update();

    // 移除離開畫面的 block
    blocks.erase(
        std::remove_if(blocks.begin(), blocks.end(),
            [](const Block& b) { return b.getX() + b.getWidth() < 0; }),
        blocks.end()
    );
}


void BlockManager::checkCollision(Dog* dog) {
    DataCenter *DC = DataCenter::get_instance();
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

        // ⭐ 加入詳細的 debug 訊息

        // std::cout << "=== Collision Check ===" << "\n";
        // std::cout << "Block: x=" << bx << ", y=" << by 
        //           << ", w=" << bw << ", h=" << bh << "\n";
        // std::cout << "Block bounds: left=" << bx << ", right=" << (bx + bw)
        //           << ", top=" << by << ", bottom=" << (by + bh) << "\n";
        // std::cout << "Dog: x=" << dx << ", y=" << dy 
        //           << ", w=" << dw << ", h=" << dh << "\n";
        // std::cout << "Dog bounds: left=" << dx << ", right=" << (dx + dw)
        //           << ", top=" << dy << ", bottom=" << (dy + dh) << "\n";

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
                case BLOCKTYPE::WEAPON_LEVEL_UP:
                    DC -> weaponLevel = DC -> idelWeaponLevel;
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