#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <cmath>

using namespace std;


int LEVELS_COUNT = 5;
clock_t time1;
clock_t time2;
int SPEED = 50;
int PASENGER_WEIGHT = 70;
int ELEVATOR_MOVE_TIME = 3000;

class Elevator {
private:

    int maxWeight = 600;     // Maksymalny udźwig windy
    int passengers_count = 0; // Lista pasażerów w windzie
    vector<int> queue;
    int time = 0;
    int empty_time = 0;
    int currentFloor = 0;  // Aktualne piętro windy

public:
    Elevator(int maxWeight) : maxWeight(maxWeight){}

    void addToQueue() {
        int level;
        cin >> level;
        if ((level >= 0) && (level < LEVELS_COUNT)) {
            queue.push_back(level);
        }
    }

    void showElevator() {
        for (int i = LEVELS_COUNT - 1; i >= 0; i--) {
            string level = "| |";
            if(this->currentFloor == i){
                level = "|O| masa przewozonych pasazerow: " + std::to_string(get_current_weight()) + "kg";
            }
            cout << " - " << endl;
            cout << level << endl;
        }
        cout << " - " << endl;
    }

    void showQueue() {
        cout << "QUEUE: ";
        for (int i : queue)
            cout << i << ", ";
        cout << endl;
    }

    int get_current_weight() {
        return passengers_count * PASENGER_WEIGHT;
    }
    void enter_action() {
        cout << "Ile osob wysiada: ";
        int wysiada;
        cin >> wysiada;

        cout << "Ile osob wsiada: ";
        int wsiada;
        cin >> wsiada;
        passengers_count = passengers_count + wsiada - wysiada;
        if (passengers_count < 0)
            passengers_count = 0;
        else if (get_current_weight() > maxWeight) {
            int wait_passengers_count = std::ceil(double(get_current_weight() - maxWeight) / PASENGER_WEIGHT);
            cout << "Przepraszamy, ale " + std::to_string(wait_passengers_count) + " osoby musza poczekac, maksymalne obciazenie windy to " + std::to_string(maxWeight);
            passengers_count -= wait_passengers_count;
            Sleep(3000);
        }
    }
    void moveToFloor() {
        if (time % ELEVATOR_MOVE_TIME == 0 && queue.size()) {
            currentFloor = queue.front();
            queue.erase(queue.begin());
            empty_time = 0;
            //this->enter_action();
        }
        if (empty_time >= 5000 && currentFloor != 0) {
            currentFloor = 0;
            empty_time = 0;
            //this->enter_action();
        }
        if (!queue.size()) {
            empty_time += SPEED;
        }
        time += SPEED;

    }
};

void showMenu() {
    cout << "Instrukcja: " << endl;
    cout << "Przywolaj winde: 'a {numer_pietra}'" << endl;
    cout << "wprowadz pasazerow: 's {liczba_wysiadajacych} {liczba_wsiadajacych}'" << endl;
}

void idzdoxy(int x, int y)
{
    HANDLE hCon;
    COORD dwPos;

    dwPos.X = x;
    dwPos.Y = y;

    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, dwPos);
}



void wait(clock_t koniec_czekania, Elevator& elevator)
{
    do
    {
        if (_kbhit())
        {

            switch (_getch())
            {
                case 97: //a
                    elevator.addToQueue();
                    break;
                case 115: //s
                    elevator.enter_action();
                    break;
            }
            break;
        }
    } while (clock() < koniec_czekania);
}

int main()
{
    Elevator elevator(600);
    while (1) {

        time1 = clock();
        system("cls"); //mercedes
        idzdoxy(0, 0);
        showMenu();
        elevator.showElevator();
        elevator.showQueue();
        elevator.moveToFloor();

        time2 = clock();
        wait(clock() + SPEED - time2 + time1, elevator);
    }

    return 0;
}

