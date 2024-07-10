#pragma once

#include "../Exceptions/Exceptions.h"

#include <cinttypes>
#include <string>
#include <vector>

const uint16_t kEncodeBitsInBlock = 4;
const uint16_t kDecodeBitsInBlock = 7;

const uint16_t kFileLongArgSize = 7;
const uint16_t kEncodeLongArgSize = 5;
const uint16_t kDecodeLongArgSize = 6;

class Ability {
 public:
    Ability() = default;
    Ability(const Ability& ability) = default;
    ~Ability() = default;

    Ability(int32_t argc, char** argv);


    bool create_{};
    bool list_{};
    bool extract_{};
    bool append_{};
    bool delete_{};
    bool concatenate_{};

    bool is_encode_bits_in_block_custom_{};
    bool is_decode_bits_in_block_custom_{};

    uint16_t encode_bits_in_block_ = kEncodeBitsInBlock;
    uint16_t decode_bits_in_block_ = kDecodeBitsInBlock;

    std::vector<std::string> file_names_{};
    std::pair<std::string, std::string> concatenated_archives_{};
    std::string archive_name_{};

    std::string last_argument_{};
};

/*
    -c, --create - создание нового архива

    -f, --file=[ARHCNAME] - имя файла с архивом
    Не более одного параметра данного вида за один запуск

    -l, --list - вывести список файлов в архиве

    -x, --extract - извлечь файлы из архива (если не указано, то все файлы)
    Не более одного параметра данного вида за один запуск

    -a, --append - добавить файл в архив
    Не более одного параметра данного вида за один запуск

    -d, --delete - удалить файл из архива
    Не более одного параметра данного вида за один запуск
    Одновременное использование запросов -x, -a, -d запрещено

    -A, --concatenate - смерджить два архива
    Не более одного запроса данного вида за один запуск
    Параметры кодирования и декодирования для двух архивов должны совпадать

    -i, --in=[VALUE] - число битов в одном блоке при КОДИРОВНИИ
    Оптимальное значение - 4 (для кодирования (7, 4))
    Иначе предпочтительна делимость на 8
    Не более одного параметра данного вида за один запуск
    Только при условии создания нового архива
        ^
        | ---- должны идти вместе
        v
    -o, --out=[VALUE] - число битов в одном блоке при ДЕКОДИРОВНИИ
    Не более одного параметра данного вида за один запуск
    Только при условии создания нового архива
*/