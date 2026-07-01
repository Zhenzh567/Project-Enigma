#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>

struct Config {
    static inline const std::string abc = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    static inline const std::vector<std::string> rotors = {
        "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
        "AJDKSIRUXBLHWTMCQGZNPYFVOE",
        "BDFHJLCPRTXVZNYEIWGAKMUSQO",
        "ESOVPZJAYQUIRHXLNFTGKDCMWB",
        "VZBRGITYUPSDNHLXAWMJQOFECK"
    };

    static inline const std::vector<char> notches = {'Q', 'E', 'V', 'J', 'Z'};
    static inline const std::string ref = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
};

inline int mod26(int val) {
    return (val % 26 + 26) % 26;
}

inline int getIndex(char c) {
    return c - 'A';
}

void saveToFile(const std::string& text, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << text;
        file.close();
        std::cout << "Успешно сохранено в " << filename << "\n";
    } else {
        std::cout << "Ошибка открытия файла\n";
    }
}

std::string encode(const std::string& src, std::string key, int r1, int r2, int r3) {
    int p1 = getIndex(toupper(key[0]));
    int p2 = getIndex(toupper(key[1]));
    int p3 = getIndex(toupper(key[2]));

    std::string res = "";

    for (char c : src) {
        char up = toupper(c);

        if (up < 'A' || up > 'Z') {
            if (c == ' ') res += ' ';
            continue;
        }

        int state = getIndex(up);

        p3 = mod26(p3 + 1);
        if (p3 == getIndex(Config::notches[r3])) {
            p2 = mod26(p2 + 1);
            if (p2 == getIndex(Config::notches[r2])) {
                p1 = mod26(p1 + 1);
            }
        }

        state = mod26(getIndex(Config::rotors[r3][mod26(state + p3)]) - p3);
        state = mod26(getIndex(Config::rotors[r2][mod26(state + p2)]) - p2);
        state = mod26(getIndex(Config::rotors[r1][mod26(state + p1)]) - p1);

        state = getIndex(Config::ref[state]);

        state = mod26((int)Config::rotors[r1].find(Config::abc[mod26(state + p1)]) - p1);
        state = mod26((int)Config::rotors[r2].find(Config::abc[mod26(state + p2)]) - p2);
        state = mod26((int)Config::rotors[r3].find(Config::abc[mod26(state + p3)]) - p3);

        res += Config::abc[state];
    }

    return res;
}

int main() {
    std::string key = "ABC";
    int r1 = 0, r2 = 1, r3 = 2;

    while (true) {
        std::cout << "\n--- МЕНЮ ---\n";
        std::cout << "Роторы: " << (r1 + 1) << " " << (r2 + 1) << " " << (r3 + 1) << "\n";
        std::cout << "Ключ: " << key << "\n";
        std::cout << "1. Изменить роторы\n";
        std::cout << "2. Изменить ключ\n";
        std::cout << "3. Зашифровать текст\n";
        std::cout << "4. Выход\n";
        std::cout << "Выбор: ";

        int act;
        if (!(std::cin >> act)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n');

        if (act == 1) {
            std::cout << "Введите 3 номера роторов (1-5): ";
            int n1, n2, n3;
            if (std::cin >> n1 >> n2 >> n3 && n1 >= 1 && n1 <= 5 && n2 >= 1 && n2 <= 5 && n3 >= 1 && n3 <= 5) {
                r1 = n1 - 1;
                r2 = n2 - 1;
                r3 = n3 - 1;
                std::cout << "Готово.\n";
            } else {
                std::cout << "Неверный ввод.\n";
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        else if (act == 2) {
            std::cout << "Введите ключ (3 буквы): ";
            std::string tmp;
            std::getline(std::cin, tmp);

            tmp.erase(std::remove_if(tmp.begin(), tmp.end(), ::isspace), tmp.end());

            if (tmp.length() == 3 && std::isalpha(tmp[0]) && std::isalpha(tmp[1]) && std::isalpha(tmp[2])) {
                key = tmp;
                for (char &sym : key) sym = toupper(sym);
                std::cout << "Готово.\n";
            } else {
                std::cout << "Ошибка в ключе.\n";
            }
        }
        else if (act == 3) {
            std::cout << "Введите текст: ";
            std::string msg;
            std::getline(std::cin, msg);

            std::string output = encode(msg, key, r1, r2, r3);
            std::cout << "Результат: " << output << "\n";

            std::cout << "Сохранить в файл? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore(10000, '\n');

            if (tolower(choice) == 'y') {
                std::cout << "Имя файла: ";
                std::string fname;
                std::getline(std::cin, fname);
                saveToFile(output, fname);
            }
        }
        else if (act == 4) {
            break;
        }
    }
    return 0;
}

