#include "MonsterManager.h"
#include "Monster_Lv1_slime.h"
#include "Monster_Lv2_zombie.h"
#include "Monster_Lv3_dragon.h"
#include "Monster_Lv4_wizard.h"
// ... 其他怪物

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

MonsterManager::MonsterManager()
{
    frame_counter = 0;
    spawn_frame_interval = 180;
    hp_multiplier = 1.0f;  // ⭐ 初始倍率為 1
    srand(time(nullptr));
}

MonsterManager::~MonsterManager()
{
    clear();
}

void MonsterManager::load_all_resources()
{
    Monster_Lv1_slime::load_resources();
    Monster_Lv2_zombie::load_resources();
    Monster_Lv3_dragon::load_resources();
    Monster_Lv4_wizard::load_resources();
    // 其他怪物...
}

void MonsterManager::unload_all_resources()
{
    Monster_Lv1_slime::unload_resources();
    Monster_Lv2_zombie::unload_resources();
    Monster_Lv3_dragon::unload_resources();
    Monster_Lv4_wizard::unload_resources();
    // 其他怪物...
}

void MonsterManager::init()
{
    clear();
    frame_counter = 0;
    hp_multiplier = 1.0f;
}

void MonsterManager::clear()
{
    for (auto* m : monsters)
    {
        delete m;
    }
    monsters.clear();
}

// ⭐ 根據玩家等級自動計算血量倍率 (未啟用)
void MonsterManager::updateHPByPlayerLevel(int playerLevel)
{
    // 範例公式：每提升一級，怪物血量增加 20%
    // 你可以根據遊戲平衡調整這個公式
    
    // 方案 1: 線性成長
    // hp_multiplier = 1.0f + (playerLevel - 1) * 0.2f;
    
    // 方案 2: 指數成長
    hp_multiplier = pow(1.15f, playerLevel - 1);
    
    // 方案 3: 階梯式成長
    // if (playerLevel <= 5)
    //     hp_multiplier = 1.0f;
    // else if (playerLevel <= 10)
    //     hp_multiplier = 2.0f;
    // else
    //     hp_multiplier = 3.0f;
}

Monster* MonsterManager::createMonster(int level)
{
    Monster* monster = nullptr;
    
    switch (level)
    {
        case 1:
            monster = new Monster_Lv1_slime();
            break;
        case 2:
            monster = new Monster_Lv2_zombie();
            break;
        case 3:
            monster = new Monster_Lv3_dragon();
            break;
        case 4:
            monster = new Monster_Lv4_wizard();
            break;
        default:
            monster = new Monster_Lv1_slime();
            break;
    }
    
    return monster;
}

void MonsterManager::update()
{
    frame_counter++;

    if (frame_counter >= spawn_frame_interval)
    {
        frame_counter = 0;

        int level = rand() % 4 + 1;
        
        Monster* monster = createMonster(level);
        
        int startX = 2000;
        int startY = 700;
        
        monster->init(startX, startY);
        
        // ⭐ 生成時套用血量倍率
        monster->setHPMultiplier(hp_multiplier);
        
        monsters.push_back(monster);
    }

    for (auto* m : monsters)
    {
        m->update();
    }

    auto it = monsters.begin();
    while (it != monsters.end())
    {
        if ((*it)->getX() < -200 || (*it)->isDead())
        {
            delete *it;
            it = monsters.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void MonsterManager::draw()
{
    for (auto* m : monsters)
    {
        m->draw();
    }
}