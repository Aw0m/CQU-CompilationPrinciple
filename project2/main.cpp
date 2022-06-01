#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>
//#include <bits/stdc++.h>
using namespace std;

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
    map<string, string> funcResType;
    string inputPath;
    string outputPath;
    bool isInt(char c) {
        return c <= '9' && c >= '0';
    }

    bool isLetter(char c) {
        return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c == '_');
    }

    string lower(string str) {
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
                    }
                    else {
                        lexRes.emplace_back("IDENFR", temp);
                    }
                    currCol--;
                }

                else {
                    switch (c) {
                        case '+':
                            lexRes.emplace_back("PLUS", string(1, c));
                            break;

                        case '-':
                            lexRes.emplace_back("MINU", string(1, c));
                            break;

                        case '*':
                            lexRes.emplace_back("MULT", string(1, c));
                            break;

                        case '/':
                            lexRes.emplace_back("DIV", string(1, c));
                            break;

                        case '<':
                            if (codeRecorder[currRow][currCol + 1] == '=') {
                                lexRes.emplace_back("LEQ", "<=");
                                currCol++;
                            }
                            else {
                                lexRes.emplace_back("LSS", string(1, c));
                            }
                            break;

                        case '>':
                            if (codeRecorder[currRow][currCol + 1] == '=') {
                                lexRes.emplace_back("GEQ", ">=");
                                currCol++;
                            }
                            else {
                                lexRes.emplace_back("GRE", string(1, c));
                            }
                            break;

                        case '=':
                            if (codeRecorder[currRow][currCol + 1] == '=') {
                                lexRes.emplace_back("EQL","==");
                                currCol++;
                            }
                            else {
                                lexRes.emplace_back("ASSIGN", string(1, c));
                            }
                            break;

                        case '!':
                            if (codeRecorder[currRow][currCol + 1] == '=') {
                                lexRes.emplace_back("NEQ", "!=");
                                currCol++;
                            }
                            else {
                                // out << " " << "!" << endl;
                                // 还没有非
                            }
                            break;
                        case ':':
                            lexRes.emplace_back("COLON", string(1, c));
                            break;
                        case ';':
                            lexRes.emplace_back("SEMICN", string(1, c));
                            break;
                        case ',':
                            lexRes.emplace_back("COMMA", string(1, c));
                            break;
                        case '(':
                            lexRes.emplace_back("LPARENT", string(1, c));
                            break;
                        case ')':
                            lexRes.emplace_back("RPARENT", string(1, c));
                            break;
                        case '[':
                            lexRes.emplace_back("LBRACK", string(1, c));
                            break;
                        case ']':
                            lexRes.emplace_back("RBRACK", string(1, c));
                            break;
                        case '{':
                            lexRes.emplace_back("LBRACE", string(1, c));
                            break;
                        case '}':
                            lexRes.emplace_back("RBRACE", string(1, c));
                            break;

                        case '\'':
                            lexRes.emplace_back("CHARCON", string(1, codeRecorder[currRow][currCol + 1]));
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
    bool varIDENFR(const string& str) {
        if (str == "INTTK" || str == "CHARTK") {
            return true;
        }
        return false;
    }

    void outputLexRes(ofstream& out) {
        out << lexRes[currPos].first << " " << lexRes[currPos].second << endl;
        currPos++;
    }

    // ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
    void constDescribe(ofstream& out) {
        outputLexRes(out);
        constDefine(out);
        outputLexRes(out);
        while (lexRes[currPos].first == "CONSTTK") {
            outputLexRes(out);
            constDefine(out);
            outputLexRes(out);
        }
        out << "<常量说明>" << endl;
    }

    // ＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}    | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
    void constDefine(ofstream& out) {
        string temp = lexRes[currPos].first;
        outputLexRes(out);
        outputLexRes(out);
        outputLexRes(out);
        if (temp == "INTTK") {
            // 整数
            intNum(out);
        } else {
            // 字符
            outputLexRes(out);
        }
        while (lexRes[currPos].first == "COMMA") {
            outputLexRes(out);
            outputLexRes(out);
            outputLexRes(out);
            if (temp == "INTTK" ) {
                // 整数
                intNum(out);
            } else {
                // 字符
                outputLexRes(out);
            }
        }
        out << "<常量定义>" << endl;
    }

    // 变量说明
    void varDecribe(ofstream& out) {
        while (varIDENFR(lexRes[currPos].first) && (lexRes[currPos + 2].first) != "LPARENT") {
            string temp;
            // 变量定义 -> <变量定义无初始化> | <变量定义及初始化>
            do {
                // 类型 or ，
                outputLexRes(out);
                // 标识符
                outputLexRes(out);
                vector<int> numRecord;
                while (lexRes[currPos].first == "LBRACK") {
                    // [
                    outputLexRes(out);
                    numRecord.push_back(atoi(lexRes[currPos].second.c_str()));
                    // 无符号整数 】
                    unsignedInteger(out);
                    outputLexRes(out);
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
                        constNum(out);
                    }
                    else {
                        int totalElem = 1;
                        for (auto x : numRecord) {
                            totalElem *= x;
                        }
                        while (totalElem) {
                            //TODO  INTTK / CHARTK --> const
                            if (lexRes[currPos].first == "INTCON" || lexRes[currPos].first == "CHARCON") {
                                constNum(out);
                                totalElem--;
                            }
                            else {
                                outputLexRes(out);
                            }
                        }
                        // 输出右括号
                        for (int i = 0; i < numRecord.size(); i++) {
                            outputLexRes(out);
                        }
                    }
//                    out << "<变量定义及初始化>" << endl;
                    temp = "<变量定义及初始化>";
                }
            } while(currPos < lexRes.size() && lexRes[currPos].first == "COMMA");
            out << temp << endl;
            out << "<变量定义>" << endl;
            // 这里本来应该判断是否是 ';'  SEMICN
            outputLexRes(out);
        }
        out << "<变量说明>" << endl;
//        currPos--;
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
            outputLexRes(out);
            // 语句
            statement(out);
            out << "<循环语句>" << endl;
        }

        //  for '('  <标识符> ＝ <表达式> ; <条件> ; <标识符> ＝ <标识符> ( + | -) <步长> ')' <语句>
        else if (lexRes[currPos].first == "FORTK") {
            // for ( <标识符> ＝
            for (int i = 0; i < 4; i++) {
                outputLexRes(out);
            }
            // <表达式>
            expression(out);
            // ;
            outputLexRes(out);
            // <条件>
            condition(out);
            // <标识符> ＝ <标识符> ( + | -)
            for (int i = 0; i < 5; i++) {
                outputLexRes(out);
            }
            // 步长
            step(out);
            // )
            outputLexRes(out);
            // 语句
            statement(out);
            out << "<循环语句>" << endl;
        }

        // <条件语句>  ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
        else if (lexRes[currPos].first == "IFTK") {
            outputLexRes(out);
            outputLexRes(out);
            condition(out);
            outputLexRes(out);
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
            // <标识符> '(' ＜值参数表＞  ')'
            outputLexRes(out);
            outputLexRes(out);
            valueParameterTable(out);
            outputLexRes(out);

            out << temp << endl;
            // ;
            outputLexRes(out);
        }

        // <读语句>  ::=  scanf '('＜标识符＞')'
        else if (lexRes[currPos].first == "SCANFTK") {
            for (int i = 0; i < 4; i++) {
                outputLexRes(out);
            }
            out << "<读语句>" << endl;
            // ;
            outputLexRes(out);
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
            outputLexRes(out);

            out << "<写语句>" << endl;
            // ;
            outputLexRes(out);
        }

        // <情况语句>   ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’
        else if (lexRes[currPos].second == "switch") {
            // switch
            outputLexRes(out);
            // '(' <表达式> ')'
            outputLexRes(out);
            expression(out);
            outputLexRes(out);

            // ‘{’＜情况表＞＜缺省＞‘}’
            outputLexRes(out);
            situationTable(out);
            defaultRes(out);
            outputLexRes(out);

            out << "<情况语句>" << endl;
        }

        // <返回语句>   ::=  return['('<表达式>')']
        else if (lexRes[currPos].first == "RETURNTK") {
            // return
            outputLexRes(out);
            if (lexRes[currPos].first == "LPARENT") {
                // '(' <表达式> ')'
                outputLexRes(out);
                expression(out);
                outputLexRes(out);
            }
            out << "<返回语句>" << endl;
            //;
            outputLexRes(out);
        }

        // <赋值语句>
        // <标识符>＝<表达式> |   <标识符>'['<表达式>']'=<表达式>    |<标识符>'['<表达式>']''['<表达式>']' =<表达式>
        else if (lexRes[currPos].first == "IDENFR") {
            // 标识符
            outputLexRes(out);

            if (lexRes[currPos].first == "ASSIGN") {
                // ＝  表达式
                outputLexRes(out);
                expression(out);
            } else {
                // [ 表达式 ]
                outputLexRes(out);
                expression(out);
                outputLexRes(out);
                if (lexRes[currPos].first == "ASSIGN") {
                    // ＝ 表达式
                    outputLexRes(out);
                    expression(out);
                }
                else if (lexRes[currPos].first == "LBRACK") {
                    // [ 表达式 ]
                    outputLexRes(out);
                    expression(out);
                    outputLexRes(out);

                    // = 表达式
                    outputLexRes(out);
                    expression(out);
                }
            }
            out << "<赋值语句>" << endl;
            //;
            outputLexRes(out);
        }

        else {
            cout << "Error!" << endl;
            throw system_error();
        }

        out << "<语句>" << endl;
    }

    //<表达式>::= ［＋｜－］<项>{<加法运算符><项>}
    void expression(ofstream& out) {
        // 如果有 [- +] 先输出
        if (lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU") {
            outputLexRes(out);
        }
        term(out);
        while (lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU") {
            outputLexRes(out);
            term(out);
        }
        out << "<表达式>" << endl;
    }

    //<项> ::= <因子>{<乘法运算符><因子>}
    void term(ofstream& out) {
        factor(out);
        while (lexRes[currPos].first == "MULT" || lexRes[currPos].first == "DIV") {
            outputLexRes(out);
            factor(out);
        }
        out << "<项>" << endl;
    }

    //todo <因子> ::= <标识符> ｜ <标识符>'['<表达式>']'  |  <标识符>'['<表达式>']''['<表达式>']' | '('<表达式>')' ｜ <整数> |  <字符> ｜   <有返回值函数调用语句>
    void factor(ofstream& out) {
        // <有返回值函数调用语句>
        if (funcResType.find(lexRes[currPos].second) != funcResType.end()) {
            // <标识符> '(' <值参数表> ')'
            outputLexRes(out);
            outputLexRes(out);
            if (lexRes[currPos].first != "RPARENT") {
                valueParameterTable(out);
            } else {
                out << "<值参数表>" << endl;
            }
            outputLexRes(out);
            out << "<有返回值函数调用语句>" << endl;
        }

        // <字符> ::=  '<加法运算符>'｜'<乘法运算符>'｜'<字母>'｜'<数字>'，不知道会不会出现问题
        else if (lexRes[currPos].first == "CHARCON") {
            outputLexRes(out);
//            out << "<字符> " << endl;
        }

        // <整数>
        else if (lexRes[currPos].first == "INTCON") {
            outputLexRes(out);
            out << "<无符号整数>" << endl;
            out << "<整数>" << endl;
        }
        else if ((lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU") && lexRes[currPos + 1].first == "INTCON") {
            outputLexRes(out);
            outputLexRes(out);
            out << "<无符号整数>" << endl;
            out << "<整数>" << endl;
        }

        // '('  <表达式>  ')'
        else if (lexRes[currPos].first == "LPARENT") {
            outputLexRes(out);
            expression(out);
            outputLexRes(out);
        }
        // 标识符
        else {
            // <标识符>
            outputLexRes(out);
            if (lexRes[currPos].first == "LBRACK") {
                //  '['  <表达式>   ']'
                outputLexRes(out);
                expression(out);
                outputLexRes(out);
                if (lexRes[currPos].first == "LBRACK") {
                    //  '['  <表达式>   ']'
                    outputLexRes(out);
                    expression(out);
                    outputLexRes(out);
                }
            }
        }
        out << "<因子>" << endl;
    }
    // <值参数表> ::= <表达式>{,<表达式>}｜<空>
    void valueParameterTable(ofstream& out) {
        if (lexRes[currPos].first == "RPARENT") {
            out << "<值参数表>" << endl;
            return;
        }
        expression(out);
        while (lexRes[currPos].first == "COMMA") {
            outputLexRes(out);
            expression(out);
        }
        out << "<值参数表>" << endl;
    }

    // <条件>  ::=  ＜表达式＞＜关系运算符＞＜表达式＞
    void condition(ofstream& out) {
        expression(out);
        outputLexRes(out);
        expression(out);
        out << "<条件>" << endl;
    }

    void func(ofstream& out) {
        string funcType;
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

        funcResType[lexRes[currPos].second] = funcType;

        // 标识符 / Main
        outputLexRes(out);

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
        else {
            // 先输出 "<声明头部>" 再输出左括号
            if (funcType == "<有返回值函数定义>") {
                out << "<声明头部>" << endl;
            }
            // (
            outputLexRes(out);
            // <参数表>
            outputLexRes(out);
            outputLexRes(out);
            while (lexRes[currPos].first == "COMMA") {
                // ,
                outputLexRes(out);
                outputLexRes(out);
                outputLexRes(out);
            }
            if (funcType != "<主函数>") {
                out << "<参数表>" << endl;
            }
        }
        // 输出右括号
        outputLexRes(out);
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
    }

    void step(ofstream& out) {
        unsignedInteger(out);
        out << "<步长>" << endl;
    }

    // ＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
    void situationTable(ofstream& out) {
        situation(out);
        while (lexRes[currPos].first == "CASETK") {
            situation(out);
        }
        out << "<情况表>" << endl;
    }

    // ＜情况子语句＞  ::=  case＜常量＞：＜语句＞
    void situation(ofstream& out) {
        outputLexRes(out);
        constNum(out);
        outputLexRes(out);
        statement(out);
        out << "<情况子语句>" << endl;
    }

    // TODO 常量 字符这一块不知道要不要改一下
    void constNum(ofstream& out) {
        if (lexRes[currPos].first == "INTCON" ||
            ((lexRes[currPos + 1].first == "INTCON") && (lexRes[currPos].first == "PLUS" || lexRes[currPos].first == "MINU"))) {
            intNum(out);
        }
        else if (lexRes[currPos].first == "CHARCON") {
            outputLexRes(out);
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

    void execute() {
        this->lexicalAnalysis();
        ofstream out(outputPath);
        for (currPos = 0; currPos < lexRes.size(); currPos++) {
            // 常量说明
            if (lexRes[currPos].first == "CONSTTK") {
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
                func(out);
            }
            // 变量说明
            else if (varIDENFR(lexRes[currPos].first)  && (lexRes[currPos + 2].first) != "LPARENT") {
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