#ifndef DOG_H_INCLUDED
#define DOG_H_INCLUDED

#include <allegro5/bitmap.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>   // 如果使用 TTF 字型

class Dog
{
public:
    Dog() {};
    ~Dog();
    void init();
    void update();
    void draw();

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return dog1_img ? al_get_bitmap_width(dog1_img) * 0.4f : 50; }
    float getHeight() const { return dog1_img ? al_get_bitmap_height(dog1_img) * 0.4f : 50; }

    int num_dogs = 1;

    // --- 正確宣告：不要加 Dog:: ---
    void splitDogs(int total, int& n3, int& n2, int& n1);

    struct DogDrawInfo {
        ALLEGRO_BITMAP* bmp;
        float scale;
    };

    // --- 也不要加 Dog:: ---
    DogDrawInfo getDogInfo(int type);

private:
    ALLEGRO_BITMAP *dog1_img = nullptr;
    ALLEGRO_BITMAP *dog2_img = nullptr;
    ALLEGRO_BITMAP *dog3_img = nullptr;

    float x, y;

    float vy;
    float gravity;
    float jump_speed;
    bool on_ground;

    ALLEGRO_FONT* font = nullptr;  // 新增字體指標

    int jump_count;
    int max_jump;
};

#endif
