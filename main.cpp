#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

class Process{
private:
    string name = ""; // название процесса
    int timeLife = -1; // время жизни процесса
    vector <pair<int, int> > inMemory; // места расположения процесса в памяти
    int priority = -1; // приоритет процесса
    int memory = 0; // сколько занимает места процесс в оперативной памяти
    char swapping = 0; //указывает, находится ли процесс в файле своппинга или нет
    
public:
    //Process(Name, Memory, Priority, Time Life)
    Process(string n, int mem, int pr, int time){
        name = n;
        memory = mem;
        priority = pr;
        timeLife = time;
    };

    string getName(){
        return name;
    }

    bool getSwapping(){
        return (swapping == 1 ? true : false);
    }

    int getMemory(){
        return memory;
    }
};

int MEMORY[1000]; // Наша оперативная память = 1.000.000 байт, хранится в 1.000 блоках каждый по 1.000 байт
vector <Process> query; // наша приоритетная очередь приложений
vector <Process> procInMemory; // процессы, которые хранятся в памяти и работают в данный момент

//если памяти недостаточно, то подается текущий процесс в файл своппинга и ожидает, когда освободиться память достаточно для того,
//чтобы выгрузить ее в память для дальнейшей работы с ним

void updateStatusMonitor(){

    //для работы с цветом в консоли
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //15 - цвет фона, 0 - цвет текста
    //SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 0));

    //test memory slots
    MEMORY[50] = 1;
    MEMORY[0] = 1;
    MEMORY[100] = 1;
    MEMORY[350] = 1;

    //monitor status
    system("cls");
    cout << "MONITOR STATUS\n";

    cout << "\n\nRandom Access Memory:\n";
    cout << " ";
    for (int i = 0; i < 100; i++)
        cout << "-";
    cout << " \n|";
    char flag = 0;
    for (int i = 0; i < 100; i++){
        flag = 0;
        for (int j = 0; j < 10; j++)
            if (MEMORY[10 * i + j] == 1){
                SetConsoleTextAttribute(hConsole, (WORD) ((14 << 4) | 8));
                cout << " "; 
                flag = 1;
                break;
            }
        if (!flag){
            SetConsoleTextAttribute(hConsole, (WORD) ((8 << 4) | 14));
            cout << " "; 
        }
    }
    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 15));
    cout << "|\n";
    cout << " ";
    for (int i = 0; i < 100; i++)
        cout << "-";
    cout << " \n";
    
        

    cout << "\n\nCurrent opening processes:\n";
    for (int i = 0; i < query.size(); i++){
        cout << i + 1 << ". " << query[i].getName() << " - " << query[i].getMemory() << " - ";
        //сделать вывод мест хранения в памяти
        if (query[i].getSwapping())
            cout << "IN SWAP FILE\n";
        else
            cout << "EXECUTABLE IN MEMORY\n";
    }
    cout << "\n";
}

int main(){

    updateStatusMonitor();

    system("pause");
}