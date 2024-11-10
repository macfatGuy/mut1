#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;



int clientCount = 0;
int maxClients = 10;


mutex clientMutex;

condition_variable clientCV;


void clientThread() {
    while (true) {
       
        unique_lock<mutex> lock(clientMutex);
        clientCount++;

       
        cout << "������ �������� � �������. ����� ��������: " << clientCount << endl;

  
        if (clientCount >= maxClients) {
            cout << "������� �����������. ����� ������ ������������." << endl;
            clientCount--;
            lock.unlock();
            break;
        }
    
        clientCV.notify_one();
       
        this_thread::sleep_for(2000ms);
        lock.unlock();
    }
}

void operatorThread() {
    while (clientCount != 0) {
       
        unique_lock<mutex> lock(clientMutex);

        
        clientCV.wait(lock, [] { return clientCount > 0; });

    
        clientCount--;

      
        cout << "������ �������������. �������� ��������: " << clientCount << endl;


        if (clientCount == 0) {
            cout << "��� ������� �����������!" << endl;
            lock.unlock();
            break;
        }

     
        this_thread::sleep_for(1000ms);
        lock.unlock();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "������� ������������ ���������� ��������: ";
    cin >> maxClients;

    thread one(clientThread);
    thread two(operatorThread);

    one.join();
    two.join();

    return 0;
}
