#include <iostream>

unsigned int increment(unsigned int num);

unsigned int even_bytes_counter(unsigned int num) {
    unsigned int counter = 0;
    while (num) {
        if (num & 1) {
            counter = increment(counter);
        }
        num >>= 2;
    }
    return counter;
}

unsigned int increment(unsigned int num) {
    unsigned int bit = 1;
    for (; !((num ^= bit) & bit); bit <<= 1);
    return (num |= bit);
}

int main() {
    unsigned int num;
    std::cin >> num;

    std::cout << even_bytes_counter(num) << std::endl;

    return 0;
}
