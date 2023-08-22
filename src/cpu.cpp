#include <memory>
#include <cstring>
#include <vector>
#include <cstdint>
#include "./gbfile.cpp"

// regs
#define rAF 0
#define rBC 1
#define rDE 2
#define rHL 3
#define rSP 4
#define rPC 5

#define RAM_SIZE 131072

void err(uint16_t regs[6], uint8_t ram[RAM_SIZE], std::vector<uint8_t>& rom) {
    exit(1);
}

class CPU {
public:
    std::shared_ptr<GBFile> rom;
    uint16_t regs[6];
    uint8_t ram[RAM_SIZE];

    void reset() {
        memset(regs, 0, 12);
        regs[rPC] = (uint16_t)0x100;
    }

    void load(std::shared_ptr<GBFile> gbFile) {
        rom = gbFile;
        reset();
    }

    void step() {
        // TODO: handle interrupts
        static void (* instrTable[128])(uint16_t[6], uint8_t[RAM_SIZE], std::vector<uint8_t>&) {nullptr};
        if (instrTable[0x00] == nullptr) {
            for (size_t x = 0x00; x <= 0xff; ++x)
                instrTable[x] = err;
        }
        uint8_t opcode = (*rom)[regs[rPC]];
        instrTable[opcode](regs, ram, rom->bytes);
    }
};