#include "Archiver.h"

Archiver::Archiver() {
    this->path_to_archive_ = R"(C:\ITMO\C++\labwork6-W-y-l-t)";
    this->archive_name_ = kDefaultArchiveName;

    this->data_.encode_bits_in_block = kEncodeBitsInBlock;
    this->data_.decode_bits_in_block = kDecodeBitsInBlock;
    this->data_.files_count = 0;
}

Archiver::Archiver(const Archiver &archiver) {
    this->path_to_archive_ = archiver.path_to_archive_;
    this->archive_name_ = archiver.archive_name_;

    this->data_.encode_bits_in_block = archiver.data_.encode_bits_in_block;
    this->data_.decode_bits_in_block = archiver.data_.decode_bits_in_block;
    this->data_.files_count = archiver.data_.files_count;
}

Archiver::Archiver(const std::string &archive_name) {
    this->path_to_archive_ = R"(C:\ITMO\C++\labwork6-W-y-l-t)";
    this->path_to_archive_ /= archive_name;

    std::ifstream in;
    in.open(this->path_to_archive_,
            std::ifstream::binary | std::ifstream::in);

    if (in.fail()) {
        MyExceptions::FileError(archive_name);
    }

    ArchiverData this_archiver_data;
    in.read(reinterpret_cast<char*>(&this_archiver_data),
            sizeof (ArchiverData));
    in.close();

    this->archive_name_ = archive_name;
    this->data_.files_count = this_archiver_data.files_count;
    this->data_.encode_bits_in_block = this_archiver_data.encode_bits_in_block;
    this->data_.decode_bits_in_block = this_archiver_data.decode_bits_in_block;
}

Archiver::Archiver(const std::string &archive_name,
                   uint16_t decode_bits_in_block,
                   uint16_t encode_bits_in_block) {

    this->path_to_archive_ = R"(C:\ITMO\C++\labwork6-W-y-l-t)";
    this->archive_name_ = archive_name;
    this->data_.decode_bits_in_block = decode_bits_in_block;
    this->data_.encode_bits_in_block = encode_bits_in_block;
}

std::filesystem::path Archiver::GetPath() {
    return this->path_to_archive_;
}

void Archiver::CreateArchive(const std::string &archive_name) {
    this->archive_name_ = (archive_name.empty() ?
                           this->archive_name_ : archive_name);
    this->path_to_archive_ /= this->archive_name_;

    this->data_.files_count = 0;

    std::filesystem::create_directories(this->path_to_archive_.parent_path());

    std::ofstream out(this->path_to_archive_,
                      std::ofstream::binary | std::ofstream::ate);

    ArchiverData data(this->data_.encode_bits_in_block,
                      this->data_.decode_bits_in_block,
                      this->data_.files_count);

    out.write(reinterpret_cast<char*>(&data),
              sizeof (ArchiverData));

    out.close();
}

void Archiver::CreateArchive() {
    CreateArchive("");
}

void Archiver::AddOneFileInArchiveHeader(const std::string &file_name) {
    ArchiverData archiver_data;

    std::ifstream in;

    in.open(this->path_to_archive_,
            std::ifstream::binary | std::ifstream::ate);
    in.seekg(0, std::ifstream::beg);
    in.read(reinterpret_cast<char*>(&archiver_data),
            sizeof (ArchiverData));

    std::filesystem::path path = this->GetPath();
    size_t a = std::filesystem::file_size(path);
    size_t b = sizeof (ArchiverData);
    auto size = static_cast<int64_t>(a - b);
    char* current_file = new char[size + 1];
    in.read(current_file, size);

    in.close();

    archiver_data.files_count += 1;

    std::ofstream out;

    out.open(this->path_to_archive_,
             std::ofstream::binary | std::ofstream::out);
    out.seekp(0, std::ofstream::beg);

    out.write(reinterpret_cast<char*>(&archiver_data),
              sizeof (ArchiverData));
    out.write(current_file, size);
    out.close();
}

