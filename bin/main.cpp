#include "../lib/Archiver/Archiver.h"

int32_t main(int32_t argc, char** argv) {
    Ability arguments(argc, argv);

    uint16_t encode_bits_in_block =
        (arguments.is_encode_bits_in_block_custom_ ?
        arguments.encode_bits_in_block_ : kEncodeBitsInBlock);
    uint16_t decode_bits_in_block =
        (arguments.is_decode_bits_in_block_custom_ ?
        arguments.decode_bits_in_block_ : kDecodeBitsInBlock);

    if (!HammingCode::CheckValidityOfEDParameters(encode_bits_in_block,
                                                  decode_bits_in_block)) {
        MyExceptions::EDDataIncorrect();
    }

    if (arguments.delete_ + arguments.append_ + arguments.extract_ > 1) {
        MyExceptions::TooManyArguments();
    } else {
        if (arguments.delete_ + arguments.append_ + arguments.extract_ == 1 &&
                    arguments.archive_name_.empty()) {

            MyExceptions::NotEnoughArguments();
        }
        if (arguments.concatenate_ && arguments.archive_name_.empty()) {
            MyExceptions::NotEnoughArguments();
        }
    }

    if (arguments.create_) {
        Archiver new_archiver(arguments.archive_name_,
                              decode_bits_in_block,
                              encode_bits_in_block);
        new_archiver.CreateArchive();
    }

    if (arguments.concatenate_) {
        Archiver merged_archive(arguments.archive_name_);

        merged_archive.Concatenate(arguments.concatenated_archives_.first,
                                   arguments.concatenated_archives_.second);
    }

    Archiver archive(arguments.archive_name_);

    for (const std::string &name : arguments.file_names_) {
        if (name.starts_with("C:")) {
            continue;
        }
        if (arguments.extract_) {
            archive.ExtractFile(name, Flags::ExtractMethod);
        } else if (arguments.append_) {
            archive.AddFile(name);
        } else if (arguments.delete_) {
            archive.ExtractFile(name, Flags::DeleteMethod);
        }
    }

    if (arguments.list_) {
        if (arguments.archive_name_.empty()) {
            MyExceptions::NotEnoughArguments();
        }

        std::vector<std::string> files = archive.ListFiles();

        if (files.empty()) {
            std::cout << "Archive " + arguments.archive_name_ + " is empty!\n";
        } else {
            std::cout << "Archive " + arguments.archive_name_ +
                    "contains next files :\n";
        }

        for (const auto &name : files) {
            std::cout << name << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}

// -c -f "MyArchive.haf" -a "S.txt" "Homework7.3.jpg" "TripAside.txt" "My favourite photo.docx"
// -f "MyArchive.haf" -x "S.txt" "Homework7.3.jpg" "TripAside.txt" "My favourite photo.docx"
// -c -f "MyArchive.haf" -a "S.txt" "Homework7.3.jpg" "TripAside.txt" "My favourite photo.docx" -i 11 -o 15
// -f "MyArchive.haf" -x "S.txt" "Homework7.3.jpg" "TripAside.txt" "My favourite photo.docx" -i 11 -o 15