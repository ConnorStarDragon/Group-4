#ifndef RESERVATIONFUNCTIONS_H
#define RESERVATIONFUNCTIONS_H

#include "reservation.h"
#include "resource.h"

#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <vector>

using namespace std;

class ReservationQueue{
    private:
        queue<Reservation> q;
        Resource resource;

    public:
        ReservationQueue(vector<IndexReservation> reservations);
        ReservationQueue(vector<IndexReservation> reservations, const Resource& resource);
        void DisplayQueue() const;
        queue<Reservation> GetQueue() const;
        bool RemoveReservation(int studentId, Reservation& removed);
        void RestoreReservation(const Reservation& reservation);
        Resource GetResource() const;
        Reservation AddReservation(int& resIdIndex);
};

int GetQueueIndex(const vector<ReservationQueue>&, string id);

class CancelationStack{
    private:
        stack<Reservation> c;

    public:
        void Cancel(vector<ReservationQueue>& queues, list<Reservation>& reservations, int& canceledIndex);
        void Undo(vector<ReservationQueue>& queues, list<Reservation>& reservations, int i);
};

#endif