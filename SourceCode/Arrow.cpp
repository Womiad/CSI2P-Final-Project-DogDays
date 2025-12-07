#include "Arrow.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

// 注意：這裡的函數簽名必須和 Arrow.h 中的宣告一致
void Arrow::init(float startX, float startY, float sp, ALLEGRO_BITMAP* bmp, float sc)
{
    x = startX;
    y = startY;
    speed = sp;
    img = bmp;
    scale = sc;
}

void Arrow::update()
{
    x += speed;
}

void Arrow::draw()
{
    if (!img) {
        // 如果沒有圖片，畫一個簡單的箭頭形狀便於除錯
        al_draw_filled_circle(x, y, 3 * scale, al_map_rgb(255, 0, 0));
        return;
    }

    int w = al_get_bitmap_width(img);
    int h = al_get_bitmap_height(img);
    
    // 根據 scale 縮放箭的大小
    al_draw_scaled_bitmap(
        img, 0, 0, w, h,
        x, y,
        w * scale, h * scale,
        0
    );
}

float Arrow::getX() const
{
    return x;
}

bool Arrow::isOut() const
{
    return x > 1600;  // 超出螢幕右邊界
}