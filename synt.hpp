#include <bits/stdc++.h>
#include <algorithm>
#include <array>
using namespace std;

//конфигурация состояния МП-автомата в виде {a|b|y}, где
//a - непрочитанная часть входной цепочки
//b - содержимое стека МП-автомата
//y - последовательность применённых правил
//в данной программе это:
// a - innerStr (27 строка)
// b - Stack (28 строка)
// y - pravila (29 строка)

struct ChainLexem
{
    string lexem;
    types lexemType;
};

struct ruleAndLexem
{
    int rule;
    vector <ChainLexem> genLexem;
}; vector <ruleAndLexem> forGen;

vector <ChainLexem> innerStr; //входная цепочка
vector <ChainLexem> Stack;    //стек
vector <int> pravila;         //последовательность правил
ChainLexem curLexema;         //текущая считываемая лексема входной цепочки

char matrix [11][11]={ // матрица операторного предшествования
    	  /*for  do    a   :=    <    >    =    (    )    ;    K*/
    /*for*/{' ', '=', ' ', ' ', ' ', ' ', ' ', '<', ' ', ' ', ' '},
    /*do*/ {'<', ' ', '<', ' ', ' ', ' ', ' ', ' ', '>', '>', ' '},
    /*a*/  {' ', ' ', ' ', '=', '=', '=', '=', ' ', '>', '>', ' '},
    /*:=*/ {' ', ' ', '<', ' ', ' ', ' ', ' ', ' ', '>', '>', ' '},
    /*<*/  {' ', ' ', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', ' '},
    /*>*/  {' ', ' ', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', ' '},
    /*=*/  {' ', ' ', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', ' '},
    /*(*/  {'<', ' ', '<', ' ', ' ', ' ', ' ', ' ', '=', '<', ' '},
    /*)*/  {' ', '>', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    /*;*/  {'<', ' ', '<', ' ', ' ', ' ', ' ', ' ', '>', '=', '>'},
    /*Н*/  {'<', ' ', '<', ' ', ' ', ' ', ' ', ' ', ' ', '<', ' '}
	};

array <string, 10> symbols = {"for", "do", "a", ":=", "<", ">", "=", "(", ")", ";"}; //список терминальных символов

array <string, 12> rules =
{
    "E;",       //правило 0
    "forEdoE",  //правило 1
    "a:=E",     //правило 2
    "(E)",      //правило 3
    "E;E;E",    //правило 4
    ";E;E",     //правило 5
    "E;E;",     //правило 6
    ";E;",      //правило 7
    "a<E",      //правило 8
    "a>E",      //правило 9
    "a=E",      //правило 10
    "a"         //правило 11
};

int getIndex(ChainLexem noIndex) //получение индекса для поиска в матрице предшествования
{
    if(noIndex.lexem == "К" || noIndex.lexem == "Н")return symbols.size(); //возвращает последний строку/столбец в матрице
    if(noIndex.lexemType == ID || noIndex.lexemType == rom)return 2; //возвращает номер терминала "а"
    for(int i=0; i<10; i++) //10 возможных символов
        if(noIndex.lexem == symbols[i])return i; //возвращает строку/столбец
}

int getID(string ID)
{
    for(int i=0; i<tableID.hashOrder.size(); i++)
        if(tableID.hashTable[tableID.hashOrder[i]] == ID)return tableID.hashOrder[i];
    return -1;
}

void getinnerStr(vector <LEXEM> vectorLexem) //формирование входной цепочки
{
    ChainLexem buf;
    for(int i=0; i<vectorLexem.size(); i++)
    {
        buf.lexem = vectorLexem[i].lexema;
        buf.lexemType = vectorLexem[i].lexemType;
        innerStr.push_back(buf);
    }
}

char getStepChar() //вовзращает символ для дальнейшего шага синтаксического разбора
{
    int str; //номер строки в матрице предшествования
    if(Stack.back().lexem == "E") //если в стеке нетерминальный символ, берётся терминальный символ
        str = getIndex(Stack[Stack.size()-2]);
    else str = getIndex(Stack.back());

    int col = getIndex(curLexema); //номер столбца в матрице предшествования

    switch(matrix[str][col])
    {
        case '<': case '=': return 'П';  //перенос
        case '>': return 'С';            //свёртка
        default: return 'О';             //ошибка
    }

}

void stepOut(char step, ofstream& out) //вывод конфигурации состояния МП-автомата в виде {a|b|y} (см. шапку файла)
{
    out<<"{";
    for (int i = 0; i < innerStr.size(); i++)
        out<<innerStr[i].lexem<<" "; //вывод содержимого a

    out<<" | ";
    for (int i = 0; i < Stack.size(); i++)
        out<<Stack[i].lexem<<" "; //вывод содержимого b

    out<<" | ";
    for (int i = 0; i < pravila.size(); i++)
        out<<pravila[i]<<" "; //вывод содержимого y

    switch(step)
    {
        case 'П': out<<"} (перенос)\n"; break;
        case 'С': out<<"} (свертка)\n"; break;
        case 'К': out<<"} (конец)\n"; break;
    }
}

int checkRule(vector <ChainLexem> pravilo) //проверка на существование правила
{
    ChainLexem buf, genTemp; buf.lexem = ""; ruleAndLexem temp;
    for(int i=0; i<pravilo.size(); i++)
        if(pravilo[i].lexemType == ID || pravilo[i].lexemType == rom)
        {
            genTemp.lexem = pravilo[i].lexem;
            genTemp.lexemType = pravilo[i].lexemType;
            temp.genLexem.push_back(genTemp);
            buf.lexem += "a";
        }
        else
        {
            genTemp.lexem = pravilo[i].lexem;
            genTemp.lexemType = pravilo[i].lexemType;
            temp.genLexem.push_back(genTemp);
            buf.lexem += pravilo[i].lexem;       //накопление строки для сверки с правилом
        }


    for(int i=0; i<12; i++)//12 возможных правил
        if(buf.lexem == rules[i])
            {
                temp.rule = i; forGen.push_back(temp);
                return i; //возвращает номер правила
            }

    return -1; //или возвращает -1, если нет подходящего правила
}

bool svertka()
{
    vector <ChainLexem> currule;         //выделение памяти под текущее правило
    currule.push_back(Stack.back()); //занесение вершины в вектор
    Stack.pop_back();                //удаление вершины стека

    ChainLexem term; //терминальный символ
    if(currule.back().lexem == "E") term.lexem = ""; //если встречается нетерминальный символ, то пропустить
    else term.lexem = currule.back().lexem; //иначе записать терминальный символ

    bool isSearching = true;
    while(isSearching && !Stack.empty()) //формирование правила
    {
        if(Stack.back().lexem == "E")
        {
            currule.push_back(Stack.back()); Stack.pop_back();
        }
        else
        {
            if(term.lexem == "")//если не найден терминальный символ
            {
                term = Stack.back();
                currule.push_back(Stack.back());
                Stack.pop_back();
            }
            else //терминальный символ найден, сравнивается со следующим по матрице предшествования
            {
                int str = getIndex(Stack.back());  //текущий терминал
                int col = getIndex(term);          //терминал из стека

                if(matrix[str][col] == '=')
                {
                    term = Stack.back();
                    currule.push_back(Stack.back());
                    Stack.pop_back();
                } else isSearching = false;
            }
        }
    } //правило составлено в обратном порядке
    reverse(currule.begin(), currule.end()); //восстанавление прямого порядка

    //проверка правила на принадлежность к языку
    int flag = checkRule(currule);
    if(flag != -1)
    {
        ChainLexem temp;
        temp.lexem = "E"; temp.lexemType = nonTerm;
        Stack.push_back(temp);

        pravila.push_back(flag);
        return true;
    } else
    {
        cout<<"ОШИБКА: такого правила не существует: ";
        for(int i=0; i<currule.size(); i++)
            cout<<currule[i].lexem<<" ";
        cout<<endl<<endl; return false;
    }
}

bool razbor(string outFile)
{
    ofstream out(outFile.c_str(), ios_base::ate|ios_base::out);

    if(out)
    {
        ChainLexem temp;
        temp.lexem = "К"; temp.lexemType = znak;
        innerStr.push_back(temp);
        while(1)
        {
            curLexema = innerStr[0];
            switch(getStepChar())
            {
                case 'П': //перенос
                    stepOut('П', out); //вывод в файл
                    Stack.push_back(curLexema); //занесение лексемы в стек
                    innerStr.erase(innerStr.begin()); //удаление лексемы из входной цепочки
                    break;
                case 'С': //свёртка
                    stepOut('С', out); //вывод в файл
                    if(!svertka()){out.close(); return false;} //прекратить работу, если нет правила
                    break;
                case 'О': //обработка ошибки
                    cout<<"ОШИБКА: между "<<curLexema.lexem<<" и "<<Stack.back().lexem
                        <<" нет отношения предшествования.\n";
                    out.close(); return false;
            }
            if(curLexema.lexem == "К")break; //входная цепочка закончилась
        }
        stepOut('К', out);
    } //сформирован вектор с обратным порядком применённых правил
    else {cout<<"\nОшибка: файл "<<outFile<<" не открыт."; return false;}

    reverse(pravila.begin(), pravila.end()); //восстановление прямого порядка

    out.close(); return true;
}

vector <int> getPravila(string outFile, vector <LEXEM> vectorLexem)
{
    ChainLexem temp;
    temp.lexem = "Н"; temp.lexemType = znak;
    Stack.push_back(temp);

    getinnerStr(vectorLexem);
    if(razbor(outFile))return pravila;
    else return {};
}
