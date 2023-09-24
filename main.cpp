/**
Please note that if your RE is
like (a/b)*abb then for the program write it
like (a/b)*.a.b.b because '.' is defined here
as concat operation.
**/
#include <bits/stdc++.h>
using namespace std;

struct trans {
  int from;
  int to;
  char symbol;
};
class DFA {
public:
  vector<trans> transitions;
  set<int> final_state;
  int start_state;
  DFA() {}
  void set_transitions(int f, int s, char symb) {
    trans new_trans;
    new_trans.from = f;
    new_trans.to = s;
    new_trans.symbol = symb;
    transitions.push_back(new_trans);
  }

  void set_final_state(int f) { final_state.insert(f); }

  void set_start_state(int s) { start_state = s; }

  set<int> get_final_state() { return final_state; }

  int get_start_state() { return start_state; }

  void display() {
    int i;
    for (i = 0; i < transitions.size(); i++) {
      cout << "From: " << transitions[i].from << " -- "
           << "To: " << transitions[i].to << " -- "
           << "Symbol: " << transitions[i].symbol << endl;
    }
    cout << "------------------------------------------------------------------"
            "--------------"
         << endl;
    cout << "Start State: " << get_start_state() << " and ";
    cout << "Final States: ";
    for (auto j : final_state) {
      cout << j << ",";
    }
    cout << endl;
  }
};

class NFA {
public:
  vector<trans> transitions;
  int final_state;
  int start_state;
  NFA() {}
  void set_transitions(int f, int s, char symb) {
    trans new_trans;
    new_trans.from = f;
    new_trans.to = s;
    new_trans.symbol = symb;
    transitions.push_back(new_trans);
  }

  void set_final_state(int f) { final_state = f; }

  void set_start_state(int s) { start_state = s; }

  int get_final_state() { return final_state; }

  int get_start_state() { return start_state; }

  void display() {
    int i;
    for (i = 0; i < transitions.size(); i++) {
      cout << "From: " << transitions[i].from << " -- "
           << "To: " << transitions[i].to << " -- "
           << "Symbol: " << transitions[i].symbol << endl;
    }
    cout << "------------------------------------------------------------------"
            "--------------"
         << endl;
    cout << "Start State: " << get_start_state() << " and ";
    cout << "Final State: " << get_final_state() << endl;
  }

  vector<vector<pair<int, char>>> make_adjacency_list() {
    int mini = INT_MAX;
    int maxi = INT_MIN;
    int i;
    for (i = 0; i < transitions.size(); i++) {
      if (transitions[i].from > maxi) {
        maxi = transitions[i].from;
      }
      if (transitions[i].to > maxi) {
        maxi = transitions[i].to;
      }
      if (transitions[i].from < mini) {
        mini = transitions[i].from;
      }
      if (transitions[i].to < mini) {
        mini = transitions[i].to;
      }
    }
    int size = (maxi - mini) + 2;
    vector<vector<pair<int, char>>> adj(size);
    for (i = 0; i < transitions.size(); i++) {
      adj[transitions[i].from].push_back(
          {transitions[i].to, transitions[i].symbol});
    }
    return adj;
  }

  vector<char> make_symbols_list() {
    set<char> s;
    vector<char> result;
    int i;
    for (i = 0; i < transitions.size(); i++) {
      if (transitions[i].symbol != '$' &&
          s.find(transitions[i].symbol) == s.end()) {
        s.insert(transitions[i].symbol);
        result.push_back(transitions[i].symbol);
      }
    }
    return result;
  }
};

