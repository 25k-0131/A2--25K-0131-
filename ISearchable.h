#ifndef ISEARCHABLE_H
#define ISEARCHABLE_H

#include <string>
using namespace std;

// ============================================================
//  Abstract Interface: ISearchable
//  Purpose : Guarantees that any searchable entity exposes a
//            matchesSearch() method.  The Marketplace calls
//            this without caring whether it talks to a Car,
//            a Bike, or something added in the future.
// ============================================================

class ISearchable {
public:
    // Returns true when the object matches a given keyword.
    // Each derived class decides what "matching" means for it.
    virtual bool matchesSearch(const string& keyword) const = 0;

    virtual ~ISearchable() {}
};

#endif
