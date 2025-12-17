#include "MonsterManager.h"
#include "Monster_Lv1_slime.h"
#include "Monster_Lv2_zombie.h"
#include "Monster_Lv3_dragon.h"
#include "Monster_Lv4_wizard.h"
#include "Monster_slime_boss.h"
// ... 其他怪物

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "data/DataCenter.h"

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
    Monster_slime_boss::load_resources();
    // 其他怪物...
}

void MonsterManager::unload_all_resources()
{
    Monster_Lv1_slime::unload_resources();
    Monster_Lv2_zombie::unload_resources();
    Monster_Lv3_dragon::unload_resources();
    Monster_Lv4_wizard::unload_resources();
    Monster_slime_boss::unload_resources();
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
    // hp_multiplier = pow(3.0f, playerLevel - 1);
    
    // 方案 3: 階梯式成長
    // if (playerLevel <= 5)
    //     hp_multiplier = 1.0f;
    // else if (playerLevel <= 10)
    //     hp_multiplier = 2.0f;
    // else
    //     hp_multiplier = 3.0f;

    DataCenter *DC = DataCenter::get_instance();

    if(DC -> playerLevel >= 4){
        hp_multiplier = std::sqrt(DC -> playerHighestScore) * std::sqrt(DC->weaponLevel) * 2;
    }else if(DC -> playerLevel == 3){  
        hp_multiplier = std::sqrt(DC -> playerHighestScore) * std::sqrt(DC->weaponLevel) * 1.5;
    }else if(DC -> playerLevel == 2){  
        hp_multiplier = std::sqrt(DC -> playerHighestScore) * std::sqrt(DC->weaponLevel);
    }else{  
        hp_multiplier = std::sqrt(DC -> playerHighestScore) * std::sqrt(DC->weaponLevel);
    }
}

Monster* MonsterManager::createMonster(int level)
{
    Monster* monster = nullptr;
    
    std::cout << "create monster!\n";

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
        case 5:
            monster = new Monster_slime_boss();
            break;
        default:
            monster = new Monster_Lv1_slime();
            break;
    }
    
    return monster;
}

void MonsterManager::update(Dog * player)
{
    DataCenter *DC = DataCenter::get_instance();
    frame_counter++;

    if (frame_counter >= spawn_frame_interval)
    {
        frame_counter = 0;

        int level = rand() % (DC->playerLevel) + 1;
        
        Monster* monster = createMonster(level);
        
        int startX = 1200;
        int startY = 700;
        
        monster->init(startX, startY);
        
        // ⭐ 生成時套用血量倍率
        updateHPByPlayerLevel(0);
        monster->setHPMultiplier(hp_multiplier);
        
        monsters.push_back(monster);
    }

    for (auto* m : monsters)
    {
        m->update(player);
    }

    auto it = monsters.begin();
    while (it != monsters.end())
    {
        if ((*it)->getX() < -200 || (*it)->isDead())
        {
            delete *it;
            it = monsters.erase(it);
            DC -> monsterKills += 1;
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