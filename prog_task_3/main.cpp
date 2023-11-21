#include <iostream>
#include <vector>
#include <format>

using namespace std;

// check if the problem is balanced
bool check_balance(int n, int m, vector <int> &s, vector <int> &d) {
    int d_sum = 0, s_sum = 0;
    for (int i = 0; i < n; i++) {
        d_sum += d[i];
    }
    for (int i = 0; i < m; i++) {
        s_sum += s[i];
    }
    return d_sum == s_sum;
}

// prints the cell of the table
void make_cell(const string& s, int width = 10) {
    cout << format("{:^{}}|", s, width);
}

// prints the transportation table
void print_table(int n, int m, vector <int> &s, vector <int> &d, vector <vector <int>> &c) {
    cout << "+---------+----------+----------+----------+----------+----------+\n";
    cout << "|";
    make_cell("", 9);
    make_cell("D1");
    make_cell("D2");
    make_cell("D3");
    make_cell("D4");
    make_cell("Supply");
    cout << "\n+---------+----------+----------+----------+----------+----------+\n";
    for (int i = 0; i < m; i++) {
        cout << "|";
        string supply = "S";
        supply += to_string(i + 1);
        make_cell(supply, 9);
        for (int j = 0; j < n; j++) {
            make_cell(to_string(c[i][j]));
        }
        make_cell(to_string(s[i]));
        cout << "\n+---------+----------+----------+----------+----------+----------+\n";
    }
    cout << "|";
    make_cell("Demand:", 9);
    int sum = 0;
    for (int i = 0; i < n; i++) {
        make_cell(to_string(d[i]));
        sum += d[i];
    }
    make_cell(to_string(sum));
    cout << "\n+---------+----------+----------+----------+----------+----------+\n";
}

// solution using north-west method
int north_west(int n, int m, vector <int> s, vector <int> d, vector <vector <int>> c) {
    int i = 0, j = 0; // i - row, j - column
    int ans = 0;
    while (i < m || j < n) {
        int cost = min(s[i], d[j]);
        ans += c[i][j] * cost;
        s[i] -= cost;
        d[j] -= cost;
        if (!s[i]) {
            i++;
        }
        if (!d[j]) {
            j++;
        }
    }
    return ans;
}

// allocating supply in the cell for vogel's method
void vogel_extract(int n, int m, vector <int> &s, vector <int> &d, vector <vector <int>> &c, int &ans, int ind,
                   int min_val, bool row) {
    if (row) {
        for (int j = 0; j < n; j++) {
            if (min_val == c[ind][j]) {
                int cost = min(d[j], s[ind]);
                ans += cost * c[ind][j];
                d[j] -= cost;
                s[ind] -= cost;
                return;
            }
        }
    } else {
        for (int i = 0; i < m; i++) {
            if (min_val == c[i][ind]) {
                int cost = min(d[ind], s[i]);
                ans += cost * c[i][ind];
                d[ind] -= cost;
                s[i] -= cost;
                return;
            }
        }
    }
}

// solution using vogel's method
int vogel(int n, int m, vector <int> s, vector <int> d, vector <vector <int>> c, int ans = 0) {
    vector <int> row_diff(m), column_diff(n);
    int max_row_diff = -INT_MAX, max_column_diff = -INT_MAX; // maximum difference of two minimum elements
    int min_row_val = INT_MAX, min_column_val = INT_MAX; // minimum value in the solution row/column
    int min_row_ind = 0, min_column_ind = 0; // index of the solution row/column
    for (int i = 0; i < m; i++) {
        if (!s[i]) {
            continue;
        }
        int mn1 = INT_MAX, mn2 = INT_MAX;
        // finding two minimum values available
        for (int j = 0; j < n; j++) {
            if (!d[j]) {
                continue;
            }
            if (mn1 > c[i][j]) {
                swap(mn1, mn2);
                mn1 = c[i][j];
            } else if (mn2 > c[i][j]) {
                mn2 = c[i][j];
            }
        }
        row_diff[i] = mn2 - mn1;
        if (mn2 != INT_MAX && row_diff[i] > max_row_diff) { // if the max difference can be updated
            max_row_diff = row_diff[i];
            min_row_val = mn1;
            min_row_ind = i;
        } else if (row_diff[i] == max_row_diff && min_row_val > mn1) { // if the minimum value can be updated only
            min_row_val = mn1;
            min_row_ind = i;
        }
    }
    for (int j = 0; j < n; j++) {
        if (!d[j]) {
            continue;
        }
        int mn1 = INT_MAX, mn2 = INT_MAX;
        // finding two minimum values available
        for (int i = 0; i < m; i++) {
            if (!s[i]) {
                continue;
            }
            if (mn1 > c[i][j]) {
                swap(mn1, mn2);
                mn1 = c[i][j];
            } else if (mn2 > c[i][j]) {
                mn2 = c[i][j];
            }
        }
        column_diff[j] = mn2 - mn1;
        if (mn2 != INT_MAX && column_diff[j] > max_column_diff) { // if the max difference can be updated
            max_column_diff = column_diff[j];
            min_column_val = mn1;
            min_column_ind = j;
        } else if (column_diff[j] == max_column_diff && min_column_val > mn1) { // if the minimum value can be updated
            min_column_val = mn1;
            min_column_ind = j;
        }
    }
    // if the maximum difference is in the columns, then allocate there
    if (max_row_diff < max_column_diff) {
        vogel_extract(n, m, s, d, c, ans, min_column_ind, min_column_val, false);
        return vogel(n, m, s, d, c, ans);
    }
    // if the maximum difference is in the rows, then allocate there
    if (max_row_diff > max_column_diff) {
        vogel_extract(n, m, s, d, c, ans, min_row_ind, min_row_val, true);
        return vogel(n, m, s, d, c, ans);
    }
    // if the maximum difference is equal in both columns and rows, but minimum c value is in row, then update there
    if (max_row_diff != -INT_MAX && max_row_diff == max_column_diff && min_row_val < min_column_val) {
        vogel_extract(n, m, s, d, c, ans, min_row_ind, min_row_val, true);
        return vogel(n, m, s, d, c, ans);
    }
    // if the maximum difference is equal in both columns and rows, but minimum c value is in column, then update there
    if (max_row_diff != -INT_MAX && max_row_diff == max_column_diff && min_row_val >= min_column_val) {
        vogel_extract(n, m, s, d, c, ans, min_column_ind, min_column_val, false);
        return vogel(n, m, s, d, c, ans);
    }
    // if the maximum difference has not been calculated, then find minimum single value in the table
    int mn = INT_MAX, i_ind = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (s[i] != 0 && d[j] != 0 && mn > c[i][j]) {
                mn = c[i][j];
                i_ind = i;
            }
        }
    }
    // move to the next iteration
    vogel_extract(n, m, s, d, c, ans, i_ind, mn, true);
    return ans;
}

