// Dog.h
#ifndef DOG_H_INCLUDED
#define DOG_H_INCLUDED

#include <allegro5/bitmap.h>

class Dog
{
public:
    Dog() {}
    void init();
    void update();
    void draw();

private:
    ALLEGRO_BITMAP *dog1_img;

    float x, y;          // 建議改成 float 才不會跳躍卡卡

    float vy;            // 垂直速度
    float gravity;       // 重力
    float jump_speed;    // 跳躍初速度
    bool on_ground;      // 是否在地上

    int jump_count;   // 已跳躍次數（0、1、2）
    int max_jump;     // 最大跳躍次數（例如 2）


    int width;     // 圖片寬
    int height;    // 圖片高
};

#endif