//
// Created by lenovo on 2022/5/18.
//

#include "Symbol.h"



Variable::Variable() = default;

Variable::Variable(const string &varName, const string &varType, const string &funcName, bool isConst) {
    this->varName = varName;
    this->varType = varType;
    this->funcName = funcName;
    this->isConst = isConst;

}

Function::Function() = default;

Function::Function(const string &funcName, const string &resType) {
    this->funcName = funcName;
    this->resType = resType;
}
