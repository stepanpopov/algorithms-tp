#include <iostream>

///// VECTOR
const int MAX_SIZE = 5;  // TODO const static

template<class T>
class vector {
public:
    vector(size_t size, T el = T(0), int maxsz = MAX_SIZE);

    vector();

    vector(const vector<T> &v);

    ~vector();

    void add_element(T el);

    bool delete_element(int i);

    T operator[](int i) const;

    T &operator[](int i);

    void sort();

    int get_size() const { return size; }

    int get_maxsize() const { return maxsize; }

    int find(T el);

    vector<T> &operator=(const vector<T> &v);

    friend
    std::ostream &operator<<(std::ostream &out, vector<T> &v) {
        out << "size:" << v.size << " maxsize:" << v.maxsize << std::endl;
        for (int i = 0; i < v.size; ++i) {
            out << v.pdata[i] << std::endl;
        }
        return out;
    }

protected:
    int maxsize;
    int size;
    T *pdata;
private:
    void resize();
};

// #include "vector.inl"
template<class T>
vector<T>::vector(size_t size, T el, int maxsz) : maxsize(maxsz), size(size) {
    maxsize = (maxsz > size * 2 ? maxsz : size * 2);
    pdata = new T[maxsize];
    for (int i = 0; i < size; ++i) {
        pdata[i] = el;
    }
    /*for (int i = 1; i < maxsize; ++i) {
        pdata[i] = T(0);                       // !!!!!!
    }*/
}

template<class T>
vector<T>::vector() : maxsize(MAX_SIZE), size(0) {
    pdata = new T[maxsize];
    /*for (int i = 0; i < maxsize; ++i) {
        pdata[i] = T(0);                       // !!!!!!
    }*/
}

template<class T>
vector<T>::vector(const vector<T> &v) : pdata(0) {
    *this = v;
}

template<class T>
vector<T>::~vector() {
    delete[] pdata;
}

template<class T>
void vector<T>::add_element(T el) {
    if (size == maxsize) {
        resize();
    }
    size++;
    pdata[size - 1] = el;
}

template<class T>
bool vector<T>::delete_element(int i) {
    if (size == 0) {
        return false;
    }
    if (i < 0 || i >= size) {
        return false;
    }

    for (int it = i; it < size - 1; ++it) {
        pdata[it] = pdata[it + 1];
    }
    size--;
    return true;
}

template<class T>
T vector<T>::operator[](int i) const {
    return pdata[i];
}

template<class T>
T &vector<T>::operator[](int i) {
    return pdata[i];
}

template<class T>
void vector<T>::sort() {
    T temp;

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (pdata[i] > pdata[j]) {
                temp = pdata[i];
                pdata[i] = pdata[j];
                pdata[j] = temp;
            }
        }
    }
}

/*template<>
void vector<char *>::sort() {
    char *temp;
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (strcmp(pdata[i], pdata[j]) > 0) {
                temp = pdata[i];
                pdata[i] = pdata[j];
                pdata[j] = temp;
            }
        }
    }
}*/

template<class T>
int vector<T>::find(T el) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(el, pdata[i]) == 0) {
            return i;
        }
    }
    return -1;
}

template<class T>
vector<T> &vector<T>::operator=(const vector<T> &v) {
    size = v.size;
    maxsize = v.maxsize;

    if (pdata != NULL) {
        delete[] pdata;
    }
    pdata = new T[maxsize];
    for (int i = 0; i < maxsize; ++i) {
        pdata[i] = v.pdata[i];
    }
    return *this;
}

template<class T>
void vector<T>::resize() {
    if (maxsize <= size) {
        if (maxsize < 2) {
            maxsize = MAX_SIZE;
        } else {
            maxsize *= 2;
        }
    } else {
        return;
    }

    T *newdata = new T[maxsize];
    /*for (int i = 0; i < maxsize; ++i) {
        newdata[i] = T(0);
    }*/
    for (int i = 0; i < size; ++i) {
        newdata[i] = pdata[i];
    }
    delete[] pdata;
    pdata = newdata;
}

////// VECTOR END

template<typename T, typename Comparator = std::less <T>>
class Heap {   // TODO конструктор по массиву
public:
    Heap(Comparator comp = Comparator()) : cmp(comp), arr() {}
    // Heap() : cmp(Comparator()), arr() {}

