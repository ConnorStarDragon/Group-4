#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <list>
#include <vector>

using namespace std;

//Struct to hold a specific reservations information
struct  IndexReservation {
    int reservationId;
    string resourceId;
    string studentName;
    int studentId;
    string date;
};

class Reservation {
    
    private:
	//Data Members
	int reservationId;         // Unique reservation number
    string resourceId;            // Which resource this reservation is for
    string studentName;   // Student name
    int studentId;          // Student ID
    string date;          // Date of reservation

    public:
    //Constructors Section
	Reservation() //Default
        : reservationId(0), resourceId(""), studentName(""), studentId(0), date("") {}

    Reservation(int rid, const string &resId, const string &student, int sid, const string &d)  //Copy 
        : reservationId(rid), resourceId(resId), studentName(student), studentId(sid), date(d) {}

        //accessor functions
    int GetReservationId() const;
    const string& GetResourceId() const;
    const string& GetStudentName() const;
    int GetStudentId() const;
    const string& GetDate() const;

    void DisplayReservation() const; //Display function
    
    

};

void FindReservation(const list<Reservation>& reservations, vector<IndexReservation>& found, int query); //Finds a reservations in the list by int variable
void FindReservation(const list<Reservation>& reservations, vector<IndexReservation>& found, const string& query); //Finds a reservations in the list by resource string variable
void DisplayFoundReservation(const vector<IndexReservation>& found); //Displays a found reservation in a formatted table
bool CaseInsensitiveCompare(const string& str1, const string& str2); //Compares two strings without case sensitivity, important for searching for reservations by student name or resource ID

#endif
