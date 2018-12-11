#pragma once 

#include <vector>

class UppDownTriangle {
public:


	std::vector<double> LU(std::vector<std::vector<double>> A, std::vector<double> B) {
		int n = A.size();
		std::vector<std::vector<double>> L(n, std::vector<double>(n));
		std::vector<std::vector<double>> U(n, std::vector<double>(n));
		std::vector<double> x(n);
		std::vector<double> y(n);
		U = A;

		for (int i = 0; i < n; i++)
			for (int j = i; j < n; j++)
				L[j][i] = U[j][i] / U[i][i];

		for (int k = 1; k < n; k++)
		{
			for (int i = k - 1; i < n; i++)
				for (int j = i; j < n; j++)
					L[j][i] = U[j][i] / U[i][i];

			for (int i = k; i < n; i++)
				for (int j = k - 1; j < n; j++)
					U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j];
		}

		y[0] = B[0] / L[0][0];

		for (int i = 1; i < n; i++) {
			double res = 0;
			for (int j = 0; j < i; j++) {
				res += y[j] * L[i][j];
			}
			y[i] = (B[i] - res) / L[i][i];
		}

		x[n - 1] = y[n - 1] / U[n - 1][n - 1];

		for (int i = n - 2; i >= 0; i--) {
			double res = 0;
			for (int j = n - 1; j >= i; j--) {
				res += x[j] * U[i][j];
			}
			x[i] = (y[i] - res) / U[i][i];
		}

		return x;
	}

};
