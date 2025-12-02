// Road.h
#ifndef ROAD_H_INCLUDED
#define ROAD_H_INCLUDED

#include <allegro5/bitmap.h>

class Road
{
public:
    Road() {}
    void init();
    void update();
    void draw();

	int get_offset() const { return offset; }

private:
    enum class STATE {
        MOVE,
        PAUSE
    };
    STATE state;

    ALLEGRO_BITMAP *road_img;

    int offset;    // 總偏移量（像素）
    int speed;     // 捲動速度
    int width;     // 圖片寬
    int height;    // 圖片高
};

#endif