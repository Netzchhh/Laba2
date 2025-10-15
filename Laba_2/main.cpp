#include <iostream>
#include "../include/octal.h"

int main() {
    Octal a("10");  // 8 в dec
    Octal b("7");   // 7 в dec
    std::cout << "a = " << a.to_string() << std::endl;  // 10
    std::cout << "b = " << b.to_string() << std::endl;  // 7
    Octal c = a + b;  // 15 dec = 17 oct
    std::cout << "a + b = " << c.to_string() << std::endl;  // 17
    Octal d = a - b;  // 1 dec = 1 oct
    std::cout << "a - b = " << d.to_string() << std::endl;  // 1
    Octal e = a * b;  // 56 dec = 70 oct
    std::cout << "a * b = " << e.to_string() << std::endl;  // 70
    Octal f = -a;     // Инверсия: для "10" -> "67"
    std::cout << "-a = " << f.to_string() << std::endl;     // 67
    return 0;
}