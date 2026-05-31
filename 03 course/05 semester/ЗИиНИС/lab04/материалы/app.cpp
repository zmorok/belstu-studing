#include <Bits.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <bitset>

using namespace std;


static string ReadLineFromFile(const string& filePath, int targetLine) {
    ifstream in(filePath);
    if (!in) {
        cerr << "Ошибка открытия файла: " << filePath << "\n";
        exit(1);
    }
    string line;
    for (int i = 0; i < targetLine; ++i) {
        if (!getline(in, line)) {
            cerr << "Не удалось прочитать строку " << targetLine << " из файла\n";
            exit(1);
        }
    }
    // убрать \r в конце, если файл в Windows-формате
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return line;
}

static string StringToBinary(const string& input, int bitsPerChar) {
    string out;
    out.reserve(input.size() * bitsPerChar);
    for (unsigned char ch : input) {
        for (int b = bitsPerChar - 1; b >= 0; --b) {
            out.push_back(((ch >> b) & 1) ? '1' : '0');
        }
    }
    return out;
}

static void IntroduceRandomErrors(string& s, int errorCount, mt19937& rng) {
    if (s.empty() || errorCount <= 0) return;
    uniform_int_distribution<int> dist(0, (int)s.size() - 1);
    for (int i = 0; i < errorCount; ++i) {
        int pos = dist(rng);
        s[pos] = (s[pos] == '1') ? '0' : '1';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Дет. инициализация ГПСЧ от времени
    mt19937 rng((uint32_t)chrono::high_resolution_clock::now().time_since_epoch().count());

    // ###### ЗАДАНИЕ 1
    const string fileContent = ReadLineFromFile("lab04.txt", 1);
    cout << "\n###### ЗАДАНИЕ 1\n\n";
    cout << "Исходный текст из файла: " << fileContent << "\n";

    const string bitMessage = StringToBinary(fileContent, 8);
    cout << "Битовое сообщение: " << bitMessage << "\n";

    // ###### ЗАДАНИЕ 2
    cout << "\n###### ЗАДАНИЕ 2\n\n";
    const int k = (int)bitMessage.size();
    if (k == 0) {
        cout << "Файл пуст, обработка невозможна.\n";
        return 0;
    }

    int r = 0;
    while ((1 << r) < k) r++;
    r = r + 1;
    const int n = k + r;

    cout << "Длина исходного слова: " << k << "\n";
    cout << "Длина избыточной части: " << r << "\n";
    cout << "Полная длина кодового слова: " << n << "\n";

    // Матрица r x n, как в исходном коде (заполняем '-' как «нет бита»)
    vector<vector<char>> parityMatrix(r, vector<char>(n, '-'));

    const int totalComb = 1 << r;
    int col = 0;

    // Первые k столбцов — комбинации с >=2 единицами
    for (int i = 1; i < totalComb && col < k; ++i) {
        int ones = bitset<32>(i).count();
        if (ones >= 2) {
            // Запишем двоичное представление длины r сверху-вниз
            for (int b = 0; b < r; ++b) {
                int bit = (i >> (r - 1 - b)) & 1;
                parityMatrix[b][col] = bit ? '1' : '0';
            }
            ++col;
        }
    }

    // Затем r «чисто единичных» столбцов (как в исходнике)
    for (int i = 0; i < r && col < n; ++i) {
        parityMatrix[i][col] = '1';
        ++col;
    }

    cout << "Проверочная матрица Хэмминга:\n";
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < n; ++j) {
            char c = parityMatrix[i][j];
            cout << (c == '-' ? '0' : c) << ' ';
        }
        cout << '\n';
    }

    // ###### ЗАДАНИЕ 3
    cout << "\n###### ЗАДАНИЕ 3\n\n";
    string redundantBits(r, '0');
    for (int i = 0; i < r; ++i) {
        int count1 = 0;
        for (int j = 0; j < k; ++j) {
            if (bitMessage[j] == '1' && parityMatrix[i][j] == '1') count1++;
        }
        redundantBits[i] = (count1 % 2) ? '1' : '0';
    }

    string fullMsg = bitMessage + redundantBits;

    cout << "Информационные биты:\t" << bitMessage << "\n";
    cout << "Избыточные биты:\t" << redundantBits << "\n";
    cout << "Полное сообщение:\t" << fullMsg << "\n";

    // ###### ЗАДАНИЕ 4
    cout << "\n###### ЗАДАНИЕ 4\n\n";
    string msgNoErr = fullMsg;
    string msgOneErr = fullMsg;
    string msgTwoErr = fullMsg;
    IntroduceRandomErrors(msgOneErr, 1, rng);
    IntroduceRandomErrors(msgTwoErr, 2, rng);

    cout << "Без ошибок :\t\t" << msgNoErr << "\n";
    cout << "С 1 ошибкой:\t\t" << msgOneErr << "\n";
    cout << "С 2 ошибками:\t\t" << msgTwoErr << "\n";

    // ###### ЗАДАНИЕ 5
    cout << "\n###### ЗАДАНИЕ 5\n\n";
    auto calcControl = [&](const string& msgInfo) {
        string ctrl(r, '0');
        for (int i = 0; i < r; ++i) {
            int sum = 0;
            for (int j = 0; j < k; ++j) {
                if (msgInfo[j] == '1' && parityMatrix[i][j] == '1') sum++;
            }
            ctrl[i] = (sum % 2) ? '1' : '0';
        }
        return ctrl;
        };

    string controlA = calcControl(msgNoErr);
    string controlB = calcControl(msgOneErr);
    string controlC = calcControl(msgTwoErr);

    cout << "Избыточные биты исходного сообщения: " << redundantBits << "\n";
    cout << "Контрольные Yr0:\t" << controlA << "\n";
    cout << "Контрольные Yr1:\t" << controlB << "\n";
    cout << "Контрольные Yr2:\t" << controlC << "\n";

    // ###### ЗАДАНИЕ 6
    cout << "\n###### ЗАДАНИЕ 6\n\n";
    string recvA = msgNoErr.substr(k, r);
    string recvB = msgOneErr.substr(k, r);
    string recvC = msgTwoErr.substr(k, r);

    auto xorStrings01 = [](const string& a, const string& b) {
        string s(a.size(), '0');
        for (size_t i = 0; i < a.size(); ++i)
            s[i] = (a[i] == b[i]) ? '0' : '1';
        return s;
        };

    string syndA = xorStrings01(controlA, recvA);
    string syndB = xorStrings01(controlB, recvB);
    string syndC = xorStrings01(controlC, recvC);

    cout << "Синдром без ошибок: " << syndA << "\n";
    cout << "Синдром с одной ошибкой: " << syndB << "\n";
    cout << "Синдром с двумя ошибками: " << syndC << "\n\n";

    if (any_of(syndA.begin(), syndA.end(), [](char c) {return c == '1'; }))
        cout << "Ошибка обнаружена в первом сообщении.\n";
    if (any_of(syndB.begin(), syndB.end(), [](char c) {return c == '1'; }))
        cout << "Ошибка обнаружена во втором сообщении!\n";
    if (any_of(syndC.begin(), syndC.end(), [](char c) {return c == '1'; }))
        cout << "Ошибка обнаружена в третьем сообщении!\n\n";

    auto findErrorVector = [&](const string& syndrome) {
        string errVec(n, '0');
        for (int c = 0; c < n; ++c) {
            bool match = true;
            for (int rIdx = 0; rIdx < r; ++rIdx) {
                char a = parityMatrix[rIdx][c];
                char s = syndrome[rIdx];
                if (a == '-' && s == '0') continue;   // 0 считается совпадением с '-'
                if (a == s) continue;
                match = false; break;
            }
            if (match) { errVec[c] = '1'; break; }
        }
        return errVec;
        };

    string errVec1 = findErrorVector(syndB);
    string errVec2 = findErrorVector(syndC);

    auto fixByXor = [](const string& a, const string& b) {
        string out(a.size(), '0');
        for (size_t i = 0; i < a.size(); ++i)
            out[i] = (a[i] == b[i]) ? '0' : '1';
        return out;
        };

    string fixedMsg1 = fixByXor(msgOneErr, errVec1);
    string fixedMsg2 = fixByXor(msgTwoErr, errVec2);

    cout << "Переданное исходное сообщение: " << fullMsg << "\n\n";
    cout << "Сообщение с 1 ошибкой: " << msgOneErr << "\n";
    cout << "Вектор ошибки: " << errVec1 << "\n";
    cout << "Исправленное сообщение: " << fixedMsg1 << "\n\n";

    bool correct1 = (fixedMsg1 == fullMsg);
    if (!correct1) cout << "Сообщение не удалось исправить!\n";

    cout << "Сообщение с 2 ошибками: " << msgTwoErr << "\n";
    cout << "Вектор ошибки: " << errVec2 << "\n";
    cout << "Исправленное сообщение: " << fixedMsg2 << "\n";
    bool correct2 = (fixedMsg2 == fullMsg);
    if (!correct2) cout << "Сообщение не удалось исправить!\n";

    return 0;
}
