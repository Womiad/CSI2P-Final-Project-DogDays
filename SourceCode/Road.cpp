// Road.cpp
#include "Road.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

constexpr char road_img_path[] = "./assets/image/road.png";

void Road::init() {
    // DataCenter *DC = DataCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();

    // ✅ 錯誤處理：檢查圖片是否載入成功
    road_img = IC->get(road_img_path);
    if (!road_img) {
        debug_log("ERROR: Failed to load road image!\n");
        // 創建一個簡單的替代圖案
        road_img = al_create_bitmap(1280, 720);
        if (road_img) {
            ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();
            al_set_target_bitmap(road_img);
            al_clear_to_color(al_map_rgb(50, 50, 50));
            al_set_target_bitmap(prev_target);
        }
    }
    
    width  = al_get_bitmap_width(road_img);
    height = al_get_bitmap_height(road_img);
    
    // ✅ 驗證圖片尺寸
    if (width <= 0 || height <= 0) {
        debug_log("ERROR: Invalid road image dimensions!\n");
        width = 1280;
        height = 720;
    }
    
    offset = 0;
    speed = 10;
    state = STATE::MOVE;
    
    debug_log("<Road> Initialized: %dx%d, speed=%d\n", width, height, speed);
}

void Road::update() {
    int overlap = 13;

    // 每幀增加 speed
    offset += speed;

    // 防止 offset 無限增長，並考慮重疊
    int effective_width = width - overlap;   // 實際移動循環寬度
    offset %= effective_width;
}

void Road::draw() {
    int overlap = 13;                   // 重疊 10px
    int x = -(offset % width);

    // 左圖：比右邊多畫 overlap
    al_draw_bitmap(road_img, x - width + overlap, 0, 0);
    // 中圖
    al_draw_bitmap(road_img, x, 0, 0);
    // 右圖：比左邊少畫 overlap
    al_draw_bitmap(road_img, x + width - overlap, 0, 0);
}
