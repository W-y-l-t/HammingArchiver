#include "HammingCode.h"

HammingCode::HammingCode(const std::string &raw_string) {
    this->raw_string_ = raw_string;
    this->raw_string_length_ = raw_string.size();
}

void HammingCode::GetCBCDecode(const std::string &s) {
    size_t count = 0;

    while ((1 << count) <= s.size()) {
        ++count;
    }

    this->control_bits_count_ = count;
}

void HammingCode::GetCBCEncode(const std::string &s) {
    size_t count = 0;
    size_t n = s.size();

    while ((1 << count) <= n + count + 1) {
        ++count;
    }

    this->control_bits_count_ = count - 1;
}

bool HammingCode::IsControlBitCorrect(size_t bit, std::vector<uint16_t> &pref) {
    uint16_t sum = 0;
    size_t current = (1 << bit) - 1;
    size_t total = this->raw_string_length_;

    for (size_t k = current; k < total; k += (1 << bit) * 2) {
        size_t l = k;
        size_t r = std::min(k + current, total - 1);

        sum ^= (l == 0 ? pref[r] : pref[r] ^ pref[l - 1]);
    }

    return (sum == 0);
}

void HammingCode::Check() {
    size_t current_index = 0;
    bool current_is_mistake = false;

    std::vector<uint16_t> prefix_xor(this->raw_string_length_, 0);
    for (size_t j = 0; j < this->raw_string_length_; ++j) {
        prefix_xor[j] = (j == 0 ? (this->raw_string_[j] - '0') :
                         (this->raw_string_[j] - '0') ^ prefix_xor[j - 1]);
    }

    for (size_t i = 0; i < this->control_bits_count_; ++i) {
        if (IsControlBitCorrect(i, prefix_xor)) {
            continue;
        }

        current_index += (1 << i);
        current_is_mistake = true;
    }

    this->problem_ =  {current_index - 1,
                       current_is_mistake};
}

size_t HammingCode::CalculateHash(const std::string &s) {
    return std::hash<std::string>{}(s);
}

void HammingCode::HandleMistakes() {
    if (this->problem_.is_mistake &&
        this->problem_.position < this->raw_string_length_) {

        this->raw_string_[this->problem_.position] =
            static_cast<char>(
                (static_cast<int>(
                    this->raw_string_[this->problem_.position]
                        - '0') ^ 1) + '0');
    }
}

void HammingCode::Decode(const std::string &s, size_t hash) {
    this->hash_ = hash;
    this->raw_string_ = s;
    this->raw_string_length_ = s.size();

    GetCBCDecode(s);
    Check();
    HandleMistakes();

    for (size_t k = 0; k < this->raw_string_length_; ++k) {
        if ((1 << static_cast<uint64_t>(log2(k + 1))) != k + 1) {
            this->decoded_string_.push_back(this->raw_string_[k]);
        }
    }

    size_t hash_after_correction = CalculateHash(this->decoded_string_);

    if (hash_after_correction != this->hash_) {
        MyExceptions::AmbiguousDecoding();
    }

    this->hash_ = hash_after_correction;
}

void HammingCode::Encode(const std::string &s) {
    this->raw_string_ = s;
    this->raw_string_length_ = s.size();

    GetCBCEncode(s);

    size_t total = this->raw_string_length_ + this->control_bits_count_;

    this->encoded_string_.resize(total, '0');

    size_t already = 0;

    for (size_t k = 0; k < total; ++k) {
        if ((1 << static_cast<uint64_t>(log2(k + 1))) != k + 1) {
            this->encoded_string_[k] = this->raw_string_[k - already];
        } else {
            already += 1;
        }
    }

    std::vector<uint16_t> prefix_xor(total, 0);
    for (size_t j = 0; j < total; ++j) {
        prefix_xor[j] = (j == 0 ? (this->encoded_string_[j] - '0') :
            (this->encoded_string_[j] - '0') ^ prefix_xor[j - 1]);
    }

    for (size_t j = 0; j < this->control_bits_count_; ++j) {
        uint16_t sum = 0;
        size_t current = (1 << j) - 1;
        for (size_t k = current; k < total; k += (1 << j) * 2) {
            size_t l = k;
            size_t r = std::min(k + current, total - 1);

            sum ^= (l == 0 ? prefix_xor[r] : prefix_xor[r] ^ prefix_xor[l - 1]);
        }
        this->encoded_string_[current] = static_cast<char>(sum + '0');
    }

    this->hash_ = CalculateHash(this->raw_string_);
}

std::string HammingCode::GetEncodedString() {
    return this->encoded_string_;
}

std::string HammingCode::GetDecodedString() {
    return this->decoded_string_;
}

size_t HammingCode::GetHash() const {
    return this->hash_;
}

size_t HammingCode::GetControlBitsCount() const {
    return this->control_bits_count_;
}

void HammingCode::Decode(const uint8_t* data, size_t hash, size_t d_bits) {
    std::string s;
    for (size_t i = 0; i < d_bits; ++i) {
        s.push_back(static_cast<char>(data[i] + '0'));
    }

    Decode(s, hash);
}

bool HammingCode::CheckValidityOfEDParameters(uint16_t encode,
                                              uint16_t decode) {
    uint16_t k = decode - encode;
    uint64_t pow = (1 << k);

    return (pow >= decode + 1);
}
