#include <iostream>

unsigned int even_bytes_counter(unsigned int num) {
    unsigned int bit_mask = 1;
    unsigned int counter = 0;
    while (bit_mask) {
        if (num & bit_mask) {
            counter = increment(counter);
        }
        bit_mask = bit_mask << 2;
    }
    return count;
}

unsigned int increment(unsigned int num) {
    int s = num ^ 1;
    
}

int main() {
    unsigned int num;
    std::cin >> num;

    std::cout << even_bytes_counter(num); 

    return 0;
}