    Heap(size_t size, T el, Comparator comp = Comparator()) : cmp(comp), arr(size, el) {}

    Heap(const Heap &) = default;

    Heap &operator=(const Heap &) = default;

    ~Heap() = default;

    void push(T el) {
        arr.add_element(el);
        sift_up(arr.get_size() - 1);
    }

    T top() const {
        return arr[0];  // почему копия
    }

    void pop() {
        arr[0] = arr[arr.get_size() - 1];
        arr.delete_element(arr.get_size() - 1);
        sift_down(0);
    }

    friend
    std::ostream &operator<<(std::ostream &out, Heap<T, Comparator> heap) {
        out << "size:" << heap.arr.get_size() << std::endl;
        for (int i = 0; i < heap.arr.get_size(); ++i) {
            out << heap.arr[i] << " ";
        }
        out << std::endl;
        return out;
    }

private:
    void sift_down(size_t i) {
        size_t left_ind = 2 * i + 1;
        size_t right_ind = 2 * i + 2;

        size_t max_ind = i;
        if (left_ind < arr.get_size() && cmp(arr[i], arr[left_ind])) {
            max_ind = left_ind;
        }
        if (right_ind < arr.get_size() && cmp(arr[max_ind], arr[right_ind])) {
            max_ind = right_ind;
        }

        if (i != max_ind) {
            T temp = arr[max_ind];
            arr[max_ind] = arr[i];
            arr[i] = temp;

            sift_down(max_ind);
        }
    }

    void sift_up(size_t i) {
        if (i == 0) return;

        size_t parent_ind = (i - 1) / 2;
        if (!cmp(arr[parent_ind], arr[i])) {
            return;
        } else {
            T temp = arr[parent_ind];
            arr[parent_ind] = arr[i];
            arr[i] = temp;
        }
        sift_up(parent_ind);
        /*while( i > 0 ) {
            int parent = ( i - 1 ) / 2;
            if( arr[i] <= arr[parent] )
                return;
            std::swap( arr[i], arr[parent] );
            i = parent;
        }*/
    }

    void build_heap() {
        for (size_t i = arr.get_size() / 2 - 1; i >= 0; --i) {
            sift_down(i);
        }
    }

    Comparator cmp;

    vector<T> arr;
};

void test() {
    // auto lambda = [](int a, int b) { return a < b; };
    Heap<int, std::less < int>>
    heap((std::less<int>()));

    heap.push(5);
    heap.push(56);
    heap.push(-7);
    heap.push(99);
    heap.push(3);

    std::cout << heap;
    heap.pop();
    heap.pop();
    heap.pop();

    std::cout << heap;
}

struct array_iter {
    int *buf = nullptr;
    size_t pos = 0;
    size_t size = 0;
};

vector<int> heap_merge(array_iter array_of_iters[], size_t k, size_t n) {
    vector<int> res;

    auto cmp = [](array_iter it1, array_iter it2) {
        return it1.buf[it1.pos] > it2.buf[it2.pos];
    };

    Heap<array_iter, decltype(cmp)> heap(cmp);

    for (size_t i = 0; i < k; ++i) {
        heap.push(array_of_iters[i]);
    }

    for (size_t i = 0; i < n; ++i) {
        array_iter top = heap.top();
        heap.pop();

        res.add_element(top.buf[top.pos]);

        top.pos++;
        if (top.pos < top.size) {
            heap.push(top);
        }
    }

    return res;
}

int main() {
    size_t k = 0;
    size_t n = 0;
    std::cin >> k;

    array_iter *array_of_iters = new array_iter[k];

    for (size_t i = 0; i < k; ++i) {
        size_t size_buf = 0;
        std::cin >> size_buf;

        array_iter iter;
        iter.buf = new int[size_buf];
        iter.size = size_buf;

        for (size_t j = 0; j < size_buf; ++j) {
            std::cin >> iter.buf[j];
        }
        array_of_iters[i] = iter;
        n += size_buf;
    }

    vector<int> result = heap_merge(array_of_iters, k, n);

    for (size_t i = 0; i < n; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;


    for (size_t i = 0; i < k; ++i) {
        delete[] array_of_iters[i].buf;
    }
    delete[] array_of_iters;

    return 0;
}