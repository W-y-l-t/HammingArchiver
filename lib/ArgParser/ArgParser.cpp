#include "ArgParser.h"

Ability::Ability(int32_t argc, char **argv) {
    std::vector<std::string> arguments(argv, argv + argc);

    for (size_t i = 0; i < argc; ++i) {
        std::string current_arg = arguments[i];

        if (current_arg == "-c" || current_arg == "--create") {
            this->create_ = true;

        } else if (current_arg == "-f") {
            if (!this->archive_name_.empty()) {
                MyExceptions::TooManyArguments();
            }

            if (i + 1 == argc) {
                MyExceptions::InvalidArguments();
            }

            this->archive_name_ = arguments[i + 1];
            ++i;

        } else if (current_arg.starts_with("--file=")) {
            this->archive_name_ = current_arg.substr(kFileLongArgSize);

        } else if (current_arg == "-l" || current_arg == "--list") {
            this->list_ = true;

        } else if (current_arg == "-x" || current_arg == "--extract") {
            this->extract_ = true;

        } else if (current_arg == "-a" || current_arg == "--append") {
            this->append_ = true;

        } else if (current_arg == "-d" || current_arg == "--delete") {
            this->delete_ = true;

        } else if (current_arg == "-A" || current_arg == "--concatenate") {
            if (this->concatenate_) {
                MyExceptions::TooManyArguments();
            }

            this->concatenate_ = true;

            if (i + 1 == argc || i + 2 == argc) {
                MyExceptions::InvalidArguments();
            }

            this->concatenated_archives_ =
                {arguments[i + 1], arguments[i + 2]};

        } else if (current_arg == "-i") {
            this->is_encode_bits_in_block_custom_ = true;

            if (i + 1 == argc) {
                MyExceptions::InvalidArguments();
            }

            this->encode_bits_in_block_ = std::stoull(arguments[i + 1]);
            ++i;

        } else if (current_arg == "--in=") {
            this->encode_bits_in_block_ =
                std::stoull(current_arg.substr(kEncodeBitsInBlock));

        } else if (current_arg == "-o") {
            this->is_decode_bits_in_block_custom_ = true;

            if (i + 1 == argc) {
                MyExceptions::InvalidArguments();
            }

            this->decode_bits_in_block_ = std::stoull(arguments[i + 1]);
            ++i;

        } else if (current_arg == "--out=") {
            this->decode_bits_in_block_ =
                std::stoull(current_arg.substr(kDecodeBitsInBlock));

        } else {
            file_names_.push_back(current_arg);
        }
    }
}

