#include "reservation.h"
#include <iostream>

using namespace std;

// displays the reservation from the header
void displayReservation(const Reservation &reservation)
{
    cout << "Reservation ID: " << reservation.reservationId << endl;
    cout << "Resource ID: " << reservation.resourceId << endl;
    cout << "Student Name: " << reservation.studentName << endl;
}

/*int main()
{
    // Creates a sample reservation
    Reservation reservation1(1, 2, "Kaden");

    // Displays the sample reservation
    displayReservation(reservation1);
}
*/