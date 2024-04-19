#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <ctype.h>
#include <list>
#include <algorithm>

#include "lex.cpp"
#include "lex.h"

int main(int argc, char *argv[]) {
    std::string filename;
    bool allFlag = false;
    bool intFlag = false;
    bool realFlag = false;
    bool strFlag = false;
    bool idFlag = false;
    bool kwFlag = false;

    std::list<int>intList;
    std::list<float>realList;
    std::list<std::string>strList;
    std::map<std::string, int>idMap;
    std::map<std::string, int>kwMap;


    if(argc > 1){
        filename = argv[1];
    }

    else{
        std::cout << "NO SPECIFIED INPUT FILE." << std::endl;
        exit(1);
    }

    for(int i = 2; i < argc; i++){
        if(std::string(argv[i]) == "-all"){
            allFlag = true;
        }
        else if(std::string(argv[i]) == "-int"){
            intFlag = true;
        }
        else if(std::string(argv[i]) == "-real"){
            realFlag = true;
        }
        else if(std::string(argv[i]) == "-str"){
            strFlag = true;
        }
        else if(std::string(argv[i]) == "-id"){
            idFlag = true;
        }
        else if(std::string(argv[i]) == "-kw"){
            kwFlag = true;
        }
        else if(std::string(argv[i])[0] == '-'){
            std::cout << "UNRECOGNIZED FLAG {" << argv[i] << "}" << std::endl;
            exit(1);
        }

        else{
            std::cout << "ONLY ONE FILE NAME IS ALLOWED." << std::endl;
            exit(1);
        }
    }

    std::ifstream readFile;
    readFile.open(filename.c_str());

    if(!readFile.is_open()){
        std::cout << "CANNOT OPEN THE FILE " + filename << std::endl;
        exit(1);
    }

    if(readFile.peek() == EOF){
        std::cout << "Empty File." << std::endl;
        exit(1);
    }
    int numLines = 1;
    int numTokens = 0;

    LexItem nextToken;
    while((nextToken = getNextToken(readFile, numLines)).GetToken() != ERR && nextToken.GetToken() != DONE){

        map<Token, string> enumStringMap = {
        {Token::IF, "IF"},
        {Token::ELSE, "ELSE"},
        {Token::PRINT, "PRINT"},
        {Token::INTEGER, "INTEGER"},
        {Token::REAL, "REAL"},
        {Token::CHARACTER, "CHARACTER"},
        {Token::END, "END"},
        {Token::THEN, "THEN"},
        {Token::PROGRAM, "PROGRAM"},
        {Token::LEN, "LEN"},
        {Token::IDENT, "IDENT"},
        {Token::ICONST, "ICONST"},
        {Token::RCONST, "RCONST"},
        {Token::SCONST, "SCONST"},
        {Token::BCONST, "BCONST"},
        {Token::PLUS, "PLUS"},
        {Token::MINUS, "MINUS"},
        {Token::MULT, "MULT"},
        {Token::DIV, "DIV"},
        {Token::ASSOP, "ASSOP"},
        {Token::EQ, "EQ"},
        {Token::POW, "POW"},
        {Token::GTHAN, "GTHAN"},
        {Token::LTHAN, "LTHAN"},
        {Token::CAT, "CAT"},
        {Token::COMMA, "COMMA"},
        {Token::LPAREN, "LPAREN"},
        {Token::RPAREN, "RPAREN"},
        {Token::DOT, "DOT"},
        {Token::DCOLON, "DCOLON"},
        {Token::DEF, "DEF"},
        {Token::ERR, "ERR"},
        {Token::DONE, "DONE"},
        {Token::THEN, "THEN"}

    };

        numLines = nextToken.GetLinenum();
        numTokens += 1;

        //keyword adder:
        if((nextToken.GetToken() == IF) || (nextToken.GetToken() == ELSE) || (nextToken.GetToken() == PRINT) ||(nextToken.GetToken() == INTEGER) || 
        (nextToken.GetToken() == REAL) || (nextToken.GetToken() == CHARACTER) || (nextToken.GetToken() == END) || (nextToken.GetToken() == THEN) ||
         (nextToken.GetToken() == PROGRAM) || (nextToken.GetToken() == LEN)){
            
            string key = nextToken.GetLexeme();
            auto iter = kwMap.find(key);

                if(iter != kwMap.end()){
                    iter->second += 1;
                }
                else{
                    kwMap[key] = 1;
                }
        }
            //identifier adder
            if(nextToken.GetToken() == IDENT){
                string key = nextToken.GetLexeme();
                auto iter = idMap.find(key);

                if(iter != idMap.end()){
                    iter->second += 1;
                }
                else{
                    idMap[key] = 1;
                }
            }
            //string adder
            if(nextToken.GetToken() == SCONST){
                strList.push_back(nextToken.GetLexeme());

            }
            //real adder
            if(nextToken.GetToken() == RCONST){
                
                auto it = std::find(realList.begin(), realList.end(), std::stof(nextToken.GetLexeme()));

                if(it == realList.end()){
                    realList.push_back(std::stof(nextToken.GetLexeme()));
                }
                
            }
            //int adder
            if(nextToken.GetToken() == ICONST){

                auto it = std::find(intList.begin(), intList.end(), std::stoi(nextToken.GetLexeme()));

                if(it == intList.end()){
                    intList.push_back(std::stoi(nextToken.GetLexeme()));
                }

            }
            if(allFlag){
                std::cout << nextToken << endl;
            }

    }

    if(nextToken == ERR){
        cout << "Error in line " << nextToken.GetLinenum() << ": Unrecognized Lexeme {" << nextToken.GetLexeme() << "}" << endl;
        readFile.close();
        exit(1);
    }
    if(nextToken == DONE){

        cout << endl;
        cout << "Lines: " << numLines - 1 << endl;
        cout << "Total Tokens: " << numTokens << endl;
        cout << "Identifiers: " << idMap.size() << endl;
        cout << "Integers: " << intList.size() << endl;
        cout << "Reals: " << realList.size() << endl;
        cout << "Strings: " << strList.size() << endl;

        if(idFlag){
            if(!(kwMap.empty())){
                    
                cout << "IDENTIFIERS:" << endl;
                auto last = idMap.end();
                --last;
                auto it = idMap.begin();

                for (; it != last; ++it) {
                    cout << it->first << " (" << it->second << "), ";
                                
                }
                cout << it->first << " (" << it->second << ")" << endl;

            }
        }

        if(kwFlag){
            //check empty
            if(!(kwMap.empty())){
                cout << "KEYWORDS:" << endl;
                auto last = kwMap.end();
                --last;
                auto it = kwMap.begin();

                
                for (; it != last; ++it) {
                    
                    cout << it->first << " (" << it->second << "), ";
                                
                }
                cout << it->first << " (" << it->second << ")" << endl;
    

            }
        }

        if(intFlag){
            if(!(intList.empty())){
                intList.sort();

                cout << "INTEGERS:" << endl;
                auto last = intList.end();
                --last;
                auto it = intList.begin();

                for (; it != last; ++it) {
                    cout << *it << ", ";
                                
                }
                cout << *it << endl;

            }
        }

        if(realFlag){
            if(!(realList.empty())){
                realList.sort();

                cout << "REALS:" << endl;
                auto last = realList.end();
                --last;
                auto it = realList.begin();

                for (; it != last; ++it) {
                    cout << *it << ", ";          
                }
            cout << *it << endl;

            }
        }

        if(strFlag){
            std::string temp;
            if(!(strList.empty())){
                cout << "STRINGS:" << endl;
            auto last = strList.end();
            --last;
            auto it = strList.begin();

            for (; it != last; ++it) {
                string temp = *it;
                temp = temp.substr(1, (temp.size() - 2));
                cout << "\"" << temp << "\", ";
                  
            }
            
            string temp = *it;
            temp = temp.substr(1, (temp.size() - 2));
            cout << "\"" << temp << "\"" << endl;
            }
        }

        //temp soln:
        //std::cout << "End of File reached." << std::endl;
        //readFile.close();
        //exit(0);

    }    

    readFile.close();
    exit(0);
    
}