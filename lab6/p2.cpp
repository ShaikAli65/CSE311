// lalith dutt

#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
vector<vector<double>> get_cofactor(vector<vector<double>>
                                        &matrix,
                                    int i, int j)
{
    int n = matrix.size();
    int r = 0;
    int c = 0;
    vector<vector<double>> cf(n - 1, vector<double>(n - 1));
    for (int x = 0; x < n; x++)
    {
        for (int y = 0; y < n; y++)
        {
            if (x != i && y != j)
            {
                cf[r][c] = matrix[x][y];
                c++;
                if (c == n - 1)
                {
                    c = 0;
                    r++;
                }
            }
        }
    }
    return cf;
}
double get_determinant(vector<vector<double>> &matrix)
{
    double det = 0;
    int n = matrix.size();
    if (n == 1)
        return matrix[0][0];
    if (n == 2)
        return matrix[0][0] * matrix[1][1] - matrix[0][1] *
                                                 matrix[1][0];
    for (int col = 0; col < n; col++)
    {
        int sign = col % 2 == 1 ? -1 : 1;
        vector<vector<double>> minor = get_cofactor(matrix, 0, col);
        det += matrix[0][col] * sign * get_determinant(minor);
    }
    return det;
}

vector<vector<double>> transpose(vector<vector<double>>
                                     &cofactor)
{
    int n = cofactor.size();
    vector<vector<double>> adj(n, vector<double>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            adj[i][j] = cofactor[j][i];
        }
        return adj;
    }
}

vector<vector<double>> get_adjoint(vector<vector<double>>
                                       &matrix)
{
    int n = matrix.size();
    vector<vector<double>> cofactor(n, vector<double>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int sign = ((i + j) % 2 == 1) ? -1 : 1;
            vector<vector<double>> cf = get_cofactor(matrix, i,
                                                     j);
            cofactor[i][j] = sign * get_determinant(cf);
        }
    }
    vector<vector<double>> adjoint = transpose(cofactor);
    return adjoint;
}

int main()
{
    double start_time = omp_get_wtime();
    int n;
    vector<vector<double>> inverse(n, vector<double>(n));
    cout << "Enter n value :";
    cin >> n;
    cout << "Enter the matrix : " << endl;
    vector<vector<double>> matrix(n, vector<double>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> matrix[i][j];
        }
    }
    vector<vector<double>> adj = get_adjoint(matrix);
    cout << "-----------------" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
    double det = get_determinant(matrix);
    cout << "determinant : " << det << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            inverse[i][j] = (adj[i][j]) / det;
        }
    }
    cout << "-----------------" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << inverse[i][j] << " | ";
        }
        cout << "\n";
        cout << "-----------------" << endl;
    }
    double end_time = omp_get_wtime();
    cout << "Execution time: " << (end_time - start_time) << " seconds" << endl;
    return 0;
}
