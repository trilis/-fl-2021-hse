#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

struct DFA {
    std::set<int>vertexes;
    std::set<char>alphabet;
    std::map<int, int> terminality;
    std::map<std::pair<int, char>, int>graph;

    bool analyse(int v_now, std::string& s, int ind) {
        if (s.size() == ind) {
            return terminality[v_now] == 1;
        }
        int v_next = graph[{v_now, s[ind]}];
        if (!v_next) {
            return 0;
        }
        return analyse(v_next, s, ind + 1);
    }
};

int count_num(std::string& s, int& ind) {
    int num = 0;
    while (ind < s.size() and int(s[ind]) < 58 and int(s[ind]) > 47) {
        num *= 10;
        num += int(s[ind]) - 48;
        ind++;
    }
    return num;
}

int parser(DFA& dfa, std::string& s) {
    int n = s.size() - 1, ind = 1;
    if (s[0] != '!' or s[n] != '!') {
        return 0;
    }
    while (ind < n and s[ind] != '(') {
        int num = count_num(s, ind);
        if (num == 0) {
            return 0;
        }
        if (!dfa.vertexes.count(num)) { // Состояния автомата уникальны.
            dfa.vertexes.insert(num);
        }
        else {
            return 0;
        }
        if (s[ind] == 't') {
            dfa.terminality[num] = 1;
        }
        else if (s[ind] == 'n') {
            dfa.terminality[num] = 2;
        }
        else {
            return 0;
        }
        ind++;
    }
    if (!dfa.vertexes.count(1)) { // У автомата есть начальное состояние.
        return 0;
    }
    while (ind < n) {
        if (s[ind] != '(') {
            return 0;
        }
        ind++;
        int v_in = count_num(s, ind);
        if (!dfa.vertexes.count(v_in) or s[ind] != ',') {
            return 0;
        }
        ind++;
        int v_out = count_num(s, ind);
        if (!dfa.vertexes.count(v_in) or ind >= n or s[ind] != ',') {
            return 0;
        }
        ind++;
        if (ind >= n) {
            return 0;
        }
        char c = s[ind];
        dfa.alphabet.insert(c);
        if (dfa.graph[{v_in, c}] == 0) {
            dfa.graph[{v_in, c}] = v_out;
        }
        else {
            return 0; 
        }
        ind++;
        if (ind >= n or s[ind] != ')') {
            return 0;
        }
        ind++;
    }
    for (int v : dfa.vertexes) {
        for (char c : dfa.alphabet) {
            if (dfa.graph[{v, c}] == 0) { // Конечный автомат полон.
                return 0;
            }
        }
    }
    return 1;
}

void launch(std::string file1, std::string file2, std::string file_out) {
    std::ifstream f_in1, f_in2;
    std::ofstream f_out;
    f_in1.open(file1);
    f_in2.open(file2);
    f_out.open(file_out);
    std::string s;
    f_in1 >> s;
    DFA dfa;
    if (!parser(dfa, s)) {
        f_out << "Invalid dfa format!\n";
    }
    else {
        f_out << "Correct DFA.\n";
        f_out << "vertexes: ";
        for (int v : dfa.vertexes) {
            f_out << v << " ";
        }
        f_out << "\nalphabet: ";
        for (char c : dfa.alphabet) {
            f_out << "'" << c << "'" << " ";
        }
        f_out << "\nedges:\n";
        for (int v : dfa.vertexes) {
            for (char c : dfa.alphabet) {
                f_out << "       " << v << ' ' << dfa.graph[{v, c}] << ' ' << c << '\n';
            }
        }
        while (f_in2 >> s) {
            if (dfa.analyse(1, s, 0)) {
                f_out << "The DFA recognized the word\n";
            }
            else {
                f_out << "The DFA did not recognize the word\n";
            }
        }
    }
}

void Tests(int n) {
    for (int i = 1; i <= n; i++) {
        std::string s = std::to_string(i);
        launch("tests/test" + s + ".1.txt", "tests/test" + s + ".2.txt", "tests/test" + s + ".out.txt");
    }
}

// Я добавил перед скобками инфу про терминальные вершины: номер + t(терминальная)/n(нетерминальная)

int main(int argc, char* argv[]) {
    Tests(6);
    launch(argv[1], argv[2], "f_out.txt");
}