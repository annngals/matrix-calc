#include "pch.h"
#include <iostream>
#include <vector>
#include <random>
#include <exception>

using namespace std;

class Error : public exception
{
	string m_str;
public:
	Error(const char* mess) :m_str(mess) {}

	const char* what() const noexcept override
	{
		return m_str.c_str();
	}
};

class Matrix
{
public:

	Matrix() = default;
	Matrix(size_t rows, size_t cols)
		: m_rows(rows), m_cols(cols), m_data(rows*cols) {}

	void rand_int(int min, int max)
	{
		random_device rd;
		uniform_int_distribution<int> uid{ min, max };
		for (size_t idx{}; idx < m_data.size(); ++idx)
			m_data[idx] = uid(rd);
	}

	size_t rows() const
	{
		return m_rows;
	}

	size_t cols() const
	{
		return m_cols;
	}

	const double* operator[](size_t row) const
	{
		return m_data.data() + m_cols * row;
	}

	double* operator[](size_t row)
	{
		return m_data.data() + m_cols * row;
	}

	Matrix operator+(const Matrix& other) const
	{
		if (m_rows != other.m_rows && m_cols != other.m_cols)
			throw Error("Matrix::operator+");

		Matrix ret(m_rows, m_cols);
		for (size_t idx{}; idx < m_data.size(); ++idx)
			ret.m_data[idx] = m_data[idx] + other.m_data[idx];

		return move(ret);
	}

	Matrix operator-(const Matrix& other) const
	{
		if (m_rows != other.m_rows && m_cols != other.m_cols)
			throw Error("Matrix::operator-");

		Matrix ret(m_rows, m_cols);
		for (size_t idx{}; idx < m_data.size(); ++idx)
			ret.m_data[idx] = m_data[idx] - other.m_data[idx];

		return move(ret);
	}

	Matrix operator*(const Matrix& other) const
	{
		if (m_cols != other.m_rows)
			throw Error("Matrix::operator*");

		Matrix ret(m_rows, m_cols);
		for (size_t row{}; row < m_rows; ++row)
			for (size_t col{}; col < other.m_cols; ++col)
			{
				ret[row][col] = 0;
				for (size_t k{}; k < m_cols; ++k)
					ret[row][col] += (*this)[row][k] * other[k][col];
			}

		return move(ret);
	}

private:

	size_t m_rows{};
	size_t m_cols{};
	vector<double> m_data;

};

ostream& operator<<(ostream& os, const Matrix& m)
{
	for (size_t row{}; row < m.rows(); ++row)
	{
		for (size_t col{}; col < m.cols(); ++col)
			os << m[row][col] << ' ';
		os << endl;
	}
	return os;
}

int main()
{
	size_t n;

	cout << "n = ";
	cin >> n;

	Matrix A(n, n);
	A.rand_int(-50, 50);
	cout << "A:" << endl;
	cout << A << endl;

	Matrix B(n, n);
	B.rand_int(-50, 50);
	cout << "B:" << endl;
	cout << B << endl;

	cout << "A + B:" << endl;
	cout << A + B << endl;

	cout << "A - B:" << endl;
	cout << A - B << endl;

	cout << "A * B:" << endl;
	cout << A * B << endl;
}