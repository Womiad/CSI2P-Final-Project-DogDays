#ifndef MONSTERMANAGER_H
#define MONSTERMANAGER_H

#include <vector>
#include "Monster.h"

class MonsterManager
{
private:
    std::vector<Monster*> monsters;
    int frame_counter;
    int spawn_frame_interval;
    float hp_multiplier;  // ⭐ 新增：血量倍率

    Monster* createMonster(int level);

public:
    MonsterManager();
    ~MonsterManager();

    static void load_all_resources();
    static void unload_all_resources();

    void init();
    void update();
    void draw();

    // ⭐ 新增：設定怪物血量倍率
    void setHPMultiplier(float multiplier) { hp_multiplier = multiplier; }
    float getHPMultiplier() const { return hp_multiplier; }
    
    // ⭐ 新增：根據玩家等級計算血量倍率
    void updateHPByPlayerLevel(int playerLevel);

    const std::vector<Monster*>& getMonsters() const { return monsters; }
    void clear();
};

#endif