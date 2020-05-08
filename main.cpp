#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

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

void updateStatusMonitor(){

    //��� ������ � ������ � �������
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //15 - ���� ����, 0 - ���� ������
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
        //������� ����� ���� �������� � ������
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