// allocation of supply for russel's method
void russel_allocate(vector <int> &s, vector <int> &d, vector <vector <int>> &c, int &ans, int i, int j) {
    int cost = min(s[i], d[j]);
    ans += cost * c[i][j];
    s[i] -= cost;
    d[j] -= cost;
}

// solution using russell's method
int russell(int n, int m, vector <int> s, vector <int> d, vector <vector <int>> c, int ans = 0) {
    vector <int> max_row_val(m, -INT_MAX), max_column_val(n, -INT_MAX); // max values in rows and columns
    vector <vector <int>> delta(m); // delta table, delta[i][j] = c[i][j] - (max_row_val[i] + max_column_val[j])
    // calculate maximum row and column values
    for (int i = 0; i < m; i++) {
        if (!s[i]) {
            continue;
        }
        for (int j = 0; j < n; j++) {
            if (!d[j]) {
                continue;
            }
            max_row_val[i] = max(max_row_val[i], c[i][j]);
            max_column_val[j] = max(max_column_val[j], c[i][j]);
        }
    }
    // calculating delta table and updating the coordinates of the solution cell
    int min_delta = INT_MAX, min_i = 0, min_j = 0, min_c;
    for (int i = 0; i < m; i++) {
        if (!s[i]) {
            continue;
        }
        delta[i].resize(n, INT_MAX);
        for (int j = 0; j < n; j++) {
            if (!d[j]) {
                continue;
            }
            delta[i][j] = c[i][j] - max_row_val[i] - max_column_val[j]; // calculating delta
            if (delta[i][j] < 0 && min_delta > delta[i][j]) { // if best solution can be updated
                min_delta = delta[i][j];
                min_i = i;
                min_j = j;
                min_c = c[i][j];
            } else if (delta[i][j] < 0 && min_delta == delta[i][j] && c[i][j] < min_c) { // if c value can be updated
                min_i = i;
                min_j = j;
                min_c = c[i][j];
            }
        }
    }
    if (min_delta == INT_MAX) { // if best solution was not found, then it is the last iteration
        return ans;
    }
    // move to the next iteration
    russel_allocate(s, d, c, ans, min_i, min_j);
    return russell(n, m, s, d, c, ans);
}

int main() {
    int n = 4, m = 3; // n - number of destinations, m = number of sources
    vector <int> s(m), d(n); // s - sources, d - destinations
    vector <vector <int>> c(m); // table of cost values
    for (auto &it: s) {
        cin >> it;
    }
    for (auto &it: c) {
        it.resize(n);
        for (auto &to: it) {
            cin >> to;
        }
    }
    for (auto &it: d) {
        cin >> it;
    }
    if (!check_balance(n, m, s, d)) { // check if the problem is balanced
        cout << "The problem is not balanced!" << endl;
        return 0;
    }
    print_table(n, m, s, d, c); // demonstrate the input parameter table according to the conditions
    cout << format("North-west method: {}\n", north_west(n, m, s, d, c));
    cout << format("Vogel's method: {}\n", vogel(n, m, s, d, c));
    cout << format("Russell's method: {}\n", russell(n, m, s, d, c));
    return 0;
}
