// MonsterManager.h
#ifndef MONSTERMANAGER_H
#define MONSTERMANAGER_H

#include "Monster.h"
#include <vector>

class MonsterManager
{
private:
    std::vector<Monster> monsters;
    int frame_counter;           // 幀計數器
    int spawn_frame_interval;    // 生成間隔（幀數）

public:
    MonsterManager();
    
    void init();
    void update();
    void draw();
    
    std::vector<Monster>& getMonsters() { return monsters; }
    void setSpawnInterval(int frames) { spawn_frame_interval = frames; }
};

#endif