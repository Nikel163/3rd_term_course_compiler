//условные обозначения
// s/ - разделитель
// c/ - знак сравнения
// i/ - идентификатор
// e/ - знак присваивания
// k/ - ключевое слово
// r/ - римское число
#include <bits/stdc++.h>
using namespace std;

enum states {start,f1,f2,f3,d1,d2,id,A,A1,B,E,m1,m2,m3,
             E1,G,H,H1,K,c1,c2,c3,x1,x2,x3,i1,i2,i3,coment} state;

enum types {lim, comp, eq, keyword, ID, rom, st, znak, nonTerm} type;
string buf, lexem; vector <string> v;
char c;
bool isLexem = true;
struct LEXEM
{
    string lexema;
    types lexemType;
}; vector <LEXEM> vectorLexem; //список лексем

const int mountID = 500; // количество полей в таблице идентификаторов

struct IDTABLE // таблица идентификаторов
{
    vector <int> hashOrder; // массив идентификаторов
    string hashTable[mountID]; // хэш-таблица
    IDTABLE(){for(int i=0; i<mountID; i++)hashTable[i] = "";} // обнуление таблицы при создании
}tableID;

bool check(types type, ifstream& in)
{
    switch(type)
    {
        case st:
            switch(c)
            {
                case ';': v.push_back("s/;"); return false;
                case '(': v.push_back("s/("); return false;
                case ')': v.push_back("s/)"); return false;
                case '<': v.push_back("c/<"); return false;
                case '>': v.push_back("c/>"); return false;
                case '=': v.push_back("c/="); return false;
                case '{': state = coment; return false;
                case ' ': case '\0': case '\n': state = start; return false;
                case ':': if((c=in.get()) == '='){v.push_back("e/:="); return false;}
                                          else {in>>buf; cout<<"\nОшибка: ожидалось '=' в "<<buf<<endl; isLexem = false; return false;}
                case '0'...'9': in>>buf; cout<<"\nОшибка: ожидалось '=' в "<<buf<<endl; isLexem = false; return false;
                default: return true;
            } break;
        case ID:
            switch(c)
            {
                case ';': v.push_back("i/"+lexem); v.push_back("s/;"); state = start; return false;
                case '(': v.push_back("i/"+lexem); v.push_back("s/("); state = start; return false;
                case ')': v.push_back("i/"+lexem); v.push_back("s/)"); state = start; return false;
                case '<': v.push_back("i/"+lexem); v.push_back("c/<"); state = start; return false;
                case '>': v.push_back("i/"+lexem); v.push_back("c/>"); state = start; return false;
                case '=': v.push_back("i/"+lexem); v.push_back("c/="); state = start; return false;
                case '{': state = coment; return false;
                case ' ': case '\0': case '\n': v.push_back("i/"+lexem); state = start; return false;
                case ':': if((c = in.get()) == '='){v.push_back("i/"+lexem);
                                                    v.push_back("e/:="); state = start; return false;}
                          else {in>>buf; cout<<"\nОшибка: ожидалось '=' в "<<lexem<<":"<<buf<<endl; state = start; isLexem = false; return false;}
                default: return true;
            } break;
        case keyword:
            switch(c)
            {
                case ';': v.push_back("k/"+lexem); v.push_back("s/;"); state = start; return false;
                case '(': v.push_back("k/"+lexem); v.push_back("s/("); state = start; return false;
                case ')': v.push_back("k/"+lexem); v.push_back("s/)"); state = start; return false;
                case '<': v.push_back("k/"+lexem); v.push_back("c/<"); state = start; return false;
                case '>': v.push_back("k/"+lexem); v.push_back("c/>"); state = start; return false;
                case '=': v.push_back("k/"+lexem); v.push_back("c/="); state = start; return false;
                case '{': state = coment; return false;
                case ' ': case '\0': case '\n': v.push_back("k/"+lexem); state = start; return false;
                case ':': if((c = in.get()) == '='){v.push_back("k/"+lexem);
                                                    v.push_back("e/:="); state = start; return false;}
                          else {in>>buf; cout<<"\nОшибка: ожидалось '=' в "<<buf<<endl; isLexem = false; return false;}
                default: return true;
            } break;
        case rom:
            switch(c)
            {
                case ';': v.push_back("r/"+lexem); v.push_back("s/;"); state = start; return false;
                case '(': v.push_back("r/"+lexem); v.push_back("s/("); state = start; return false;
                case ')': v.push_back("r/"+lexem); v.push_back("s/)"); state = start; return false;
                case '<': v.push_back("r/"+lexem); v.push_back("c/<"); state = start; return false;
                case '>': v.push_back("r/"+lexem); v.push_back("c/>"); state = start; return false;
                case '=': v.push_back("r/"+lexem); v.push_back("c/="); state = start; return false;
                case '{': state = coment; return false;
                case ' ': case '\0': case '\n': v.push_back("r/"+lexem); state = start; return false;
                case ':': if((c = in.get()) == '='){v.push_back("r/"+lexem);
                                                    v.push_back("e/:="); state = start; return false;}
                          else {in>>buf; cout<<"\nОшибка: ожидалось '=' в "<<lexem<<":"<<buf<<endl; state = start; isLexem = false; return false;}
                default: return true;
            } break;
    }
}

