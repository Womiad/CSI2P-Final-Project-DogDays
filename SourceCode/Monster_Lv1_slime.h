#ifndef Monster_Lv1_SLIME_H
#define Monster_Lv1_SLIME_H

#include "Monster.h"

class Monster_Lv1_slime : public Monster
{
private:
    static ALGIF_ANIMATION* slime_gif;

public:
    Monster_Lv1_slime();
    
    void init(int startX, int startY) override;
    void load_gif() override;
    
    static void load_resources();
    static void unload_resources();
};

#endif