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
#include <cctype>

using namespace std;


namespace {

string reservationDateKey(const string& date) {
    return date.substr(6, 4) + date.substr(0, 2) + date.substr(3, 2);
}

bool reservationComesBefore(const IndexReservation& left, const IndexReservation& right) {
    if (left.date.empty()) {
        return false;
    }
    if (right.date.empty()) {
        return true;
    }
    return reservationDateKey(left.date) < reservationDateKey(right.date);
}

void mergeSortReservations(vector<IndexReservation>& reservations,
                           vector<IndexReservation>& scratch,
                           size_t begin,
                           size_t end) {
    if (end - begin < 2) {
        return;
    }

    const size_t middle = begin + (end - begin) / 2;
    mergeSortReservations(reservations, scratch, begin, middle);
    mergeSortReservations(reservations, scratch, middle, end);

    size_t left = begin;
    size_t right = middle;
    size_t output = begin;
    while (left < middle && right < end) {
        if (reservationComesBefore(reservations[right], reservations[left])) {
            scratch[output++] = reservations[right++];
        } else {
            scratch[output++] = reservations[left++];
        }
    }
    while (left < middle) {
        scratch[output++] = reservations[left++];
    }
    while (right < end) {
        scratch[output++] = reservations[right++];
    }
    for (size_t i = begin; i < end; ++i) {
        reservations[i] = scratch[i];
    }
}

void printBorder() {
    cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
}

void printHeader() {
    printBorder();
    cout << left << setw(15) << setfill(' ') << "|reservationId" << setw(11) << "|studentId" << setw(20) << "|studentName" << setw(12) << "|resourceId" << setw(12) << "|date" << "|" << endl;
    printBorder();
}

void printRow(const Reservation& r) {
    cout << "|" << left << setw(14) << setfill(' ') << r.GetReservationId()
         << "|" << setw(10) << r.GetStudentId()
         << "|" << setw(19) << r.GetStudentName()
         << "|" << setw(11) << r.GetResourceId()
         << "|" << setw(11) << r.GetDate() << "|" << endl;
    printBorder();
}

IndexReservation toIndex(const Reservation& r) {
    return IndexReservation{
        r.GetReservationId(),
        r.GetResourceId(),
        r.GetStudentName(),
        r.GetStudentId(),
        r.GetDate()
    };
}

// reads an int, re-prompting on bad input
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number, try again." << endl;
    }
}

}


// =====================================================================
// WaitingList
// =====================================================================

void WaitingList::Add(const Reservation& r) {
    byDate[r.GetDate()].push_back(r);   // creates this date's list if it doesn't exist yet
}

bool WaitingList::PopNext(const string& date, Reservation& next) {
    auto it = byDate.find(date);
    if (it == byDate.end() || it->second.empty()) {
        return false;
    }
    next = it->second.front();
    it->second.pop_front();
    if (it->second.empty()) {
        byDate.erase(it);
    }
    return true;
}

void WaitingList::PushFront(const Reservation& r) {
    byDate[r.GetDate()].push_front(r);
}

void WaitingList::InsertAt(const Reservation& r, size_t pos) {
    deque<Reservation>& line = byDate[r.GetDate()];
    if (pos > line.size()) {
        pos = line.size();
    }
    line.insert(line.begin() + pos, r);
}

bool WaitingList::Remove(int reservationId, Reservation& removed, size_t& pos) {
    for (auto it = byDate.begin(); it != byDate.end(); ++it) {
        deque<Reservation>& line = it->second;
        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i].GetReservationId() == reservationId) {
                removed = line[i];
                pos = i;
                line.erase(line.begin() + i);
                if (line.empty()) {
                    byDate.erase(it);
                }
                return true;
            }
        }
    }
    return false;
}

int WaitingList::Count() const {
    int total = 0;
    for (const auto& entry : byDate) {
        total += static_cast<int>(entry.second.size());
    }
    return total;
}

vector<Reservation> WaitingList::All() const {
    vector<Reservation> out;
    for (const auto& entry : byDate) {
        for (const Reservation& r : entry.second) {
            out.push_back(r);
        }
    }
    return out;
}


// =====================================================================
// ReservationQueue
// =====================================================================

ReservationQueue::ReservationQueue(vector<IndexReservation> reservations){
    vector<IndexReservation> scratch(reservations.size());
    mergeSortReservations(reservations, scratch, 0, reservations.size());

    for (const IndexReservation& reservation : reservations) {
        if (reservation.date.empty()) {
            continue;
        }
        q.push(Reservation(
            reservation.reservationId,
            reservation.resourceId,
            reservation.studentName,
            reservation.studentId,
            reservation.date
        ));
    }
}

ReservationQueue::ReservationQueue(vector<IndexReservation> reservations, const Resource& resource)
    : ReservationQueue(reservations) {
    this->resource = resource;
}

