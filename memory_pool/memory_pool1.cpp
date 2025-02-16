#include <iostream>
#include <vector>

class MemoryPool {
public:
    MemoryPool(size_t blockSize, size_t numBlocks)
        : blockSize_(blockSize), numBlocks_(numBlocks), pool_(blockSize * numBlocks) {
        // Initialize the free list
        for (size_t i = 0; i < numBlocks_; ++i) {
            freeList_.push_back(&pool_[i * blockSize_]);
        }
    }

    void* allocate() {
        if (freeList_.empty()) {
            std::cerr << "Memory pool is out of memory!" << std::endl;
            return nullptr;
        }

        void* block = freeList_.back();
        freeList_.pop_back();
        return block;
    }

    void deallocate(void* block) {
        freeList_.push_back(block);
    }

    ~MemoryPool() {
        // Clean up the memory pool
        freeList_.clear();
    }

private:
    size_t blockSize_;
    size_t numBlocks_;
    std::vector<char> pool_;
    std::vector<void*> freeList_;
};

int main() {
    const size_t blockSize = 64; // 64 bytes per block
    const size_t numBlocks = 10; // 10 blocks in the pool

    MemoryPool pool(blockSize, numBlocks);

    // Allocate some memory blocks
    void* block1 = pool.allocate();
    void* block2 = pool.allocate();

    std::cout << "Allocated block1: " << block1 << std::endl;
    std::cout << "Allocated block2: " << block2 << std::endl;

    // Deallocate the memory blocks
    pool.deallocate(block1);
    pool.deallocate(block2);

    std::cout << "Deallocated block1 and block2" << std::endl;

    return 0;
}