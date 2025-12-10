#ifndef BLOCKMANAGER_H_INCLUDED
#define BLOCKMANAGER_H_INCLUDED

#include <vector>
#include "Block.h"
#include "Dog.h"

class BlockManager
{
public:
    BlockManager() : state(STATE::MOVE) {}

    void init();
    void update(Dog* dog);
    void draw();

    void checkCollision(Dog* dog);

    std::vector<Block> getBlocks() { return blocks;}

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
