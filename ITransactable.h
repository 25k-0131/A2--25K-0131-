#ifndef ITRANSACTABLE_H
#define ITRANSACTABLE_H

#include <string>
using namespace std;

// ============================================================
//  Abstract Interface: ITransactable
//  Purpose : Represents anything that can be bought/sold.
//            Buyer and future Payment classes depend on this
//            interface, not on concrete vehicle types.
// ============================================================

class ITransactable {
public:
    virtual float       getPrice() const = 0;
    virtual string      getTitle() const = 0;   // e.g. "Toyota Corolla 2020"
    virtual bool        isAvailable() const = 0;

    virtual ~ITransactable() {}
};

#endif