NFA or_op(NFA temp1, NFA temp2, int &state) {
  int prev_start_state1 = temp1.get_start_state();
  int prev_start_state2 = temp2.get_start_state();
  int prev_final_state1 = temp1.get_final_state();
  int prev_final_state2 = temp2.get_final_state();
  int new_start_state = state++;
  int new_final_state = state++;

  NFA result;

  int i;
  for (i = 0; i < temp1.transitions.size(); i++) {
    result.set_transitions(temp1.transitions[i].from, temp1.transitions[i].to,
                           temp1.transitions[i].symbol);
  }

  for (i = 0; i < temp2.transitions.size(); i++) {
    result.set_transitions(temp2.transitions[i].from, temp2.transitions[i].to,
                           temp2.transitions[i].symbol);
  }

  result.set_transitions(new_start_state, prev_start_state1, '$');
  result.set_transitions(new_start_state, prev_start_state2, '$');
  result.set_transitions(prev_final_state1, new_final_state, '$');
  result.set_transitions(prev_final_state2, new_final_state, '$');

  result.set_final_state(new_final_state);
  result.set_start_state(new_start_state);

  return result;
}

NFA concat(NFA temp1, NFA temp2) {
  int prev_start_state1 = temp1.get_start_state();
  int prev_start_state2 = temp2.get_start_state();
  int prev_final_state1 = temp1.get_final_state();
  int prev_final_state2 = temp2.get_final_state();

  NFA result;
  int i;
  for (i = 0; i < temp1.transitions.size(); i++) {
    if (temp1.transitions[i].from != prev_final_state1 &&
        temp1.transitions[i].to != prev_final_state1) {
      result.set_transitions(temp1.transitions[i].from, temp1.transitions[i].to,
                             temp1.transitions[i].symbol);
    }

    if (temp1.transitions[i].from == prev_final_state1) {
      temp1.transitions[i].from = prev_start_state2;
      result.set_transitions(temp1.transitions[i].from, temp1.transitions[i].to,
                             temp1.transitions[i].symbol);
    }

    if (temp1.transitions[i].to == prev_final_state1) {
      temp1.transitions[i].to = prev_start_state2;
      result.set_transitions(temp1.transitions[i].from, temp1.transitions[i].to,
                             temp1.transitions[i].symbol);
    }
  }

  for (i = 0; i < temp2.transitions.size(); i++) {
    result.set_transitions(temp2.transitions[i].from, temp2.transitions[i].to,
                           temp2.transitions[i].symbol);
  }

  result.set_final_state(prev_final_state2);
  result.set_start_state(prev_start_state1);

  return result;
}

NFA kleene_closure(NFA temp, int &state) {
  int prev_start_state = temp.get_start_state();
  int prev_final_state = temp.get_final_state();
  int new_start_state = state++;
  int new_final_state = state++;
  NFA result;

  int i;
  for (i = 0; i < temp.transitions.size(); i++) {
    result.set_transitions(temp.transitions[i].from, temp.transitions[i].to,
                           temp.transitions[i].symbol);
  }

  result.set_transitions(new_start_state, prev_start_state, '$');
  result.set_transitions(prev_final_state, new_final_state, '$');
  result.set_transitions(new_start_state, new_final_state, '$');
  result.set_transitions(prev_final_state, prev_start_state, '$');

  result.set_final_state(new_final_state);
  result.set_start_state(new_start_state);

  return result;
}

NFA re_to_nfa(string re) {
  int state = 0;
  int i;
  stack<NFA> operands;
  stack<char> operators;
  NFA *temp_nfa;
  for (i = 0; i < re.size(); i++) {
    if (re[i] != '(' && re[i] != ')' && re[i] != '*' && re[i] != '/' &&
        re[i] != '.') {
      temp_nfa = new NFA();
      int s = state++;
      int f = state++;
      temp_nfa->set_transitions(s, f, re[i]);
      temp_nfa->set_final_state(f);
      temp_nfa->set_start_state(s);
      operands.push(*temp_nfa);
      delete (temp_nfa);
    }

    else if (re[i] == '*') {
      NFA kleene = operands.top();
      operands.pop();

      operands.push(kleene_closure(kleene, state));
    }

    else if (re[i] == '(') {
      operators.push(re[i]);
    } else if (re[i] == '.') {
      operators.push(re[i]);
    } else if (re[i] == '/') {
      operators.push(re[i]);
    } else {
      NFA a, b;
      while (operators.top() != '(') {
        char op = operators.top();
        operators.pop();
        b = operands.top();
        operands.pop();
        a = operands.top();
        operands.pop();
        if (op == '.') {
          operands.push(concat(a, b));
        }

        if (op == '/') {
          operands.push(or_op(a, b, state));
        }
      }
      operators.pop();
    }
  }
  if (operators.size() > 0) {
    NFA a, b;
    while (operators.size() != 0) {
      char op = operators.top();
      operators.pop();
      b = operands.top();
      operands.pop();
      a = operands.top();
      operands.pop();

      if (op == '.') {
        operands.push(concat(a, b));
      }

      if (op == '/') {
        operands.push(or_op(a, b, state));
      }
    }
  }
  return operands.top();
}

