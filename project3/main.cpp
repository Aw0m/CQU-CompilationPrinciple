#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include "Symbol.h"
//#include <bits/stdc++.h>
using namespace std;
static ofstream errOut("error.txt");
map<string, Variable*> globalSymbolTable;
vector<Variable*> localSymbolTable;
map<string, Function*> funcTable;
class Parser {
public:
    vector<string> codeRecorder;
    vector<pair<string, string>> lexRes;
    int currPos;
    int currRow;
    int currCol;
    map<string, string> keyword = {
            {"const", "CONSTTK"}, {"int", "INTTK"}, {"char", "CHARTK"}, {"void", "VOIDTK"}, {"main", "MAINTK"}, {"if", "IFTK"}, {"else", "ELSETK"}, {"switch", "SWITCHTK"},
            {"case", "CASETK"}, {"default", "DEFAULTTK"}, {"while", "WHILETK"}, {"for", "FORTK"}, {"scanf", "SCANFTK"}, {"printf", "PRINTFTK"}, {"return", "RETURNTK"}};


    fstream in;
    string currFunc;
    map<string, string> funcResType;
    map<int, int> rowRecord;
//    map<string, Symbol> globalSymbolTable;
    // 0:全局  1:局部
    int scope = 0;
    string inputPath;
    string outputPath;

    static bool isInt(char c) {
        return c <= '9' && c >= '0';
    }

