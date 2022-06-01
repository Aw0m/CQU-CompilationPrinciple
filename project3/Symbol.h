//
// Created by lenovo on 2022/5/18.
//

#ifndef PROJECT3_SYMBOL_H
#define PROJECT3_SYMBOL_H

#include <string>
#include <vector>

using namespace std;

class Variable {
public:
    string varName;
    string varType;
    string funcName;
    bool isConst{};
    Variable();
    Variable(const string& varName, const string& varType, const string& funcName, bool isConst);
};

class Function {
public:
    string funcName;
    string resType;
    vector<pair<string, string>> paramType;
    bool hasReturn = false;
    Function();
    Function(const string& funcName, const string& resType);
};

#endif //PROJECT3_SYMBOL_H
