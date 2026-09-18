#include "reservation.h"
#include <iostream>
#include <iomanip>
#include <list>
#include <cctype>
#include <vector>

using namespace std;

bool CaseInsensitiveCompare(const string& str1, const string& str2){
    if (str1.length() != str2.length()) {
        return false;
    }
    for (int i = 0; i < str1.length(); ++i) {
        if (tolower(str1[i]) != tolower(str2[i])) {
            return false;
        }
    }
    return true;
}

// displays the reservation from the header
/*void displayReservation(const Reservation &reservation)
{
    cout << "Reservation ID: " << reservation.reservationId << endl;
    cout << "Resource ID: " << reservation.resourceId << endl;
    cout << "Student Name: " << reservation.studentName << endl;
}
*/
// displays a reservation as part of a formatted table
void Reservation::DisplayReservation() const
{
    cout << "|" << left << setw(14) << setfill(' ') << reservationId << "|" << setw(10) << studentId << "|" << setw(19) << studentName << "|" << setw(11) << resourceId << "|" << setw(11) << date << "|" << endl;
    cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
}

// displays a found reservation in a formatted table
void DisplayFoundReservation(const vector<IndexReservation>& found) {
    cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
    cout << left << setw(15) << setfill(' ') << "|reservationId" << setw(11) << "|studentId"<< setw(20) << "|studentName" << setw(12) << "|resourceId" << setw(12) << "|date" << "|" << endl;
    cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
    for (const auto& r : found) {
        cout <<"|"  << left<<setw(14)<<setfill(' ')<<r.reservationId<<"|"<<setw(10)<<r.studentId<<"|"<<setw(19)<<r.studentName<<"|"<<setw(11)<<r.resourceId<<"|"<<setw(11)<<r.date<<"|"<<endl;
        cout << "+" << right << setw(15) << setfill('-') << "+" << setw(11) << "+" << setw(20) << "+" << setw(12) << "+" << setw(12) << "+" << endl;
    }
}

// Accessor functions
int Reservation::GetReservationId() const { return reservationId; }
const string& Reservation::GetResourceId() const { return resourceId; }
const string& Reservation::GetStudentName() const { return studentName; }
int Reservation::GetStudentId() const { return studentId; }
const string& Reservation::GetDate() const { return date; }

void FindReservation(const list<Reservation>& reservations, vector<IndexReservation>& found, int query) {
    //bool for if its the first found item or not
    bool firstFound = true;
    //search using reservation ID
    if (query < 1000 && query > 0){
        list<Reservation>::const_iterator it = reservations.begin();
        while (it != reservations.end()) {
            if (it->GetReservationId() == query){
                IndexReservation foundReservation;
                foundReservation.reservationId = it->GetReservationId();
                foundReservation.resourceId = it->GetResourceId();
                foundReservation.studentName = it->GetStudentName();
                foundReservation.studentId = it->GetStudentId();
                foundReservation.date = it->GetDate();
                found.push_back(foundReservation);
                it++;
                firstFound = false;
            }
            else{
                it++;
            }
    }
        if (firstFound){
            cout << "Error: Reservation ID not found." << endl;
        }
    }
    //search using student ID
    else if (query >= 1000 && query < 10000){
        list<Reservation>::const_iterator it = reservations.begin();
        while (it != reservations.end()) {
            if (it->GetStudentId() == query){
                IndexReservation foundReservation;
                foundReservation.reservationId = it->GetReservationId();
                foundReservation.resourceId = it->GetResourceId();
                foundReservation.studentName = it->GetStudentName();
                foundReservation.studentId = it->GetStudentId();
                foundReservation.date = it->GetDate();
                found.push_back(foundReservation);
                it++;
                firstFound = false;
            }
            else{
                it++;
            }
        }
        if (firstFound){
            cout << "Error: Student ID not found." << endl;
        }

    }
    else{
        cout << "Error: Invalid query. Please enter a valid reservation ID or student ID." << endl;
        found.push_back(IndexReservation{0, "", "", 0, ""}); // Return an empty IndexReservation to indicate not found
    }
}
void FindReservation(const list<Reservation>& reservations, vector<IndexReservation>& found, const string& query) {
    //bool for if its the first found item or not
    bool firstFound = true;
    //search using student name
    if (isalpha(query[0])){
        if (isalpha(query[1])){
            list<Reservation>::const_iterator it = reservations.begin();
            while (it != reservations.end()) {
                if (CaseInsensitiveCompare(it->GetStudentName(), query)){
                    IndexReservation foundReservation;
                    foundReservation.reservationId = it->GetReservationId();
                    foundReservation.resourceId = it->GetResourceId();
                    foundReservation.studentName = it->GetStudentName();
                    foundReservation.studentId = it->GetStudentId();
                    foundReservation.date = it->GetDate();
                    found.push_back(foundReservation);
                    ++it;
                    firstFound = false;
                }
                else{
                    ++it;
                }
            }
            if (firstFound){
                    cout << "Error: Student name not found." << endl;
                    found.push_back(IndexReservation{0, "", "", 0, ""}); // Return an empty IndexReservation to indicate not found
            }
        }
        //search using resource ID
        else{
            list<Reservation>::const_iterator it = reservations.begin();
            while (it != reservations.end()) {
                
                if (CaseInsensitiveCompare(it->GetResourceId(), query)){
                    IndexReservation foundReservation;
                    foundReservation.reservationId = it->GetReservationId();
                    foundReservation.resourceId = it->GetResourceId();
                    foundReservation.studentName = it->GetStudentName();
                    foundReservation.studentId = it->GetStudentId();
                    foundReservation.date = it->GetDate();
                    found.push_back(foundReservation);
                    ++it;
                    firstFound = false;
                }
                else{
                    ++it;
                }
            }
            if (firstFound){
                cout << "Error: Resource ID not found." << endl;
                found.push_back(IndexReservation{0, "", "", 0, ""}); // Return an empty IndexReservation to indicate not found
            }
}
    }
    //search using date
    else if(query[2] == '/'){
        list<Reservation>::const_iterator it = reservations.begin();
        while (it != reservations.end()) {
            if (it->GetDate() == query){
                IndexReservation foundReservation;
                foundReservation.reservationId = it->GetReservationId();
                foundReservation.resourceId = it->GetResourceId();
                foundReservation.studentName = it->GetStudentName();
                foundReservation.studentId = it->GetStudentId();
                foundReservation.date = it->GetDate();
                found.push_back(foundReservation);
                return;
                ++it;
                firstFound = false;
            }
            else{
                ++it;
            }       
        }
        if (firstFound){
            cout << "Error: Date not found." << endl;
            found.push_back(IndexReservation{0, "", "", 0, ""}); // Return an empty IndexReservation to indicate not found
        }
    }
    else{
        cout << "Error: Invalid query. Please enter a valid resource ID or student name." << endl;
        found.push_back(IndexReservation{0, "", "", 0, ""}); // Return an empty IndexReservation to indicate not found
    }
}

/*int main()
{
    // Creates a example reservation
    Reservation reservation1(1, 2, "Kaden");

    // Displays the sample reservation
    displayReservation(reservation1);
}
*/
