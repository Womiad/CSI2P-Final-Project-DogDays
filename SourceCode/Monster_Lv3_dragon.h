#ifndef Monster_Lv3_dragon_H
#define Monster_Lv3_dragon_H

#include "Monster.h"

class Monster_Lv3_dragon : public Monster
{
private:
    static ALGIF_ANIMATION* slime_gif;

public:
    Monster_Lv3_dragon();
    
    void init(int startX, int startY) override;
    void load_gif() override;
    
    static void load_resources();
    static void unload_resources();
};

#endif