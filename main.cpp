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
int PASSENGER_WEIGHT = 70;
int ELEVATOR_MOVE_TIME = 3000;

class Elevator {
private:
    int maxWeight = 600;     // Maximum weight the elevator can carry
    int passengers_count = 0; // Number of passengers in the elevator
    vector<int> queue;
    int time = 0;
    int empty_time = 0;
    int currentFloor = 0;  // Current floor of the elevator

public:
    Elevator(int maxWeight) : maxWeight(maxWeight) {}

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
                level = "|O| Passenger weight: " + std::to_string(get_current_weight()) + "kg";
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
        return passengers_count * PASSENGER_WEIGHT;
    }

    void enter_action() {
        cout << "How many passengers are getting off: ";
        int getting_off;
        cin >> getting_off;

        cout << "How many passengers are getting on: ";
        int getting_on;
        cin >> getting_on;
        passengers_count = passengers_count + getting_on - getting_off;
        if (passengers_count < 0)
            passengers_count = 0;
        else if (get_current_weight() > maxWeight) {
            int wait_passengers_count = std::ceil(double(get_current_weight() - maxWeight) / PASSENGER_WEIGHT);
            cout << "Sorry, but " + std::to_string(wait_passengers_count) + " passengers must wait, the elevator's weight limit is " + std::to_string(maxWeight);
            passengers_count -= wait_passengers_count;
            Sleep(3000);
        }
    }

    void moveToFloor() {
        if (time % ELEVATOR_MOVE_TIME == 0 && queue.size()) {
            currentFloor = queue.front();
            queue.erase(queue.begin());
            empty_time = 0;
        }
        if (empty_time >= 5000 && currentFloor != 0) {
            currentFloor = 0;
            empty_time = 0;
        }
        if (!queue.size()) {
            empty_time += SPEED;
        }
        time += SPEED;
    }
};

void showMenu() {
    cout << "Instructions: " << endl;
    cout << "Call the elevator: 'a {floor_number}'" << endl;
    cout << "Manage passengers: 's {number_getting_off} {number_getting_on}'" << endl;
}

void move_cursor_to(int x, int y)
{
    HANDLE hCon;
    COORD dwPos;

    dwPos.X = x;
    dwPos.Y = y;

    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, dwPos);
}

void wait(clock_t end_time, Elevator& elevator)
{
    do
    {
        if (_kbhit())
        {
            switch (_getch())
            {
                case 97: // 'a'
                    elevator.addToQueue();
                    break;
                case 115: // 's'
                    elevator.enter_action();
                    break;
            }
            break;
        }
    } while (clock() < end_time);
}

int main()
{
    Elevator elevator(600);
    while (1) {
        time1 = clock();
        system("cls"); // clear screen
        move_cursor_to(0, 0);
        showMenu();
        elevator.showElevator();
        elevator.showQueue();
        elevator.moveToFloor();

        time2 = clock();
        wait(clock() + SPEED - time2 + time1, elevator);
    }

    return 0;
}
