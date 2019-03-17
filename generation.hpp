// Для перевода в арабские числа
enum romSymb {I,V,X,L,C,D,M,Ha};
int value = 0;
bool isGenerate = true;
struct variable
{
    string lexem;
    string asmCode;
}; vector <variable> asmVars;

void sum(char symbol, romSymb state) //функция суммирования римских цифр
{
  switch (state)
  {
    case Ha: // начальное состояние автомата
        switch(symbol)
        {
            case 'I': state = I; value++; break;
            case 'V': state = V; value+=5; break;
            case 'X': state = X; value+=10; break;
            case 'L': state = L; value+=50; break;
            case 'C': state = C; value+=100; break;
            case 'D': state = D; value+=500; break;
            case 'M': state = M; value+=1000; break;
            default: break;
        }break;

    case I:
        switch(symbol)
        {
            case 'I': value++; break;
            case 'V': value+=3; break;
            case 'X': value+=8; break;
            default: break;
        }break;

    case V:
      if(symbol == 'I')value++;
      break;

    case X:
        switch(symbol)
        {
            case 'I': state = I; value++; break;
            case 'V': state = V; value+=5; break;
            case 'X': value+=10; break;
            case 'C': value+=80; break;
            case 'L': value+=30; break;
            default: break;
        }break;

    case L:
        switch(symbol)
        {
            case 'I': state = I; value++; break;
            case 'V': state = V; value+=5; break;
            case 'X': value+=10; break;
            default: break;
        }break;

    case C:
        switch(symbol)
        {
            case 'I': state = I; value++; break;
            case 'V': state = V; value+=5; break;
            case 'X': state = X; value+=10; break;
            case 'L': state = L; value+=50; break;
            case 'C': value+=100; break;
            case 'D': value+=300; break;
            case 'M': value+=800; break;
            default: break;
        }break;

    case D:
        switch(symbol)
        {
            case 'I': state = I; value++; break;
            case 'V': state = V; value+=5; break;
            case 'X': state = X; value+=10; break;
            case 'L': state = L; value+=50; break;
            case 'C': value+=100; break;

        }break;

    case M:
        switch(symbol)
        {
            case 'I': state = I; value++; break;
            case 'V': state = V; value+=5; break;
            case 'X': state = X; value+=10; break;
            case 'L': state = L; value+=50; break;
            case 'C': state = C; value+=100; break;
            case 'D': state = D; value+=500; break;
            case 'M': value+=1000; break;
            default: break;
        }break;
    }
}

int convertToArabic (string rom) //конвертация из римских в арабские
{
  value = 0;
  romSymb state = Ha;
  for (int i = 0; i<rom.length(); i++)
    sum(rom[i], state);
  return value;
}

void romToArab() //перевод римских чисел в арабские
{
    for(int i=0; i<forGen.size(); i++)
    {
        switch(forGen[i].rule)
        {
            case 2: //a:=E
                if(forGen[i].genLexem[2].lexemType == rom)
                {
                    int x = convertToArabic(forGen[i].genLexem[2].lexem);
                    forGen[i].genLexem[2].lexem = to_string(x);
                } break;

            case 8: //a<E
                if(forGen[i].genLexem[0].lexemType == rom)
                {
                    int x = convertToArabic(forGen[i].genLexem[0].lexem);
                    forGen[i].genLexem[0].lexem = to_string(x);
                }

                if(forGen[i].genLexem[2].lexemType == rom)
                {
                    int x = convertToArabic(forGen[i].genLexem[0].lexem);
                    forGen[i].genLexem[2].lexem = to_string(x);
                } break;

            case 9: //a>E
                if(forGen[i].genLexem[0].lexemType == rom)
                {
                    int x = convertToArabic(forGen[i].genLexem[0].lexem);
                    forGen[i].genLexem[0].lexem = to_string(x);
                }

                if(forGen[i].genLexem[2].lexemType == rom)
                {
                    int x = convertToArabic(forGen[i].genLexem[0].lexem);
                    forGen[i].genLexem[2].lexem = to_string(x);
                } break;
            case 10: //a=E
                if(forGen[i].genLexem[0].lexemType == rom)
                {
                    int x = convertToArabic(forGen[i].genLexem[0].lexem);
                    forGen[i].genLexem[0].lexem = to_string(x);
                }

                if(forGen[i].genLexem[2].lexemType == rom)
                {
                    int x = convertToArabic(forGen[i].genLexem[0].lexem);
                    forGen[i].genLexem[2].lexem = to_string(x);
                } break;

            case 11: //a
                if(forGen[i].genLexem[0].lexemType == rom)
                {
                    int x = convertToArabic(forGen[i].genLexem[0].lexem);
                    forGen[i].genLexem[0].lexem = to_string(x);
                } break;
        }

    }
}

