#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <chrono>
#include <thread>
#include "reservation.h"
#include "resource.h"
#include "reservationfunctions.h"

using namespace std;
using namespace std::chrono_literals;


// Function to display all reservations in a list
void DisplayReservations(const list<Reservation>& r) {
    cout << endl << endl;
    cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
    cout << left << setw(15) << setfill(' ') << "|reservationId" << setw(11) << "|studentId"<< setw(20) << "|studentName" << setw(12) << "|resourceId" << setw(12) << "|date" << "|" << endl;
    cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
    for (const Reservation& r : r) {
        r.DisplayReservation();
    }
}

// Function to display all resources in a vector
void DisplayResources(const vector<Resource>& r) {
    cout << endl << endl;
    cout << "+" << right << setw(12) << setfill('-') << "+" << setw(17) << "+" << setw(22) << "+" << setw(15) << "+" << endl;
    cout << left << setw(12) << setfill(' ') << "|resourceId" << setw(17) << "|resourceName"<< setw(22) << "|resourceType" << setw(15) << "|isAvailable" << "|" << endl;
    cout << "+" << right << setw(12) << setfill('-') << "+" << setw(17) << "+" << setw(22) << "+" << setw(15) << "+" << endl;
    for (const Resource& r : r) {
        r.DisplayResource();
    }
}

void SearchReservations(const list<Reservation>& reservations, vector<IndexReservation>& found) {
    string query;
    int intval;
    cout << "Enter a reservation ID, student ID, student name, resource ID, or date to search for a reservation: ";
    getline(cin, query);
    stringstream ss(query);
    ss >> intval;
    if (intval > 0 && ss.eof()) {  
        FindReservation(reservations, found, intval);

    }
    else{
        FindReservation(reservations, found, query);
    }
    if (found[0].reservationId != 0) {
        DisplayFoundReservation(found);
    }

}

void SearchResources(const vector<Resource>& resources, vector<IndexResource>& found) {
    string query;
    cout << "Enter a resource ID, resource name, resource type, or availability(available/not available) to search for a resource.\n seperate multiple queries with a comma: ";
    cin.ignore();
    getline(cin, query);
    stringstream ss(query);
    vector<string> queries;
    string q;
    while (getline(ss >> ws, q, ',')) {
        queries.push_back(q);
    }
        FindResource(resources, found, queries);
    if (found[0].resourceId != "") {
        DisplayFoundResource(found);
    }
}

int main(){

    ifstream reservationIn("reservations.txt");
    ifstream resourceIn("resources.txt");

    if (!reservationIn.is_open()){
        cout << "Error: could not open \"reservations.txt\"" << endl;
        return 1;
    }
    if (!resourceIn.is_open()){
        cout << "Error: could not open \"resources.txt\"" << endl;
        return 1;
    }

    //create linked list of reservations
    list<Reservation> reservations;

    //get info from file and store in linked list
    while (reservationIn.eof() == false){
        string resourceId, stuName, date, reservationId, stuId;
        getline(reservationIn, reservationId, '|');
        getline(reservationIn, stuId, '|');
        getline(reservationIn, stuName, '|');
        getline(reservationIn, resourceId, '|');
        getline(reservationIn, date, '\n');
        if (reservationId.empty()){
            break;
        }
        Reservation r(stoi(reservationId), resourceId, stuName, stoi(stuId), date);
        reservations.push_back(r);
    }

    int resIdIndex = reservations.back().GetReservationId();

    //creat vector of resources
    vector<Resource> resources;

    //get info from file and store in vector
    while (resourceIn.eof() == false){
        string resourceId, resourceName, resourceType, isavailable;
        getline(resourceIn, resourceId, '|');
        getline(resourceIn, resourceName, '|');
        getline(resourceIn, resourceType, '|');
        getline(resourceIn, isavailable, '\n');
        if (resourceId.empty()){
            break;
        }
        Resource r(resourceId, resourceName, resourceType, (isavailable == "Available"));
        resources.push_back(r);
        
    }

    vector<ReservationQueue> reservationQueues;

    for (auto r : resources){
        vector<IndexReservation> unsortedQueue;
        FindReservation(reservations, unsortedQueue, r.GetId());
        ReservationQueue q(unsortedQueue, r);
        reservationQueues.push_back(q);
    }
   
    vector<IndexReservation> foundReservation;
    vector<IndexResource> foundResource;

    CancelationStack cancelations;

    //determines if a reservation has recently been deleted
    bool canceled = false;
    int canceledIndex;

    system("cls");

    cout << "  _____                                _   _                _____           _                 " << endl;
    cout << " |  __ \\                              | | (_)              / ____|         | |                " << endl;
    cout << " | |__) |___  ___  ___ _ ____   ____ _| |_ _  ___  _ __   | (___  _   _ ___| |_ ___ _ __ ___  " << endl;
    cout << " |  _  // _ \\/ __|/ _ \\ '__\\ \\ / / _` | __| |/ _ \\| '_ \\   \\___ \\| | | / __| __/ _ \\ '_ ` _ \\ " << endl;
    cout << " | | \\ \\  __/\\__ \\  __/ |   \\ V / (_| | |_| | (_) | | | |  ____) | |_| \\__ \\ ||  __/ | | | | |" << endl;
    cout << " |_|  \\_\\___||___/\\___|_|    \\_/ \\__,_|\\__|_|\\___/|_| |_| |_____/ \\__, |___/\\__\\___|_| |_| |_|" << endl;
    cout << " ==================================================================__/ |======================" << endl;
    cout << "                                                                  |___/                       " << endl;














    while (true){
        cout << "\n\nPlease enter the number corresponding to the action you would like to take." << endl;
        cout << "1. Display all reservations" << endl;
        cout << "2. Display all resources" << endl;
        cout << "3. Search for a reservation" << endl;
        cout << "4. Search for a resource" << endl;
        cout << "5. Add a reservation" << endl;
        cout << "6. Cancel a reservation" << endl;
        cout << "7. Check a reservation Queue" << endl;
        cout << "8. Exit the program" << endl;
        if (canceled){
            cout << "0. Undo reservation cancelation" << endl;
        }

        int input;
        cin >> input;
        switch (input) {
            case 1:
                DisplayReservations(reservations);
                break;
            case 2:
                DisplayResources(resources);
                break;
            case 3:
                SearchReservations(reservations, foundReservation);
                break;
            case 4:
                SearchResources(resources, foundResource);
                break;
            case 5:{
                cout << "Enter resource ID of the resource you want to reserve" << endl;
                string id;
                cin >> id;
                int queueIndex = GetQueueIndex(reservationQueues, id);
                if (queueIndex == -1) {
                    cout << "Error: Resource ID not found." << endl;
                    break;
                }
                Reservation added = reservationQueues[queueIndex].AddReservation(resIdIndex);
                reservations.push_back(added);
                break;
            }
            case 6:
                cancelations.Cancel(reservationQueues, reservations, canceledIndex);
                canceled = true;
                break;
            case 7:{
                cout << "Enter resource ID: ";
                string id;
                cin >> id;
                vector<IndexReservation> unsortedQueue;
                FindReservation(reservations, unsortedQueue, id);
                ReservationQueue q(unsortedQueue);
                q.DisplayQueue();

                break;
            }
            case 8:
                cout << "Exiting the program." << endl;
                return 0;
                break;
            case 0:
                if (canceled){
                    cancelations.Undo(reservationQueues, reservations, canceledIndex);
                    canceled = false;
                    break;
                }
            default:
                cout << "Invalid input. Please enter a number between 1 and 7." << endl;
    }
}



    return 0;
}