#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <time.h>
#include <conio.h> // for getch()

using namespace std;

class Process{
private:
    string name = ""; // название процесса
    int timeLife = -1; // время жизни процесса
    int priority = -1; // приоритет процесса
    int memory = 0; // сколько занимает места процесс в оперативной памяти
    clock_t timeCreate; // время создания процесса
    
    char swapping = 0; //указывает, находится ли процесс в файле своппинга или нет
    pair<int, int> inMemory; // место расположения процесса в памяти
    
public:
    //Process(Name, Memory, Priority, Time Life)
    Process(string n, int mem, int pr, int time){
        this->name = n;
        this->memory = mem;
        this->priority = pr;
        this->timeLife = time;
    };

    string getName(){
        return this->name;
    }
    int getMemory(){
        return this->memory;
    }
    int getTimeLife(){
        return this->timeLife;
    }
    int getPriority(){
        return this->priority;
    }

    void setTimeLife(int time){
        this->timeLife = time;
    }

    
    bool getSwapping(){
        return (this->swapping == 1 ? true : false);
    }
};

int MEMORY[1000]; // Наша оперативная память = 1.000.000 байт, хранится в 1.000 блоках каждый по 1.000 байт
vector <Process> query; // наша приоритетная очередь приложений
vector <Process> procInMemory; // процессы, которые хранятся в памяти и работают в данный момент

//если памяти недостаточно, то подается текущий процесс в файл своппинга и ожидает, когда освободиться память достаточно для того,
//чтобы выгрузить ее в память для дальнейшей работы с ним

bool updateStatusMonitor(clock_t timeProgramm){

    //для работы с цветом в консоли
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //15 - цвет фона, 0 - цвет текста
    //SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 0));

    //monitor status
    clock_t myTime = clock() / CLOCKS_PER_SEC, oldTime = myTime;
    char exit = 'a';
    cout << "Press key for update...\n";
    while (exit != 'q'){
        myTime = clock() / CLOCKS_PER_SEC;

        if (myTime - oldTime  >= 1){
            //test memory slots
            //MEMORY[rand() % 1000] = rand() % 2;

            system("cls");
            cout << "MONITOR STATUS\t\tWORKING TIME PROGRAMM: " << myTime << "sec\n";
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
                cout << i + 1 << ". " << query[i].getName() << " (Priority " << query[i].getPriority() << ") - ";
                cout << query[i].getMemory() << " byte - ";
                cout << "TIME LEFT: " << query[i].getTimeLife() << " - ";
                //сделать вывод мест хранения в памяти
                if (query[i].getSwapping())
                    cout << "IN SWAP FILE\n";
                else
                    cout << "EXECUTE IN MEMORY\n";
            }
            cout << "\nPress 'q' for exit from monitor status or another key for update...\n";
            oldTime = myTime;
        }
        exit = getch();
    }
    return 1;
}

bool createProcess(){
    system("cls");
    cout << "Enter a name of process: ";
    string name;
    cin >> name;
    cout << "Enter a priority of process (0 - 5): ";
    int prior; cin >> prior;
    cout << "How long the process will be able (in sec)? ";
    int timeLife; cin >> timeLife;
    cout << "How much memory is needed? ";
    int mem; cin >> mem;

    Process q(name, mem, prior, timeLife);
    query.push_back(q);

    return 1;
}

int main(){
    srand(time(NULL));//для тестирование памяти
    clock_t myTime = clock() / CLOCKS_PER_SEC, oldTime = myTime;
    char exit = 1;
    while (exit){
        char choose;
        do{
            system("cls");
            cout << "\tOS Memory Management Laboratory Work, main menu:\n";
            cout << "\t\tChoose the right number!\n";
            cout << "1. Create process\n";
            cout << "2. Status monitor\n";
            cout << "3. Exit\n";
            choose = getch();
        }while (choose < '1' || choose > '3');
        switch (choose)
        {
            case '1':
                cout << "EXECUTE 1\n";
                createProcess();
            break;

            case '2':
                cout << "EXECUTE 2\n";
                updateStatusMonitor(myTime);
            break;
            
            case '3':
                exit = 0;
                cout << "Exit from the programm, press any symbol...\n";
                getch();
            break;
        }
    }
}