void correctRules()  //вывод списка правил
{
    reverse(forGen.begin(), forGen.end());
    romToArab();
    for(int i=0; i<forGen.size(); i++)
    {
        if(i>=1 && forGen[i+1].rule == 11)
        {
            for(int j=0; j<forGen[i].genLexem.size(); j++)
            {
                if(forGen[i].genLexem[j].lexem == "E")
                {
                    forGen[i].genLexem[j].lexem = forGen[i+1].genLexem[0].lexem;
                    forGen[i].genLexem[j].lexemType = forGen[i+1].genLexem[0].lexemType;
                }
            }
        }
    }
}


bool checkSecondRule() //проверка на соответствие типов во 2 правиле
{
    for(int i=0; i<forGen.size(); i++)
    {
        if(forGen[i].rule == 2)
        {
            if(forGen[i].genLexem[0].lexemType != ID || (forGen[i].genLexem[2].lexemType != ID && forGen[i].genLexem[2].lexemType != rom))
            {
                cout<<"\nОшибка: в конструкции "<<forGen[i].genLexem[0].lexem<<":="<<forGen[i].genLexem[2].lexem
                    <<" нарушены типы операндов. \nОжидалось ID:=ID или ID:=rom\n"; isGenerate = false;
                return false;
            }
        }
    } return true;
}

void allocateID() //выделение памяти для переменных
{
    for(int i=1; i<=tableID.hashOrder.size(); i++)
    {
        variable temp;
        temp.lexem = tableID.hashTable[tableID.hashOrder[i-1]];
        temp.asmCode = "DWORD PTR [rbp-"+to_string(4*i)+"]";
        asmVars.push_back(temp);
    }
}

void mov(ofstream &assembTXT, ruleAndLexem forG) //операция присваивания
{
    assembTXT<<"mov\teax, ";
    for(int i=0; i<asmVars.size(); i++) //вывод первого операнда
        if(asmVars[i].lexem == forG.genLexem[0].lexem)
        {
            assembTXT<<asmVars[i].asmCode<<endl;
            break;
        }

    assembTXT<<"mov\t";
    if(forG.genLexem[2].lexemType == ID) //вывод второго операнда
    {
        for(int i=0; i<asmVars.size(); i++)
        if(asmVars[i].lexem == forG.genLexem[2].lexem)
        {
            assembTXT<<asmVars[i].asmCode<<", eax"<<endl;
            break;
        }
    } else assembTXT<<forG.genLexem[2].lexem<<", eax"<<endl;
}

void cmp(ofstream &assembTXT, ruleAndLexem forG, int rulNumb) //операция сравнения
{
    assembTXT<<"cmp\t";
    if(forG.genLexem[0].lexemType == ID) //вывод первого операнда
    {
        for(int i=0; i<asmVars.size(); i++)
        if(asmVars[i].lexem == forG.genLexem[0].lexem)
        {
            assembTXT<<asmVars[i].asmCode<<", ";
            break;
        }
    } else assembTXT<<forG.genLexem[0].lexem<<", ";

    if(forG.genLexem[2].lexemType == ID) //вывод второго операнда
    {
        for(int i=0; i<asmVars.size(); i++)
        if(asmVars[i].lexem == forG.genLexem[2].lexem)
        {
            assembTXT<<asmVars[i].asmCode<<endl;
            break;
        }
    } else assembTXT<<forG.genLexem[2].lexem<<endl;

    switch(rulNumb)
    {
        case 8: assembTXT<<"ja\t.exit:"<<endl; break; //<
        case 9: assembTXT<<"jl\t.exit:"<<endl; break; //>
        case 10: assembTXT<<"jne\t.exit:"<<endl; break; //=
        case 88: assembTXT<<"ja\t.for:"<<endl; break; //варианты правил в случае использования forEdoE
        case 99: assembTXT<<"jl\t.for:"<<endl; break;
        case 1010: assembTXT<<"jne\t.for:"<<endl; break;
    }
}

void error()
{
    cout<<"\nОшибка: логика программы нарушена.\n"; isGenerate = false;
}

