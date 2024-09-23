#include <bits/stdc++.h>
#include <vector>
#include<omp.h>
using namespace std;

int main()
{
    double start_time = omp_get_wtime();
    int n;
    cout << "Enter the number of equations : ";
    cin >> n;
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> B(n);
    cout << "Fill matrix A : " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }
    cout<<"---------------------------\n";
    cout << "Enter matrix B : " << endl;
    for (int i = 0; i < n; i++) {
        cin >> B[i];
    }
    vector<vector<double>> L(n, vector<double>(n, 0.0));
    vector<vector<double>> U(n, vector<double>(n));
    cout<<"---------------------------\n";
    cout << "U matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << U[i][j] << " ";
        }
        cout << endl;
    }
    // Decomposing matrix into L and U
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            U[i][j] = A[i][j];
        }
        for (int j = i + 1; j < n; j++) {
            L[j][i] = A[j][i] / A[i][i];  // Factor

            for (int k = i; k < n; k++) {
                A[j][k] -= L[j][i] * A[i][k];
            }
        }
    }
    // Set diagonal elements of L to 1
    for (int i = 0; i < n; i++) {
        L[i][i] = 1.0;
    }
    // Display L matrix
    cout<<"---------------------------\n";
    cout << "L matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << L[i][j] << " ";
        }
        cout << endl;
    }
    // Display U matrix
    cout<<"---------------------------\n";
    cout << "U matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << U[i][j] << " ";
        }
        cout << endl;
    }
    // Forward substitution to solve LY = B
    vector<double> y(n);
    for (int i = 0; i < n; i++) {
        y[i] = B[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
    }
    cout<<"---------------------------\n";
    cout << "Y matrix: " << endl;
    for (int i = 0; i < n; i++) {
        cout << y[i] << endl;
    }
    // Backward substitution to solve UX = Y
    vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }
    cout<<"---------------------------\n";
    cout << "X matrix (solution): " << endl;
    for (int i = 0; i < n; i++) {
        cout << x[i] << endl;
    }
    cout<<"---------------------------\n";
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;
    elapsed_time = elapsed_time*1e3;
    cout << "Execution time: " << elapsed_time << " milli seconds" <<endl;
    return 0;
}