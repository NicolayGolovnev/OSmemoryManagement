#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <time.h>
#include <conio.h> // for getch()

#define CAPACITY 1000

using namespace std;

class Process{
private:
    string name; // �������� ��������
    int memory; // ������� �������� ����� ������� � ����������� ������
    int priority; // ��������� ��������
    int timeLife; // ����� ����� ��������
    clock_t lastAccess; // ����� ���������� ��������� � ��������
    
    int swapping = -1; // ���������, ��������� �� ������� � ����� ��������� ��� ���
    pair<int, int> inMemory; // ����� ������������ �������� � ������
    
public:
    // Process(Name, Memory, Priority, Time Life)
    Process(string n, int mem, int priority, int time){
        this->name = n;
        this->memory = mem;
        this->priority = priority;
        this->timeLife = time;
        // ��������� ����� ������������� ��������, ��� ����������� ����������� ������� �������
        this->lastAccess = clock() / CLOCKS_PER_SEC;
    }

    // Process(Name, Memory, Priority, Time Life, Pos in Memory)
    // Process(name, prior, memory, timeLife, lastAccess, inMem1, inMem2)
    Process(string n, int priority, int mem, int timeLife, clock_t lastAccess, int inMem1, int inMem2){
        this->name = n;
        this->memory = mem;
        this->priority = priority;
        this->timeLife = timeLife;
        // ��������� ����� ������������� ��������, ��� ����������� ����������� ������� �������
        this->lastAccess = lastAccess;
        this->inMemory = make_pair(inMem1, inMem2);
    }

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

// ���������� ���������� ��� ���������� �������-�������
bool operator>(Process a, Process b){
    return (a.getPriority() > b.getPriority() ? true : false);
}
bool operator<(Process a, Process b){
    return (a.getPriority() < b.getPriority() ? true : false);
}

int MEMORY[CAPACITY]; // ���� ����������� ������
vector <Process> query; // ���� ������������ ������� ����������
vector <Process> procInMemory; // ��������, ������� �������� � ������ � �������� � ������ ������

// ���� ������ ������������, �� �������� ������� ������� � ���� ��������� � �������, ����� ������������ ������ ���������� ��� ����,
// ����� ��������� �� � ������ ��� ���������� ������ � ���

// ������� �������� ��������
// ���������� ���� ������ ���������� ��������
// ��� �������� ������ ������ ���������, ������� ����� ��������� �������� � ��������-�����
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

// ���������� ������� ����� �������� � ������ ������
void updMemory(clock_t time){
    vector <int> forDelete;

    for (int i = 0; i < query.size(); i++){
        if (query[i].getSwapping() == 0){
            query[i].setTimeLife(query[i].getTimeLife() - (time - query[i].getLastAccess()));
            query[i].setLastAccess(time);
        }
        // ������� ������
        if (query[i].getTimeLife() <= 0){
            for (int refresh = query[i].getInMemory().first; refresh <= query[i].getInMemory().second; refresh++)
                MEMORY[refresh] = 0;
            forDelete.push_back(i);
        }
    }
    //�������� ��������� �� �������, ��� ��� ��� ��� ������ �� �����
    if (!forDelete.empty()){
        sort(forDelete.begin(), forDelete.end());
        for (int i = 0; i < forDelete.size(); i++){
            query.erase(query.begin() + forDelete[i]);
            for (int j = i; j < forDelete.size(); j++)
                forDelete[j]--;
        }
    }
    
}

// �������, ������� ��������� ������, ������� �� �������
// � ������ ��� ���������� � ����-����
int goInMemory(Process* exe){
    // �������, ������� ����� ������ ��� ��������
    int memory = exe->getMemory();
    int begin;
    for (int i = 0; i < CAPACITY; i++){
        int noSpace = 0;
        for (int j = i; j < i + memory; j++)
        // ������
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

// ���������� ������ ���������
void updProcesses(){
    // ��������� �������� � ������� ����������� ���������� ����������
    sort(query.begin(), query.end());
    for (int i = 0; i < query.size(); i++){
        // ���� ������� ������ ������� � �� ��������� �� � ������, �� � ����-�����
        if (query[i].getSwapping() == -1){
            //cout << "1";
            // �������, ������ �� � ������ ������ �������� ������� ������, ������� �� ������
            int successfullyEntered = goInMemory(&query[i]);
            //cout << "POS: " << query[i].getInMemory().first << " - " << query[i].getInMemory().second << endl;
            // ���� �������, �� ��������� ������ � �������� ������� � ����������
            if (successfullyEntered){
                for (int j = query[i].getInMemory().first; j <= query[i].getInMemory().second; j++)
                    MEMORY[j] = 1;
                query[i].setSwapping(0);
            }
            // ����� ������� ������� � ����-����
            else 
                query[i].setSwapping(1);
        }
        // ���� ������� ��������� � ����-�����, �� ������� ����������� � ������
        else if (query[i].getSwapping() == 1){
            // �������, ������ �� � ������ ������ �������� ������� ������, ������� �� ������
            int successfullyEntered = goInMemory(&query[i]);
            //cout << "POS: " << query[i].getInMemory().first << " - " << query[i].getInMemory().second << endl;
            // ���� �������, �� ��������� ������ � �������� ������� � ����������
            if (successfullyEntered){
                for (int j = query[i].getInMemory().first; j <= query[i].getInMemory().second; j++)
                    MEMORY[j] = 1;
                query[i].setSwapping(0);
            }
        }
    } 
}

// ���������� ������������ ����������� ����������� ������
bool updStatusMonitor(clock_t timeProgram){

    //��� ������ � ������ � �������
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //15 - ���� ����, 0 - ���� ������
    //SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 0));

    //monitor status
    clock_t myTime = clock() / CLOCKS_PER_SEC, oldTime = myTime;
    char exit = 'a';
    cout << "Press key for update...\n";
    while (exit != 'q'){
        myTime = clock() / CLOCKS_PER_SEC;
        // ������ ���������� ������� ���������� ���������� ����������
        if (myTime - oldTime  >= 1){
            //test memory slots
            //MEMORY[rand() % 1000] = rand() % 2;
            //updProcesses();

            system("cls");
            updMemory(myTime);
            updProcesses();
            
            cout << "MONITOR STATUS\t\tWORKING TIME PROGRAMM: " << myTime << "sec\n";
            cout << "\n\nCAPACITY OF MEMORY - " << CAPACITY << " byte";
            // ��������� ����������� ������
            cout << "\nRandom Access Memory:\n";
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
            // ��������� ���� ��������� �� ����������
            cout << "\n\nCurrent opening processes:\n";
            for (int i = 0; i < query.size(); i++){
                cout << endl << i + 1 << ". " << query[i].getName() << " (Priority " << query[i].getPriority() << ") - ";
                cout << query[i].getMemory() << " byte, pos(" << query[i].getInMemory().first << ":" << query[i].getInMemory().second;
                cout << ") - " << "TIME LEFT: " << query[i].getTimeLife() << " - ";
                //������� ����� ���� �������� � ������
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

// �������� ��������� - ��������� ���� ����
int main(){
    srand(time(NULL));//��� ������������ ������
    clock_t myTime = clock() / CLOCKS_PER_SEC, oldTime = myTime;
    ifstream in("processes.txt");
    if (in){
        int n; in >> n;
        string name;
        int prior, memory, timeLife, swap, inMem1, inMem2;
        clock_t lastAccess;
        for (int i = 0; i < n; i++){
            in >> name >> prior >> memory >> timeLife >> lastAccess >> swap >> inMem1 >> inMem2;
            query.push_back(Process(name, prior, memory, timeLife, lastAccess, inMem1, inMem2));
        }
    }
    char exit = 1;
    while (exit){
        char choose;
        do{
            system("cls");
            cout << "\tOS Memory Management Laboratory Work, main menu:\n";
            cout << "\t\tChoose the right number!\n";
            cout << "1. Create process\n";
            cout << "2. Kill the process\n";
            cout << "\n3. Status monitor\n";
            cout << "5. Exit\n";
            choose = getch();
        }while (choose < '1' || choose > '5');
        switch (choose)
        {
            case '1':
                //cout << "EXECUTE 1\n";
                createProcess();
            break;

            case '3':
                //cout << "EXECUTE 2\n";
                updStatusMonitor(myTime);
            break;
            
            case '5':
                exit = 0;
                cout << "Exit from the programm, press any symbol...\n";
                ofstream out("processes.txt");
                out << query.size() << endl;
                for (int i = 0; i < query.size(); i++){
                    out << query[i].getName() << " " << query[i].getPriority() << " " << query[i].getMemory() << " ";
                    out << query[i].getTimeLife() << " " << (int)(query[i].getLastAccess()) << " " << query[i].getSwapping() << " ";
                    out << query[i].getInMemory().first << " " << query[i].getInMemory().second << endl;
                }
                out.close();
                getch();
            break;
        }
    }
}
