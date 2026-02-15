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
};