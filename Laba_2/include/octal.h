#ifndef OCTAL_H
#define OCTAL_H

#include <string>
#include <algorithm>
#include <stdexcept>
#include <cstddef>
#include <initializer_list>
#include <cstring> 

class Array {
private:
    unsigned char* data_;
    std::size_t size_;
public:
    Array();
    // Constructor with size and common start digit
    Array(std::size_t n, unsigned char val = 0);
    // Конструктор из initializer_list
    Array(const std::initializer_list<unsigned char>& init);

    Array(const std::string& s);
    // Copy constructoy
    Array(const Array& other);
    // moving constructor
    Array(Array&& other) noexcept;
    // Destructor
    virtual ~Array() noexcept;
    // equal by copy
    Array& operator=(const Array& other);
    // equal by moving
    Array& operator=(Array&& other) noexcept;
    // get by
    unsigned char& operator[](std::size_t i);
    const unsigned char& operator[](std::size_t i) const;
    std::size_t size() const;
    // Unar+
    Array operator+() const;
    // Unar-
    Array operator-() const;
    // Opeartors: +, -, *
    Array operator+(const Array& other) const;
    Array operator-(const Array& other) const;
    Array operator*(const Array& other) const;
};

class Octal {
private:
    Array digits_;

    // Delete the leading Zeros
    void trim() {
        std::size_t new_size = digits_.size();
        while (new_size > 0 && digits_[new_size - 1] == 0) {
            --new_size;
        }
        if (new_size == 0) {
            digits_ = Array(1, 0); 
        } else if (new_size < digits_.size()) {
            Array new_digits(new_size);
            for (std::size_t i = 0; i < new_size; ++i) {
                new_digits[i] = digits_[i];
            }
            digits_ = std::move(new_digits);
        }
    }

public:
    Octal();
    // Construct from text
    Octal(const std::string& s);
    // Get by index
    unsigned char& operator[](std::size_t i);
    const unsigned char& operator[](std::size_t i) const;
    std::size_t size() const;
    // Unar+
    Octal operator+() const;
    // Unar-
    Octal operator-() const;
    // Opeartors: +, -, *
    Octal operator+(const Octal& other) const;
    Octal operator-(const Octal& other) const;
    Octal operator*(const Octal& other) const;
    std::string to_string() const;
};
#endif // OCTAL_H
