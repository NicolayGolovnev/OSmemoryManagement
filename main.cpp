#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <time.h>
#include <conio.h> // for getch()

#define CAPACITY 1000000

using namespace std;

class Process{
private:
    string name; // название процесса
    int memory; // сколько занимает места процесс в оперативной памяти
    int priority; // приоритет процесса
    int timeLife; // время жизни процесса
    clock_t lastAccess; // время последнего обращения к процессу
    
    int swapping = -1; //указывает, находится ли процесс в файле своппинга или нет
    pair<int, int> inMemory; // место расположения процесса в памяти
    
public:
    //Process(Name, Memory, Priority, Time Life)
    Process(string n, int mem, int pr, int time){
        this->name = n;
        this->memory = mem;
        this->priority = pr;
        this->timeLife = time;
        this->lastAccess = clock() / CLOCKS_PER_SEC;
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
    clock_t getLastAccess(){
        return this->lastAccess;
    }
    pair <int, int> getInMemory(){
        return this->inMemory;
    }
    int getSwapping(){
        if (this->swapping == -1) return -1;
        else return (this->swapping == 1 ? true : false);
    }

    void setTimeLife(int time){
        this->timeLife = time;
    }
    void setLastAccess(clock_t time){
        this->lastAccess = time;
    }
    void setSwapping(int swap){
        this->swapping = swap;
    }
    void setInMemory(int a, int b){
        this->inMemory = make_pair(a, b);
    }

    
};

//перегрузка операторов для сортировки вектора-массива
bool operator>(Process a, Process b){
    return (a.getPriority() > b.getPriority() ? true : false);
}
bool operator<(Process a, Process b){
    return (a.getPriority() < b.getPriority() ? true : false);
}

int MEMORY[CAPACITY]; // Наша оперативная память = 1.000.000 байт, хранится в 1.000 блоках каждый по 1.000 байт
vector <Process> query; // наша приоритетная очередь приложений
vector <Process> procInMemory; // процессы, которые хранятся в памяти и работают в данный момент

//если памяти недостаточно, то подается текущий процесс в файл своппинга и ожидает, когда освободиться память достаточно для того,
//чтобы выгрузить ее в память для дальнейшей работы с ним

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

    cout << "Process successfully created!\n";
    cout << "Press any symbol for continue...";
    getch();

    return 1;
}

void updMemory(clock_t time){
    vector <int> forDelete;

    for (int i = 0; i < query.size(); i++){
        if (query[i].getSwapping() == 0){
            query[i].setTimeLife(query[i].getTimeLife() - (time - query[i].getLastAccess()));
            query[i].setLastAccess(time);
        }
        if (query[i].getTimeLife() <= 0){
            for (int refresh = query[i].getInMemory().first; refresh <= query[i].getInMemory().second; refresh++)
                MEMORY[refresh] = 0;
            forDelete.push_back(i);
        }
    }
    //удаление процессов из вектора, так как они нам больше не нужны
    if (!forDelete.empty()){
        sort(forDelete.begin(), forDelete.end());
        for (int i = 0; i < forDelete.size(); i++){
            query.erase(query.begin() + forDelete[i]);
            for (int j = i; j < forDelete.size(); j++)
                forDelete[j]--;
        }
    }
    
}

int goInMemory(Process* exe){
    //смотрим, сколько нужно памяти для процесса
    int memory = exe->getMemory();
    int begin;
    for (int i = 0; i < CAPACITY; i++){
        int noSpace = 0;
        for (int j = i; j < i + memory; j++)
            if (MEMORY[j] == 1){
                noSpace = 1;
                break;
            }
        
        if (!noSpace){
            //cout << "POS: " << i << " - " << i + memory << endl;
            exe->setInMemory(i, i + memory);
            return 1;
        }
    }
    return 0;
}

void updProcesses(){
    sort(query.begin(), query.end());
    for (int i = 0; i < query.size(); i++){
        if (query[i].getSwapping() == -1){
            //пишем в память этот процесс
            int prior = query[i].getPriority();
            //cout << "1";
            int successfullyEntered = goInMemory(&query[i]);
            //cout << "POS: " << query[i].getInMemory().first << " - " << query[i].getInMemory().second << endl;
            //меняем параметр своппинга
            if (successfullyEntered){
                for (int j = query[i].getInMemory().first; j <= query[i].getInMemory().second; j++)
                    MEMORY[j] = 1;
                query[i].setSwapping(0);
            }
            else 
                query[i].setSwapping(1);
        }
        else if (query[i].getSwapping() == 1){
            int successfullyEntered = goInMemory(&query[i]);
            //cout << "POS: " << query[i].getInMemory().first << " - " << query[i].getInMemory().second << endl;
            //меняем параметр своппинга
            if (successfullyEntered){
                for (int j = query[i].getInMemory().first; j <= query[i].getInMemory().second; j++)
                    MEMORY[j] = 1;
                query[i].setSwapping(0);
            }
        }
    } 
}

bool updStatusMonitor(clock_t timeProgram){

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
            //updProcesses();
            system("cls");
            updMemory(myTime);
            updProcesses();
            
            cout << "MONITOR STATUS\t\tWORKING TIME PROGRAMM: " << myTime << "sec\n";
            cout << "\n\nRandom Access Memory:\n";
            cout << " ";
            for (int i = 0; i < 100; i++)
                cout << "-";
            cout << " \n|";
            char flag = 0;
            for (int i = 0; i < 100; i++){
                flag = 0;
                for (int j = 0; j < CAPACITY / 100; j++)
                    if (MEMORY[CAPACITY / 100 * i + j] == 1){
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
                cout << endl << i + 1 << ". " << query[i].getName() << " (Priority " << query[i].getPriority() << ") - ";
                cout << query[i].getMemory() << " byte, pos(" << query[i].getInMemory().first << ":" << query[i].getInMemory().second;
                cout << ") - " << "TIME LEFT: " << query[i].getTimeLife() << " - ";
                //сделать вывод мест хранения в памяти
                if (query[i].getSwapping() == 1)
                    cout << "IN SWAP FILE";
                else if (query[i].getSwapping() == 0)
                    cout << "EXECUTE IN MEMORY";
                else
                    cout << "JUST CREATED";
            }
            cout << "\n\n\nPress 'q' for exit from monitor status or another key for update...\n";
            oldTime = myTime;
        }
        exit = getch();
    }
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
                //cout << "EXECUTE 1\n";
                createProcess();
            break;

            case '2':
                //cout << "EXECUTE 2\n";
                updStatusMonitor(myTime);
            break;
            
            case '3':
                exit = 0;
                cout << "Exit from the programm, press any symbol...\n";
                getch();
            break;
        }
    }
}
