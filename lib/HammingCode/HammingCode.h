#include "../ArgParser/ArgParser.h"

#include <cmath>
#include <unordered_map>

struct Problem {
    size_t position;
    bool is_mistake;

    Problem() :
            position{}, is_mistake(false) {};

    Problem(const Problem &problem) = default;
    Problem(size_t position, bool is_mistake) :
            position(position), is_mistake(is_mistake) {};

    ~Problem() = default;
};

class HammingCode {
 public:
    HammingCode() = default;
    HammingCode(const HammingCode &hamming_code) = default;
    explicit HammingCode(const std::string &raw_string);
    ~HammingCode() = default;

    void GetCBCDecode(const std::string &s);
    void GetCBCEncode(const std::string &s);

    void Check();
    void HandleMistakes();

    bool IsControlBitCorrect(size_t bit, std::vector<uint16_t> &pref);

    static size_t CalculateHash(const std::string &s);

    void Decode(const std::string &s, size_t hash);
    void Decode(const uint8_t data[], size_t hash, size_t d_bits);
    void Encode(const std::string &s);

    std::string GetEncodedString();
    std::string GetDecodedString();

    [[nodiscard]] size_t GetHash() const;

    [[nodiscard]] size_t GetControlBitsCount() const;

    static bool CheckValidityOfEDParameters(uint16_t encode, uint16_t decode);

 private:
    std::string raw_string_;
    std::string encoded_string_;
    std::string decoded_string_;

    size_t hash_{};

    size_t raw_string_length_{};
    size_t control_bits_count_{};

    std::vector<uint8_t> storage_;

    Problem problem_;
};