void DFS(char symb, vector<vector<pair<int, char>>> adj, int i, set<int> &s) {
  for (auto j : adj[i]) {
    if (j.second == symb && (s.find(j.first) == s.end())) {
      s.insert(j.first);
      DFS(symb, adj, j.first, s);
    }
  }
  return;
}

DFA nfa_to_dfa(vector<vector<pair<int, char>>> adj, vector<char> symbols,
               int start_state, int final_state) {
  int state = 1;
  DFA result;
  map<int, set<int>> null_enclosures;
  map<set<int>, int> mp;
  int i;
  for (int i = 0; i < adj.size(); i++) {
    set<int> s;
    s.insert(i);
    DFS('$', adj, i, s);
    null_enclosures[i] = s;
  }
  set<int> st;
  queue<set<int>> null_stack;
  null_stack.push(null_enclosures[start_state]);
  mp[null_enclosures[start_state]] = state++;
  while (1) {
    if (null_stack.size() == 0)
      break;
    st = null_stack.front();
    null_stack.pop();
    cout << endl;
    for (i = 0; i < symbols.size(); i++) {
      char sym = symbols[i];
      set<int> s;
      for (auto j : st) {
        set<int> temp;
        set<int> store;
        DFS(sym, adj, j, temp);
        set_union(temp.begin(), temp.end(), s.begin(), s.end(),
                  inserter(store, store.end()));
        s = store;
      }
      set<int> sick;
      for (auto j : s) {
        set<int> temp = null_enclosures[j];
        set<int> store;
        set_union(temp.begin(), temp.end(), sick.begin(), sick.end(),
                  inserter(store, store.end()));
        sick = store;
      }
      cout << endl;
      if (sick.size() > 0 && mp.find(sick) == mp.end()) {
        mp[sick] = state++;
        null_stack.push(sick);
      }
      if (sick.size() > 0) {
        result.set_transitions(mp[st], mp[sick], sym);
        if (st.find(start_state) != st.end()) {
          result.set_start_state(mp[st]);
        }
        if (sick.find(start_state) != sick.end()) {
          result.set_start_state(mp[sick]);
        }
        if (st.find(final_state) != st.end()) {
          result.set_final_state(mp[st]);
        }
        if (sick.find(final_state) != sick.end()) {
          result.set_final_state(mp[sick]);
        }
      }
    }
  }
  return result;
}

int main() {
  cout << "Enter RE: " << endl;
  string re;
  cin >> re;
  cout << endl;
  NFA ans;
  ans = re_to_nfa(re);
  cout << "From RE to NFA using Thompson's construction: " << endl;
  cout << "--------------------------------------------------------------------"
          "------------"
       << endl;
  ans.display();
  vector<vector<pair<int, char>>> adj = ans.make_adjacency_list();
  vector<char> symbols = ans.make_symbols_list();
  int start_state = ans.get_start_state();
  int final_state = ans.get_final_state();
  DFA ans2 = nfa_to_dfa(adj, symbols, start_state, final_state);
  cout << "From NFA to DFA using Subset construction: " << endl;
  cout << "--------------------------------------------------------------------"
          "------------"
       << endl;
  ans2.display();
}