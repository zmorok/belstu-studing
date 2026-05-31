#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
using namespace std;

static string poly_rem(string dividend_bits, const string& divisor_bits) {
    auto lstrip = [](const string& s)->string {
        size_t p = s.find('1');
        return (p == string::npos) ? "0" : s.substr(p);
        };
    string a = lstrip(dividend_bits);
    string b = lstrip(divisor_bits);
    if (b == "0") throw runtime_error("zero divisor");

    int m = (int)a.size(), n = (int)b.size();
    if (m < n) {
        if (n == 1) return "0";
        return string((n - 1) - m, '0') + a;
    }
    vector<char> w(a.begin(), a.end());
    for (int i = 0; i <= m - n; ++i) {
        if (w[i] == '1') {
            for (int j = 0; j < n; ++j)
                w[i + j] = (w[i + j] == b[j] ? '0' : '1');
        }
    }
    string rem = (n > 1) ? string(w.end() - (n - 1), w.end()) : "0";
    if ((int)rem.size() < n - 1) rem = string((n - 1) - (int)rem.size(), '0') + rem;
    return rem;
}

static void printMat(const vector<vector<int>>& M, const string& title) {
    cout << title << ":\n";
    for (auto& row : M) {
        for (int x : row) cout << x;
        cout << "\n";
    }
    cout << "\n";
}

static vector<vector<int>> firstGenFromXk(const string& Xk, int r, int rows /*=k*/) {
    string base = Xk + string(r, '0'); // длина n
    int n = (int)base.size();
    vector<int> cur(n);
    for (int i = 0;i < n;++i) cur[i] = (base[i] == '1');

    vector<vector<int>> G(rows, vector<int>(n));
    vector<int> tmp = cur;
    for (int i = 0;i < rows;++i) {
        G[i] = tmp;
        rotate(tmp.begin(), prev(tmp.end()), tmp.end()); // сдвиг вправо
    }
    return G;
}

static vector<vector<int>> canonicalize_left_I(vector<vector<int>> G, int k) {
    int m = (int)G.size();
    int n = (int)G[0].size();
    for (int col = 0, row = 0; col < k && row < k; ++col, ++row) {
        if (G[row][col] == 0) {
            int found = -1;
            for (int i = row + 1;i < m;++i) if (G[i][col] == 1) { found = i;break; }
            if (found != -1) for (int j = 0;j < n;++j) G[row][j] ^= G[found][j];
        }
        if (G[row][col] == 1) {
            for (int i = 0;i < m;++i) if (i != row && G[i][col] == 1)
                for (int j = 0;j < n;++j) G[i][j] ^= G[row][j];
        }
    }
    return G;
}

static vector<vector<int>> build_user_G_from_Hcanon(const vector<vector<int>>& Hcanon, int k, int r) {
    int n = k + r; // 13
    vector<vector<int>> P(k, vector<int>(r, 0));
    for (int i = 0;i < k;++i)
        for (int j = 0;j < r;++j)
            P[i][j] = Hcanon[i][k + j];

    vector<vector<int>> Pt(r, vector<int>(k, 0));
    for (int i = 0;i < k;++i)
        for (int j = 0;j < r;++j)
            Pt[j][i] = P[i][j];

    vector<vector<int>> Guser(r, vector<int>(n, 0));
    for (int i = 0;i < r;++i) {
        for (int j = 0;j < k;++j) Guser[i][j] = Pt[i][j];
        Guser[i][k + i] = 1; 
    }
    return Guser;
}

static vector<vector<int>> build_H_from_userG(const vector<vector<int>>& Guser, int k, int r) {
    return Guser;
}

static string vecToBits(const vector<int>& v) { string s; s.reserve(v.size()); for (int x : v) s.push_back(x ? '1' : '0'); return s; }
static vector<int> bitsToVec(const string& s) { vector<int> v(s.size()); for (int i = 0;i < (int)s.size();++i) v[i] = (s[i] == '1'); return v; }

