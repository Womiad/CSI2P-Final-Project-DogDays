#ifndef BLOCKMANAGER_H_INCLUDED
#define BLOCKMANAGER_H_INCLUDED

#include <vector>
#include "Block.h"

class BlockManager
{
public:
    BlockManager() : state(STATE::MOVE) {}

    void init();
    void update();
    void draw();

private:
    enum class STATE {
        MOVE,
        PAUSE
    };
    STATE state;

    std::vector<Block> blocks;

    // spawn 設定
    float spawnFrame = 0.0f;   // 計時器
    float spawnFrameInterval = 2.0f; // 每 2 秒產生一個 block

    // 兩種高度
    int midY;
    int highY;
};

#endif
