 #ifndef H_GRAPH
 #define H_GRAPH
 
#include <vector> 
#include <iostream>
#include <string>

struct Edge {
    int in_vertex;
    char in_vertex_state;
    char* letter;
    int out_vertex;
    char out_vertex_state;
    Edge(int v, char state, char* letter_, int v2, char state2) {
        in_vertex = v;
        in_vertex_state = state;
        letter = letter_;
        out_vertex  = v2;
        out_vertex_state = state2;
    }
};

struct Graph {
    int start_vertex;
    std::vector<char*> alphabet;
    std::vector<int> states;
    std::vector<Edge*> edges;
    Graph() {}
    Graph* addEdge(Edge *e) {
        edges.push_back(e);
        return this;
    }
};

void save_graph(Graph& res);

void save_alphabet(std::vector<char*> &res);

void save_states(std::vector<int> &res);

void check(std::string s);

int check_automata_correctness();


#endif