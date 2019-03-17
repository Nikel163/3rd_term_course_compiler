#include "lexem.hpp"
#include "synt.hpp"
#include "generation.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    string codeFile = "main.txt", lexemFile = "lexems.txt", syntFile = "syntax.txt";
    if(argc == 4)
    {
        codeFile = argv[1];
        lexemFile = argv[2];
        syntFile = argv[3];
    } else cout<<"Вызваны параметры по умолчанию\n\n";

    lexems(codeFile,lexemFile); //разбор лексем из codeFile, запись в lexemFile
    if(isLexem)
    {
        cout<<"Лексический анализ проведён успешно.\n";
        setLexemStruct(lexemFile);    //заполнение вектора лексем и генерация таблицы идентификаторов
        vector <int> spisok = getPravila(syntFile, vectorLexem);
        if(!spisok.empty()) //правила сформулированы в обратном порядке
        {
            cout<<"Синтаксический анализ проведён успешно.\n";
            correctRules(); //исправление на прямой порядок и восстановление переменных, которые заменились на Е при сдвиг-свёртки
            generateCode(); //генерация кода
            if(isGenerate)
            {
                cout<<"Генерация кода проведена успешно.\n";
            } else
            {
                cout<<"\nГенерация кода была провалена...\n";
            }
        } else {cout<<"Синтаксический анализ провалился...\n";}
    } else {cout<<"Лексический анализ провалился...\n";}
    cout<<endl;
    system("pause");
}
