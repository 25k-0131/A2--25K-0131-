#ifndef IDISPLAYABLE_H
#define IDISPLAYABLE_H

// ============================================================
//  Abstract Interface: IDisplayable
//  Purpose : Forces every class that inherits it to implement
//            its own display() method.  This is pure abstraction
//            — callers only need to know "this thing can display
//            itself", not HOW it does it.
// ============================================================

class IDisplayable {
public:
    // Pure virtual — no default body.  Any concrete class MUST
    // override this, otherwise it cannot be instantiated.
    virtual void display() const = 0;

    // Virtual destructor so derived objects are cleaned up
    // correctly when deleted through a base pointer.
    virtual ~IDisplayable() {}
};

#endif
