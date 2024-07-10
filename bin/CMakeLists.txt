add_executable(${PROJECT_NAME} main.cpp
        ../lib/Exceptions/Exceptions.h
        ../lib/Exceptions/Exceptions.cpp
        ../lib/HammingCode/HammingCode.h
        ../lib/HammingCode/HammingCode.h
        ../lib/ArgParser/ArgParser.h
        ../lib/ArgParser/ArgParser.cpp
        ../lib/Archiver/Archiver.cpp
        ../lib/Archiver/Archiver.h
        ../lib/FilesWorker/FileHeader.cpp
        ../lib/FilesWorker/FileHeader.h)

target_link_libraries(${PROJECT_NAME} PRIVATE MyArchiver)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})