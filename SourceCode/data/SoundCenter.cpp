#include "SoundCenter.h"
#include "../Utils.h"

using namespace std;

// fixed settings
namespace SoundSetting {
	constexpr int RESERVED_SAMPLES = 16;
	constexpr int UPDATE_PERIOD = 60;
}

SoundCenter::SoundCenter () : update_period{SoundSetting::UPDATE_PERIOD} {}

SoundCenter::~SoundCenter() {
	for(auto &[path, sample_pair] : samples) {
		auto &[sample, insts] = sample_pair;
		al_destroy_sample(sample);
		for(ALLEGRO_SAMPLE_INSTANCE *inst : insts)
			al_destroy_sample_instance(inst);
	}
}

/**
 * @brief Reserve samples to have default mixer work.
 */
bool
SoundCenter::init() {
	return (al_get_default_mixer() != nullptr);
}

/**
 * @brief The update function searches all sample instances and destroy instances that have finished playing.
 */
void
SoundCenter::update() {
	if (update_period == 0) {
		update_period = SoundSetting::UPDATE_PERIOD;
		for(auto &[path, audio_pair] : samples) {
			auto &[sample, insts] = audio_pair;
			for(auto it = insts.begin(); it != insts.end();) {
				if(al_get_sample_instance_playing(*it)) ++it;
				else if(al_get_sample_instance_position(*it) != 0) ++it;
				else if(al_get_sample_instance_playmode(*it) == ALLEGRO_PLAYMODE_LOOP) ++it;
				else {
					al_destroy_sample_instance(*it);
					it = insts.erase(it);
				}
			}
		}
	} else {
		--update_period;
	}
}

/**
 * @brief Remove a sample.
 */
bool
SoundCenter::erase_sample(const std::string &path) {
	auto it = samples.find(path);
	if(it == samples.end()) {
		return false;
	}
	auto &[sample, insts] = it->second;
	for(auto inst : insts) {
		al_destroy_sample_instance(inst);
	}
	insts.clear();
	al_destroy_sample(sample);
	return true;
}

/**
 * @brief Play an audio.
 * @param path the audio file path.
 * @param mode the play mode defined by allegro5.
 * @return The corresponding played ALLEGRO_SAMPLE_INSTANCE* instance.
 */
ALLEGRO_SAMPLE_INSTANCE*
SoundCenter::play(const string &path, ALLEGRO_PLAYMODE mode) {
	printf("[DEBUG] SoundCenter::play called with path: %s\n", path.c_str());
	
	// 檢查 default mixer 是否存在
	ALLEGRO_MIXER* mixer = al_get_default_mixer();
	if (!mixer) {
		printf("[ERROR] No default mixer available!\n");
		return nullptr;
	}
	printf("[DEBUG] Default mixer found: %p\n", mixer);

	auto it = samples.find(path);
	if(it == samples.end()) {
		// 第一次載入這個音檔
		printf("[DEBUG] Loading new sample: %s\n", path.c_str());
		ALLEGRO_SAMPLE *sample = al_load_sample(path.c_str());
		
		if (sample == nullptr) {
			printf("[ERROR] Failed to load sample: %s\n", path.c_str());
			printf("[ERROR] Make sure the file exists and is a valid audio format\n");
			return nullptr;
		}
		
		printf("[DEBUG] Sample loaded successfully: %p\n", sample);
		it = samples.insert({path, {sample, {}}}).first;
	} else {
		printf("[DEBUG] Using cached sample for: %s\n", path.c_str());
	}
	
	auto &[sample, insts] = it->second;
	
	// 確保 sample 不為空
	if (sample == nullptr) {
		printf("[ERROR] Sample is null for path: %s\n", path.c_str());
		return nullptr;
	}
	
	printf("[DEBUG] Creating sample instance from sample: %p\n", sample);
	ALLEGRO_SAMPLE_INSTANCE *instance = al_create_sample_instance(sample);
	
	if (instance == nullptr) {
		printf("[ERROR] Failed to create sample instance!\n");
		return nullptr;
	}
	
	printf("[DEBUG] Sample instance created: %p\n", instance);
	insts.emplace_back(instance);

	al_set_sample_instance_playmode(instance, mode);
	
	bool attached = al_attach_sample_instance_to_mixer(instance, mixer);
	if (!attached) {
		printf("[ERROR] Failed to attach sample instance to mixer!\n");
		al_destroy_sample_instance(instance);
		insts.pop_back();
		return nullptr;
	}
	
	printf("[DEBUG] Sample instance attached to mixer\n");
	
	bool playing = al_play_sample_instance(instance);
	if (!playing) {
		printf("[ERROR] Failed to play sample instance!\n");
	} else {
		printf("[DEBUG] Sample instance playing successfully\n");
	}
	
	return instance;
}

/**
 * @brief Check if an instance is currently playing.
 */
bool
SoundCenter::is_playing(const ALLEGRO_SAMPLE_INSTANCE *const inst) {
	if (inst == nullptr) return false;
	return al_get_sample_instance_playing(inst);
}

/**
 * @brief Pause or play an audio, depends on its current playing state.
 */
void
SoundCenter::toggle_playing(ALLEGRO_SAMPLE_INSTANCE *inst) {
	if (inst == nullptr) return;
	
	bool is_playing = al_get_sample_instance_playing(inst);
	if(is_playing) {
		unsigned int pos = al_get_sample_instance_position(inst);
		al_stop_sample_instance(inst);
		// As the sample stops, allegro will automatically reset the play position to 0. We need to set it back to be able to resume.
		al_set_sample_instance_position(inst, pos);
	} else al_play_sample_instance(inst);
}