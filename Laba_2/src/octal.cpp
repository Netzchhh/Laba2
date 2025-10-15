#include "../include/octal.h"

// Array realizations
Array::Array() : data_(nullptr), size_(0) {}

Array::Array(std::size_t n, unsigned char val) : size_(n), data_(n ? new unsigned char[n] : nullptr) {
    if (n > 0) {
        std::fill(data_, data_ + n, val);
    }
}
Array::Array(const std::initializer_list<unsigned char>& init) 
    : size_(init.size()), data_(init.size() ? new unsigned char[init.size()] : nullptr) {
    if (size_ > 0) {
        std::copy(init.begin(), init.end(), data_);
    }
}
Array::Array(const std::string& s) : size_(s.size()), data_(s.size() ? new unsigned char[s.size()] : nullptr) {
    if (size_ > 0) {
        std::copy(s.begin(), s.end(), data_);
    }
}
Array::Array(const Array& other) : size_(other.size_), data_(other.size_ ? new unsigned char[other.size_] : nullptr) {
    if (size_ > 0) {
        std::copy(other.data_, other.data_ + size_, data_);
    }
}
Array::Array(Array&& other) noexcept : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;
}
Array::~Array() noexcept {
    delete[] data_;
}
Array& Array::operator=(const Array& other) {
    if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        data_ = other.size_ ? new unsigned char[other.size_] : nullptr;
        if (size_ > 0) {
            std::copy(other.data_, other.data_ + size_, data_);
        }
    }
    return *this;
}
Array& Array::operator=(Array&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}
unsigned char& Array::operator[](std::size_t i) {
    if (i >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i];
}
const unsigned char& Array::operator[](std::size_t i) const {
    if (i >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i];
}
std::size_t Array::size() const {
    return size_;
}

Array Array::operator+() const {
    return *this;
}

Array Array::operator-() const {
    Array result(size_);
    for (std::size_t i = 0; i < size_; ++i) {
        result[i] = ~data_[i];
    }
    return result;
}
Array Array::operator+(const Array& other) const {
    if (size_ != other.size_) {
        throw std::invalid_argument("Arrays must have the same size");
    }
    Array result(size_);
    for (std::size_t i = 0; i < size_; ++i) {
        result[i] = data_[i] + other.data_[i];
    }
    return result;
}
Array Array::operator-(const Array& other) const {
    if (size_ != other.size_) {
        throw std::invalid_argument("Arrays must have the same size");
    }
    Array result(size_);
    for (std::size_t i = 0; i < size_; ++i) {
        result[i] = data_[i] - other.data_[i];
    }
    return result;
}
Array Array::operator*(const Array& other) const {
    if (size_ != other.size_) {
        throw std::invalid_argument("Arrays must have the same size");
    }
    Array result(size_);
    for (std::size_t i = 0; i < size_; ++i) {
        result[i] = data_[i] * other.data_[i];
    }
    return result;
}
// Octal realizations
Octal::Octal() : digits_(1, 0) {}

Octal::Octal(const std::string& s) {
    if (s.empty()) {
        *this = Octal();
        return;
    }
    digits_ = Array(s.length());
    bool valid = true;
    for (std::size_t i = 0; i < s.length(); ++i) {
        char c = s[s.length() - 1 - i];  // little-endian
        unsigned char d = c - '0';
        if (d > 7) {
            valid = false;
            break;
        }
        digits_[i] = d;
    }
    if (!valid) {
        throw std::invalid_argument("Invalid octal digit");
    }
    trim();
}
unsigned char& Octal::operator[](std::size_t i) {
    return digits_[i];
}
const unsigned char& Octal::operator[](std::size_t i) const {
    return digits_[i];
}
std::size_t Octal::size() const {
    return digits_.size();
}
Octal Octal::operator+() const {
    return *this;
}
Octal Octal::operator-() const {
    Octal result = *this;
    for (std::size_t i = 0; i < result.size(); ++i) {
        result[i] = 7 - result[i];
    }
    // Для двух's complement добавляем 1
    return result;
}
Octal Octal::operator+(const Octal& other) const {
    std::size_t max_size = std::max(size(), other.size());
    Octal result;
    result.digits_ = Array(max_size + 1, 0);
    int carry = 0;
    for (std::size_t i = 0; i < max_size || carry; ++i) {
        int sum = carry;
        if (i < size()) sum += (*this)[i];
        if (i < other.size()) sum += other[i];
        if (i < result.size()) {
            result.digits_[i] = sum % 8;
        } else {
            result.digits_ = Array(i + 1, 0);
            result.digits_[i] = sum % 8;
        }
        carry = sum / 8;
    }
    result.trim();
    return result;
}
Octal Octal::operator-(const Octal& other) const {
    // Предполагаем *this >= other
    Octal result = *this;
    int borrow = 0;
    std::size_t max_size = std::max(size(), other.size());
    for (std::size_t i = 0; i < max_size; ++i) {
        int diff = (i < size() ? (*this)[i] : 0) - borrow;
        if (i < other.size()) diff -= other[i];
        if (diff < 0) {
            diff += 8;
            borrow = 1;
        } else {
            borrow = 0;
        }
        if (i < result.size()) {
            result[i] = diff;
        }
    }
    result.trim();
    return result;
}
Octal Octal::operator*(const Octal& other) const {
    Octal result;
    result.digits_ = Array(size() + other.size(), 0);
    for (std::size_t i = 0; i < size(); ++i) {
        int carry = 0;
        for (std::size_t j = 0; j < other.size() || carry; ++j) {
            long long mul = result.digits_[i + j] +
                            (long long)(*this)[i] * (j < other.size() ? other[j] : 0) + carry;
            if (i + j < result.size()) {
                result.digits_[i + j] = mul % 8;
            }
            carry = mul / 8;
        }
    }
    result.trim();
    return result;
}
std::string Octal::to_string() const {
    if (size() == 0 || (size() == 1 && digits_[0] == 0)) return "0";
    std::string res;
    for (std::size_t i = size() - 1; i != std::size_t(-1); --i) {
        res += '0' + digits_[i];
    }
    return res;
}