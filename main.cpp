#include <bits/stdc++.h>

using namespace std;

class Simplex {
public:
    int n;
    int m;
    vector <double> z;
    vector <vector <double> > table;
    vector <double> sol;
    double ans = 0;
    Simplex(int n, int m, vector <double> &c, vector <vector <double>> &a, vector <double> &b) {
        this->n = n;
        this->m = m;
        z.resize(m);
        for (int i = 0; i < m; i++) {
            z[i] = -c[i];
        }
        sol.resize(n);
        for (int i = 0; i < n; i++) {
            sol[i] = b[i];
        }
        table.resize(n);
        for (int i = 0; i < n; i++) {
            table[i].resize(m);
            for (int j = 0; j < m; j++) {
                table[i][j] = a[i][j];
            }
        }
    }
    void print_info() {
        cout << "_____________________" << endl;
        for (auto &it: z) {
            cout << it << ' ';
        }
        cout << "| " << ans << endl;
        for (int i = 0; i < n; i++) {
            for (auto &to: table[i]) {
                cout << to << ' ';
            }
            cout << "| " << sol[i] << endl;
        }
    }
    int pivot_column() {
        double min = DBL_MAX;
        int ind = -1;
        for (int i = 0; i < m; i++) {
            if (z[i] < 0 && z[i] < min) {
                min = z[i];
                ind = i;
            }
        }
        return ind;
    }
    int pivot_row(int column) {
        double min = DBL_MAX;
        int ind = -1;
        for (int i = 0; i < n; i++) {
            double cur = sol[i] / table[i][column];
            if (cur >= 0 && cur < min) {
                min = cur;
                ind = i;
            }
        }
        return ind;
    }
    void normalize_pivot_row(int row, int column) {
        for (int i = 0; i < m; i++) {
            if (i == column) {
                continue;
            }
            table[row][i] /= table[row][column];
        }
        sol[row] /= table[row][column];
        table[row][column] = 1;
    }
    void normalize_all_rows(int row, int column) {
        for (int i = 0; i < n; i++) {
            if (i == row) {
                continue;
            }
            double k = table[i][column] / table[row][column];
            for (int j = 0; j < m; j++) {
                table[i][j] -= table[row][j] * k;
            }
            sol[i] -= k * sol[row];
        }
        double k = z[column] / table[row][column];
        for (int i = 0; i < m; i++) {
            z[i] -= k * table[row][i];
        }
        ans -= k * sol[row];
    }
    void solve() {
        while (true) {
            int pivotColumn = pivot_column();
            if (pivotColumn == -1) {
                return;
            }
            int pivotRow = pivot_row(pivotColumn);
            if (pivotRow == -1) {
                return;
            }
            normalize_pivot_row(pivotRow, pivotColumn);
            normalize_all_rows(pivotRow, pivotColumn);
        }
    }
    void print_ans() {
        cout << "X*: ";
        for (auto &it: sol) {
            cout << it << ' ';
        }
        cout << endl;
        cout << "Max: " << ans << endl;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    vector <double> c(m);
    vector <vector <double>> a(n);
    vector <double> b(n);
    for (int i = 0; i < m; i++) {
        cin >> c[i];
    }
    for (int i = 0; i < n; i++) {
            a[i].resize(m);
        for (int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }
    Simplex simplex(n, m, c, a, b);
    simplex.solve();
    simplex.print_ans();
}