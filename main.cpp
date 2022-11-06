#include <iostream>
#include <cassert>

struct bounds_t {
    size_t l;
    size_t r;
};

bounds_t exp_search(const long arr[], size_t n) {
    size_t l = 0, r = 1;
    while (true) {
        if (arr[r] < arr[r - 1]) {  // если массив, начиная с какого то индекса >= l до r, убывает то диапазон верный
            return bounds_t{l, r};
        }

        if (r == n - 1) {
            break;
        }

        l = r;
        r = (r * 2 >= n ? n - 1 : r * 2);
    }
    return bounds_t{0, 0};  // если массив нигде не убывает возвращаем {0, 0}
}

size_t bin_search(const long arr[], size_t l, size_t r) {
    while (r - l > 1) {
        size_t mid = (l + r) / 2;

        if (arr[mid] > arr[mid + 1] &&
            arr[mid] > arr[mid - 1]) {
            return mid;
        }

        if (arr[mid] < arr[mid - 1]) {
            r = mid - 1;
        }
        if (arr[mid] < arr[mid + 1]) {
            l = mid + 1;
        }
    }
    return (arr[l] > arr[r] ? l : r);  // при оставшихся двух или одном элементах возвращаем индекс большего
}

size_t search(const long arr[], size_t n) {
    bounds_t bounds = exp_search(arr, n);

    if (bounds.l == 0 && bounds.r == 0) {
        return n - 1;  // если массив нигде не убывает, значит m = n - 1
    }

    return bin_search(arr, bounds.l, bounds.r);
}

void test(size_t res, const long arr[], size_t n) {  // функция для тестирования, использующая линейный поиск
    long max = arr[0];
    size_t max_ind = 0;
    for(size_t i = 0; i < n; ++i) {
        if (arr[i] > max) {
            max = arr[i];
            max_ind = i;
        }
    }
    assert(max_ind == res);
}

int main() {
    size_t n;
    std::cin >> n;
    long *arr = new long[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    size_t m = search(arr, n);

    std::cout << m << std::endl;

    delete[] arr;
    return 0;
}
