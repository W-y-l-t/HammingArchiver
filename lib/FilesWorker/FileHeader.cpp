#include "FileHeader.h"

FileHeader::FileHeader() {
    std::string str = "File";
    this->data_.file_name_hash = std::hash<std::string>{}(str);
}

FileHeader::FileHeader(const FileHeader &file_header) {
    this->data_.file_name_hash = file_header.data_.file_name_hash;
    this->data_.file_size = file_header.data_.file_size;
}

FileHeader::FileHeader(const std::string &file_name) {
    this->data_.file_name_hash = std::hash<std::string>{}(file_name);
}

void FileHeader::SetFileSize(const std::string &file_name) {
    std::ifstream in(file_name,
                     std::ifstream::binary |
                     std::ifstream::app);

    in.seekg(0, std::ios::end);
    this->data_.file_size = in.tellg();
    in.close();
}
