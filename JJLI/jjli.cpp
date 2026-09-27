#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <cctype>

using Str = std::string;
using Siz = size_t;
using Uch = unsigned char;
template<typename T> using Vec = std::vector<T>;
template<typename K, typename V> using Ump = std::unordered_map<K, V>;
template<typename T> using Stk = std::stack<T>;

Str prepMuls(const Str& in) {
    Str res;
    Siz i = 0;
    while (i < in.length()) {
        char op = in[i];
        if (op == '+' || op == '-') {
            res += op;
            ++i;
            if (i < in.length() && in[i] == '*') {
                int mul = 0;
                while (i < in.length() && in[i] == '*') {
                    mul += 10;
                    ++i;
                }
                int add = 0;
                while (i < in.length() && in[i] == op) {
                    add++;
                    ++i;
                }
                for (int k = 1; k < mul + add; ++k) {
                    res += op;
                }
            }
        } else {
            res += in[i];
            ++i;
        }
    }
    return res;
}

class JJLI {
private:
    Ump<Siz, Siz> jmp;
    Vec<Uch> tape;
    Siz ptr;
    Str prog;
    Siz pc;
    Siz len;

    void getJmp() {
        Stk<Siz> s;
        for (Siz i = 0; i < len; ++i) {
            if (prog[i] == '[') {
                s.push(i);
            } else if (prog[i] == ']') {
                if (!s.empty()) {
                    Siz top = s.top();
                    s.pop();
                    jmp[top] = i;
                    jmp[i] = top;
                }
            }
        }
    }

    void skipSp() {
        while (pc < len && std::isspace(prog[pc])) {
            pc++;
        }
    }

    int evalObj() {
        skipSp();
        if (pc >= len) return 0;
        int val = 0;
        if (prog[pc] == '$') {
            val = tape[ptr];
            pc++;
            return val;
        }
        if (prog[pc] == '@') {
            Siz tar = 0;
            while (pc < len && prog[pc] == '@') {
                tar++;
                pc++;
            }
            tar--;
            if (tar >= tape.size()) tape.resize(tar + 1024, 0);
            return tape[tar];
        }
        if (prog[pc] == '<' || prog[pc] == '>') {
            Siz tmp = ptr;
            while (pc < len && (prog[pc] == '<' || prog[pc] == '>')) {
                if (prog[pc] == '>') tmp++;
                else if (prog[pc] == '<' && tmp > 0) tmp--;
                pc++;
            }
            if (tmp >= tape.size()) tape.resize(tmp + 1024, 0);
            return tape[tmp];
        }
        if (prog[pc] == '+' || prog[pc] == '-') {
            while (pc < len && (prog[pc] == '+' || prog[pc] == '-')) {
                if (prog[pc] == '+') val++;
                else if (prog[pc] == '-') val--;
                pc++;
            }
            return val;
        }
        return 0;
    }

    void execBr() {
        skipSp();
        if (pc >= len) return;
        if (prog[pc] == '@') {
            Siz tar = 0;
            while (pc < len && prog[pc] == '@') {
                tar++;
                pc++;
            }
            ptr = tar - 1;
        } else if (prog[pc] == '>') {
            ptr++;
            pc++;
        } else if (prog[pc] == '<') {
            if (ptr > 0) ptr--;
            pc++;
        } else if (prog[pc] == '+') {
            tape[ptr]++;
            pc++;
        } else if (prog[pc] == '-') {
            tape[ptr]--;
            pc++;
        } else if (prog[pc] == '.') {
            std::cout << tape[ptr];
            pc++;
        } else if (prog[pc] == ',') {
            std::cin >> tape[ptr];
            pc++;
        }
    }

    void skipDelim(char d1, char d2 = '\0') {
        int dpt = 0;
        while (pc < len) {
            if (prog[pc] == '[') dpt++;
            else if (prog[pc] == ']') dpt--;
            else if (dpt == 0 && (prog[pc] == d1 || (d2 != '\0' && prog[pc] == d2))) return;
            pc++;
        }
    }

public:
    JJLI(Str raw) : tape(32768, 0), ptr(0), pc(0) {
        prog = prepMuls(raw);
        len = prog.length();
        getJmp();
    }

    void run() {
        while (pc < len) {
            if (ptr >= tape.size()) {
                tape.resize(ptr + 1024, 0);
            }
            char ch = prog[pc];
            if (ch == '@') {
                Siz tar = 0;
                while (pc < len && prog[pc] == '@') {
                    tar++;
                    pc++;
                }
                ptr = tar - 1;
                continue;
            }
            switch (ch) {
                case '>': ptr++; pc++; break;
                case '<': if (ptr > 0) ptr--; pc++; break;
                case '+': tape[ptr]++; pc++; break;
                case '-': tape[ptr]--; pc++; break;
                case '.': std::cout << tape[ptr]; pc++; break;
                case ',': std::cin >> tape[ptr]; pc++; break;
                case '[':
                    if (tape[ptr] == 0 && jmp.count(pc)) pc = jmp[pc];
                    else pc++;
                    break;
                case ']':
                    if (tape[ptr] != 0 && jmp.count(pc)) pc = jmp[pc];
                    else pc++;
                    break;
                default:
                    if (ch == '=' || ch == '!' || ch == '$') {
                        Siz st = pc;
                        int lval = evalObj();
                        skipSp();
                        if (pc < len && (prog[pc] == '=' || prog[pc] == '!')) {
                            Str op = "";
                            op += prog[pc++];
                            if (pc < len && prog[pc] == '=') op += prog[pc++];
                            int rval = evalObj();
                            skipSp();
                            bool met = false;
                            if (op == "==") met = (lval == rval);
                            else if (op == "!=") met = (lval != rval);
                            if (pc < len && prog[pc] == ':') {
                                pc++;
                                if (met) {
                                    execBr();
                                    skipDelim('\n'); // FIXED: Skip rest of line after IF branch
                                } else {
                                    skipDelim(':');
                                    if (pc < len && prog[pc] == ':') {
                                        pc++;
                                        execBr();
                                        skipDelim('\n');
                                    }
                                }
                                continue;
                            }
                        }
                        pc = st + 1;
                    } else {
                        pc++;
                    }
                    break;
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::ifstream f(argv[1]);
    if (!f.is_open()) return 1;
    std::stringstream b;
    b << f.rdbuf();
    JJLI jjl(b.str());
    jjl.run();
    return 0;
}
