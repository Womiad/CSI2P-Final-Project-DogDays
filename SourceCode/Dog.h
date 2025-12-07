#ifndef DOG_H_INCLUDED
#define DOG_H_INCLUDED

#include <allegro5/bitmap.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>   // 如果使用 TTF 字型
#include "Weapon.h"


class Dog
{
public:
    Dog();
    ~Dog();

    void init();
    void update();
    void draw();

    void addDogs(int v) { num_dogs += v; if (num_dogs < 1) num_dogs = 1; }
    // 狗的總數（以 dog1 單位）
    int num_dogs;

    float getX() const { return x; }
    float getY() const { return y; }

    float getWidth() const { return w; }
    float getHeight() const { return h;}


private:

    // ─────────── 每隻狗的資料 ───────────
    struct DogUnit {
        float x, y;
        float offset_x, offset_y;  // 新增這兩行
        int type;
        Weapon weapon;
    };

    std::vector<DogUnit> dogs;
    int prev_num_dogs;  // 新增這行

    // 主狗的位置（參考點）
    float x, y;

    float w, h;

    // 物理
    float vy;
    float gravity;
    float jump_speed;
    bool on_ground;

    int jump_count;
    int max_jump;

    // 資源
    ALLEGRO_BITMAP* dog1_img = nullptr;
    ALLEGRO_BITMAP* dog2_img = nullptr;
    ALLEGRO_BITMAP* dog3_img = nullptr;

    ALLEGRO_BITMAP* bow_img = nullptr;
    ALLEGRO_BITMAP* arrow_img = nullptr;

    ALLEGRO_FONT* font = nullptr;


    // 依數量分配各階層 dog1/dog2/dog3
    void splitDogs(int total, int& n3, int& n2, int& n1);

    // 重新產生 DogUnit
    void refreshDogUnits(int n1, int n2, int n3);

    // 畫狗時的資訊
    struct DogDrawInfo {
        ALLEGRO_BITMAP* bmp;
        float scale;
    };

    DogDrawInfo getDogInfo(int type);
};

#endif