void Archiver::AddFile(const std::string &file_name) {
    std::filesystem::path file_path =
        this->path_to_archive_.parent_path() /= file_name;

    AddOneFileInArchiveHeader(file_name);

    std::ifstream in;
    std::ofstream out;

    out.open(this->path_to_archive_,
             std::ofstream::binary | std::ofstream::app);

    FileHeader new_header(file_name);
    char char_file_name[256];
    std::strcpy(char_file_name, file_name.c_str());
    new_header.data_.file_size =
        static_cast<size_t>(std::filesystem::file_size(file_path));

    out.write(reinterpret_cast<char *>(&new_header.data_),
              sizeof (FileHeaderData));
    out.write(char_file_name,sizeof (char) * 256);

    in.open(file_path,
            std::ifstream::binary |
            std::ifstream::in | std::ifstream::ate);

    if (in.fail()) {
        MyExceptions::FileError(file_name);
    }

    in.seekg(0, std::ifstream::beg);

    size_t lcm = std::lcm(this->data_.encode_bits_in_block,
                            kBitsInByte);
    size_t bytes = lcm / kBitsInByte;
    size_t blocks = lcm / this->data_.encode_bits_in_block;

    for (size_t i = 0; i < new_header.data_.file_size; i += bytes) {
        std::string buff;

        for (size_t j = 0; j < bytes; ++j) {
            uint8_t byte;
            in.read((char *)(&byte), 1);
            auto byte_in_int = static_cast<int32_t>(byte - '0');

            for (size_t k = 0; k < kBitsInByte; ++k) {
                auto c = static_cast<char>(
                    static_cast<bool>(
                        byte_in_int & (1 << (kBitsInByte - 1 - k))) + '0');

                buff.push_back(c);
            }
        }

        for (size_t j = 0; j < blocks; ++j) {
            std::string current = buff.substr(
                j * this->data_.encode_bits_in_block,
                this->data_.encode_bits_in_block);

            HammingCode object;
            object.Encode(current);

            Block block(object.GetEncodedString(), object.GetHash());

            for (size_t k = 0; k < this->data_.decode_bits_in_block; ++k) {
                out.write((char*)(&block.value[k]),
                          sizeof (uint8_t));
            }
            out.write((char*)(&block.hash), sizeof (size_t));
        }
    }

    out.close();
    in.close();

    remove(file_path);
}

uint8_t Archiver::GetByteFromString(const std::string &s) {
    uint8_t temp = 0;
    for (size_t i = 0; i < kBitsInByte; ++i) {
        temp = (temp | (1 << (kBitsInByte - 1 - i)) *
            static_cast<int16_t>(s[i] - '0'));
    }

    return temp;
}

void Archiver::DecodeFile(std::ifstream &in,
                          std::ofstream &out) const {

    FileHeaderData extracted_file_header;
    char char_file_name[256];
    in.read(reinterpret_cast<char*>(&extracted_file_header),
            sizeof (FileHeaderData));
    in.read(char_file_name, sizeof (char) * 256);

    size_t lcm = std::lcm(this->data_.encode_bits_in_block,
                          kBitsInByte);
    size_t bytes = lcm / kBitsInByte;
    size_t blocks = lcm / this->data_.encode_bits_in_block;

    std::string fill;
    fill.resize(this->data_.decode_bits_in_block, '0');
    for (size_t i = 0; i < extracted_file_header.file_size; i += bytes) {
        std::string buff;

        for (size_t j = 0; j < blocks; ++j) {
            Block current_block(fill, 0);

            for (size_t k = 0; k < this->data_.decode_bits_in_block; ++k) {
                in.read((char*)(&current_block.value[k]),
                          sizeof (uint8_t));
            }
            in.read((char*)(&current_block.hash),
                      sizeof (size_t));

            HammingCode object;
            object.Decode(current_block.value,
                          current_block.hash,
                          this->data_.decode_bits_in_block);

            buff += object.GetDecodedString();
        }

        for (size_t j = 0; j < bytes; ++j) {
            std::string current_byte = buff.substr(j * kBitsInByte,
                                                   kBitsInByte);

            uint8_t byte_out = GetByteFromString(current_byte);
            char symbol = static_cast<char>(byte_out + '0');
            out.write(reinterpret_cast<char*>(&symbol),sizeof (char));
        }
    }

    out.close();
}

void Archiver::DeleteFileInfoFromArchive(int64_t offset,
                                         const std::string &file_name,
                                         Flags method)
                                         const {

    std::filesystem::path file_path =
        this->path_to_archive_.parent_path() /= file_name;

    std::ifstream in(this->path_to_archive_,
                     std::ifstream::binary |
                         std::ifstream::in);
    in.seekg(0, std::ifstream::beg);

    ArchiverData archiver_data;
    in.read(reinterpret_cast<char*>(&archiver_data),
            sizeof (ArchiverData));

    offset -= sizeof (ArchiverData);
    char* before_file = new char[offset + 1];
    in.read(before_file, static_cast<int64_t>(offset * sizeof (char)));

    archiver_data.files_count -= 1;

    std::ofstream extracted_file;
    extracted_file.open(file_path,
                        std::ofstream::binary |
                        std::ofstream::out);
    DecodeFile(in, extracted_file);

    if (method == Flags::DeleteMethod) {
        remove(file_path);
    }

    size_t current_position = in.tellg();

    auto remain = static_cast<int64_t>(
        std::filesystem::file_size(this->path_to_archive_) -
        current_position);

    char* after_file = new char[remain + 1];
    in.read(after_file,static_cast<int64_t>(remain * sizeof (char)));

    in.close();

    std::ofstream new_archive;
    new_archive.open(this->path_to_archive_,
                     std::ofstream::binary |
                     std::ofstream::out);

    new_archive.write(reinterpret_cast<char*>(&archiver_data),
                      sizeof (ArchiverData));
    new_archive.write(before_file,
                      static_cast<int64_t>(offset * sizeof (char)));
    new_archive.write(after_file,
                      static_cast<int64_t>(remain * sizeof (char)));

    new_archive.close();
}

