#ifndef RESERVATIONFUNCTIONS_H
#define RESERVATIONFUNCTIONS_H

#include "reservation.h"
#include "resource.h"

#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <vector>
#include <map>
#include <deque>
#include <string>

using namespace std;

enum class AddResult { Added, Waitlisted, Declined };

// Waiting list for ONE resource. Keyed by date (MM/DD/YYYY); each date has its own FIFO line.
// operator[] on the map creates a date's list the first time it is needed and reuses it afterwards.
class WaitingList {
    private:
        map<string, deque<Reservation>> byDate;

    public:
        void Add(const Reservation& r);                                   // back of that date's line
        bool PopNext(const string& date, Reservation& next);              // front of that date's line
        void PushFront(const Reservation& r);                             // Undo of a promotion
        void InsertAt(const Reservation& r, size_t pos);                  // Undo of a waiting-list cancel
        bool Remove(int reservationId, Reservation& removed, size_t& pos);// cancel someone still waiting
        int Count() const;                                                // total waiting, all dates
        vector<Reservation> All() const;                                  // for display (date order)
};

// Everything Undo needs to reverse one cancelation.
struct CancelRecord {
    Reservation canceled;
    Reservation promoted;        // valid only if wasPromoted
    bool wasPromoted = false;    // someone moved from the waiting list into the queue
    bool wasWaiting = false;     // the canceled reservation was on a waiting list, not in the queue
    size_t waitPos = 0;          // position it held in its date's waiting line (if wasWaiting)
    int queueIndex = -1;         // which resource's ReservationQueue this belongs to
};

class ReservationQueue{
    private:
        queue<Reservation> q;
        Resource resource;
        WaitingList waiting;

    public:
        ReservationQueue(vector<IndexReservation> reservations);
        ReservationQueue(vector<IndexReservation> reservations, const Resource& resource);

        void DisplayQueue() const;
        void DisplayWaiting() const;
        queue<Reservation> GetQueue() const;
        Resource GetResource() const;

        bool IsDateTaken(const string& date) const;
        AddResult AddReservation(const Reservation& r, bool joinWaitlist);

        // Removes from the queue (promoting the next waiting person for that date)
        // or, failing that, from a waiting list. Fills in rec. Returns false if not found.
        bool RemoveReservation(int reservationId, CancelRecord& rec);

        void RestoreReservation(const Reservation& reservation);   // sorted re-insert into q
        void DemoteToWaiting(const Reservation& promoted);         // pulls out of q, puts at FRONT of waiting
        void RestoreToWaiting(const Reservation& r, size_t pos);   // puts back at its old waiting position

        int WaitingCount() const;
};

int GetQueueIndex(const vector<ReservationQueue>&, string id);

bool IsValidDate(const string& date);   // MM/DD/YYYY, real calendar date

// Prompts for everything and adds (or waitlists) a reservation
void AddReservationFlow(vector<ReservationQueue>& queues, list<Reservation>& reservations, int& resIdIndex);

class CancelationStack{
    private:
        stack<CancelRecord> c;

    public:
        void Cancel(vector<ReservationQueue>& queues, list<Reservation>& reservations);
        void Undo(vector<ReservationQueue>& queues, list<Reservation>& reservations);
        void ShowCancelations();
        bool CanUndo() const { return !c.empty(); }
};

// waiting list statistics
void WaitingListStatistics(const vector<ReservationQueue>& queues);

#endif
