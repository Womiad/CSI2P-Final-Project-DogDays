#include "Game.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include "Player.h"
#include "Level.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include "MonsterManager.h"
#include "Monster.h"
#include <vector>
#include <cstring>

// fixed settings
constexpr char game_icon_img_path[] = "./assets/image/game_icon.png";
// constexpr char game_start_sound_path[] = "./assets/sound/growl.wav";
constexpr char background_img_path[] = "./assets/image/Background.png";
constexpr char background_sound_path[] = "./assets/sound/Journey_fixed.wav";

constexpr char start_menu_banner_path[] = "./assets/image/banner_menu.png";
constexpr char start_menu_dog_path[] = "./assets/image/dog1.png";
constexpr char start_menu_btn_path[] = "./assets/image/start_btn.png";
constexpr char start_menu_btn_hover_path[] = "./assets/image/start_btn_hover.png";

/**
 * @brief Game entry.
 * @details The function processes all allegro events and update the event state to a generic data storage (i.e. DataCenter).
 * For timer event, the game_update and game_draw function will be called if and only if the current is timer.
 */
void
Game::execute() {
    DataCenter *DC = DataCenter::get_instance();
    bool run = true;
    bool redraw = false;
    
    while(run) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        
        // ✅ 批次處理所有累積的事件
        do {
            switch(event.type) {
                case ALLEGRO_EVENT_TIMER: {
                    run &= game_update();
                    redraw = true;
                    break;
                }
                case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                    run = false;
                    break;
                }
                case ALLEGRO_EVENT_KEY_DOWN: {
                    DC->key_state[event.keyboard.keycode] = true;
                    break;
                }
                case ALLEGRO_EVENT_KEY_UP: {
                    DC->key_state[event.keyboard.keycode] = false;
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_AXES: {
                    DC->mouse.x = event.mouse.x;
                    DC->mouse.y = event.mouse.y;
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                    DC->mouse_state[event.mouse.button] = true;
                    break;
                }
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                    DC->mouse_state[event.mouse.button] = false;
                    break;
                }
                default: break;
            }
        } while(al_get_next_event(event_queue, &event));  // ✅ 處理所有事件
        
        // 所有事件處理完後才繪製
        if(redraw) {
            game_draw();
            redraw = false;
        }
    }
}

/**
 * @brief Initialize all allegro addons and the game body.
 * @details Only one timer is created since a game and all its data should be processed synchronously.
 */
Game::Game(bool testMode) {
    GAME_ASSERT(al_init(), "failed to initialize allegro.");

    // initialize addons not related to audio
    bool addon_init = true;
    addon_init &= al_init_primitives_addon();
    addon_init &= al_init_font_addon();
    addon_init &= al_init_ttf_addon();
    addon_init &= al_init_image_addon();
    // !!! Do NOT init acodec here !!!
    GAME_ASSERT(addon_init, "failed to initialize allegro addons.");
    DataCenter *DC = DataCenter::get_instance();

    if (testMode) {
        timer = nullptr;
        event_queue = nullptr;
        display = nullptr;
        debug_log("Game initialized in test mode.\n");
        return;
    }

    // --- initialize events & audio (correct order) ---
	bool event_init = true;
	event_init &= al_install_keyboard();
	event_init &= al_install_mouse();
	event_init &= al_install_audio();         // MUST be before acodec
	GAME_ASSERT(event_init, "failed to initialize allegro events.");

	// 建立 voice
	ALLEGRO_VOICE* voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	GAME_ASSERT(voice, "Failed to create voice");

	// 建立 mixer
	ALLEGRO_MIXER* mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	GAME_ASSERT(mixer, "Failed to create mixer");

	// attach mixer 到 voice
	bool attached = al_attach_mixer_to_voice(mixer, voice);
	GAME_ASSERT(attached, "Failed to attach mixer to voice");

	// 設為 default mixer
	al_set_default_mixer(mixer);

	// 現在初始化 acodec
	GAME_ASSERT(al_init_acodec_addon(), "failed to init acodec addon.");

	// 不需要 al_reserve_samples，因為已經手動建立 mixer 了

    // optional debug
    printf("[DEBUG] default mixer = %p\n", al_get_default_mixer());

    // --- initialize game body ---
    GAME_ASSERT(
        timer = al_create_timer(1.0 / DC->FPS),
        "failed to create timer."
    );
    GAME_ASSERT(
        event_queue = al_create_event_queue(),
        "failed to create event queue."
    );

    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
    al_set_new_display_option(ALLEGRO_VSYNC, 0, ALLEGRO_SUGGEST);  // ← 關閉 VSync
    // 不設定 SWAP_METHOD，讓系統自動選擇
    
    GAME_ASSERT(
        display = al_create_display(DC->window_width, DC->window_height),
        "failed to create display."
    );

    debug_log("Game initialized.\n");
    game_init();
}


