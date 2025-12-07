#ifndef ARROW_H
#define ARROW_H

#include <allegro5/allegro.h>

class Arrow {
public:
    void init(float startX, float startY, float sp, ALLEGRO_BITMAP* bmp, float sc = 1.0f);
    void update();
    void draw();
    float getX() const;
    bool isOut() const;
    
private:
    float x, y;
    float speed;
    float scale = 1.0f;
    ALLEGRO_BITMAP* img = nullptr;
};

#endif