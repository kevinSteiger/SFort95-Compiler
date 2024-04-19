#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <map>

#include "lex.h"

//using namespace std;

LexItem Id_or_kw(string &lexeme, int linenum){
    std::map<std::string, Token> keywords;

    keywords["program"] = Token::PROGRAM;
    keywords["end"] = Token::END;
    keywords["else"] = Token::ELSE;
    keywords["if"] = Token::IF;
    keywords["integer"] = Token::INTEGER;
    keywords["real"] = Token::REAL;
    keywords["character"] = Token:: CHARACTER;
    keywords["print"] = Token:: PRINT;
    keywords["len"] = Token::LEN;
    keywords["then"] = Token::THEN;

    string temp;
    for(char c : lexeme){
        temp += tolower(c);
    }

    auto iter = keywords.find(temp);

    if(iter != keywords.end()){
        return LexItem(iter->second, lexeme, linenum);
    }
    else{
        return LexItem(IDENT, lexeme, linenum);
    }

}

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

ostream& operator<< (ostream& out, const LexItem& item){
    

    if(item.GetToken() == SCONST){
        string printed = item.GetLexeme().substr(1, item.GetLexeme().size() - 2);
        out << "SCONST: \"" << printed << "\"";
        return out;
    }
    if(item.GetToken() == IDENT){
        out << "IDENT: '" << item.GetLexeme() << "'";
        return out;
    }
    if(item.GetToken() == ICONST){
        out << "ICONST: (" << item.GetLexeme() << ")";
        return out;
    }
    if(item.GetToken() == RCONST){
        out << "RCONST: (" << item.GetLexeme() << ")";
        return out;
    }
    else{
        auto it = enumStringMap.find(item.GetToken());
        string temp = it->second;

        out << temp;
        return out;
    }
    
    //return out;
}

LexItem getNextToken(istream& in, int& linenum){

    enum TokState{START, INID, ININT, INSTR, DONE, ERR} lexState = START;
    string lexeme = "";
    char ch;

    while(in.get(ch) && !in.eof()){

        
        if(ch == '\n'){
                linenum += 1;
                continue;
            }
        else if(isspace(ch) && lexState != INSTR){
                continue;
            }
        else if(ch == '!'){

            while(in.peek() != '\n'){
                    in.get();
                }
                //linenum += 1;
                continue;
                //break;
            }
        
        
        switch(lexState){
            

            case START:
                if(isalpha(ch)){
                    lexeme += ch;
                    if((!isalnum(in.peek())) && !(in.peek() == '_')){
                        return LexItem(IDENT, lexeme, linenum); 
                    }
                    else{
                        lexState = INID;
                        break;
                    }
                    
                }
                if(isdigit(ch)){
                    lexeme += ch;

                    if(!(isdigit(in.peek())) && !(in.peek() == '.') && !(ch == '.')){
                        return LexItem(ICONST, lexeme, linenum);
                    }
                    lexState = ININT;
                    break;
                }
                if(ch == '\"' || ch == '\''){
                    lexeme += ch;
                    lexState = INSTR;
                    break;
                }

                else{
                    Token ttype = Token::ERR;
                    lexeme = ch;
                    switch(ch){
                        case '+':
                            ttype = PLUS;
                            break;
                        case '-':
                            ttype = MINUS;
                            break;
                        case '*':
                            if(in.peek() == '*'){
                                ttype = POW;
                                in.get();
                                break;
                            }
                            else if(in.peek() == ','){
                                ttype = DEF;
                                //in.get();
                                break;
                            }
                            else{
                                ttype = MULT;
                                break;
                            }
                        case '/':
                            if(in.peek() == '/'){
                                ttype = CAT;
                                in.get();
                                break;
                            }
                            else{
                                ttype = DIV;
                                break;
                            }
                        case '=':
                            if(in.peek() == '='){
                                ttype = EQ;
                                in.get();
                                break;
                            }
                            else{
                                ttype = ASSOP;
                                break;
                            }
                        case '<':
                            ttype = LTHAN;
                            break;
                        case '>':
                            ttype = GTHAN;
                            break;
                        case ',':
                            ttype = COMMA;
                            break;
                        case '(':
                            ttype = LPAREN;
                            break;
                        case ')':
                            ttype = RPAREN;
                            break;
                        case ':':
                            if(in.peek() == ':'){
                                ttype = DCOLON;
                                in.get();
                                break;
                            }
                        
                        case '.':
                            if(!isdigit(in.peek())){
                                ttype = DOT;
                                break;
                            }
                            lexState = ININT;
                            continue;

                    }
                    return LexItem(ttype, lexeme, linenum);   

                }
            case INID:{
                    lexeme += ch;

                    if(isalpha(in.peek()) || isdigit(in.peek()) || in.peek() == '_'){
                        continue;
                    }
                    else{
                        return(Id_or_kw(lexeme, linenum));
                    }
            }   
            case ININT:{
                Token ttype = Token::ERR;
                lexeme += ch;
                int count = 0;

                if(isdigit(in.peek()) || in.peek() == '.' || ch == '.'){
                        for(char c : lexeme){
                        if(c == '.'){
                            count += 1;
                        }
                    }

                    if(count > 1){
                    return LexItem(Token::ERR, lexeme, linenum);
                    }
                    if(ch == '.' && !isdigit(in.peek())){
                        return LexItem(Token::ERR, lexeme, linenum);
                    }
                    continue;                    
                }

                for(char c : lexeme){
                        if(c == '.'){
                            count += 1;
                        }
                    }

                if(count == 1){
                    ttype = RCONST;
                    return LexItem(ttype, lexeme, linenum);
                }
                
                else{
                    ttype = ICONST;
                    return LexItem(ttype, lexeme, linenum);
                    }
                
                }


            case INSTR:{ 
                lexeme += ch;
                
                int count1 = 0;
                int count2 = 0;
                for(char c : lexeme){
                    if(c == '\"'){
                            count1 += 1;
                        }
                }
                for(char c: lexeme){
                    if(c == '\''){
                        count2 += 1;
                    }
                }

                if(count1 == 2 || count2 == 2){
                    return LexItem(SCONST, lexeme, linenum);
                }
                if((count1 == 1) && (count2 == 1)){
                    return LexItem(Token::ERR, lexeme, linenum);
                }
                if(in.peek() == '\n'){
                    return LexItem(Token::ERR, lexeme, linenum);
                }
                
                continue;
            }

            case ERR:
                return LexItem(Token::ERR, lexeme, linenum);
                
            case DONE:
                return LexItem(Token::DONE, "", linenum);
        }

    }
    //EOF:
    return LexItem(Token::DONE, "", linenum);
    
}