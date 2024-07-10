#include "../lib/HammingCode/HammingCode.h"

#include <gtest/gtest.h>

TEST(HammingTestSuite, Encode1) {
    std::string input = "1010";

    std::string predict = "1011010";

    HammingCode object;
    object.Encode(input);

    ASSERT_EQ(object.GetEncodedString(), predict);
}

TEST(HammingTestSuite, Encode2) {
    std::string input = "1010100101001010101010100001"
                        "01010010101010101001010101000";

    std::string predict = "0110010110010100010101010101000"
                          "10101010010101010101001010101000";

    HammingCode object;
    object.Encode(input);

    ASSERT_EQ(object.GetEncodedString(), predict);
}

TEST(HammingTestSuite, Encode3) {
    std::string input = "1010100101111111111111111010101010"
                        "0101010010111111111110000000001010"
                        "1010010101010100101010101010100101"
                        "0000001010101110101000101010101001"
                        "0101000000000000001010100101001010"
                        "1110101010111110010101010010010000"
                        "0101011001010101010010101010111100"
                        "101010101";

    std::string predict = "11110101100101101111111111111100"
                          "10101010010101001011111111111001"
                          "00000001010101001010101010010101"
                          "01010101001010000001010101110101"
                          "10001010101010010101000000000000"
                          "00101010010100101011101010101111"
                          "10010101010010010000010101100101"
                          "0101010010101010111100101010101";

    HammingCode object;
    object.Encode(input);

    ASSERT_EQ(object.GetEncodedString(), predict);
}

TEST(HammingTestSuite, Decode1) {
    std::string input = "1011010";

    std::string predict = "1010";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;
    object.Decode(input, p_h);

    ASSERT_EQ(object.GetDecodedString(), predict);
}

TEST(HammingTestSuite, Decode2) {
    std::string input = "0110010110010100010101010101000"
                        "10101010010101010101001010101000";

    std::string predict = "1010100101001010101010100001"
                          "01010010101010101001010101000";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;
    object.Decode(input, p_h);

    ASSERT_EQ(object.GetDecodedString(), predict);
}

TEST(HammingTestSuite, Decode3) {
    std::string input = "11110101100101101111111111111100"
                        "10101010010101001011111111111001"
                        "00000001010101001010101010010101"
                        "01010101001010000001010101110101"
                        "10001010101010010101000000000000"
                        "00101010010100101011101010101111"
                        "10010101010010010000010101100101"
                        "0101010010101010111100101010101";

    std::string predict = "1010100101111111111111111010101010"
                          "0101010010111111111110000000001010"
                          "1010010101010100101010101010100101"
                          "0000001010101110101000101010101001"
                          "0101000000000000001010100101001010"
                          "1110101010111110010101010010010000"
                          "0101011001010101010010101010111100"
                          "101010101";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;
    object.Decode(input, p_h);

    ASSERT_EQ(object.GetDecodedString(), predict);
}

TEST(HammingTestSuite, DetectMistake1) {
    std::string input = "1001010";

    std::string predict = "1010";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;
    object.Decode(input, p_h);

    ASSERT_EQ(object.GetDecodedString(), predict);
}

TEST(HammingTestSuite, DetectMistake2) {
    std::string input = "0110010110010100010101010101000"
                        "10101010010101010001001010101000";

    std::string predict = "1010100101001010101010100001"
                          "01010010101010101001010101000";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;
    object.Decode(input, p_h);

    ASSERT_EQ(object.GetDecodedString(), predict);
}

TEST(HammingTestSuite, DetectMistake3) {
    std::string input = "11110101100101101111111111111100"
                        "10101010010101001011111111111001"
                        "00000001010100001010101010010101"
                        "01010101001010000001010101110101"
                        "10001010101010010101000000000000"
                        "00101010010100101011101010101111"
                        "10010101010010010000010101100101"
                        "0101010010101010111100101010101";

    std::string predict = "1010100101111111111111111010101010"
                          "0101010010111111111110000000001010"
                          "1010010101010100101010101010100101"
                          "0000001010101110101000101010101001"
                          "0101000000000000001010100101001010"
                          "1110101010111110010101010010010000"
                          "0101011001010101010010101010111100"
                          "101010101";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;
    object.Decode(input, p_h);

    ASSERT_EQ(object.GetDecodedString(), predict);
}

TEST(HammingTestSuite, DetectAmbgiousMistake1) {
    std::string input = "1001011";

    std::string predict = "1010";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;

    ASSERT_DEATH(object.Decode(input, p_h), "");
}

TEST(HammingTestSuite, DetectAmbgiousMistake2) {
    std::string input = "0110010010010100010101010101000"
                        "10101010010101010001001010101000";

    std::string predict = "1010100101001110101010100001"
                          "01010010101010101001010101000";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;

    ASSERT_DEATH(object.Decode(input, p_h), "");
}

TEST(HammingTestSuite, DetectAmbgiousMistake3) {
    std::string input = "11110101100101101111111111111100"
                        "10101010010101001011111111111001"
                        "00000001010100001010101010010101"
                        "01010101001010000001010101110101"
                        "10001010101010010101000000000000"
                        "00101010011100101011101010101111"
                        "10010101010010010000010101100101"
                        "0101010010101010111100101010101";

    std::string predict = "1010100101111111111111111010101010"
                          "0101010010111111111110000000001010"
                          "1010010101010100101010101010100101"
                          "0000001010101110101000101010101001"
                          "0101000000000000001010100101001010"
                          "1110101010111011010101010010010000"
                          "0101011001010101010010101010111100"
                          "101010101";
    size_t p_h = std::hash<std::string>{}(predict);

    HammingCode object;

    ASSERT_DEATH(object.Decode(input, p_h), "");
}