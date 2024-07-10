#pragma once

#include "../HammingCode/HammingCode.h"
#include "../FilesWorker/FileHeader.h"

#include <iostream>
#include <string>
#include <vector>
#include <cinttypes>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <numeric>

const uint16_t kBitsInByte = 8;
const uint16_t kDefaultArchiveNameSize = 9;

const std::string kDefaultArchiveName = "MyArchive";


struct Block {
    uint8_t* value;
    size_t hash{};

    Block() {
        this->value = new uint8_t [1];
        this->hash = 0;
    }

    Block(const std::string &value, size_t hash) {
        this->value = new uint8_t [value.size()];
        for (size_t i = 0; i < value.size(); ++i) {
            this->value[i] = static_cast<uint8_t>(value[i] - '0');
        }

        this->hash = hash;
    }

    ~Block() = default;
};

struct ArchiverData {
    uint16_t encode_bits_in_block{};
    uint16_t decode_bits_in_block{};

    uint16_t files_count{};

    ArchiverData() = default;

    ArchiverData(uint16_t encode_bits_in_block,
         uint16_t decode_bits_in_block,
         uint16_t files_count) {

        this->encode_bits_in_block = encode_bits_in_block;
        this->decode_bits_in_block = decode_bits_in_block;
        this->files_count = files_count;
    }

    ~ArchiverData() = default;
};

enum Flags {
    ExtractMethod,
    DeleteMethod
};

class Archiver {
 public:
    Archiver();
    Archiver(const Archiver &archiver);
    explicit Archiver(const std::string &archive_name);
    explicit Archiver(const std::string &archive_name,
                      uint16_t decode_bits_in_block,
                      uint16_t encode_bits_in_block);
    ~Archiver() = default;

    std::filesystem::path GetPath();
    static uint8_t GetByteFromString(const std::string &s);

    void CreateArchive(const std::string &path);
    void CreateArchive();

    void AddFile(const std::string &file_name);

    void ExtractFile(const std::string &file_name, Flags method) const;

    void AddOneFileInArchiveHeader(const std::string &file_name);
    void DeleteFileInfoFromArchive(int64_t offset,
                                   const std::string &file_name,
                                   Flags method) const;

    void DecodeFile(std::ifstream &in, std::ofstream &out) const;

    std::vector<std::string> ListFiles() const;

    void Concatenate(const std::string &first_name,
                     const std::string &second_name);


    std::filesystem::path path_to_archive_{};

    ArchiverData data_{};

    std::string archive_name_{};
};