#include "Monster.h"
#include "Dog.h"  // ⭐ 引入 Dog 標頭檔
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include "data/DataCenter.h"

Monster::Monster()
{
    x = 1200;
    y = 700;
    width = height = 50;
    speed = 2;
    hp = hp_max = base_hp = 10;
    scale = 1.0f;
    gif = nullptr;
    
    // ⭐ 初始化新成員
    damagePerSecond = 1;        // 預設每秒扣 1 隻狗
    damageTimer = 0.0f;
    hasReachedTarget = false;
}

Monster::~Monster()
{
}

void Monster::setHPMultiplier(float multiplier)
{
    hp_max = (int)(base_hp * multiplier);
    hp = hp_max;
    updateScale();
}

void Monster::setHP(int newHP)
{
    hp_max = newHP;
    hp = hp_max;
    updateScale();
}

void Monster::updateScale()
{
    // 方案 2: 平方根縮放（較溫和，推薦）
    scale = sqrt((float)hp_max / base_hp);
    
    // 限制最大縮放
    if (scale > 3.0f) scale = 3.0f;
    if (scale < 0.5f) scale = 0.5f;
}

// ⭐ 新增：設定每秒傷害
void Monster::setDamagePerSecond(int damage)
{
    damagePerSecond = damage;
}

// ⭐ 新增：預設的 init 實作（子類別可以覆寫）
void Monster::init(int startX, int startY)
{
    x = startX;
    y = startY;
}

// ⭐ 新增：預設的 load_gif 實作（子類別可以覆寫）
void Monster::load_gif()
{
    // 預設為空，子類別自行實作
}

void Monster::update(Dog* player)
{
    DataCenter* DC = DataCenter::get_instance();
    int score = DC->nowScore;
    int weaponLevel = DC->weaponLevel;

    // 1️⃣ 移動：走到 x = 150 就停
    if (x > 150)
    {
        x -= speed;
        if (x < 150) x = 150;
        hasReachedTarget = false;  // 還在移動中
    }
    else
    {
        hasReachedTarget = true;  // ⭐ 已到達攻擊位置
    }

    // 2️⃣ 每 frame 扣血（怪物受到玩家傷害）
    float damage = (float)score / 61.0f * weaponLevel;
    
    if(DC->playerLevel >= 4){
        damage = (float)score / 61.0f * weaponLevel / DC->playerLevel / DC->playerLevel;
    }else if(DC->playerLevel == 3){  
        damage = (float)score / 61.0f * std::sqrt(weaponLevel) / std::sqrt(DC->playerLevel);
    }else if(DC->playerLevel == 2){  
        damage = (float)score / 61.0f * std::sqrt(weaponLevel) / std::sqrt(std::sqrt(DC->playerLevel)) * 3;
    }else{  
        damage = (float)score / 61.0f * weaponLevel;
    }
    hp -= damage;

    if (hp < 0) hp = 0;

    // 3️⃣ ⭐ 如果到達目標位置，開始對玩家造成傷害
    if (hasReachedTarget && player != nullptr)
    {
        // 假設遊戲是 60 FPS，每 frame 增加 1/60 秒
        damageTimer += 1.0f / 60.0f;
        
        // 每累積 1 秒，扣除玩家血量
        if (damageTimer >= 1.0f)
        {
            player->num_dogs -= damagePerSecond;
            
            // 防止血量變負數
            if (player->num_dogs < 0)
                player->num_dogs = 0;
            
            // 重置計時器（保留小數部分，避免誤差累積）
            damageTimer -= 1.0f;
        }
    }
    else
    {
        // 如果還沒到達或玩家不存在，重置計時器
        damageTimer = 0.0f;
    }
}

void Monster::draw()
{
    if (!gif) return;
    
    ALLEGRO_BITMAP* bmp = algif_get_bitmap(gif, al_get_time());
    if (!bmp) return;
    
    float scaledWidth = width * scale;
    float scaledHeight = height * scale;
    
    al_draw_scaled_bitmap(
        bmp,
        0, 0,
        width, height,
        x, y - scaledHeight,
        scaledWidth, scaledHeight,
        0
    );
    
    drawHealthBar();
}

void Monster::drawHealthBar()
{
    float scaledWidth = width * scale;
    float scaledHeight = height * scale;
    
    float barW = scaledWidth;
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