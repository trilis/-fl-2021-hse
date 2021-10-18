#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

struct AutomataState {
  std::string name;
  std::unordered_map<char, AutomataState *> transition_map;
  AutomataState* go(char c) {
    if (transition_map.find(c) == transition_map.end()) {
      if (transition_map.find('*') != transition_map.end()) {
        return transition_map['*'];
      }
      throw std::logic_error(std::string("There is no transition by symbol '") + std::to_string(c) + std::string("' in state with name ") + name);
    }
    return transition_map[c];
  } 
  void add_transition(char c, AutomataState *dest) {
    if (transition_map.find(c) != transition_map.end()) {
      throw std::logic_error("Automata is not deterministic!");
    }
    transition_map[c] = dest;
  }

  bool is_complete_state() {
    if (transition_map.find('*') != transition_map.end()) return true;
    for (char i = 'a'; i <= 'z'; ++i) {
      if (transition_map.find(i) == transition_map.end()) return false;
    }
    for (char i = 'A'; i <= 'Z'; ++i) {
      if (transition_map.find(i) == transition_map.end()) return false;
    }
    for (char i = '0'; i <= '9'; ++i) {
      if (transition_map.find(i) == transition_map.end()) return false;
    }
    return true;
  }
};

struct Transition {
  std::string from, to;
  std::vector<char> symbols;
};