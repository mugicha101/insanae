#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdint>

// represents data in .gb or .gbc file
struct GBFile {
    struct CartridgeType {
        enum MBCType { NO_MBC, MBC1, MBC2, MBC3, MBC5, MMM01 };
        enum RAMType { NO_RAM, RAM, SRAM };
        enum Extra { NO_EXTRA, RUMBLE, TIMER };
        bool valid;
        MBCType mbcType;
        RAMType ramType;
        bool hasBattery;
        Extra extra;
    };
    std::vector<uint8_t> bytes;
    std::string inline parseString(size_t firstByte, size_t lastByte) const {
        return std::string(bytes.begin() + firstByte, bytes.begin() + lastByte + 1);
    }

    GBFile(std::string path) {
        std::ifstream file(path, std::ios::binary);
        bytes = std::vector<uint8_t>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    }

    std::string getTitle() const {
        return parseString(0x134, 0x142);
    }

    bool isColor() const {
        return bytes[0x143] == (uint8_t)0x80;
    }

    bool isSuperGB() const {
        return bytes[0x146] == (uint8_t)0x03;
    }

    CartridgeType cartridgeType() const {
        switch (bytes[0x147]) {
            case 0x00: return { true, CartridgeType::NO_MBC, CartridgeType::NO_RAM, false, CartridgeType::NO_EXTRA };
            
            case 0x01: return { true, CartridgeType::MBC1, CartridgeType::NO_RAM, false, CartridgeType::NO_EXTRA };
            case 0x02: return { true, CartridgeType::MBC1, CartridgeType::RAM, false, CartridgeType::NO_EXTRA };
            case 0x03: return { true, CartridgeType::MBC1, CartridgeType::RAM, true, CartridgeType::NO_EXTRA };
            
            case 0x05: return { true, CartridgeType::MBC2, CartridgeType::NO_RAM, false, CartridgeType::NO_EXTRA };
            case 0x06: return { true, CartridgeType::MBC2, CartridgeType::NO_RAM, true, CartridgeType::NO_EXTRA };
            
            case 0x08: return { true, CartridgeType::NO_MBC, CartridgeType::RAM, false, CartridgeType::NO_EXTRA };
            case 0x09: return { true, CartridgeType::NO_MBC, CartridgeType::RAM, true, CartridgeType::NO_EXTRA };
            
            case 0x0B: return { true, CartridgeType::MMM01, CartridgeType::NO_RAM, false, CartridgeType::NO_EXTRA };
            case 0x0C: return { true, CartridgeType::MMM01, CartridgeType::SRAM, false, CartridgeType::NO_EXTRA };
            case 0x0D: return { true, CartridgeType::MMM01, CartridgeType::SRAM, true, CartridgeType::NO_EXTRA };
            
            case 0x0F: return { true, CartridgeType::MBC3, CartridgeType::NO_RAM, true, CartridgeType::TIMER };
            case 0x10: return { true, CartridgeType::MBC3, CartridgeType::RAM, true, CartridgeType::TIMER };
            case 0x11: return { true, CartridgeType::MBC3, CartridgeType::NO_RAM, false, CartridgeType::NO_EXTRA };
            case 0x12: return { true, CartridgeType::MBC3, CartridgeType::RAM, false, CartridgeType::NO_EXTRA };
            case 0x13: return { true, CartridgeType::MBC3, CartridgeType::RAM, true, CartridgeType::NO_EXTRA };
            
            case 0x19: return { true, CartridgeType::MBC5, CartridgeType::NO_RAM, false, CartridgeType::NO_EXTRA };
            case 0x1A: return { true, CartridgeType::MBC5, CartridgeType::RAM, false, CartridgeType::NO_EXTRA };
            case 0x1B: return { true, CartridgeType::MBC5, CartridgeType::RAM, true, CartridgeType::NO_EXTRA };
            case 0x1C: return { true, CartridgeType::MBC5, CartridgeType::NO_RAM, false, CartridgeType::RUMBLE };
            case 0x1D: return { true, CartridgeType::MBC5, CartridgeType::SRAM, false, CartridgeType::RUMBLE };
            case 0x1E: return { true, CartridgeType::MBC5, CartridgeType::SRAM, true, CartridgeType::RUMBLE };
            
            default: return { false, CartridgeType::NO_MBC, CartridgeType::NO_RAM, false, CartridgeType::NO_EXTRA };
        }
    }

    size_t romSize() const {
        const size_t BANK = 1 << 14; // 1 bank = 16 KB
        switch (bytes[0x148]) {
            case 0x52: return BANK * 72;
            case 0x53: return BANK * 80;
            case 0x54: return BANK * 96;
            default: return BANK << (1 + bytes[0x148]);
        }
    }

    size_t ramSize() const {
        const size_t BANK = 1 << 11; // 1 bank = 8 KB
        return (BANK >> 2) << (bytes[0x149] << 1);
    }

    bool isJapanese() const {
        return bytes[0x14A] == 0;
    }
};