/**
 * @brief Initialize all auxiliary resources.
 */
void
Game::game_init() {
	DataCenter *DC = DataCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();
	// set window icon
	game_icon = IC->get(game_icon_img_path);
	al_set_display_icon(display, game_icon);

	// register events to event_queue
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// init sound setting
	SC->init();

	// init font setting
	FC->init();

	// ui = new UI();
	// ui->init();


	// DC->level->init();

	// game start
	background = IC->get(background_img_path);
	// start menu resources
	menu_banner = IC -> get(start_menu_banner_path);
	menu_dog= IC -> get(start_menu_dog_path);
	menu_start_btn = IC -> get(start_menu_btn_path);
	menu_start_btn_hover = IC -> get(start_menu_btn_hover_path);

    

	// playing scene resources
	road = new Road();
	road->init();
    dog = new Dog();
    dog -> init();

    // block = new Block();
    // block -> init(DC -> window_width, 225, 100, 225, 13);
    blockManager = new BlockManager();
    blockManager -> init();

    MonsterManager::load_all_resources();
    monsterManager.init();

    gameOverFrame = new GameOverFrame();
    gameOverFrame -> init();


    int refresh_rate = al_get_display_refresh_rate(display);
    printf("[INFO] 顯示器刷新率: %d Hz\n", refresh_rate);

	debug_log("Game state: change to START\n");
	state = STATE::START;
	al_start_timer(timer);
}

/**
 * @brief The function processes all data update.
 * @details The behavior of the whole game body is determined by its state.
 * @return Whether the game should keep running (true) or reaches the termination criteria (false).
 * @see Game::STATE
 */