    static bool isLetter(char c) {
        return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c == '_');
    }

    static string lower(const string& str) {
        string temp = str;
        for (int i = 0; i < str.size(); i++) {
            if (temp[i] <= 'Z' && temp[i] >= 'A') {
                temp[i] -= ('A' - 'a');
            }
        }
        return temp;
    }

    Parser() {

//        inputPath = "D:\\Programming\\Cpp\\ClionProjects\\CompilerPrinciple\\Project2\\cmake-build-debug\\testfile.txt";
//        outputPath = "D:\\Programming\\Cpp\\ClionProjects\\CompilerPrinciple\\Project2\\cmake-build-debug\\output.txt";
        inputPath = "testfile.txt";
        outputPath = "output.txt";
        in.open(inputPath, ios::in);
        currPos = 0;
        currRow = 0;
        currCol = 0;
        while (in.peek() != EOF) {
            string input;
            getline(in, input);
            codeRecorder.push_back(input);
        }
        in.close();
    }



    void lexicalAnalysis() {
        for (currRow = 0; currRow < codeRecorder.size(); currRow++) {
            string str;
            for (currCol = 0; currCol < codeRecorder[currRow].size(); currCol++) {
                char c = codeRecorder[currRow][currCol];

                if (isInt(c)) {
                    string temp;
                    for (c = codeRecorder[currRow][currCol]; currCol < codeRecorder[currRow].size() && isInt(c); c = codeRecorder[currRow][++currCol]) {
                        temp.push_back(c);
                    }
                    lexRes.emplace_back("INTCON", temp);
                    rowRecord[lexRes.size()] = currRow;
                    currCol--;
                }

                else if (isLetter(c)) {
                    string temp;
                    for (c = codeRecorder[currRow][currCol]; currCol < codeRecorder[currRow].size() && (isLetter(c) || isInt(c)); c = codeRecorder[currRow][++currCol]) {
                        temp.push_back(c);
                    }
                    string lowerString = lower(temp);
                    if (keyword.find(lowerString) != keyword.end()) {
                        lexRes.emplace_back(keyword[lowerString], temp);
                        rowRecord[lexRes.size()] = currRow;
                    }
                    else {
                        lexRes.emplace_back("IDENFR", temp);
                        rowRecord[lexRes.size()] = currRow;
                    }
                    currCol--;
                }

                else {
                    switch (c) {
                        case '+':
                            lexRes.emplace_back("PLUS", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;

                        case '-':
                            lexRes.emplace_back("MINU", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;

                        case '*':
                            lexRes.emplace_back("MULT", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;

                        case '/':
                            lexRes.emplace_back("DIV", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;

                        case '<':
                            if (codeRecorder[currRow][currCol + 1] == '=') {
                                lexRes.emplace_back("LEQ", "<=");
                                rowRecord[lexRes.size()] = currRow;
                                currCol++;
                            }
                            else {
                                lexRes.emplace_back("LSS", string(1, c));
                                rowRecord[lexRes.size()] = currRow;
                            }
                            break;

                        case '>':
                            if (codeRecorder[currRow][currCol + 1] == '=') {
                                lexRes.emplace_back("GEQ", ">=");
                                rowRecord[lexRes.size()] = currRow;
                                currCol++;
                            }
                            else {
                                lexRes.emplace_back("GRE", string(1, c));
                                rowRecord[lexRes.size()] = currRow;
                            }
                            break;

                        case '=':
                            if (codeRecorder[currRow][currCol + 1] == '=') {
                                lexRes.emplace_back("EQL","==");
                                rowRecord[lexRes.size()] = currRow;
                                currCol++;
                            }
                            else {
                                lexRes.emplace_back("ASSIGN", string(1, c));
                                rowRecord[lexRes.size()] = currRow;
                            }
                            break;

                        case '!':
                            if (codeRecorder[currRow][currCol + 1] == '=') {
                                lexRes.emplace_back("NEQ", "!=");
                                rowRecord[lexRes.size()] = currRow;
                                currCol++;
                            }
                            else {
                                // out << " " << "!" << endl;
                                // 还没有非
                            }
                            break;
                        case ':':
                            lexRes.emplace_back("COLON", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;
                        case ';':
                            lexRes.emplace_back("SEMICN", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;
                        case ',':
                            lexRes.emplace_back("COMMA", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;
                        case '(':
                            lexRes.emplace_back("LPARENT", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;
                        case ')':
                            lexRes.emplace_back("RPARENT", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;
                        case '[':
                            lexRes.emplace_back("LBRACK", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;
                        case ']':
                            lexRes.emplace_back("RBRACK", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;
                        case '{':
                            lexRes.emplace_back("LBRACE", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;
                        case '}':
                            lexRes.emplace_back("RBRACE", string(1, c));
                            rowRecord[lexRes.size()] = currRow;
                            break;

                        case '\'':
                            lexRes.emplace_back("CHARCON", string(1, codeRecorder[currRow][currCol + 1]));
                            rowRecord[lexRes.size()] = currRow;
                            currCol += 2;
                            break;
                        case '\"':
                            string strConTemp;
                            int i;
                            for (i = currCol + 1; i < codeRecorder[currRow].size() && codeRecorder[currRow][i] != '\"'; i++) {
                                strConTemp.push_back(codeRecorder[currRow][i]);
                            }
                            if (codeRecorder[currRow][i] == '\"') {
                                lexRes.emplace_back("STRCON", strConTemp);
                                rowRecord[lexRes.size()] = currRow;
                                currCol = i;
                            }
                            else {
                                cout << "出现错误！" << endl;
                                return;
                            }
                            break;
                    }
                }
            }
        }
    };


    // 类型标识符
    static bool varIDENFR(const string& str) {
        if (str == "INTTK" || str == "CHARTK") {
            return true;
        }
        return false;
    }

    void outputLexRes(ofstream& out) {
        out << lexRes[currPos].first << " " << lexRes[currPos].second << endl;
        currPos++;
    }

    void outputError(char typeNum) {
        if (typeNum != 'k')
            errOut << rowRecord[currPos + 1] + 1<< " " << typeNum << endl;
        else
            errOut << rowRecord[currPos + 1]<< " " << typeNum << endl;
    }

    // ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
    void constDescribe(ofstream& out) {
        outputLexRes(out);
        constDefine(out);
        // ;
        semicolon(out);
        while (lexRes[currPos].first == "CONSTTK") {
            outputLexRes(out);
            constDefine(out);
            // ;
            semicolon(out);
        }
        out << "<常量说明>" << endl;
    }

    // ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}    | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
    void constDefine(ofstream& out) {
        string varTempType = lexRes[currPos].first;
        outputLexRes(out);

        // 局部变量
        //TODO 修改的常量定义的 名字重定义 判断
        // 全局
        string varName = lower(lexRes[currPos].second);
        identifierVarError(varName, true, varTempType);
        // 标识符
        outputLexRes(out);
        // =
        outputLexRes(out);
        if (varTempType == "INTTK") {
            if (lexRes[currPos].first == "CHARCON") {
                outputError('o');
            }
            // 整数
            intNum(out);
        } else {
            if (lexRes[currPos].first != "CHARCON") {
                outputError('o');
            }
            // 字符
            character(out);
        }
        while (lexRes[currPos].first == "COMMA") {
            outputLexRes(out);
            varName = lower(lexRes[currPos].second);
            identifierVarError(varName, true, varTempType);
            outputLexRes(out);
            outputLexRes(out);
            if (varTempType == "INTTK" ) {
                if (lexRes[currPos].first == "CHARCON") {
                    outputError('o');
                }
                // 整数
                intNum(out);
            } else {
                if (lexRes[currPos].first != "CHARCON") {
                    outputError('o');
                }
                // 字符
                character(out);
            }
        }
        out << "<常量定义>" << endl;
    }

    // 变量说明
    void varDecribe(ofstream& out) {
        while (varIDENFR(lexRes[currPos].first) && (lexRes[currPos + 2].first) != "LPARENT") {
            string temp;
            string typeTemp = lexRes[currPos].first;


            // 变量定义 -> <变量定义无初始化> | <变量定义及初始化>
            do {
                // 类型 or ，
                outputLexRes(out);
                //TODO 变量说明的 标识符 重定义 判断
                // 修改的常量定义的 名字重定义 判断
                string varName = lower(lexRes[currPos].second);
                identifierVarError(varName, false, typeTemp);
                // 标识符
                outputLexRes(out);

                vector<int> numRecord;
                while (lexRes[currPos].first == "LBRACK") {
                    // [
                    outputLexRes(out);
                    numRecord.push_back(atoi(lexRes[currPos].second.c_str()));
                    // 无符号整数 ]
                    unsignedInteger(out);
                    rBracket(out);
                }

                if (lexRes[currPos].first != "ASSIGN") {
                    temp = "<变量定义无初始化>";
//                    out << "<变量定义无初始化>" << endl;
                }
                    // 2. <变量定义及初始化>
                else {
                    // 2. <变量定义及初始化>
                    // ＜类型标识符＞＜标识符＞=＜常量＞|＜类型标识符＞＜标识符＞'['＜无符号整数＞']'='{'＜常量＞{,＜常量＞}'}' |
                    // ＜类型标识符＞＜标识符＞'['＜无符号整数＞']''['＜无符号整数＞']'='{''{'＜常量＞{,＜常量＞}'}'{, '{'＜常量＞{,＜常量＞}'}'}'}'
                    // 测试程序需出现简单变量初始化、一维数组初始化、二维数组初始化3种情况

                    // 输出等号
                    outputLexRes(out);
                    if (numRecord.empty()) {
                        if (typeTemp == "CHARTK" ^ lexRes[currPos].first == "CHARCON") {
                            outputError('o');
                        }
                        constNum(out);
                    }
                    else {
                        int totalElem = 1;
                        for (auto x : numRecord) {
                            totalElem *= x;
                        }
                        while (totalElem) {
                            if (lexRes[currPos].first == "SEMICN") {
                                outputLexRes(out);
                                outputError('n');
                                goto label;
                            }
                            //TODO  INTTK / CHARTK --> const
                            if (lexRes[currPos].first == "INTCON" || lexRes[currPos].first == "CHARCON") {
                                constNum(out);
                                totalElem--;
                            }
                            else {
                                outputLexRes(out);
                            }
                        }
                        // 输出右括号 }
                        for (int i = 0; i < numRecord.size(); i++) {
                            outputLexRes(out);
                        }

                        if (lexRes[currPos].first != "SEMICN") {
                            while (lexRes[currPos].first != "SEMICN") {
                                currPos++;
                            }
                            outputLexRes(out);
                            outputError('n');
                            goto label;
                        }

                    }
//                    out << "<变量定义及初始化>" << endl;
                    temp = "<变量定义及初始化>";
                }
            } while(currPos < lexRes.size() && lexRes[currPos].first == "COMMA");
            out << temp << endl;
            out << "<变量定义>" << endl;
            // ;
            semicolon(out);
        }
        label:
        out << "<变量说明>" << endl;
//        currPos--;
    }

    void identifierVarError(const string& varName, bool isConst, const string& type) {
        if (this->scope == 0) {
            if (globalSymbolTable.find(varName) != globalSymbolTable.end()) {
                outputError('b');
            } else {
                auto* variable = new Variable(varName, type, "", isConst);
                globalSymbolTable[varName] = variable;
            }
        }
        // 局部
        else {
            bool flag = false;
            for (auto x : localSymbolTable) {
                if (x->funcName == this->currFunc && x->varName == varName) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                outputError('b');
            } else {
                auto* variable = new Variable(varName, type, this->currFunc, isConst);
                localSymbolTable.push_back(variable);
            }
        }
    }

    Variable* varFindError(const string& varName) {
        Variable* resVar = nullptr;
        if (this->scope == 1) {
            for (auto x : localSymbolTable) {
                if (x->funcName == this->currFunc && x->varName == varName) {
                    resVar = x;
                    break;
                }
            }
            // 找到了
            if (resVar != nullptr) {
                return resVar;
            }
        }
        // 全局里面找 还是没找到
        if (globalSymbolTable.find(varName) == globalSymbolTable.end()) {
            outputError('c');
            return nullptr;
        }
        return globalSymbolTable[varName];
    }

    // <语句列>
    void statementCol(ofstream& out) {
        while (lexRes[currPos].first != "RBRACE") {
            statement(out);
        }
        out << "<语句列>" << endl;
    }

    // <语句>
    void statement(ofstream& out) {
        // <空>;
        if (lexRes[currPos].first == "SEMICN") {
            // ;
            outputLexRes(out);
        }

            // '{'<语句列>'}'
        else if (lexRes[currPos].first == "LBRACE") {
            // '{'
            outputLexRes(out);
            // <语句列>
            statementCol(out);
            // '}'
            outputLexRes(out);
        }

            // <循环语句>  while '('  <条件>  ')' <语句>
        else if (lexRes[currPos].first == "WHILETK") {
            // while (
            outputLexRes(out);
            outputLexRes(out);
            // <条件>
            condition(out);
            // )
            // 输出右括号
            if (lexRes[currPos].first == "RPARENT") {
                outputLexRes(out);
            } else {
                outputError('l');
            }
            // 语句
            statement(out);
            out << "<循环语句>" << endl;
        }

            //  for '('  <标识符> ＝ <表达式> ; <条件> ; <标识符> ＝ <标识符> ( + | -) <步长> ')' <语句>
        else if (lexRes[currPos].first == "FORTK") {
            // for ( <标识符> ＝
            for (int i = 0; i < 4; i++) {
                if (lexRes[currPos].first == "IDENFR") {
                    string varName = lexRes[currPos].second;
                    varFindError(varName);
                }
                outputLexRes(out);
            }
            // <表达式>
            expression(out);
            // ;
            semicolon(out);
            // <条件>
            condition(out);
            // ;
            semicolon(out);
            // <标识符> ＝ <标识符> ( + | -)
            for (int i = 0; i < 4; i++) {
                if (lexRes[currPos].first == "IDENFR") {
                    string varName = lexRes[currPos].second;
                    varFindError(varName);
                }
                outputLexRes(out);
            }
            // 步长
            step(out);
            // )
            // 输出右括号
            if (lexRes[currPos].first == "RPARENT") {
                outputLexRes(out);
            } else {
                outputError('l');
            }
            // 语句
            statement(out);
            out << "<循环语句>" << endl;
        }

            // <条件语句>  ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
        else if (lexRes[currPos].first == "IFTK") {
            outputLexRes(out);
            outputLexRes(out);
            condition(out);
            // 输出右括号
            if (lexRes[currPos].first == "RPARENT") {
                outputLexRes(out);
            } else {
                outputError('l');
            }
            // 语句
            statement(out);
            if (lexRes[currPos].first == "ELSETK") {
                outputLexRes(out);
                // 语句
                statement(out);
            }
            out << "<条件语句>" << endl;
        }

            // <有/无 返回值函数调用语句>
        else if (funcResType.find(lexRes[currPos].second) != funcResType.end()) {
            string temp = (funcResType[lexRes[currPos].second] == "<无返回值函数定义>") ? "<无返回值函数调用语句>" : "<有返回值函数调用语句>";
            // <标识符>
            string funcName = lower(lexRes[currPos].second);
            funcFindError(funcName);
            outputLexRes(out);
            // '(' ＜值参数表＞  ')'
            outputLexRes(out);
            valueParameterTable(out, funcName);
            // 输出右括号
            if (lexRes[currPos].first == "RPARENT") {
                outputLexRes(out);
            } else {
                outputError('l');
            }

            out << temp << endl;
            // ;
            semicolon(out);
        }

            // <读语句>  ::=  scanf '('＜标识符＞')'
        else if (lexRes[currPos].first == "SCANFTK") {
            for (int i = 0; i < 3; i++) {
                if (lexRes[currPos].first == "IDENFR") {
                    string varName = lower(lexRes[currPos].second);
                    auto var = varFindError(varName);
                    if (var->isConst) {
                        outputError('j');
                    }
                }
                outputLexRes(out);
            }
            // 输出右括号
            if (lexRes[currPos].first == "RPARENT") {
                outputLexRes(out);
            } else {
                outputError('l');
            }
            out << "<读语句>" << endl;
            // ;
            semicolon(out);
        }

            // <写语句>  ::= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')'
        else if (lexRes[currPos].first == "PRINTFTK") {
            // printf '('
            outputLexRes(out);
            outputLexRes(out);

            if (lexRes[currPos].first == "STRCON") {
                // 字符串
                outputLexRes(out);
                out << "<字符串>" << endl;
                if (lexRes[currPos].first == "COMMA") {
                    // , ＜表达式＞
                    outputLexRes(out);
                    expression(out);
                }
            }
            else {
                expression(out);
            }
            // )
            // 输出右括号
            if (lexRes[currPos].first == "RPARENT") {
                outputLexRes(out);
            } else {
                outputError('l');
            }

            out << "<写语句>" << endl;
            // ;
            semicolon(out);
        }

            // <情况语句>   ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’
        else if (lexRes[currPos].second == "switch") {
            // switch
            outputLexRes(out);
            // '(' <表达式> ')'
            outputLexRes(out);
            string resType = expression(out);
            outputLexRes(out);

            // ‘{’＜情况表＞＜缺省＞‘}’
            outputLexRes(out);
            situationTable(out, resType);
            if (lexRes[currPos].first == "DEFAULTTK") {
                defaultRes(out);
            } else {
                outputError('p');
            }
            outputLexRes(out);

            out << "<情况语句>" << endl;
        }

            // <返回语句>   ::=  return['('<表达式>')']
        else if (lexRes[currPos].first == "RETURNTK") {
            auto func = funcTable[this->currFunc];
            string resType;
            // return
            outputLexRes(out);
            if (lexRes[currPos].first == "LPARENT") {
                // '(' <表达式> ')'
                func->hasReturn = true;
                if (func->resType == "VOIDTK") {
                    outputError('g');
                }
                outputLexRes(out);
                if (lexRes[currPos].first != "RPARENT") {
                    resType = expression(out);
                    func->hasReturn = true;
                }
                // ')'
                // 输出右括号
                if (lexRes[currPos].first == "RPARENT") {
                    outputLexRes(out);
                } else {
                    outputError('l');
                }
            }
            out << "<返回语句>" << endl;
//            auto func = funcTable[this->currFunc];
            if (resType != func->resType && func->resType != "VOIDTK") {
                outputError('h');
            }
            // ;
            semicolon(out);
        }

            // <赋值语句>
            // <标识符>＝<表达式> |   <标识符>'['<表达式>']'=<表达式>    |<标识符>'['<表达式>']''['<表达式>']' =<表达式>
        else if (lexRes[currPos].first == "IDENFR") {
            // 标识符
            string varName = lower(lexRes[currPos].second);
            auto var = varFindError(varName);
            if (var->isConst) {
                outputError('j');
            }
            outputLexRes(out);

            if (lexRes[currPos].first == "ASSIGN") {
                // ＝  表达式
                outputLexRes(out);
                expression(out);
            } else {
                // [ 表达式 ]
                outputLexRes(out);
                string resType1 = expression(out);
                if (resType1 == "CHARTK") {
                    outputError('i');
                }
                rBracket(out);
                if (lexRes[currPos].first == "ASSIGN") {
                    // ＝ 表达式
                    outputLexRes(out);
                    expression(out);
                }
                else if (lexRes[currPos].first == "LBRACK") {
                    // [ 表达式 ]
                    outputLexRes(out);
                    string resType2 = expression(out);
                    if (resType2 == "CHARTK") {
                        outputError('i');
                    }
                    rBracket(out);

                    // = 表达式
                    outputLexRes(out);
                    expression(out);
                }
            }
            out << "<赋值语句>" << endl;
            // ;
            semicolon(out);
        }

        else {
            //TODO ;
            cout << "Error!" << endl;
            throw system_error();
        }

        out << "<语句>" << endl;
    }

    //<表达式>::= ［＋｜－］<项>{<加法运算符><项>}
    string expression(ofstream& out) {
        bool intType = false;
        // 如果有 [- +] 先输出
        if (lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU") {
            intType = true;
            outputLexRes(out);
        }
        string resType;
        resType = term(out);
        while (lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU") {
            intType = true;
            outputLexRes(out);
            term(out);
        }
        out << "<表达式>" << endl;
        if (intType) {
            return "INTTK";
        }
        return resType;
    }

    //<项> ::= <因子>{<乘法运算符><因子>}
    string term(ofstream& out) {
        string resType;
        resType = factor(out);
        bool intType = false;
        while (lexRes[currPos].first == "MULT" || lexRes[currPos].first == "DIV") {
            intType = true;
            outputLexRes(out);
            factor(out);
        }
        out << "<项>" << endl;
        if (intType) {
            return "INTTK";
        }
        return resType;
    }

    //todo <因子> ::= <标识符> ｜ <标识符>'['<表达式>']'  |  <标识符>'['<表达式>']''['<表达式>']' | '('<表达式>')' ｜ <整数> |  <字符> ｜   <有返回值函数调用语句>
    string factor(ofstream& out) {
        string resType;
        // <有返回值函数调用语句>
        if (funcResType.find(lexRes[currPos].second) != funcResType.end()) {
            string funcName = lower(lexRes[currPos].second);
            // 先获得返回的类型
            auto func = funcTable[funcName];
            resType = func->resType;

            // <标识符> '(' <值参数表> ')'
            outputLexRes(out);
            outputLexRes(out);
            if (lexRes[currPos].first != "RPARENT") {
                valueParameterTable(out, funcName);
            } else {
                out << "<值参数表>" << endl;
            }
            // 输出右括号
            if (lexRes[currPos].first == "RPARENT") {
                outputLexRes(out);
            } else {
                outputError('l');
            }
            out << "<有返回值函数调用语句>" << endl;
        }

            // <字符> ::=  '<加法运算符>'｜'<乘法运算符>'｜'<字母>'｜'<数字>'，不知道会不会出现问题
        else if (lexRes[currPos].first == "CHARCON") {
            resType = "CHARTK";
            character(out);
        }

            // <整数>
        else if (lexRes[currPos].first == "INTCON") {
            resType = "INTTK";
            outputLexRes(out);
            out << "<无符号整数>" << endl;
            out << "<整数>" << endl;
        }
        else if ((lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU") && lexRes[currPos + 1].first == "INTCON") {
            resType = "INTTK";
            outputLexRes(out);
            outputLexRes(out);
            out << "<无符号整数>" << endl;
            out << "<整数>" << endl;
        }

            // '('  <表达式>  ')'
        else if (lexRes[currPos].first == "LPARENT") {
            resType = "INTTK";
            outputLexRes(out);
            expression(out);
            outputLexRes(out);
        }
            // 标识符
        else {
            // <标识符>
            string varName = lower(lexRes[currPos].second);
            auto var = varFindError(varName);
            resType = var != nullptr ? var->varType : "VOIDTK";
            outputLexRes(out);
            if (lexRes[currPos].first == "LBRACK") {
                //  '['  <表达式>   ']'
                outputLexRes(out);
                string resType1 = expression(out);
                if (resType1 == "CHARTK") {
                    outputError('i');
                }
                rBracket(out);
                if (lexRes[currPos].first == "LBRACK") {
                    //  '['  <表达式>   ']'
                    outputLexRes(out);
                    string resType2 = expression(out);
                    if (resType2 == "CHARTK") {
                        outputError('i');
                    }
                    rBracket(out);
                }
            }
        }
        out << "<因子>" << endl;
        return resType;
    }
    // <值参数表> ::= <表达式>{,<表达式>}｜<空>
    void valueParameterTable(ofstream& out, const string& funcName) {
        int varNum = 0;
        auto func = funcTable[funcName];
        string paramTemp;
        int paramIdx = 0;
        if (lexRes[currPos].first == "RPARENT") {
            out << "<值参数表>" << endl;
            if (!func->paramType.empty()) {
                outputError('d');
            }
            return;
        } else if (lexRes[currPos].first == "SEMICN") {
            return;
        }
        paramTemp = expression(out);
        if (paramIdx < func->paramType.size()) {
            if (paramTemp != func->paramType[paramIdx++].first) {
                outputError('e');
            }
        }
        varNum++;
        while (lexRes[currPos].first == "COMMA") {
            outputLexRes(out);
            paramTemp = expression(out);
            if (paramIdx < func->paramType.size()) {
                if (paramTemp != func->paramType[paramIdx++].first) {
                    outputError('e');
                }
            }
            varNum++;
        }
        out << "<值参数表>" << endl;
        if (func->paramType.size() != varNum) {
            outputError('d');
        }
    }

    // <条件>  ::=  ＜表达式＞＜关系运算符＞＜表达式＞
    void condition(ofstream& out) {
        string resType1 = expression(out);
        outputLexRes(out);
        string resType2 = expression(out);
        if (resType1 != "INTTK" || resType2 != "INTTK") {
            outputError('f');
        }
        out << "<条件>" << endl;
    }

    void identifierFuncError(const string& funcName, const string& resType) {
        if (funcTable.find(funcName) != funcTable.end()) {
            outputError('b');
        } else {
            auto* function = new Function(funcName, resType);
            funcTable[funcName] = function;
        }
    }

    void funcFindError(const string& funcName) {
        if (funcTable.find(funcName) == funcTable.end()) {
            outputError('c');
        }
    }


    void func(ofstream& out) {
        string funcType;
        string funcTypeStr = lexRes[currPos].first;

        if (lexRes[currPos + 1].first == "MAINTK") {
            funcType = "<主函数>";
        }
        else if (lexRes[currPos].first == "VOIDTK") {
            funcType = "<无返回值函数定义>";
        }
        else {
            funcType = "<有返回值函数定义>";
        }
        // void | int | char
        outputLexRes(out);
        //TODO 判断是否有重复的funcName
        string funcName = lower(lexRes[currPos].second);
        this->currFunc = funcName;
        identifierFuncError(funcName, funcTypeStr);

        funcResType[lexRes[currPos].second] = funcType;

        // 标识符 / Main
        outputLexRes(out);

        // 没有参数情况
        if (lexRes[currPos + 1].first == "RPARENT") {
            if (funcType == "<有返回值函数定义>") {
                out << "<声明头部>" << endl;
            }
            // 输出 (
            outputLexRes(out);
            if (funcType != "<主函数>") {
                out << "<参数表>" << endl;
            }
        }
        // 有参数情况
        else {
            // 先输出 "<声明头部>" 再输出左括号
            if (funcType == "<有返回值函数定义>") {
                out << "<声明头部>" << endl;
            }
            // (
            outputLexRes(out);
            // <参数表> ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
            auto func = funcTable[funcName];
            string varTempType = lexRes[currPos].first;
            outputLexRes(out);
            string varTempName = lexRes[currPos].second;
            outputLexRes(out);
            identifierVarError(varTempName, false, varTempType);
            func->paramType.emplace_back(varTempType, varTempName);
            while (lexRes[currPos].first == "COMMA") {
                // ,
                outputLexRes(out);
                // ＜类型标识符＞＜标识符＞
                varTempType = lexRes[currPos].first;
                outputLexRes(out);
                varTempName = lexRes[currPos].second;
                outputLexRes(out);
                identifierVarError(varTempName, false, varTempType);
                func->paramType.emplace_back(varTempType, varTempName);
            }
            if (funcType != "<主函数>") {
                out << "<参数表>" << endl;
            }
        }
        // 输出右括号
        if (lexRes[currPos].first == "RPARENT") {
            outputLexRes(out);
        } else {
            outputError('l');
        }
        //输出 左大括号
        outputLexRes(out);

        // 常量说明
        if (lexRes[currPos].first == "CONSTTK") {
            // <常量说明> ::=  const<常量定义>;{ const<常量定义>;}
            constDescribe(out);
        }
        // 变量说明
        if (varIDENFR(lexRes[currPos].first)  && (lexRes[currPos + 2].first) != "LPARENT") {
            varDecribe(out);
        }

        statementCol(out);
        out << "<复合语句>" << endl;
        // 输出右大括号
        outputLexRes(out);
        // 输出函数类型
        out << funcType << endl;
        currPos--;
        // 应该有返回但是没有返回
        auto func = funcTable[funcName];
        if (funcType == "<有返回值函数定义>" && !(func->hasReturn)) {
            outputError('h');
        }
    }

    void step(ofstream& out) {
        unsignedInteger(out);
        out << "<步长>" << endl;
    }

    // ＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
    void situationTable(ofstream& out, const string& type) {
        situation(out, type);
        while (lexRes[currPos].first == "CASETK") {
            situation(out, type);
        }
        out << "<情况表>" << endl;
    }

    // ＜情况子语句＞  ::=  case＜常量＞：＜语句＞
    void situation(ofstream& out, const string& type) {
        outputLexRes(out);
        if (type == "CHARTK" ^ lexRes[currPos].first == "CHARCON") {
            outputError('o');
        }
        constNum(out);
        outputLexRes(out);
        statement(out);
        out << "<情况子语句>" << endl;
    }

    // 常量 ::=  ＜整数＞|＜字符＞
    void constNum(ofstream& out) {
        if (lexRes[currPos].first == "INTCON" ||
            ((lexRes[currPos + 1].first == "INTCON") && (lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU"))) {
            intNum(out);
        }
        else if (lexRes[currPos].first == "CHARCON") {
            character(out);
        }
        out << "<常量>" << endl;
    }

    void intNum(ofstream& out) {
        if (lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU") {
            // + | -
            outputLexRes(out);
        }
        unsignedInteger(out);
        out << "<整数>" << endl;
    }

    void unsignedInteger(ofstream& out) {
        outputLexRes(out);
        while (lexRes[currPos].first == "INTTK") {
            outputLexRes(out);
        }
        out << "<无符号整数>" << endl;
    }

    // 缺省  ::=  default :＜语句＞
    void defaultRes(ofstream& out) {
        outputLexRes(out);
        outputLexRes(out);
        statement(out);
        out << "<缺省>" << endl;
    }

    // 输出字符＋错误检测
    void character(ofstream& out) {
        if (lexRes[currPos].second.empty()) {
            outputError('a');
            currPos++;
            return;
        }
        if (lexRes[currPos].second.size() == 1) {
            char c = lexRes[currPos].second[0];
            switch (c) {
                case '+':
                case '-':
                case '*':
                case '/':
                    outputLexRes(out);
                    return;
                default:
                    if ((('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) || ('0' <= c && c <= '9')) {
                        outputLexRes(out);
                        return;
                    }
            }
        }
        // 错误判断
        outputError('a');
        currPos++;
    }

    void semicolon(ofstream& out) {
        if (lexRes[currPos].second == ";") {
            outputLexRes(out);
            return;
        }
        outputError('k');
    }
    void rBracket(ofstream& out) {
        // 输出右括号
        if (lexRes[currPos].first == "RBRACK") {
            outputLexRes(out);
        } else {
            outputError('m');
        }
    }

    void execute() {
        this->lexicalAnalysis();
        ofstream out(outputPath);
        for (currPos = 0; currPos < lexRes.size(); currPos++) {
            // 常量说明
            if (lexRes[currPos].first == "CONSTTK") {
                // 全局变量
                scope = 0;
                // <常量说明> ::=  const<常量定义>;{ const<常量定义>;}
                constDescribe(out);
                currPos--;
            }

                // 标识符（函数调用）
            else if (currPos + 5 < lexRes.size() && (lexRes[currPos].first == "CHARTK"
                                                     || lexRes[currPos].first == "INTTK"
                                                     || lexRes[currPos].first == "VOIDTK") &&
                     (lexRes[currPos+1].first == "IDENFR" || lexRes[currPos+1].first == "MAINTK") &&
                     lexRes[currPos+2].first == "LPARENT") {
                // 局部变量
                scope = 1;
                func(out);
            }
                // 变量说明
            else if (varIDENFR(lexRes[currPos].first)  && (lexRes[currPos + 2].first) != "LPARENT") {
                // 全局变量
                scope = 0;
                varDecribe(out);
                currPos--;
            }

        }
        out << "<程序>" << endl;
    }
};


int main() {
    Parser* analyzer = new Parser();
    analyzer->execute();
    // system("pause");
    return 0;
}