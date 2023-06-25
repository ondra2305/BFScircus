#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T_>
class CMatrix1
{
public:
	explicit CMatrix1(unsigned size = 0) : size(size)
	{
		data = new T_[size];
	}

	~CMatrix1()
	{
		delete[] data;
	}

	CMatrix1 &operator=(const CMatrix1 &other)
	{
		if (this == &other)
			return *this;

		delete[] data;
		size = other.size;
		data = new T_[size];

		for (unsigned i = 0; i < size; i++)
		{
			data[i] = other.data[i];
		}
		return *this;
	}

	CMatrix1(const CMatrix1 &other)
	{
		size = other.size;
		data = new T_[size];

		for (unsigned i = 0; i < size; i++)
		{
			data[i] = other.data[i];
		}
	}

	T_ &operator[](unsigned pos)
	{
		if (pos >= size)
		{
			throw out_of_range("Index out of range!");
		}
		return data[pos];
	}
	unsigned size;

private:
	T_ *data;
};

template <typename T_>
class CMatrix2
{
public:
	explicit CMatrix2(unsigned size3 = 0, unsigned dim2 = 0) : size2(size3)
	{
		data = new CMatrix1<T_>[size2];
		for (unsigned i = 0; i < size2; i++)
		{
			data[i] = CMatrix1<T_>(dim2);
		}
	}

	~CMatrix2()
	{
		delete[] data;
	}

	CMatrix2(const CMatrix2 &other) : size2(other.size2)
	{
		data = new CMatrix1<T_>[size2];
		for (unsigned i = 0; i < size2; i++)
		{
			data[i] = other.data[i];
		}
	}

	CMatrix2 &operator=(const CMatrix2 &other)
	{
		if (this == &other)
			return *this;

		delete[] data;

		size2 = other.size2;
		data = new CMatrix1<T_>[size2];
		for (unsigned i = 0; i < size2; i++)
		{
			data[i] = other.data[i];
		}
		return *this;
	}

	CMatrix1<T_> &operator[](unsigned pos)
	{
		if (pos >= size2)
		{
			throw out_of_range("Index out of range!");
		}
		return data[pos];
	}
	unsigned size2;

private:
	CMatrix1<T_> *data;
};

template <typename T_>
class CMatrix3
{
public:
	CMatrix3(unsigned n1, unsigned n2, unsigned n3) : size3(n1)
	{
		data = new CMatrix2<T_>[n1];
		for (unsigned i = 0; i < n1; i++)
		{
			data[i] = CMatrix2<T_>(n2, n3);
		}
	}
	~CMatrix3()
	{
		delete[] data;
	}
	CMatrix3(const CMatrix3 &other)
	{
		size3 = other.size3;
		data = new CMatrix2<T_>[size3];
		for (unsigned i = 0; i < size3; i++)
		{
			data[i] = other.data[i];
		}
	}
	CMatrix3 &operator=(const CMatrix3 &other)
	{
		if (this == &other)
			return *this;

		delete[] data;
		size3 = other.size3;
		data = new CMatrix2<T_>[size3];
		for (unsigned i = 0; i < size3; i++)
		{
			data[i] = other.data[i];
		}
		return *this;
	}
	CMatrix2<T_> &operator[](unsigned pos)
	{
		if (pos >= size3)
		{
			throw out_of_range("Index out of range in 3D matrix!");
		}
		return data[pos];
	}
	// Optional: () operator for easier matrix element access
	const T_ &operator()(unsigned dim1, unsigned dim2, unsigned dim3) const
	{
		if (dim1 >= this->size3 || dim2 >= data[0].size2 || dim3 >= data[0][0].size)
		{
			throw out_of_range("Index out of range in 3D matrix!");
		}
		return data[dim1][dim2][dim3];
	}
	unsigned size3;
private:
	CMatrix2<T_> *data;
};

#ifndef __PROGTEST__
int main(void)
{
	CMatrix3<int> m(3, 4, 5);
	m[0][0][0] = 128;
	m[2][3][1] = 7;
	assert(m[0][0][0] == 128);
	assert(m[2][3][1] == 7);

	m[1][1][1] = m[0][0][0] + m[2][3][1];
	m[2][3][4] = 135;
	assert(m[1][1][1] == m[2][3][4]);

	CMatrix3<int> copy(m);
	try
	{
		copy[4][1][2];
		assert("Missing exception for access i0 = 4, limit = 2" == nullptr);
	}
	catch (out_of_range &e)
	{
	}
	catch (...)
	{
		assert("Incorrect exception thrown" == nullptr);
	}
	try
	{
		copy[1][4][2];
		assert("Missing exception for access i1 = 4, limit = 3" == nullptr);
	}
	catch (out_of_range &e)
	{
	}
	catch (...)
	{
		assert("Incorrect exception thrown" == nullptr);
	}
	try
	{
		copy[2][1][6];
		assert("Missing exception for access i2 = 6, limit = 4" == nullptr);
	}
	catch (out_of_range &e)
	{
	}
	catch (...)
	{
		assert("Incorrect exception thrown" == nullptr);
	}

	copy[2][3][1] = 42;
	assert(m[2][3][1] == 7);
	assert(copy[2][3][1] == 42);

	CMatrix3<int> n(2, 2, 2);
	n[0][1][0] = 56;
	n[1][0][1] = 42;
	m = n;
	n[0][1][0] = 0;
	assert(m[0][1][0] == 56);
	try
	{
		m[2][1][0];
		assert("Missing exception for access i0 = 2, limit = 2" == nullptr);
	}
	catch (out_of_range &e)
	{
	}
	catch (...)
	{
		assert("Incorrect exception thrown" == nullptr);
	}

	CMatrix3<string> s(2, 1, 1);
	s[0][0][0] = "Hello ";
	s[1][0][0] = "World!";
	assert((s[0][0][0] + s[1][0][0]) == string("Hello World!"));

	assert(s(0, 0, 0) == "Hello ");
	cout << "ALL ASSERTS PASSED!" << endl;
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */