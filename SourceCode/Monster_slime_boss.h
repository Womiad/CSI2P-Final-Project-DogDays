#ifndef Monster_slime_boss_H
#define Monster_slime_boss_H

#include "Monster.h"

class Monster_slime_boss : public Monster
{
private:
    static ALGIF_ANIMATION* slime_gif;

public:
    Monster_slime_boss();
    
    void init(int startX, int startY) override;
    void load_gif() override;
    
    static void load_resources();
    static void unload_resources();
};

#endif