static int find_error_pos_by_syndrome(const string& synd, const vector<vector<int>>& H) {
    int r = (int)H.size();
    int n = (int)H[0].size();
    for (int col = 0; col < n; ++col) {
        bool eq = true;
        for (int row = 0; row < r; ++row) {
            if (H[row][col] != (synd[row] == '1')) { eq = false; break; }
        }
        if (eq) return col;
    }
    return -1;
}

int main() {
	setlocale(LC_ALL, "Russian");
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int k = 7, r = 6, n = k + r;
    const string g = "1010111";
    const string Xk = "1000101";

    cout << "Параметры: k = " << k << ", r = " << r << ", n = " << n << "\n";
    cout << "g(x) двоично: " << g << "\n";
    cout << "g(x) полиномиально: x^6 + x^4 + x^2 + x + 1\n\n";

    string Xr = poly_rem(Xk + string(r, '0'), g);
    string Xn = Xk + Xr;
    cout << "Информационное сообщение (Xk): " << Xk << "\n";
    cout << "Избыточные биты (Xr):        " << Xr << "\n";
    cout << "Кодовое слово (Xn=Xk||Xr):   " << Xn << "\n\n";

    auto Gfirst = firstGenFromXk(Xk, r, k);
    printMat(Gfirst, "Порождающая матрица Хемминга");

    auto Hcanon = canonicalize_left_I(Gfirst, k);
    printMat(Hcanon, "Каноническая матрица Хемминга");

    auto G_user = build_user_G_from_Hcanon(Hcanon, k, r);
    printMat(G_user, "Каноническая порождающая матрица");

    auto H = build_H_from_userG(G_user, k, r);

    auto flip_at = [](string s, int pos) { s[pos] = (s[pos] == '1' ? '0' : '1'); return s; };
    string Y0 = Xn;
    string Y1 = flip_at(Xn, 7);
    string Y2 = flip_at(flip_at(Xn, 2), 10);

    cout << "Сообщение с 0 ошибок: " << Y0 << "\n";
    cout << "Сообщение с 1 ошибкой: " << Y1 << "\n";
    cout << "Сообщение с 2 ошибками: " << Y2 << "\n\n";

    string S0 = poly_rem(Y0, g);
    string S1 = poly_rem(Y1, g);
    string S2 = poly_rem(Y2, g);
	cout << "Синдромы:\n";
    cout << "0 ошибок: " << S0 << "\n";
    cout << "1 ошибка: " << S1 << "\n";
    cout << "2 ошибки: " << S2 << "\n\n";

    auto make_e = [&](const string& S)->vector<int> {
        vector<int> e(n, 0);
        if (S == string(r, '0')) return e;
        int pos = find_error_pos_by_syndrome(S, H);
        if (pos != -1) e[pos] = 1;
        return e;
        };
    vector<int> e0 = make_e(S0);
    vector<int> e1 = make_e(S1);
    vector<int> e2 = make_e(S2);

    cout << "En с 0 ошибок: " << vecToBits(e0) << "\n";
    cout << "En с 1 ошибкой: " << vecToBits(e1) << "\n";
    cout << "En с 2 ошибками: " << vecToBits(e2) << "\n\n";

    auto xor_str = [](const string& a, const vector<int>& b)->string {
        string out = a;
        for (int i = 0;i < (int)a.size();++i)
            out[i] = ((a[i] - '0') ^ b[i]) ? '1' : '0';
        return out;
        };
    string fix0 = xor_str(Y0, e0);
    string fix1 = xor_str(Y1, e1);
    string fix2 = xor_str(Y2, e2);

    cout << "0 ошибок: " << (fix0 == Xn ? "сообщение исправлено\n" : "сообщение не исправлено\n");
    cout << "1 ошибка: " << (fix1 == Xn ? "сообщение исправлено\n" : "сообщение не исправлено\n");
    cout << "2 ошибки: " << (fix2 == Xn ? "сообщение исправлено\n" : "сообщение не исправлено\n");

    return 0;
}
