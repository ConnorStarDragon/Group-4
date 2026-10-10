#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <vector>

using namespace std;

struct IndexResource {
    string resourceId;
    string resourceName;
    string resourceType;
    bool isAvailable;
};

class Resource {
    private:
	    //Data Members
	    string id;                 // Unique ID for the resource
        string name;
        string type;          // Type of the resource (e.g., "Lab")          // Max number of reservations allowed
        bool isavailable;      // Whether the resource is currently available

    public:
	//Constructors Section
        Resource() //Default constructor
            : id(""), name(""), type(""), isavailable(true) {}

        Resource(const string &rid, const string &rname, const string &rtype, bool available) //Resource constructor
           : id(rid), name(rname), type(rtype), isavailable(available) {}


	    //check to see if the resource is available
        bool isAvailable() const {
           return isavailable;
        }

        // accessor functions
        string GetId() const;
        const string& GetName() const;
        const string& GetType() const;
        bool GetAvailability() const;

        // for displaying a resource
        void DisplayResource() const;

};

void FindResource(const vector<Resource>& resources, vector<IndexResource>& found, const vector<string>& queries); //Finds a resource in the vector by string variable
void DisplayFoundResource(const vector<IndexResource>& found); //Displays a found resource in a formatted table
bool CaseInsensitiveCompare1(const string& str1, const string& str2); //Compares two strings without case sensitivity, important for all searches within resources

#endif
