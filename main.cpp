#include <iostream>
#include <ctime>

template <typename T, typename Comparator>
size_t partition(T *arr, size_t l, size_t r, Comparator cmp) {
    std::srand(std::time(nullptr));

    size_t pivot_rand_ind = (rand() % (r - l)) + l;
    std::swap(arr[l], arr[pivot_rand_ind]);
    T pivot = arr[l];

    size_t i = r - 1;
    size_t j = r - 1;
    for(; j >= l + 1; j--) {
        if (cmp(pivot, arr[j])) {
            std::swap(arr[i], arr[j]);
            i--;
        }
    }
    std::swap(arr[i], arr[l]);
    return i;
}

template <typename T, typename Comparator = std::less<T>>
T kth_statistics(T *arr, size_t n, size_t k_stat, Comparator cmp = Comparator()) {
    size_t l = 0, r = n;
    while(r > l) {
        size_t pivot_ind = partition(arr, l, r, cmp);

        if (pivot_ind == k_stat) {
            return arr[k_stat];
        }

        if (pivot_ind > k_stat) {
            r = pivot_ind;
        } else {
            l = pivot_ind + 1;
        }
    }
}

int main() {
    size_t n;
    std::cin >> n;

    int *arr = new int[n];
    for(size_t i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    std::cout << kth_statistics(arr, n, n * 0.1) << std::endl;
    std::cout << kth_statistics(arr, n, n * 0.5) << std::endl;
    std::cout << kth_statistics(arr, n, n * 0.9) << std::endl;

    delete[] arr;

    return 0;
}
