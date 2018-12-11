#pragma once
#include<iostream>
#include<vector>
#include<cmath>


typedef std::vector<std::vector<double>> Matrix;
typedef std::vector<double> Vector;

/*-----------------Some useful functions----------------------------------*/
double Det(Matrix A)
{
	double determinant = 0;
	if (A.size() == 2) 
	{
		determinant = A[0][0] * A[1][1] - A[0][1] * A[1][0];
		return determinant;
	}

	else
	{
		for (int i = 0; i < A.size; i++)
		{
			Matrix tmp = A;
			tmp.erase(tmp.begin());
			for (int j = 0; j < tmp.size(); j++)
				tmp[j].erase(tmp[j].begin() + i);
			determinant += pow(-1, i + 2) * A[0][i] * Det(tmp);
		}
	}
}

bool Check_det(Matrix A)
{
	return Det(A) != 0;
}

void Upper_triangle_matrix(Matrix A, Vector b)
{
	int size = b.size;
	double buf;
	for(int j = 0; j < size; j++)
		for (int i = j + 1; i < size; i++)
		{
			buf = A[i][j] / A[j][j];
			for (int k = j; k < size; k++)
				A[i][k] -= buf*A[j][k];
			b[i] -= buf*b[j];
		}
}

bool Converge(Matrix A)
{
	double sum = 0;
	int size = A.size();
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			sum += pow(A[i][j], 2);
	double eNorm = sqrt(sum);
	return (eNorm < 1);
}

double VectorNorm(const Vector &v)
{
	double tmpSum = 0;
	for (double val : v) {
		tmpSum += pow(val, 2);
	}
	return sqrt(tmpSum);
}

Vector mul_Z(Matrix A, Vector v)
{
	Vector res(v.size());
	for (int i = 0; i<v.size(); i++)
	{
		double sum = 0;
		for (int j = 0; j<v.size(); j++)
		{
			sum += A[i][j] * v[j];
		}
		res[i] = sum;
	}
	return res;
}

Vector subtr(const Vector &v1, const Vector &v2)
{
	Vector result(v1.size());
	if (v1.size() != v2.size()) {
		std::cout << "subtraction error!" << std::endl;
	}
	else {
		for (int i = 0; i < v1.size(); i++) {
			result[i] = v1[i] - v2[i];
		}
	}
	return result;
}

Vector Sum(Vector v1, Vector v2)
{
	int size = v1.size();
	Vector res(size);
	if (v1.size() != v2.size())
	{
		std::cout << "sum error! " << "v1 is " << v1.size() << " v2 is " << v2.size() << std::endl;
		return res;
	}
	for (int i = 0; i<size; i++)
	{
		res[i] = v1[i] + v2[i];
	}
	return res;
}

/*--------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////
/*----------------------------------Kramer----------------------------------*/

void Kramer(Matrix A, Vector b, Vector res)
{
	int size = A.size();
	res.clear();
	double detA = Det(A);
	Vector dets;
	if (Check_det(A))
	{
		for (int i = 0; i < size; i++)
		{
			Matrix tmp = A;
			for (int j = 0; j < tmp.size; j++)
			{
				tmp[j][i] = b[j];
			}
			dets.push_back(Det(tmp));
		}
		for (int i = 0; i < dets.size; i++)
			res.push_back(dets[i] / detA);
	}
}

/*--------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////
/*----------------------------------Gauss-----------------------------------*/

void Gauss(Matrix A, Vector b, Vector res)
{
	Matrix tmp = A;
	Vector buf = b;
	int size = b.size();
	Upper_triangle_matrix(tmp, buf);
	res[size - 1] = buf[size - 1] / tmp[size - 1][size - 1];
	for (int i = size - 2; i >= 0; i--)
	{
		double sum = 0;

		for (int j = i + 1; j < size; j++)
			sum += A[i][j] * res[j];

		res[i] = (buf[i] - sum) / A[i][i];
	}
}


/*--------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////
/*----------------------------LU-decomposition------------------------------*/

void LUDecompos(Matrix A, Vector b, Vector res)
{
	Matrix L;
	Matrix U;
	int size = b.size();
	
	for(int i = 0; i< size; i++)
		for (int j = 0; j < size; j++)
		{
			U[i][j] = 0;
			L[i][j] = 0;
			if (i == j)
				U[i][j] = 1;
		}

	for (int i = 0; i < size; i++)
	{
		L[i][0] = A[i][0];
		U[0][i] = A[0][i] / L[0][0];
	}

	for(int i = 1; i < size; i++)
		for (int j = 1; j < size; j++)
		{
			if (i >= j)
			{
				double sum = 0;
				for (int k = 0; k < j; k++)
					sum += L[i][k] * U[k][j];
				L[i][j] = A[i][j] - sum;
			}
			else
			{
				double sum = 0;
				for (int k = 0; k < i; k++)
					sum += L[i][k] * U[k][j];
				U[i][j] = (A[i][j] - sum) / L[i][i];
			}
		}

	/* Ax = b
	   LUx = b
	   Ux = y
	   Ly = b 
	*/

	Vector y;
	y[0] = b[0] / L[0][0];
	for (int i = 1; i < size; i++)
	{
		double sum = 0;
		for (int j = i - 1; j >= 0; j++)
			sum += y[j] * L[i][j];
		y[i] = (b[i] - sum) / L[i][i];
	}

	res[size - 1] = y[size - 1];
	for (int i = size - 2; i >= 0; i--)
	{
		double sum = 0;
		for (int j = i + 1; j < size; j++)
			sum += U[i][j] * res[j];
		res[i] = y[i] - sum;
	}
}

/*--------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////
/*----------------------------------Seidel----------------------------------*/

void Seidel(Matrix A, Vector b, Vector res, double eps)
{
	Matrix Buf;
	Vector tmp;
	int size = A.size();

	for (int i = 0; i<b.size(); i++)
		tmp.push_back(b[i] / A[i][i]);

	for (const Vector &v : A)
		Buf.push_back(v);

	for (int i = 0; i<size; i++) 
		for (int j = 0; j<size; j++) 
		{
			if (i != j) Buf[i][j] = -(A[i][j] / A[i][i]);
			else Buf[i][j] = 0;
		}

	if (Converge(Buf))
	{
		double currentEps = VectorNorm(subtr(b, mul_Z(A, res)));
		while (currentEps > eps) {
			res = Sum(tmp, mul_Z(Buf, res));
			currentEps = VectorNorm(subtr(b, mul_Z(A, res)));
		}
	}
}

/*--------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////
/*------------------------------Fixed-point iteration-----------------------*/

void FixedPointIter(Matrix A, Vector b, Vector res, double eps)
{
	int size = b.size();
	Vector x0(size);
	double currentEps = VectorNorm(subtr(b, mul_Z(A, res)));

	while (currentEps > eps) {

		for (int i = 0; i < size; i++) {
			x0 = res;
			double tmpX = 0;
			for (int j = 0; j < size; j++) {
				if (j == i) continue;
				tmpX -= A[i][j] * x0[j];
			}
			res[i] = (tmpX + b[i]) / A[i][i];
		}
		currentEps = VectorNorm(subtr(b, mul_Z(A, res)));
	}
}

/*--------------------------------------------------------------------------*/