bool
Game::game_update() {
	static double last_update_time = al_get_time();
    static int update_count = 0;
    
    double now = al_get_time();
    double update_delta = now - last_update_time;
    
    update_count++;
    
    // ✅ 捕捉任何超過 20ms 的 update
    if (update_delta > 0.020) {
        printf("[!!! SLOW UPDATE !!!] Update %d: %.1f ms\n",
               update_count, update_delta * 1000);
    }
    
    last_update_time = now;
	// static int frame_count = 0;
    // static double last_time = al_get_time();
    
    // frame_count++;
    
    // if(frame_count % 600 == 0) {  // 每 600 幀
    //     double now = al_get_time();
    //     double elapsed = now - last_time;
    //     double actual_fps = 600.0 / elapsed;
        
    //     printf("=== 600 幀檢查 ===\n");
    //     printf("理論時間: 10.00 秒\n");
    //     printf("實際時間: %.3f 秒\n", elapsed);
    //     printf("實際 FPS: %.2f\n", actual_fps);
    //     printf("==================\n\n");
        
    //     last_time = now;
    // }
	DataCenter *DC = DataCenter::get_instance();
    DC->update_delta_time(); 
	// OperationCenter *OC = OperationCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	static ALLEGRO_SAMPLE_INSTANCE *background_music = nullptr;

	switch(state) {
		case STATE::START: {
			// static ALLEGRO_SAMPLE_INSTANCE *instance = nullptr;
            gameOverFrame -> resetBtn();
            dog -> resetPlayerData();
            DC -> resetPlayerData();
            monsterManager.clear();

			//bgm
			static bool is_played = false;
			if(!is_played) {
				background_music = SC->play(background_sound_path, ALLEGRO_PLAYMODE_LOOP);
				is_played = true;
			}

			//如果點擊，前往下一階段
            int title_w = al_get_bitmap_width(menu_banner);
			int btn_w = al_get_bitmap_width(menu_start_btn);
            int btn_h = al_get_bitmap_height(menu_start_btn);
            int btn_x = (DC->window_width) / 5 * 2 + (title_w/2) - (btn_w/2);
            int btn_y = (DC->window_height / 2 - btn_h / 2) * 1.6;  // 放在中間

			bool mouse_on_button = (DC->mouse.x >= btn_x && 
                                   DC->mouse.x <= btn_x + btn_w &&
                                   DC->mouse.y >= btn_y && 
                                   DC->mouse.y <= btn_y + btn_h);
            
            if(mouse_on_button && (DC->mouse_state[1] && !DC->prev_mouse_state[1])) {
            	state = STATE::LEVEL;   
            }

			break;
		} case STATE::LEVEL: {
			road -> update();
            dog -> update();
            blockManager -> update(dog);
            blockManager -> checkCollision(dog);
            monsterManager.update(dog);

            if(dog -> num_dogs <= 0){
                state = STATE::PAUSE;
            }
            
            // block -> update();
			// static bool BGM_played = false;
			// if(!BGM_played) {
			// 	background = SC->play(background_sound_path, ALLEGRO_PLAYMODE_LOOP);
			// 	BGM_played = true;
			// }

			// if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
			// 	SC->toggle_playing(background);
			// 	debug_log("<Game> state: change to PAUSE\n");
			// 	state = STATE::PAUSE;
			// }
			// if(DC->level->remain_monsters() == 0 && DC->monsters.size() == 0) {
			// 	debug_log("<Game> state: change to END\n");
			// 	state = STATE::END;
			// }
			// if(DC->player->HP == 0) {
			// 	debug_log("<Game> state: change to END\n");
			// 	state = STATE::END;
			// }
			break;
		} case STATE::PAUSE: {
            gameOverFrame -> update();
            if(gameOverFrame -> btn_clicked){
                state = STATE::START;
            }
			// if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
			// 	SC->toggle_playing(background);
			// 	debug_log("<Game> state: change to LEVEL\n");
			// 	state = STATE::LEVEL;
			// }
			break;
		} case STATE::END: {
			return false;
		}
	}
	// If the game is not paused, we should progress update.
	// if(state != STATE::PAUSE) {
	// 	DC->player->update();
	// 	SC->update();
	// 	ui->update();
	// 	if(state != STATE::START) {
	// 		DC->level->update();
	// 		OC->update();
	// 	}
	// }
	// game_update is finished. The states of current frame will be previous states of the next frame.
	memcpy(DC->prev_key_state, DC->key_state, sizeof(DC->key_state));
	memcpy(DC->prev_mouse_state, DC->mouse_state, sizeof(DC->mouse_state));
	return true;
}

/**
 * @brief Draw the whole game and objects.
 */
