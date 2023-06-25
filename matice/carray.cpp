#include <stdexcept>
#include <iostream>
#include <cassert>

using namespace std;

template <typename T>
class CArray {
public:
    CArray(size_t size = 0) : size(size) {
        data = new T[size];
    }

    ~CArray() {
        delete []data;
    }

    CArray& operator = (const CArray& other) {
        if(this == &other)
            return *this;

        delete []data;
        size = other.size;
        data = new T[size];

        for(size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    CArray(const CArray& other) {
        size = other.size;
        data = new T[size];

        for(size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    T& operator [] (size_t pos) {
        if(pos >= size) {
            throw invalid_argument("Index out of range!");
        }
        return data[pos];
    }

    bool operator == (const CArray& other) const {
        if(this == &other)
            return true;
        if(size != other.size)
            return false;

        for(size_t i = 0; i < size; i++) {
            if(data[i] != other.data[i])
                return false;
        }
        return true;
    }

private:
    size_t size;
    T * data;
};

template <typename T>
class CArray2D {
public:
    CArray2D(size_t rows, size_t cols) : rows(rows), data(new CArray<T>[rows]) {
        for(size_t i = 0; i < rows; i++) {
            data[i] = CArray<T>(cols);
        }
    }

    ~CArray2D() {
        delete [] data;
    }

    CArray2D(const CArray2D& other) : rows(other.rows), data(new CArray<T>[rows]) {
        for(size_t i = 0; i < rows; i++) {
            data[i] = other.data[i];
        }
    }

    CArray2D& operator = (const CArray2D& other) {
        if(this == &other)
            return *this;

        delete [] data;

        rows = other.rows;
        data = new CArray<T>[rows];
        for(size_t i = 0; i < rows; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    CArray<T>& operator [] (size_t pos) {
        if(pos >= rows) {
            throw invalid_argument("Index out of range!");
        }
        return data[pos];
    }

private:
    size_t rows;
    CArray<T> * data;
};

int main () {
    CArray<int> arr(3);
    arr[0] = 69;
    assert(arr[0] == 69);
    arr[1] = 666;
    assert(arr[1] == 666);
    arr[2] = 420;
    assert(arr[2] == 420);

    assert(arr == arr);

    try {
        arr[3] = 666;
        assert("No exception thrown!" == nullptr);
    } catch (invalid_argument &e) {
    } catch (exception &e) {
        assert("Invalid excaption!" == nullptr);
    }

    CArray<string> strarr(2);
    strarr[0] = "Hello";
    strarr[1] = "World!";
    assert(strarr[0] + strarr[1] == "HelloWorld!"s);

    CArray<int> arr2(10);
    arr2[0] = 0;
    arr2 = arr;
    assert(arr2 == arr);

    CArray arr3(arr2);
    assert(arr3 == arr);
    assert(arr3 == arr2);
    arr3[0] = 10;
    assert(!(arr3 == arr2));

    arr3 = arr3;
    assert(arr3[1] == 666);

    cout << "ALL BASIC TESTS PASSED!" << endl;

    CArray2D<int> arr2d(3, 3);
    arr2d[0][0] = 69;
    assert(arr2d[0][0] == 69);

    try {
        arr2d[2][3] = 666;
        assert("No exception thrown!" == nullptr);
    } catch (invalid_argument &e) {
    } catch (exception &e) {
        assert("Invalid excaption!" == nullptr);
    }

    CArray2D<int> arr2d2 = arr2d;
    assert(arr2d2[0][0] == 69);

    CArray2D<int> arr2d3(arr2d);
    assert(arr2d3[0][0] == 69);

    cout << "ALL 2D ARRAY TESTS PASSED!" << endl;
    return 0;
}