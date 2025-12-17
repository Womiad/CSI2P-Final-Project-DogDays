#include "GameOverFrame.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "data/ImageCenter.h"
#include "data/DataCenter.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>   // 如果使用 TTF 字型

void GameOverFrame::init()
{
    ImageCenter* IC = ImageCenter::get_instance();
    img = IC -> get("./assets/image/gameEnd.png");
    backtoMenu = IC -> get("./assets/image/back2Menu.png");
    btn_clicked = false;
    if (!font) { 
        font = al_load_ttf_font("./assets/font/Caviar_Dreams_Bold.ttf", 32, 0);
        if (!font) font = al_create_builtin_font(); // fallback
    }
}

void GameOverFrame::update()
{
    DataCenter* DC = DataCenter::get_instance();

    if (!backtoMenu) return;

    // 1️⃣ 計算 backtoMenu 按鈕範圍（跟 draw 一樣）
    int btn_w = al_get_bitmap_width(backtoMenu);
    int btn_h = al_get_bitmap_height(backtoMenu);

    float btn_x = (DC->window_width - btn_w) / 2.0f;
    float btn_y = (DC->window_height - btn_h) / 2.0f + 200;

    // 2️⃣ 滑鼠位置
    int mx = DC->mouse.x;
    int my = DC->mouse.y;

    // 3️⃣ 是否在按鈕範圍內
    bool hover =
        mx >= btn_x && mx <= btn_x + btn_w &&
        my >= btn_y && my <= btn_y + btn_h;

    // 4️⃣ 滑鼠左鍵點擊（只觸發一次）
    if (hover && DC->mouse_state[1] && !btn_clicked)
    {
        btn_clicked = true;

        // TODO：切換回 Menu（先印 log 也可以）
        // DC->game_state = MENU;
        // 或呼叫你的 Scene / State Manager
    }

    // 5️⃣ 放開滑鼠後重置（避免卡住）
    if (!DC->mouse_state[1])
    {
        btn_clicked = false;
    }
}

void GameOverFrame::resetBtn(){
    btn_clicked = false;
}

void GameOverFrame::draw()
{
    DataCenter *DC = DataCenter::get_instance();
    // 1️⃣ 畫半透明黑色背景
    al_draw_filled_rectangle(
        0, 0,
        DC->window_width, DC->window_height,
        al_map_rgba(0, 0, 0, 160)  // 半透明黑
    );

    // 2️⃣ 畫 Game Over 圖片（置中）
    if (img)
    {
        int img_w = al_get_bitmap_width(img);
        int img_h = al_get_bitmap_height(img);

        float x = (DC->window_width - img_w) / 2.0f;
        float y = (DC->window_height - img_h) / 2.0f - 175; // 稍微往上留空間給按鈕

        al_draw_bitmap(img, x, y, 0);
    }
    if (font)
    {
        float text_x = DC->window_width / 2.0f;
        float text_y = DC->window_height / 2.0f + 20;

        std::string scoreText =
            "Highest Score: " + std::to_string(DC->playerHighestScore);

        std::string killText =
            "Kills: " + std::to_string(DC->monsterKills);

        al_draw_text(
            font,
            al_map_rgb(255, 255, 255),
            text_x,
            text_y,
            ALLEGRO_ALIGN_CENTER,
            scoreText.c_str()
        );

        al_draw_text(
            font,
            al_map_rgb(255, 255, 255),
            text_x,
            text_y + 48,
            ALLEGRO_ALIGN_CENTER,
            killText.c_str()
        );
    }

    if (backtoMenu)
    {
        int img_w = al_get_bitmap_width(backtoMenu);
        int img_h = al_get_bitmap_height(backtoMenu);

        float x = (DC->window_width - img_w) / 2.0f;
        float y = (DC->window_height - img_h) / 2.0f + 200; 

        al_draw_bitmap(backtoMenu, x, y, 0);
    }
}

