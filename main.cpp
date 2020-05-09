#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <time.h>
#include <conio.h> // for getch()

using namespace std;

class Process{
private:
    string name = ""; // �������� ��������
    int timeLife = -1; // ����� ����� ��������
    int priority = -1; // ��������� ��������
    int memory = 0; // ������� �������� ����� ������� � ����������� ������
    clock_t lastAccess; // ����� ���������� ��������� � ��������
    
    int swapping = -1; //���������, ��������� �� ������� � ����� ��������� ��� ���
    pair<int, int> inMemory; // ����� ������������ �������� � ������
    
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

    void setTimeLife(int time){
        this->timeLife = time;
    }
    void setLastAccess(clock_t time){
        this->lastAccess = time;
    }

    
    int getSwapping(){
        if (this->swapping == -1) return -1;
        else return (this->swapping == 1 ? true : false);
    }
};

int MEMORY[1000]; // ���� ����������� ������ = 1.000.000 ����, �������� � 1.000 ������ ������ �� 1.000 ����
vector <Process> query; // ���� ������������ ������� ����������
vector <Process> procInMemory; // ��������, ������� �������� � ������ � �������� � ������ ������

//���� ������ ������������, �� �������� ������� ������� � ���� ��������� � �������, ����� ������������ ������ ���������� ��� ����,
//����� ��������� �� � ������ ��� ���������� ������ � ���

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

    cout << "Process successfully created!";
    cout << "Press any symbol for continue...";
    getch();

    return 1;
}

void updMemory(clock_t time){
    vector <int> forDelete;

    for (int i = 0; i < query.size(); i++){
        query[i].setTimeLife(query[i].getTimeLife() - (time - query[i].getLastAccess()));
        query[i].setLastAccess(time);
        if (query[i].getTimeLife() <= 0){
            for (int refresh = query[i].getInMemory().first; refresh <= query[i].getInMemory().second; refresh++)
                MEMORY[refresh] = 0;
            forDelete.push_back(i);
        }
    }
    //�������� ��������� �� �������, ��� ��� ��� ��� ������ �� �����
    sort(forDelete.begin(), forDelete.end());
    for (int i = 0; i < forDelete.size(); i++){
        query.erase(query.begin() + forDelete[i]);
        for (int j = i; j < forDelete.size(); j++)
            forDelete[j]--;
    }
}

void updProcesses(){
    
}

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

        if (myTime - oldTime  >= 1){
            //test memory slots
            //MEMORY[rand() % 1000] = rand() % 2;
            //updProcesses();
            updMemory(myTime);

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
                //������� ����� ���� �������� � ������
                if (query[i].getSwapping() == 1)
                    cout << "IN SWAP FILE\n";
                else if (query[i].getSwapping() == 0)
                    cout << "EXECUTE IN MEMORY\n";
            }
            cout << "\nPress 'q' for exit from monitor status or another key for update...\n";
            oldTime = myTime;
        }
        exit = getch();
    }
    return 1;
}

int main(){
    srand(time(NULL));//��� ������������ ������
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
