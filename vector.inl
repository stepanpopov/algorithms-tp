#pragma once

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
vector<T>::vector(const vector <T> &v) : pdata(0) {
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
vector <T> &vector<T>::operator=(const vector <T> &v) {
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