void Archiver::ExtractFile(const std::string &file_name, Flags method) const {
    std::filesystem::path file_path =
        this->path_to_archive_.parent_path() /= file_name;

    std::ifstream in(this->path_to_archive_,
                     std::ifstream::binary |
                         std::ifstream::in);
    in.seekg(0, std::ifstream::beg);

    if (in.fail()) {
        MyExceptions::FileError(this->path_to_archive_.string());
    }

    int64_t bytes_before_extracted_file = 0;

    ArchiverData archiver_data;
    in.read((char *)(&archiver_data),
            sizeof(ArchiverData));

    bytes_before_extracted_file += sizeof(ArchiverData);

    size_t files_in_current_archive = archiver_data.files_count;
    while (files_in_current_archive--) {
        FileHeaderData file_header_data;
        char char_file_name[256];
        in.read(reinterpret_cast<char *>(&file_header_data),
                sizeof(FileHeaderData));
        in.read(char_file_name, sizeof (char) * 256);

        if (file_header_data.file_name_hash ==
            std::hash<std::string>{}(file_name)) {

            in.close();
            DeleteFileInfoFromArchive(bytes_before_extracted_file,
                                      file_name,
                                      method);
            return;
        }

        bytes_before_extracted_file += sizeof (FileHeaderData);
        bytes_before_extracted_file += sizeof (char) * 256;

        size_t lcm = std::lcm(this->data_.encode_bits_in_block,
                              kBitsInByte);
        size_t bytes = lcm / kBitsInByte;
        size_t blocks = lcm / this->data_.encode_bits_in_block;

        std::string fill;
        fill.resize(this->data_.decode_bits_in_block, '0');
        for (size_t i = 0; i < file_header_data.file_size; i += bytes) {
            for (size_t j = 0; j < blocks; ++j) {
                Block current_block(fill, 0);

                for (size_t k = 0; k < this->data_.decode_bits_in_block; ++k) {
                    in.read((char *)(&current_block.value[k]),
                            sizeof(uint8_t));
                    bytes_before_extracted_file += sizeof(uint8_t);
                }
                in.read((char *)(&current_block.hash),
                        sizeof(size_t));
                bytes_before_extracted_file += sizeof(size_t);
            }
        }
    }

    if (method == Flags::ExtractMethod) {
        MyExceptions::FileNotFoundExtract(file_name);
    } else {
        MyExceptions::FileNotFoundDelete(file_name);
    }
}

std::vector<std::string> Archiver::ListFiles() const {
    std::vector<std::string> list(0);

    std::ifstream archive(this->path_to_archive_,
                          std::ifstream::binary |
                          std::ifstream::in);

    ArchiverData archiver_data;
    archive.read(reinterpret_cast<char *>(&archiver_data),
                 sizeof (ArchiverData));

    size_t current_files_count = archiver_data.files_count;
    while (current_files_count --) {
        FileHeaderData file_header_data;
        char char_file_name[256];
        archive.read(reinterpret_cast<char *>(&file_header_data),
                     sizeof (FileHeaderData));
        archive.read(char_file_name, sizeof (char) * 256);

        std::string file_name = char_file_name;
        list.push_back(file_name);

        size_t lcm = std::lcm(this->data_.encode_bits_in_block,
                              kBitsInByte);
        size_t bytes = lcm / kBitsInByte;
        size_t blocks = lcm / this->data_.encode_bits_in_block;

        std::string fill;
        fill.resize(this->data_.decode_bits_in_block, '0');
        for (size_t i = 0; i < file_header_data.file_size; i += bytes) {
            for (size_t j = 0; j < blocks; ++j) {
                Block current_block(fill, 0);

                for (size_t k = 0; k < this->data_.decode_bits_in_block; ++k) {
                    archive.read((char*)(&current_block.value[k]),
                            sizeof (uint8_t));
                }
                archive.read((char*)(&current_block.hash),
                        sizeof (size_t));
            }
        }
    }

    return list;
}

void Archiver::Concatenate(const std::string &first_name,
                           const std::string &second_name) {

    Archiver first_archive(first_name);
    Archiver second_archive(second_name);

    std::vector<std::string> first_archive_files = first_archive.ListFiles();
    std::vector<std::string> second_archive_files = second_archive.ListFiles();

    for (const auto &name : first_archive_files) {
        first_archive.ExtractFile(name, Flags::ExtractMethod);
        this->AddFile(name);
    }

    for (const auto &name : second_archive_files) {
        second_archive.ExtractFile(name, Flags::ExtractMethod);
        this->AddFile(name);
    }
}
