#include "resource.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

bool CaseInsensitiveCompare1(const string& str1, const string& str2){
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


// Accessor functions
string Resource::GetId() const { return id; }
const string& Resource::GetName() const { return name; }
const string& Resource::GetType() const { return type; }
bool Resource::GetAvailability() const { return isavailable; }

// displays a resource as part of a formatted table
void Resource::DisplayResource() const {
    string available = isavailable ? "available" : "not available";
    cout << "|" << left << setw(11) << setfill(' ') << id << "|" << setw(16) << name << "|" << setw(21) << type << "|" << setw(14) << available << "|" << endl;
    cout << "+" << right << setw(12) << setfill('-') << "+" << setw(17) << "+" << setw(22) << "+" << setw(15) << "+" << endl;
}

void DisplayFoundResource(const vector<IndexResource>& found) {
    cout << "+" << right << setw(12) << setfill('-') << "+" << setw(17) << "+" << setw(22) << "+" << setw(15) << "+" << endl;
    cout << left << setw(12) << setfill(' ') << "|resourceId" << setw(17) << "|resourceName"<< setw(22) << "|resourceType" << setw(15) << "|isAvailable" << "|" << endl;
    cout << "+" << right << setw(12) << setfill('-') << "+" << setw(17) << "+" << setw(22) << "+" << setw(15) << "+" << endl;
    for (const auto& r : found) {
        string available = r.isAvailable ? "available" : "not available";
        cout <<"|"  << left<<setw(11)<<setfill(' ')<<r.resourceId<<"|"<<setw(16)<<r.resourceName<<"|"<<setw(21)<<r.resourceType<<"|"<<setw(14)<<available<<"|"<<endl;
        cout << "+" << right << setw(12) << setfill('-') << "+" << setw(17) << "+" << setw(22) << "+" << setw(15) << "+" << endl;
    }
}

void FindResource(const vector<Resource>& resources, vector<IndexResource>& found, const vector<string>& queries) {
    bool firstFound = true;
    for (const auto& r : resources) {
        int matchCount = 0;
        for (const auto& query : queries) {
            if ((CaseInsensitiveCompare1(r.GetId(), query)) || (CaseInsensitiveCompare1(r.GetName(), query)) || (CaseInsensitiveCompare1(r.GetType(), query)) || (query == "available" && r.GetAvailability()) || (query == "not available" && !r.GetAvailability())) {
                matchCount++;
            }
            if (matchCount >= queries.size()) {
                IndexResource foundResource;
                foundResource.resourceId = r.GetId();
                foundResource.resourceName = r.GetName();
                foundResource.resourceType = r.GetType();
                foundResource.isAvailable = r.GetAvailability();
                found.push_back(foundResource);
                firstFound = false;
            }
        }
    }
    if (firstFound) {
        cout << "Error: Resource not found." << endl;
        found.push_back(IndexResource{"", "", "", true}); // Return an empty IndexResource to indicate not found
    }
}
