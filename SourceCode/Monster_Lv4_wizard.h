#ifndef Monster_Lv4_wizard_H
#define Monster_Lv4_wizard_H

#include "Monster.h"

class Monster_Lv4_wizard : public Monster
{
private:
    static ALGIF_ANIMATION* slime_gif;

public:
    Monster_Lv4_wizard();
    
    void init(int startX, int startY) override;
    void load_gif() override;
    
    static void load_resources();
    static void unload_resources();
};

#endif