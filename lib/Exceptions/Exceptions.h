#pragma once

#include <iostream>

class MyExceptions : std::exception {
 public:
    MyExceptions();
    explicit MyExceptions(const char* new_message);

    [[nodiscard]] const char* what() const noexcept override {
        return message_;
    }

    static void AmbiguousDecoding();

    static void InvalidArguments();

    static void TooManyArguments();

    static void FileError(const std::string &file_name);

    static void EDDataIncorrect();

    static void NotEnoughArguments();

    static void FileNotFoundExtract(const std::string &file_name);
    static void FileNotFoundDelete(const std::string &file_name);

 private:
    const char* message_;
};
