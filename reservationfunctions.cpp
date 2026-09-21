#include "reservation.h"
#include "reservationfunctions.h"
#include "resource.h"

#include <stack>
#include <queue>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <sstream>

using namespace std;


ReservationQueue::ReservationQueue(vector<IndexReservation> reservations){
    auto dateKey = [](const string& date) {
        return date.substr(6, 4) + date.substr(0, 2) + date.substr(3, 2);
    };

    while (!reservations.empty()) {
        auto earliest = reservations.begin();
        for (auto current = next(earliest); current != reservations.end(); ++current) {
            if (current->date.empty()) {
                continue;
            }
            if (earliest->date.empty()) {
                earliest = current;
                continue;
            }
            if (dateKey(current->date) < dateKey(earliest->date)) {
                earliest = current;
            }
        }

        if (earliest->date.empty()) {
            reservations.erase(earliest);
            continue;
        }

        q.push(Reservation(
            earliest->reservationId,
            earliest->resourceId,
            earliest->studentName,
            earliest->studentId,
            earliest->date
        ));
        reservations.erase(earliest);
    }
}

ReservationQueue::ReservationQueue(vector<IndexReservation> reservations, const Resource& resource)
    : ReservationQueue(reservations) {
    this->resource = resource;
}

void AddToQueue(){
    cout << "please enter the resource ID, resource name, or resource type that you would like to cancel: ";
    string input;
    getline(cin, input);

}

void ReservationQueue::DisplayQueue() const{
    cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
    cout << left << setw(15) << setfill(' ') << "|reservationId" << setw(11) << "|studentId"<< setw(20) << "|studentName" << setw(12) << "|resourceId" << setw(12) << "|date" << "|" << endl;
    cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
    queue<Reservation> temp;
    temp = q;
    if (temp.empty()) {
        cout << "No reservations found for this resource." << endl;
        return;
    }
    while (!temp.empty()) {
        Reservation r = temp.front();
        temp.pop();
        cout <<"|"  << left<<setw(14) << setfill(' ') << r.GetReservationId() << "|" << setw(10) << r.GetStudentId() << "|"<<setw(19) << r.GetStudentName() << "|" << setw(11) << r.GetResourceId() << "|" << setw(11) << r.GetDate() << "|" << endl;
        cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
    }
}

queue<Reservation> ReservationQueue::GetQueue() const{
return q;
}

bool ReservationQueue::RemoveReservation(int reservationId, Reservation& removed){
    queue<Reservation> remaining;
    bool found = false;

    while (!q.empty()) {
        Reservation current = q.front();
        q.pop();

        if (!found && current.GetReservationId() == reservationId) {
            removed = current;
            found = true;
        }
        else {
            remaining.push(current);
        }
    }

    q = remaining;
    return found;
}

Reservation ReservationQueue::AddReservation(int& resIdIndex){
    int reservationId;
    string resourceId;
    string studentName;
    int studentId;
    string date;
    cout << "Enter student name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, studentName);
    cout << "Enter student ID: ";
    cin >> studentId;
    resIdIndex++;
    reservationId = resIdIndex;
    resourceId = resource.GetId();

    date = "01/01/1970";
    queue<Reservation> current = q;
    while (!current.empty()) {
        date = current.front().GetDate();
        current.pop();
    }

    int month = stoi(date.substr(0, 2));
    int day = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leapYear = year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    if (month == 2 && leapYear) {
        daysInMonth[1] = 29;
    }
    if (++day > daysInMonth[month - 1]) {
        day = 1;
        if (++month > 12) {
            month = 1;
            ++year;
        }
    }

    ostringstream nextDate;
    nextDate << setfill('0') << setw(2) << month << '/'
             << setw(2) << day << '/' << setw(4) << year;
    date = nextDate.str();

    Reservation added(reservationId, resourceId, studentName, studentId, date);
    q.push(added);
    return added;
}

void ReservationQueue::RestoreReservation(const Reservation& reservation){
    vector<IndexReservation> restored;
    queue<Reservation> current = q;

    while (!current.empty()) {
        Reservation item = current.front();
        current.pop();
        restored.push_back(IndexReservation{
            item.GetReservationId(),
            item.GetResourceId(),
            item.GetStudentName(),
            item.GetStudentId(),
            item.GetDate()
        });
    }

    restored.push_back(IndexReservation{
        reservation.GetReservationId(),
        reservation.GetResourceId(),
        reservation.GetStudentName(),
        reservation.GetStudentId(),
        reservation.GetDate()
    });

    ReservationQueue rebuilt(restored);
    q = rebuilt.GetQueue();
}

Resource ReservationQueue::GetResource() const{
return resource;
}

int GetQueueIndex(const vector<ReservationQueue>& q, string id){
    for (size_t i = 0; i < q.size(); ++i) {
        const queue<Reservation> reservations = q[i].GetQueue();

        if (!reservations.empty() && reservations.front().GetResourceId() == id) {
            return static_cast<int>(i);
        }

        if (reservations.empty() && q[i].GetResource().GetId() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}


void CancelationStack::Cancel(vector<ReservationQueue>& queues, list<Reservation>& reservations, int& canceledIndex){
    string rId;
    cout << "Enter resource ID: ";
    cin >> rId;
    int i = GetQueueIndex(queues, rId);
    canceledIndex = i;
    if (i == -1) {
        cout << "Error: No queue found for this resource." << endl;
        return;
    }

    if (queues[i].GetQueue().empty()) {
        cout << "Error: No reservations for this resource." << endl;
        return;
    }

    queues[i].DisplayQueue();

    int resId;
    cout << "Enter reservation ID to cancel (or 0 to cancel): ";
    cin >> resId;

    if (resId == 0) {
        return;
    }

    Reservation removed;
    if (!queues[i].RemoveReservation(resId, removed)) {
        cout << "Error: Student ID was not found." << endl;
        return;
    }

    reservations.remove_if([&removed](const Reservation& reservation) {
        return reservation.GetReservationId() == removed.GetReservationId();
    });
    c.push(removed);
    cout << "Reservation canceled." << endl;
}

void CancelationStack::Undo(vector<ReservationQueue>& queues, list<Reservation>& reservations, int i){
    if (c.empty() || i < 0 || i >= static_cast<int>(queues.size())) {
        cout << "Error: Nothing to undo." << endl;
        return;
    }

    Reservation restored = c.top();
    queues[i].RestoreReservation(restored);
    reservations.push_back(restored);

    c.pop();
}
