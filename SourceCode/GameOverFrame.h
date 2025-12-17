#ifndef GameOverFrame_H
#define GameOverFrame_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>   // 如果使用 TTF 字型

class GameOverFrame {
public:
    void init();
    void update();
    void draw();

    void resetBtn();
    bool btn_clicked = false;
    
private:
    ALLEGRO_BITMAP* img = nullptr;
    ALLEGRO_BITMAP* backtoMenu = nullptr;
    ALLEGRO_FONT* font = nullptr;
};

#endif