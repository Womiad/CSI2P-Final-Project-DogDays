#include "Monster.h"
#include <allegro5/allegro_primitives.h>
#include <cmath>

Monster::Monster()
{
    x = y = 0;
    width = height = 50;
    speed = 2;
    hp = hp_max = base_hp = 10;
    scale = 1.0f;  // ⭐ 初始縮放比例為 1
    gif = nullptr;
}

Monster::~Monster()
{
}

void Monster::setHPMultiplier(float multiplier)
{
    hp_max = (int)(base_hp * multiplier);
    hp = hp_max;
    updateScale();  // ⭐ 血量改變時更新縮放
}

void Monster::setHP(int newHP)
{
    hp_max = newHP;
    hp = hp_max;
    updateScale();  // ⭐ 血量改變時更新縮放
}

// ⭐ 根據血量計算縮放比例
void Monster::updateScale()
{
    // 方案 1: 線性縮放（血量翻倍，體積翻倍）
    // scale = (float)hp_max / base_hp;
    
    // 方案 2: 平方根縮放（較溫和，推薦）
    // 血量 x4 時，體積 x2
    scale = sqrt((float)hp_max / base_hp);
    
    // 方案 3: 立方根縮放（更溫和）
    // scale = pow((float)hp_max / base_hp, 1.0f / 3.0f);
    
    // 方案 4: 對數縮放（防止過大）
    // scale = 1.0f + log10((float)hp_max / base_hp);
    
    // 限制最大縮放（防止太大）
    if (scale > 3.0f) scale = 3.0f;
    if (scale < 0.5f) scale = 0.5f;
}

void Monster::update()
{
    x -= speed;
}

void Monster::draw()
{
    if (!gif) return;
    
    ALLEGRO_BITMAP* bmp = algif_get_bitmap(gif, al_get_time());
    if (!bmp) return;
    
    // ⭐ 使用縮放繪製
    float scaledWidth = width * scale;
    float scaledHeight = height * scale;
    
    al_draw_scaled_bitmap(
        bmp,
        0, 0,                           // 來源 x, y
        width, height,                  // 來源寬高（原始大小）
        x, y - scaledHeight,           // 目標 x, y
        scaledWidth, scaledHeight,     // 目標寬高（縮放後）
        0                               // flags
    );
    
    drawHealthBar();
}

void Monster::drawHealthBar()
{
    float scaledWidth = width * scale;
    float scaledHeight = height * scale;
    
    float barW = scaledWidth;  // ⭐ 血條寬度也跟著縮放
    float barH = 8;
    float barX = x;
    float barY = y - scaledHeight - 15;
    
    // 外框
    al_draw_rectangle(barX, barY, barX + barW, barY + barH, 
                      al_map_rgb(0,0,0), 2);
    
    // 血量
    float hpPercent = (float)hp / hp_max;
    al_draw_filled_rectangle(barX, barY, barX + barW * hpPercent, barY + barH, 
                             al_map_rgb(255,0,0));
}