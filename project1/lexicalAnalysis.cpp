 #include <iostream>
 #include <fstream>
 #include <vector>
 #include <map>
//#include <bits/stdc++.h>
using namespace std;

class LexicalAnalyzer {
public:
    vector<string> codeRecorder;
    int currRow;
    int currCol;
    map<string, string> keyword = {
        {"const", "CONSTTK"}, {"int", "INTTK"}, {"char", "CHARTK"}, {"void", "VOIDTK"}, {"main", "MAINTK"}, {"if", "IFTK"}, {"else", "ELSETK"}, {"switch", "SWITCHTK"}, 
        {"case", "CASETK"}, {"default", "DEFAULTTK"}, {"while", "WHILETK"}, {"for", "FORTK"}, {"scanf", "SCANFTK"}, {"printf", "PRINTFTK"}, {"return", "RETURNTK"}};


    fstream in;
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

    LexicalAnalyzer() {
        in.open("testfile.txt", ios::in);
        currRow = 0;
        currCol = 0;
        while (in.peek() != EOF) {
            string input;
            getline(in, input);
            codeRecorder.push_back(input);
        }
        in.close();
    }



    void execute() {
        ofstream out("output.txt");
        for (currRow = 0; currRow < codeRecorder.size(); currRow++) {
            string str = "";
            for (currCol = 0; currCol < codeRecorder[currRow].size(); currCol++) {
                char c = codeRecorder[currRow][currCol];

                if (isInt(c)) {
                    string temp = "";
                    for (c = codeRecorder[currRow][currCol]; currCol < codeRecorder[currRow].size() && isInt(c); c = codeRecorder[currRow][++currCol]) {
                        temp.push_back(c);
                    }
                    out << "INTCON " << temp << endl;
                    currCol--;
                }

                else if (isLetter(c)) {
                    string temp = "";
                    for (c = codeRecorder[currRow][currCol]; currCol < codeRecorder[currRow].size() && (isLetter(c) || isInt(c)); c = codeRecorder[currRow][++currCol]) {
                        temp.push_back(c);
                    }                    
                    // out << ""
                    string lowerString = lower(temp);
                    if (keyword.find(lowerString) != keyword.end()) {
                        out << keyword[lowerString] << " " << temp << endl;
                    }
                    else {
                        out << "IDENFR " << temp << endl;
                    }
                    currCol--;
                }

                else {
                    switch (c) {
                    case '+':
                        out << "PLUS " << c << endl;
                        break;

                    case '-':
                        out << "MINU " << c << endl;
                        break;

                    case '*':
                        out << "MULT " << c << endl;
                        break;

                    case '/':
                        out << "DIV " << c << endl;
                        break;

                    case '<':
                        if (codeRecorder[currRow][currCol + 1] == '=') {
                            out << "LEQ " << "<=" << endl;
                            currCol++;
                        }
                        else {
                            out << "LSS " << "<" << endl;
                        }
                        break;

                    case '>':
                        if (codeRecorder[currRow][currCol + 1] == '=') {
                            out << "GEQ " << ">=" << endl;
                            currCol++;
                        }
                        else {
                            out << "GRE " << ">" << endl;
                        }
                        break;
                    case '=':
                        if (codeRecorder[currRow][currCol + 1] == '=') {
                            out << "EQL " << "==" << endl;
                            currCol++;
                        }
                        else {
                            out << "ASSIGN " << "=" << endl;
                        }
                        break;

                    case '!':
                        if (codeRecorder[currRow][currCol + 1] == '=') {
                            out << "NEQ " << "!=" << endl;
                            currCol++;
                        }
                        else {
                            // out << " " << "!" << endl;
                            // 还没有非
                        }
                        break;
                    case ':':
                        out << "COLON :" << endl;
                        break;
                    case ';':
                        out << "SEMICN ;" << endl;
                        break;
                    case ',':
                        out << "COMMA ," << endl;
                        break;
                    case '(':
                        out << "LPARENT (" << endl;
                        break;
                    case ')':
                        out << "RPARENT )" << endl;
                        break;
                    case '[':
                        out << "LBRACK [" << endl;
                        break;
                    case ']':
                        out << "RBRACK ]" << endl;
                        break;
                    case '{':
                        out << "LBRACE {" << endl;
                        break;
                    case '}':
                        out << "RBRACE }" << endl;
                        break;
                    
                    case '\'':
                        out << "CHARCON " << codeRecorder[currRow][currCol + 1] << endl;
                        currCol += 2;
                        break;
                    case '\"':
                        string strConTemp = "";
                        int i;
                        for (i = currCol + 1; i < codeRecorder[currRow].size() && codeRecorder[currRow][i] != '\"'; i++) {
                            strConTemp.push_back(codeRecorder[currRow][i]);
                        }
                        if (codeRecorder[currRow][i] == '\"') {
                            out << "STRCON " << strConTemp << endl;
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
        // out << "RBRACE }" << endl;
    }
};


int main() {
    LexicalAnalyzer* analyzer = new LexicalAnalyzer();
    analyzer->execute();
    // system("pause");
    return 0;
}
