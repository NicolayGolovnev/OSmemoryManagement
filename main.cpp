#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Process{
private:
    int timeLife = -1; // время жизни процесса
    vector <pair<int, int> > inMemory; // места расположения процесса в памяти
    int priority = -1; // приоритет процесса
    int memory = 0; // сколько занимает места процесс в оперативной памяти
    

}

int MEMORY[10000000]; // Наша оперативная память = 1.000.000 байт
vector <Process> query; // наша приоритетная очередь приложений

int main(){

}