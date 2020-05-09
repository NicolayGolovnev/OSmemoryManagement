#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <time.h>
#include <conio.h> // for getch()

using namespace std;

class Process{
private:
    string name = ""; // �������� ��������
    int timeLife = -1; // ����� ����� ��������
    vector <pair<int, int> > inMemory; // ����� ������������ �������� � ������
    int priority = -1; // ��������� ��������
    int memory = 0; // ������� �������� ����� ������� � ����������� ������
    char swapping = 0; //���������, ��������� �� ������� � ����� ��������� ��� ���
    
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

int MEMORY[1000]; // ���� ����������� ������ = 1.000.000 ����, �������� � 1.000 ������ ������ �� 1.000 ����
vector <Process> query; // ���� ������������ ������� ����������
vector <Process> procInMemory; // ��������, ������� �������� � ������ � �������� � ������ ������

//���� ������ ������������, �� �������� ������� ������� � ���� ��������� � �������, ����� ������������ ������ ���������� ��� ����,
//����� ��������� �� � ������ ��� ���������� ������ � ���

bool updateStatusMonitor(clock_t timeProgramm){

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
                cout << i + 1 << ". " << query[i].getName() << " - " << query[i].getMemory() << " - ";
                //������� ����� ���� �������� � ������
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
                cout << "EXECUTE 1\n";
                //createProcess();
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
