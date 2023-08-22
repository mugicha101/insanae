#include <memory>
#include <vector>
#include <cstdint>
#include "gbfile.cpp"

#define BANK_SIZE 0x4000
#define ROM_SIZE 0x8000

struct Memory {
    std::shared_ptr<GBFile> rom;
    int bank_offset = 0;

    void load(std::shared_ptr<GBFile> gbFile) {
        rom = gbFile;
    }

    uint32_t resolveOffset(uint16_t addr) {
        if (addr < BANK_SIZE) {
            return addr;
        }
        if (addr < ROM_SIZE) {
            return addr + bank_offset;
        }
    }

    uint8_t read(uint16_t addr) {
        uint32_t offset = resolve_offset(addr);
        return (*rom)[offset];
    }
    void write(uint16_t addr, uint8_t byte) {
        uint32_t offset = resolve_offset(addr);
        if (offset < ROM_SIZE) {
            exit(1);
        }
    }
};