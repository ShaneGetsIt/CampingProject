#pragma once

#include "SupplyList.h"

// GeneralSupplies - concrete derived class for standard camping supplies
class GeneralSupplies : public SupplyList
{
public:
    // Default constructor
    GeneralSupplies();
    
    // Parameterized constructor
    GeneralSupplies(string name, int capacity, priority prio);
    
    // Override pure virtual function from base class
    virtual string getSupplyType() const override;
    
    // Virtual destructor (can use default)
    virtual ~GeneralSupplies();
    
    // Copy from base class (ADDED - alternative to operator=)
    void copyFrom(const SupplyList& other);
    
    // WEEK 06 ADDITION: Operator overloading
    // Equality operator - compares meaningful identity fields
    bool operator==(const GeneralSupplies& other) const;
    
    // WEEK 06 ADDITION: Override virtual streaming function
    virtual void toStream(ostream& out) const override;
};