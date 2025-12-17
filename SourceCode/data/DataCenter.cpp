#include "DataCenter.h"
#include <cstring>
// #include "../Level.h"
// #include "../Player.h"
// #include "../monsters/Monster.h"
// #include "../towers/Tower.h"
// #include "../towers/Bullet.h"

// fixed settings
namespace DataSetting {
	constexpr double FPS = 60;
	constexpr int window_width = 1280;
	constexpr int window_height = 832;
	constexpr int game_field_length = 600;
}

DataCenter::DataCenter() {
    this->FPS = DataSetting::FPS;
    this->window_width = DataSetting::window_width;
    this->window_height = DataSetting::window_height;
    this->game_field_length = DataSetting::game_field_length;

    delta_time = 0;
    last_time = al_get_time();
    
    memset(key_state, false, sizeof(key_state));
    memset(prev_key_state, false, sizeof(prev_key_state));
    mouse = Point(0, 0);
    memset(mouse_state, false, sizeof(mouse_state));
    memset(prev_mouse_state, false, sizeof(prev_mouse_state));

    playerHighestScore = 0;
    nowScore = 1;
    weaponLevel = 1;
    idelWeaponLevel = 1;
    playerLevel = 1;
    monsterKills = 0;

    // player = new Player();
    // level = new Level();
}

void DataCenter::resetPlayerData(){
    playerHighestScore = 0;
    nowScore = 1;
    weaponLevel = 1;
    idelWeaponLevel = 1;
    playerLevel = 1;
    monsterKills = 0;
}


DataCenter::~DataCenter() {
	// delete player;
	// delete level;
	// for(Monster *&m : monsters) {
	// 	delete m;
	// }
	// for(Tower *&t : towers) {
	// 	delete t;
	// }
	// for(Bullet *&b : towerBullets) {
	// 	delete b;
	// }
}

void DataCenter::update_delta_time() {
    double current = al_get_time();
    delta_time = current - last_time;   // 上一幀到這一幀的秒數
    last_time = current;

    if(playerHighestScore <= 50) playerLevel = 1;
    else if(playerHighestScore > 50 && playerHighestScore <= 9000) playerLevel = 2;
    else if(playerHighestScore > 9000 && playerHighestScore <= 225000) playerLevel = 3;
    else if(playerHighestScore > 225000 && playerHighestScore <= 500000) playerLevel = 4;
    else if(playerHighestScore > 500000) playerLevel = 5;
    // else if(playerHighestScore > 3200 && playerHighestScore < 12800) playerLevel = 5;
}
