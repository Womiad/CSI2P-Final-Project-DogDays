#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro.h>
#include "UI.h"
#include "Road.h"
#include "Dog.h"

#include "BlockManager.h"
#include "MonsterManager.h"
#include "Monster.h"

/**
 * @brief Main class that runs the whole game.
 * @details All game procedures must be processed through this class.
 */
class Game
{
public:
	void execute();
public:
	Game(bool testMode = false);
	~Game();
	void game_init();
	bool game_update();
	void game_draw();
private:
	/**
	 * @brief States of the game process in game_update.
	 * @see Game::game_update()
	 */
	enum class STATE {
		START, // -> LEVEL
		LEVEL, // -> PAUSE, END
		PAUSE, // -> LEVEL
		END
	};
	STATE state;
	ALLEGRO_EVENT event;
	ALLEGRO_BITMAP *game_icon;
	ALLEGRO_BITMAP *background;

	//start menu 相關資源
	ALLEGRO_BITMAP *menu_banner;
	ALLEGRO_BITMAP *menu_dog;
	ALLEGRO_BITMAP *menu_start_btn;
	ALLEGRO_BITMAP *menu_start_btn_hover;

	//遊玩階段 相關資源
	Road *road;
	Dog *dog;
	// Block *block; // 測試用
	BlockManager *blockManager;
	

private:
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event_queue;
	// UI *ui;

	MonsterManager monsterManager;
};

#endif
