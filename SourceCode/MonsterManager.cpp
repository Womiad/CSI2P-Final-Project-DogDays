// MonsterManager.cpp
#include "MonsterManager.h"
#include <cstdlib>
#include <algorithm>

MonsterManager::MonsterManager()
{
    frame_counter = 0;
    spawn_frame_interval = 600;
}

void MonsterManager::init()
{
    monsters.clear();
    frame_counter = 0;
}

void MonsterManager::update()
{
    // 幀計數器增加
    frame_counter++;

    // 每600幀生成一隻
    if (frame_counter >= spawn_frame_interval)
    {
        frame_counter = 0;

        Monster m;

        // 隨機選一個怪物種類
        int r = rand() % 1;  
        MonsterTYPE t = MonsterTYPE::LV1_SLIME;

        // 如果之後有更多類型可以這樣：
        /*
        switch (r) {
            case 0: t = MonsterTYPE::LV1_SLIME; break;
            case 1: t = MonsterTYPE::LV2; break;
            case 2: t = MonsterTYPE::LV3; break;
            case 3: t = MonsterTYPE::LV4; break;
            case 4: t = MonsterTYPE::LV5; break;
        }
        */

        // 生成的位置（畫面右邊）
        int startX = 2000;
        int startY = 700;   // 你的地面位置

        m.init(startX, startY, t);

        monsters.push_back(m);
    }

    // 更新怪物
    for (auto& m : monsters)
    {
        m.update();
    }

    // 移除走出畫面的怪
    monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(),
            [](const Monster& m){ return m.getX() < -200; }),
        monsters.end()
    );
}

void MonsterManager::draw()
{
    for (auto& m : monsters)
        m.draw();
}