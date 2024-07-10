#include "Exceptions.h"

MyExceptions::MyExceptions() {
    this->message_ = "Unpredictable error!";
}

MyExceptions::MyExceptions(const char *new_message) {
    this->message_ = new_message;
}

void MyExceptions::AmbiguousDecoding() {
    MyExceptions exception("More than one error was detected "
                         "during the decoding process. "
                         "It is impossible to restore the file!");

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}

void MyExceptions::InvalidArguments() {
    MyExceptions exception("The arguments entered in the command line "
                         "are incorrect");

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}

void MyExceptions::TooManyArguments() {
    MyExceptions exception("There are too many arguments "
                           "of some type. "
                           "See the specification");

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}

void MyExceptions::FileError(const std::string &file_name) {
    MyExceptions exception(("An error occurred while "
                           "opening the file" + file_name).c_str());

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}

void MyExceptions::EDDataIncorrect() {
    MyExceptions exception("Given arguments for encoding and "
                           "decoding are invalid");

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}

void MyExceptions::NotEnoughArguments() {
    MyExceptions exception("Not enough arguments!");

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}

void MyExceptions::FileNotFoundExtract(const std::string &file_name) {
    MyExceptions exception(("During extracting file was "
                           "got an error: There's no file with name " +
                            file_name).c_str());

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}

void MyExceptions::FileNotFoundDelete(const std::string &file_name) {
    MyExceptions exception(("During deleting file was "
                           "got an error: There's no file with name " +
                            file_name).c_str());

    std::cerr << exception.what();
    exit(EXIT_FAILURE);
}