void Game::game_draw() {
    static double last_frame_time = al_get_time();
    static int frame_count = 0;
    static double max_frame_time = 0.0;
    static int freeze_frame = -1;
    
    double now = al_get_time();
    double frame_delta = now - last_frame_time;
    
    frame_count++;
    
    // ✅ 記錄最長的幀時間
    if (frame_delta > max_frame_time) {
        max_frame_time = frame_delta;
        freeze_frame = frame_count;
    }
    
    // ✅ 捕捉任何超過 18ms 的幀
    if (frame_delta > 0.018) {
        printf("[FREEZE] Frame %d: %.1f ms | Road offset: %d | State: %d\n",
               frame_count, 
               frame_delta * 1000,
               (state == STATE::LEVEL && road) ? road->get_offset() : -1,
               (int)state);
    }
    
    last_frame_time = now;
    
    DataCenter *DC = DataCenter::get_instance();
    ALLEGRO_BITMAP* backbuffer = al_get_backbuffer(display);
    al_set_target_bitmap(backbuffer);

    // ✅ 計時每個繪製階段
    double t0 = al_get_time();
    al_clear_to_color(al_map_rgb(100, 100, 100));
    double t1 = al_get_time();
    
    if(state != STATE::END && background) {
        al_draw_bitmap(background, 0, 0, 0);
    }
    double t2 = al_get_time();
    
    switch(state) {
        case STATE::START: {
            if (menu_banner) {
                // int title_w = al_get_bitmap_width(menu_banner);
                int title_x = (DC->window_width) / 5 * 2;
                int title_y = DC->window_height / 6;
                al_draw_bitmap(menu_banner, title_x, title_y, 0);
            }
            
            if (menu_dog) {
                // int deco_w = al_get_bitmap_width(menu_dog);
                int deco_h = al_get_bitmap_height(menu_dog);
                int deco_x = DC->window_width * 0.03;
                int deco_y = (DC->window_height / 2 - deco_h / 2) * 1.2;
                al_draw_bitmap(menu_dog, deco_x, deco_y, 0);
            }
            
            if (menu_start_btn && menu_start_btn_hover) {
                int title_w = al_get_bitmap_width(menu_banner);
                int btn_w = al_get_bitmap_width(menu_start_btn);
                int btn_h = al_get_bitmap_height(menu_start_btn);
                int btn_x = (DC->window_width) / 5 * 2 + (title_w/2) - (btn_w/2);
                int btn_y = (DC->window_height / 2 - btn_h / 2) * 1.6;

                bool mouse_on_button = (DC->mouse.x >= btn_x && 
                                       DC->mouse.x <= btn_x + btn_w &&
                                       DC->mouse.y >= btn_y && 
                                       DC->mouse.y <= btn_y + btn_h);
                
                if(mouse_on_button) {
                    al_draw_bitmap(menu_start_btn_hover, btn_x, btn_y, 0);
                } else {
                    al_draw_bitmap(menu_start_btn, btn_x, btn_y, 0);
                }
            }
            break;
        }
        case STATE::LEVEL: {
            if (road) {
                road->draw();
            }
            if (dog) {
                dog->draw();
            }
            if (blockManager) {
                blockManager->draw();
            }
            monsterManager.draw();

            // Dog 的碰撞箱（綠色）// 碰撞箱有微妙的問題但它是個美麗的錯誤
            // al_draw_rectangle(
            //     dog->getX(), 
            //     dog->getY() - dog->getHeight(),
            //     dog->getX() + dog->getWidth(), 
            //     dog->getY(),
            //     al_map_rgb(0, 255, 0), 
            //     3
            // );
            
            // // Block 的碰撞箱（紅色）
            // for (const auto& block : blockManager->getBlocks()) {
            //     al_draw_rectangle(
            //         block.getX(), 
            //         block.getY(),
            //         block.getX() + block.getWidth(), 
            //         block.getY() + block.getHeight(),
            //         al_map_rgb(255, 0, 0), 
            //         3
            //     );
            // }
            break;
        }
        case STATE::PAUSE:
            if (road) {
                road->draw();
            }
            if (dog) {
                dog->draw();
            }
            if (blockManager) {
                blockManager->draw();
            }
            monsterManager.draw();

            gameOverFrame->draw();
            break;
        case STATE::END:
            break;
    }
    double t3 = al_get_time();
    
    al_flip_display();
    double t4 = al_get_time();
    
    // ✅ 每 600 幀輸出詳細報告
    if (frame_count % 600 == 0) {
        printf("\n========== 600 幀效能報告 ==========\n");
        printf("最大幀時間: %.2f ms (Frame %d)\n", max_frame_time * 1000, freeze_frame);
        printf("平均繪製時間:\n");
        printf("  - Clear: %.2f ms\n", (t1-t0)*1000);
        printf("  - Background: %.2f ms\n", (t2-t1)*1000);
        printf("  - Game objects: %.2f ms\n", (t3-t2)*1000);
        printf("  - Flip: %.2f ms\n", (t4-t3)*1000);
        printf("===================================\n\n");
        
        max_frame_time = 0.0;
        freeze_frame = -1;
    }
}

Game::~Game() {
	if(display) al_destroy_display(display);
	if(timer) al_destroy_timer(timer);
	if(event_queue) al_destroy_event_queue(event_queue);
    MonsterManager::unload_all_resources();
}
