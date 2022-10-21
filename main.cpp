#include <iostream>
#include <cstring>

using namespace std;

///// VECTOR
const int MAX_SIZE = 5;

template<class T>
class vector {
public:
    vector(T el, int maxsz = MAX_SIZE);

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

template<class T>
vector<T>::vector(T el, int maxsz) : maxsize(maxsz), size(1) {
    pdata = new T[maxsize];
    pdata[0] = el;
    for (int i = 1; i < maxsize; ++i) {
        pdata[i] = T(0);                       // !!!!!!
    }
}

template<class T>
vector<T>::vector() : maxsize(MAX_SIZE), size(0) {
    pdata = new T[maxsize];
    for (int i = 0; i < maxsize; ++i) {
        pdata[i] = T(0);                       // !!!!!!
    }
}

template<class T>
vector<T>::vector(const vector <T> &v) : pdata(0) {
    *this = v;
}

template<class T>
vector<T>::~vector() {
    delete[] pdata;
}

template<class T>
void vector<T>::add_element(T el) {
    size++;
    if (size > maxsize) {
        resize();
    }
    pdata[size - 1] = el;
}

template<class T>
bool vector<T>::delete_element(int i) {
    if (i < 0 || i >= size) {
        return false;
    }

    for (int it = i; it < size - 1; ++it) {
        pdata[it] = pdata[it + 1];
    }
    size--;
    if (size < maxsize / 2) {
        resize();
    }
    return true;
}

template<class T>
T vector<T>::operator[](int i) const {
    if (i < 0 || i >= size) {
        return 0;                           // !!!!
    } else {
        return pdata[i];
    }
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
vector <T> &vector<T>::operator=(const vector <T> &v) {
    size = v.size;
    maxsize = v.maxsize;

    if (pdata != NULL) {
        delete[]pdata;
    }
    pdata = new T[maxsize];
    for (int i = 0; i < maxsize; ++i) {
        pdata[i] = v.pdata[i];
    }
    return *this;
}

template<class T>
void vector<T>::resize() {
    if (maxsize < size) {
        if (maxsize < 2) {
            maxsize = MAX_SIZE;
        } else {
            maxsize *= 1.5;
        }
    } else {
        return;
    }

    T *newdata = new T[maxsize];
    for (int i = 0; i < maxsize; ++i) {
        newdata[i] = T(0);
    }
    for (int i = 0; i < size - 1; ++i) {
        newdata[i] = pdata[i];
    }
    delete[] pdata;
    pdata = newdata;
}
////// VECTOR END

template<typename T, typename Comparator = std::less <T>>
class Heap {
public:
    Heap();

    Heap(const Heap &);

    Heap &operator=(const Heap &);

    ~Heap() = default;

    void insert(T el);

    const T &top() const;

    void pop();

private:
    void sift_down();

    void sift_up();

    Comparator cmp;

    vector<T> vec;
};

int main() {
    vector<int> vec;

    for (int i = 0; i < 50; ++i) {
        vec.add_element(i);
    }
    std::cout << vec;

    return 0;
}