void lexems(const string &fileInput, const string &fileOutput)
{
    ifstream in(fileInput.c_str());
    v.clear(); state = start;

    while(in.get(c))
    {
            switch(state)
            {
                case start: lexem="";
                    if(check(st,in))
                        switch(c)
                            {
                                case 'f': lexem += c; state = f1; break;
                                case 'd': lexem += c; state = d1; break;

                                case 'a'...'c': case 'e': case 'g'...'z':
                                case 'A'...'B': case 'E': case 'F'...'H':
                                case 'J'...'K': case 'N'...'U': case 'W':
                                case 'Y': case 'Z': case '_': lexem += c; state = id; break;

                                case 'M': lexem += c; state = m1; break;
                                case 'D': lexem += c; state = B; break;
                                case 'C': lexem += c; state = A1; break;
                                case 'L': lexem += c; state = G; break;
                                case 'X': lexem += c; state = E1; break;
                                case 'V': lexem += c; state = K; break;
                                case 'I': lexem += c; state = H1; break;
                                default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                            } break;

                case coment: while((c = in.get()) != '}'); state = start; break;

                case f1:
                    if(check(ID,in))
                        switch(c)
                        {
                            case 'o': lexem += c; state = f2; break;
                            case '0'...'9': case 'a'...'n':
                            case 'p'...'z': case 'A'...'Z':
                            case '_': lexem += c; state = id; break;
                            default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                        } break;

                case f2:
                    if(check(ID,in))
                        switch(c)
                        {
                            case 'r': lexem += c; state = f3; break;
                            case '0'...'9': case 'a'...'q':
                            case 's'...'z': case 'A'...'Z':
                            case '_': lexem += c; state = id; break;
                            default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                        } break;

                case f3:
                    if(check(keyword,in))
                        switch(c)
                        {
                            case '0'...'9': case 'a'...'z':
                            case 'A'...'Z': case '_': lexem += c; state = id; break;
                            default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                        } break;

                case d1:
                    if(check(ID,in))
                        switch(c)
                        {
                            case 'o': lexem += c; state = d2; break;
                            case '0'...'9': case 'a'...'n':
                            case 'p'...'z': case 'A'...'Z':
                            case '_': lexem += c; state = id; break;
                            default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                        } break;

                case d2:
                    if(check(keyword,in))
                    switch(c)
                    {
                        case '0'...'9': case 'a'...'z':
                        case 'A'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case id:
                    if(check(ID,in))
                    switch(c)
                    {
                        case '0'...'9': case 'a'...'z':
                        case 'A'...'Z': case '_': lexem += c; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case m1:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'M': lexem += c; state = m2; break;
                        case 'D': lexem += c; state = B; break;
                        case 'C': lexem += c; state = A1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'X': lexem += c; state = E1; break;
                        case 'V': lexem += c; state = K; break;
                        case 'I': lexem += c; state = H1; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'B': case 'E'...'H':
                        case 'J'...'K': case 'N'...'U': case 'W': case 'Y'...'Z':
                        case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case m2:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'M': lexem += c; state = m3; break;
                        case 'D': lexem += c; state = B; break;
                        case 'C': lexem += c; state = A1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'X': lexem += c; state = E1; break;
                        case 'V': lexem += c; state = K; break;
                        case 'I': lexem += c; state = H1; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'B': case 'E'...'H':
                        case 'J'...'K': case 'N'...'U': case 'W': case 'Y'...'Z':
                        case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case m3:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'C': lexem += c; state = A1; break;
                        case 'D': lexem += c; state = B; break;
                        case 'I': lexem += c; state = H1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'V': lexem += c; state = K; break;
                        case 'X': lexem += c; state = E1; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'B': case 'E'...'H':
                        case 'J'...'K': case 'M'...'U': case 'W': case 'Y'...'Z':
                        case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case A1:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'C': lexem += c; state = c2; break;
                        case 'D': case 'M': lexem += c; state = E; break;
                        case 'I': lexem += c; state = H1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'X': lexem += c; state = E1; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'B': case 'E'...'H':
                        case 'J'...'K': case 'N'...'U': case 'W': case 'Y'...'Z':
                        case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case B:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'C': lexem += c; state = c1; break;
                        case 'I': lexem += c; state = H1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'X': lexem += c; state = E1; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'B': case 'D'...'H':
                        case 'J'...'K': case 'M'...'U': case 'W': case 'Y'...'Z':
                        case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case c1:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'C': lexem += c; state = c2; break;
                        case 'I': lexem += c; state = H1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'X': lexem += c; state = E1; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'B': case 'D'...'H':
                        case 'J'...'K': case 'M'...'U': case 'W': case 'Y'...'Z':
                        case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case c2:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'C': lexem += c; state = c3; break;
                        case 'I': lexem += c; state = H1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'X': lexem += c; state = E1; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'B': case 'D'...'H':
                        case 'J'...'K': case 'M'...'U': case 'W': case 'Y'...'Z':
                        case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case c3:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = H1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'X': lexem += c; state = E1; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H': case 'J'...'K':
                        case 'M'...'U': case 'W': case 'Y'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case E:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = H1; break;
                        case 'L': lexem += c; state = G; break;
                        case 'X': lexem += c; state = E1; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H': case 'J'...'K':
                        case 'M'...'U': case 'W': case 'Y'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case E1:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'C': lexem += c; state = H; break;
                        case 'I': lexem += c; state = H1; break;
                        case 'L': lexem += c; state = H; break;
                        case 'X': lexem += c; state = x2; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z':
                        case 'A'...'B': case 'D'...'H':
                        case 'J'...'K': case 'M'...'U':
                        case 'W': case 'Y'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case x1:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = H1; break;
                        case 'X': lexem += c; state = x2; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'U': case 'W': case 'Y'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case x2:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = H1; break;
                        case 'X': lexem += c; state = x3; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'U': case 'W': case 'Y'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case x3:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = H1; break;
                        case 'V': lexem += c; state = K; break;

                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'U': case 'W'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case G:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = H1; break;
                        case 'X': lexem += c; state = x1; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'U': case 'W': case 'Y'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case H:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = H1; break;
                        case 'V': lexem += c; state = K; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'U': case 'W'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case K:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = i1; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case H1:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = i2; break;
                        case 'X': case 'V': lexem += c; state = i3; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'U': case 'W': case 'Y'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case i1:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = i2; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case i2:
                    if(check(rom,in))
                    switch(c)
                    {
                        case 'I': lexem += c; state = i3; break;
                        case '0'...'9': case 'a'...'z': case 'A'...'H':
                        case 'J'...'Z': case '_': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;

                case i3:
                    if(check(rom,in))
                    switch(c)
                    {
                        case '0'...'9': case 'a'...'z':
                        case 'A'...'Z': lexem += c; state = id; break;
                        default: cout<<"ОШИБКА: символ '"<<c<<"' недопустим\n"; isLexem = false; return;
                    } break;
            }
    }

    cout<<endl;
    ofstream out(fileOutput.c_str(), ios_base::ate|ios_base::out);
    for (int i=0; i<v.size(); i++)
        out<<v[i]<<endl;
    out.close();
}

int getHash(string ID) // функция хэширования
{
    unsigned int hash = 642;  //магическое число
    int symb;

    for (int i=0; i<ID.size(); i++)
    {
        symb = (int)ID[i];
        hash = ((hash << 3) + hash) + symb;
    }
    if (hash > mountID) return hash%mountID;
    else return hash;
}

void fromLEXEMtoIDTABLE() //создание таблицы идентификаторов
{
    for(int i=0; i<vectorLexem.size(); i++)
    {
        if(vectorLexem[i].lexemType == ID)
        {
            int hash_line = getHash(vectorLexem[i].lexema);
            while(1)
            {
                if(tableID.hashTable[hash_line] == "" || tableID.hashTable[hash_line] == vectorLexem[i].lexema || hash_line >= mountID)break;
                hash_line = (hash_line++)%mountID;
            }

            if(hash_line >= mountID)
            {
                cout<<"\nВ ТАБЛИЦЕ НЕТ МЕСТА ДЛЯ "<<vectorLexem[i].lexema<<endl;
                continue;
            }

            if(tableID.hashTable[hash_line] == "")
            {
                tableID.hashTable[hash_line] = vectorLexem[i].lexema;
                tableID.hashOrder.push_back(hash_line);
            }
        }
    }

    ofstream out("tableID.txt");
    for(int i=0; i<tableID.hashOrder.size(); i++)
        out<<tableID.hashOrder[i]<<". "<<tableID.hashTable[tableID.hashOrder[i]]<<endl;
    out.close();
}

void setLexemStruct(const string &fileOutput) //заполнение вектора лексем и генерация таблицы идентификаторов
{
    ifstream in(fileOutput.c_str());
    vectorLexem.clear();
    LEXEM buf; char c;
    while(in.get(c))
    {
        switch(c)
        {
            case 's':
                buf.lexemType = lim;
                if((c=in.get()) == '/')in>>buf.lexema; vectorLexem.push_back(buf); break;
            case 'c':
                buf.lexemType = comp;
                if((c=in.get()) == '/')in>>buf.lexema; vectorLexem.push_back(buf); break;
            case 'i':
                buf.lexemType = ID;
                if((c=in.get()) == '/')in>>buf.lexema; vectorLexem.push_back(buf); break;
            case 'e':
                buf.lexemType = eq;
                if((c=in.get()) == '/')in>>buf.lexema; vectorLexem.push_back(buf); break;
            case 'k':
                buf.lexemType = keyword;
                if((c=in.get()) == '/')in>>buf.lexema; vectorLexem.push_back(buf); break;
            case 'r':
                buf.lexemType = rom;
                if((c=in.get()) == '/')in>>buf.lexema; vectorLexem.push_back(buf); break;
        }
    }
    in.close();

    fromLEXEMtoIDTABLE();
}
