#ifndef IRATABLE_H
#define IRATABLE_H

// ============================================================
//  Abstract Interface: IRatable
//  Purpose : Any entity that can receive a rating (Seller,
//            Listing …) must implement getRating() and
//            updateRating().  Keeps the rating contract
//            separate from business logic.
// ============================================================

class IRatable {
public:
    virtual float getrating()          const = 0;
    virtual void  updateRating(float r)      = 0;

    virtual ~IRatable() {}
};

#endif
