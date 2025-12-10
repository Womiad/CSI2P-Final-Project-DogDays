#ifndef Monster_Lv2_zombie_H
#define Monster_Lv2_zombie_H

#include "Monster.h"

class Monster_Lv2_zombie : public Monster
{
private:
    static ALGIF_ANIMATION* slime_gif;

public:
    Monster_Lv2_zombie();
    
    void init(int startX, int startY) override;
    void load_gif() override;
    
    static void load_resources();
    static void unload_resources();
};

#endif