void ReservationQueue::DisplayQueue() const{
    queue<Reservation> temp = q;
    if (temp.empty()) {
        cout << "No reservations found for this resource." << endl;
        return;
    }
    printHeader();
    while (!temp.empty()) {
        printRow(temp.front());
        temp.pop();
    }
}

void ReservationQueue::DisplayWaiting() const{
    vector<Reservation> all = waiting.All();
    if (all.empty()) {
        return;
    }
    cout << endl << "Waiting list:" << endl;
    printHeader();
    for (const Reservation& r : all) {
        printRow(r);
    }
}

queue<Reservation> ReservationQueue::GetQueue() const{
    return q;
}

Resource ReservationQueue::GetResource() const{
    return resource;
}

bool ReservationQueue::IsDateTaken(const string& date) const{
    queue<Reservation> temp = q;
    while (!temp.empty()) {
        if (temp.front().GetDate() == date) {
            return true;
        }
        temp.pop();
    }
    return false;
}

AddResult ReservationQueue::AddReservation(const Reservation& r, bool joinWaitlist){
    if (!IsDateTaken(r.GetDate())) {
        RestoreReservation(r);          // sorted insert into q
        return AddResult::Added;
    }
    if (!joinWaitlist) {
        return AddResult::Declined;
    }
    waiting.Add(r);
    return AddResult::Waitlisted;
}

bool ReservationQueue::RemoveReservation(int reservationId, CancelRecord& rec){
    rec.wasPromoted = false;
    rec.wasWaiting = false;

    queue<Reservation> remaining;
    bool found = false;
    Reservation removed;

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

    if (found) {
        rec.canceled = removed;
        // the next person waiting for that same date takes the slot
        Reservation next;
        if (waiting.PopNext(removed.GetDate(), next)) {
            RestoreReservation(next);
            rec.promoted = next;
            rec.wasPromoted = true;
        }
        return true;
    }

    // not in the queue, maybe they are on a waiting list
    Reservation waitingRemoved;
    size_t pos = 0;
    if (waiting.Remove(reservationId, waitingRemoved, pos)) {
        rec.canceled = waitingRemoved;
        rec.wasWaiting = true;
        rec.waitPos = pos;
        return true;
    }
    return false;
}

void ReservationQueue::RestoreReservation(const Reservation& reservation){
    vector<IndexReservation> restored;
    queue<Reservation> current = q;

    while (!current.empty()) {
        restored.push_back(toIndex(current.front()));
        current.pop();
    }
    restored.push_back(toIndex(reservation));

    ReservationQueue rebuilt(restored);   // constructor sorts by date
    q = rebuilt.GetQueue();
}

void ReservationQueue::DemoteToWaiting(const Reservation& promoted){
    queue<Reservation> remaining;
    while (!q.empty()) {
        Reservation current = q.front();
        q.pop();
        if (current.GetReservationId() != promoted.GetReservationId()) {
            remaining.push(current);
        }
    }
    q = remaining;
    waiting.PushFront(promoted);   // back at the head of the line
}

void ReservationQueue::RestoreToWaiting(const Reservation& r, size_t pos){
    waiting.InsertAt(r, pos);
}

int ReservationQueue::WaitingCount() const{
    return waiting.Count();
}


// =====================================================================
// Free functions
// =====================================================================

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

bool IsValidDate(const string& d){
    if (d.size() != 10 || d[2] != '/' || d[5] != '/') {
        return false;
    }
    for (size_t i = 0; i < d.size(); ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(static_cast<unsigned char>(d[i]))) return false;
    }

    int month = stoi(d.substr(0, 2));
    int day = stoi(d.substr(3, 2));
    int year = stoi(d.substr(6, 4));
    if (month < 1 || month > 12 || day < 1 || year < 1) {
        return false;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leapYear = year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    if (leapYear) {
        daysInMonth[1] = 29;
    }
    return day <= daysInMonth[month - 1];
}

void AddReservationFlow(vector<ReservationQueue>& queues, list<Reservation>& reservations, int& resIdIndex){
    cout << "Enter resource ID of the resource you want to reserve: ";
    string rid;
    cin >> rid;

    int idx = GetQueueIndex(queues, rid);
    if (idx == -1) {
        cout << "Error: Resource ID not found." << endl;
        return;
    }
    ReservationQueue& rq = queues[idx];

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter student name: ";
    string studentName;
    getline(cin, studentName);

    int studentId = readInt("Enter student ID: ");

    string date;
    while (true) {
        cout << "Enter the date you want (MM/DD/YYYY): ";
        cin >> date;
        if (IsValidDate(date)) {
            break;
        }
        cout << "Invalid date. Use the format MM/DD/YYYY." << endl;
    }

    bool joinWaitlist = false;
    if (rq.IsDateTaken(date)) {
        cout << "That date is already booked for this resource. Join the waiting list for " << date << "? (y/n): ";
        char answer;
        cin >> answer;
        joinWaitlist = (tolower(static_cast<unsigned char>(answer)) == 'y');
    }

    // take the ID up front so a waitlisted person keeps a stable ID
    Reservation r(++resIdIndex, rq.GetResource().GetId(), studentName, studentId, date);

    switch (rq.AddReservation(r, joinWaitlist)) {
        case AddResult::Added:
            reservations.push_back(r);
            cout << "Reservation added. Your reservation ID is " << r.GetReservationId() << "." << endl;
            break;
        case AddResult::Waitlisted:
            cout << "You were added to the waiting list for " << date
                 << ". Your reservation ID is " << r.GetReservationId() << "." << endl;
            break;
        case AddResult::Declined:
            --resIdIndex;   // nothing was created, give the ID back
            cout << "No reservation made." << endl;
            break;
    }
}


