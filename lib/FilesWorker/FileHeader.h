#pragma once

#include "../ArgParser/ArgParser.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cinttypes>
#include <cstring>

struct FileHeaderData {
    size_t file_size{};

    size_t file_name_hash{};

    FileHeaderData() = default;
    FileHeaderData(size_t file_size,
                   size_t file_name_hash) {

        this->file_size = file_size;
        this->file_name_hash = file_name_hash;
    }
    ~FileHeaderData() = default;
};

class FileHeader {
 public:
    FileHeader();
    FileHeader(const FileHeader &file_header);
    explicit FileHeader(const std::string &file_name);
    ~FileHeader() = default;

    void SetFileSize(const std::string &file_name);


    FileHeaderData data_{};
};