void cycle(ofstream &assembTXT) //обработка цикла
{

    switch(forGen[5].rule)
    {
        case 4:
            mov(assembTXT,forGen[10]);
            assembTXT<<endl<<".for:"<<endl;
            switch(forGen[8].rule)
            {
                case 8: cmp(assembTXT, forGen[8], 8); break;
                case 9: cmp(assembTXT, forGen[8], 9); break;
                case 10: cmp(assembTXT, forGen[8], 10); break;
                default: error(); return;
            }

            if(forGen[6].rule == 2)
                mov(assembTXT, forGen[6]);
            else {error(); return;}

            switch(forGen[2].rule)
            {
                case 2: mov(assembTXT, forGen[2]); assembTXT<<"jmp\t.for:"<<endl; break;
                case 8: cmp(assembTXT, forGen[2], 88); break;
                case 9: cmp(assembTXT, forGen[2], 99); break;
                case 10: cmp(assembTXT, forGen[2], 1010); break;
                default: error(); return;
            } break;

        case 5:
            assembTXT<<endl<<".for:"<<endl;
            switch(forGen[8].rule)
            {
                case 8: cmp(assembTXT, forGen[8], 8); break;
                case 9: cmp(assembTXT, forGen[8], 9); break;
                case 10: cmp(assembTXT, forGen[8], 10); break;
                default: error(); return;
            }

            if(forGen[6].rule == 2)
                mov(assembTXT, forGen[6]);
            else {error(); return;}

            switch(forGen[2].rule)
            {
                case 2: mov(assembTXT, forGen[2]); assembTXT<<"jmp\t.for:"<<endl; break;
                case 8: cmp(assembTXT, forGen[2], 88); break;
                case 9: cmp(assembTXT, forGen[2], 99); break;
                case 10: cmp(assembTXT, forGen[2], 1010); break;
                default: error(); return;
            } break;

        case 6:
            assembTXT<<endl<<".for:"<<endl;
            switch(forGen[6].rule)
            {
                case 8: cmp(assembTXT, forGen[6], 8); break;
                case 9: cmp(assembTXT, forGen[6], 9); break;
                case 10: cmp(assembTXT, forGen[6], 10); break;
                default: error(); return;
            }

            if(forGen[8].rule == 2)
                mov(assembTXT, forGen[8]);
            else {error(); return;}

            switch(forGen[2].rule)
            {
                case 2: mov(assembTXT, forGen[2]); assembTXT<<"jmp\t.for:"<<endl; break;
                case 8: cmp(assembTXT, forGen[2], 88); break;
                case 9: cmp(assembTXT, forGen[2], 99); break;
                case 10: cmp(assembTXT, forGen[2], 1010); break;
                default: error(); return;
            } break;

        case 7:
            assembTXT<<endl<<".for:"<<endl;
            switch(forGen[6].rule)
            {
                case 8: cmp(assembTXT, forGen[6], 8); break;
                case 9: cmp(assembTXT, forGen[6], 9); break;
                case 10: cmp(assembTXT, forGen[6], 10); break;
                default: error(); return;
            }

            switch(forGen[2].rule)
            {
                case 2: mov(assembTXT, forGen[2]); assembTXT<<"jmp\t.for:"<<endl; break;
                case 8: cmp(assembTXT, forGen[2], 88); break;
                case 9: cmp(assembTXT, forGen[2], 99); break;
                case 10: cmp(assembTXT, forGen[2], 1010); break;
                default: error(); return;
            } break;

        default: error(); return;
    }
}

void generateCode() //генерация кода
{
    if(checkSecondRule()) //если нет ошибок в правилах №2
    {
        if(forGen[0].rule == 0)
        {
            ofstream assembTXT("assembler.txt");

            allocateID();

            assembTXT<<".main:"<<endl
                     <<"push\trbp"<<endl
                     <<"mov\trbp, rsp"<<endl;

            switch(forGen[1].rule)
            {
                case 2:
                    if(forGen[2].rule == 11)
                        mov(assembTXT,forGen[1]);
                    else {error(); return;}
                    break;
                case 8:
                    if(forGen[2].rule == 11)
                        cmp(assembTXT,forGen[1],8);
                    else {error(); return;}
                    break;
                case 9:
                    if(forGen[2].rule == 11)
                        cmp(assembTXT,forGen[1],9);
                    else {error(); return;}
                    break;
                case 10:
                    if(forGen[2].rule == 11)
                        cmp(assembTXT,forGen[1],10);
                    else {error(); return;}
                    break;
                case 1:
                    if(forGen[4].rule == 3)
                        cycle(assembTXT);
                    else {error(); return;}
                    break;
            }

            assembTXT<<"\n.exit:"<<endl
                     <<"mov\teax, 0"<<endl
                     <<"pop\trbp"<<endl
                     <<"ret";
            assembTXT.close();
        } else error();
    }
}