// =====================================================================
// CancelationStack
// =====================================================================

void CancelationStack::Cancel(vector<ReservationQueue>& queues, list<Reservation>& reservations){
    string rId;
    cout << "Enter resource ID: ";
    cin >> rId;
    int i = GetQueueIndex(queues, rId);
    if (i == -1) {
        cout << "Error: No queue found for this resource." << endl;
        return;
    }

    if (queues[i].GetQueue().empty() && queues[i].WaitingCount() == 0) {
        cout << "Error: No reservations for this resource." << endl;
        return;
    }

    queues[i].DisplayQueue();
    queues[i].DisplayWaiting();

    int resId = readInt("Enter reservation ID to cancel (or 0 to cancel): ");
    if (resId == 0) {
        return;
    }

    CancelRecord rec;
    rec.queueIndex = i;
    if (!queues[i].RemoveReservation(resId, rec)) {
        cout << "Error: Reservation ID was not found." << endl;
        return;
    }

    // waiting-list entries were never in the master list, so this is a no-op for them
    int canceledId = rec.canceled.GetReservationId();
    reservations.remove_if([canceledId](const Reservation& reservation) {
        return reservation.GetReservationId() == canceledId;
    });

    if (rec.wasPromoted) {
        reservations.push_back(rec.promoted);
    }

    c.push(rec);
    cout << "Reservation canceled." << endl;
    if (rec.wasPromoted) {
        cout << rec.promoted.GetStudentName() << " was moved off the waiting list into "
             << rec.promoted.GetDate() << "." << endl;
    }
}

void CancelationStack::Undo(vector<ReservationQueue>& queues, list<Reservation>& reservations){
    if (c.empty()) {
        cout << "Error: Nothing to undo." << endl;
        return;
    }

    CancelRecord rec = c.top();
    c.pop();

    if (rec.queueIndex < 0 || rec.queueIndex >= static_cast<int>(queues.size())) {
        cout << "Error: Nothing to undo." << endl;
        return;
    }
    ReservationQueue& rq = queues[rec.queueIndex];

    if (rec.wasWaiting) {
        // they were only on the waiting list, so put them back in their old spot
        rq.RestoreToWaiting(rec.canceled, rec.waitPos);
        cout << "Waiting-list reservation restored." << endl;
        return;
    }

    if (rec.wasPromoted) {
        // bump the promoted person back to the front of the line and out of the master list
        rq.DemoteToWaiting(rec.promoted);
        int promotedId = rec.promoted.GetReservationId();
        reservations.remove_if([promotedId](const Reservation& reservation) {
            return reservation.GetReservationId() == promotedId;
        });
    }

    rq.RestoreReservation(rec.canceled);
    reservations.push_back(rec.canceled);
    cout << "Reservation restored." << endl;
}

void CancelationStack::ShowCancelations(){
    if (c.empty()) {
       cout << "No recent cancelations." << endl;
       return;
    }

    stack<CancelRecord> temp = c;

    cout << endl;
    cout << "==================== RECENT CANCELLATIONS ====================" << endl;
    printHeader();

    while (!temp.empty()) {
       printRow(temp.top().canceled);
       temp.pop();
    }
}

// Waiting-List Statistics Report
void WaitingListStatistics(const vector<ReservationQueue>& queues) {

    cout << endl;
    cout << "==================== WAITING-LIST STATISTICS ====================" << endl;

    cout << "+" << right << setw(12) << setfill('-') << "+"
         << setw(22) << "+" << setw(18) << "+" << endl;

    cout << left << setw(12) << setfill(' ') << "|Resource ID"
         << setw(22) << "|Resource Name"
         << setw(18) << "|Students Waiting" << "|" << endl;

    cout << "+" << right << setw(12) << setfill('-') << "+"
         << setw(22) << "+" << setw(18) << "+" << endl;

    for (const ReservationQueue& rq : queues) {
        Resource r = rq.GetResource();

        cout << "|" << left << setw(11) << setfill(' ') << r.GetId()
             << "|" << setw(21) << r.GetName()
             << "|" << setw(17) << rq.WaitingCount() << "|" << endl;

        cout << "+" << right << setw(12) << setfill('-') << "+"
             << setw(22) << "+" << setw(18) << "+" << endl;
    }
}
