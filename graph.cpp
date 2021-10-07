#include "graph.hpp"
#include <stdio.h>
#include <string.h>

Graph out;
std::vector<int> out_st;
std::vector<char*> out_let;

void save_graph(Graph& res) {
    out = res;
}

void save_states(std::vector<int> &res) {
    out_st = res;
}

void save_alphabet(std::vector<char*> &res) {
    out_let = res;
}

std::vector<std::string> splite_fu_str(std::string s) {
    std::vector<std::string> ret;
    int j = 0, i = 0; 
    while (i != s.size())
    {
        while (i != s.size() && !isspace(s[i]))
        {
            i++;
        }
        if (i) 
        {
           ret.push_back(s.substr(j, i - j));
        }
        j = i;
        while (j != s.size() && isspace(s[j]))
        {
            j++;
        }
        i = j;
    }
    return ret;
}

void check(std::string s) {
    int cur_ver = 0;
    char cur_state = 'S';
    auto subs = splite_fu_str(s);
    for (auto c : subs) {
        int match = 0;
        for (auto e : out.edges) {
            std::string string_e(e->letter);
            if (e->in_vertex == cur_ver && string_e==c) {
                cur_ver = e->out_vertex;
                cur_state = e->out_vertex_state;
                match = 1;
            }
        }
        if (!match){
            std::cout << "NO\n";
            return;
        }
    }
    if (cur_state == 'T') {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
}

int check_automata_correctness() {
    int match = 1;
    for (auto st : out_st) {
        if (st == 0) {
            match = 0;
            std::cout << "Start state found.\n";
        }
    }
    if (match) {
        std::cout << "No start state in the terminal!\n";
        return 1;
    }

    for (auto i = 0; i <  out_st.size(); i++) {
        for (auto j = 0; j <  out_st.size(); j++) {
            if (i != j && out_st[i] == out_st[j]) {
                match = 0;
                std::cout << "States are not unique!\n";
                return 1;
            }
        }
    }
    std::cout << "States are unique.\n";

    for (auto i = 0; i <  out_let.size(); i++) {
        for (auto j = 0; j <  out_let.size(); j++) {
            std::string si(out_let[i]);
            std::string sj(out_let[j]);
            if (i != j && si == sj) {
                match = 0;
                std::cout << "Letters are not unique!\n";
                return 1;
            }
        }
    }
    std::cout << "Letter are unique.\n";  

    int res = 0;
    for (auto i : out_st)  {
        for (auto j : out_let) {
            res = 0;
            for (auto k : out.edges) {
                std::string si(k->letter);
                std::string sj(j);
                if (k->in_vertex == i && si == sj) {
                    res+=1;
                }
            }
            if (res == 0) {
                std::cout << "Terminal isn't complite!\n";
                return 1;
            } 
            if (res > 1) {
                std::cout << "Terminal isn't deterministic!\n";
                return 1;
            }
        }
    }
    std::cout << "Terminal is complite and deterministic.\n";
    return 0;
}