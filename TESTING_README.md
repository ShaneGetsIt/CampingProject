# Unit Testing with doctest - README

## Overview
This project includes unit tests using a simplified doctest framework compatible with C++ Programming 1. The program can run in two modes:

1. **Program Mode** - Normal camping supply list generator (default)
2. **Test Mode** - Run unit tests

## How to Switch Between Modes

### Running in Program Mode (Default)
- Build and run normally
- No special configuration needed
- The program will run the camping supply list generator

### Running in Test Mode
To run unit tests, define the `_DEBUG` preprocessor symbol:

**In Visual Studio:**
1. Right-click on your project ? Properties
2. Go to: Configuration Properties ? C/C++ ? Preprocessor
3. In "Preprocessor Definitions", add: `_DEBUG`
4. Click OK and rebuild
5. Run the program - it will execute all unit tests

**Alternative method:**
Add `#define _DEBUG` at the top of CampingAssn1.cpp (before includes)

## Test Coverage

### A) Calculations Tests (4 tests) - Required: at least 4
Tests derived values, totals, averages:
- Fire starters calculation - normal case (2 fires × 3 nights = 6)
- Fire starters calculation - edge case with 0 fires
- Marshmallow weight calculation - normal case (8 fires × 4 campers ÷ 4 = 8 lbs)
- Average food quantity - guard against divide-by-zero (0 items returns 0.0)

### B) Enum Decision Logic Tests (3 tests) - Required: at least 3
Tests function using decision structure (switch) based on priority enum:
- Priority to string conversion - low priority returns "Low"
- Priority to string conversion - medium priority returns "Med"
- Priority to string conversion - high priority returns "High"

### C) Struct/Array Processing Tests (3 tests) - Required: at least 3
Tests functions that process arrays and structs:
- Add food items to array and verify count
- Add food + gear items and verify total count
- Boundary case - reject adding items when array is full

### D) Class Methods Tests (2 tests) - Required: at least 2
Tests class methods that don't require cin:
- addFoodItem() returns true on success and increments count
- clearAll() resets all counts to zero

**Total: 12 tests (4+3+3+2)**

## Test Types Included
- **Normal cases** - typical usage scenarios
- **Edge cases** - boundary conditions (0 items, empty lists, etc.)
- **Guard cases** - protection against invalid operations (divide-by-zero, overflow, etc.)

## Expected Output in Test Mode
When running in test mode, you'll see:
```
===============================================================================
Running doctest unit tests
===============================================================================

TEST CASE: Calculations: Fire starters - normal case
  PASSED

TEST CASE: Calculations: Fire starters - edge case with 0
  PASSED

... (all 12 tests)

===============================================================================
Test summary:
  12 test case(s) passed
  0 test case(s) failed
===============================================================================
```

## Code Modifications for Testing
The following changes were made to support unit testing:

1. **SupplyList class** - Added public test-friendly methods:
   - `addFoodItem()` - Add food without cin
   - `addGearItem()` - Add gear without cin
   - `getFoodCount()` - Get food count
   - `getGearCount()` - Get gear count
   - `getTotalItemCount()` - Get total items (food + gear)
   - `getAverageFoodQuantity()` - Calculate average with divide-by-zero guard
   - `getPriorityString()` - Convert enum to string using switch statement
   - `getFoodItem()` - Access food item for verification
   - `getGearItem()` - Access gear item for verification
   - `clearAll()` - Reset for clean tests

2. **Helper functions** - Extracted for testability:
   - `calculateFireStarters()` - Testable calculation (fires × nights)
   - `calculateMarshmallows()` - Testable calculation (fires × campers ÷ 4)

3. **Main function** - Uses preprocessor directives:
   - `#ifdef _DEBUG` - Runs tests when _DEBUG is defined
   - `#else` - Runs normal program when _DEBUG is not defined

## Files Added/Modified
- `doctest.h` - Simplified single-header testing framework (C++ Programming 1 compatible)
- `SupplyList.h` - Added test-friendly public methods
- `SupplyList.cpp` - Implemented test methods
- `CampingAssn1.cpp` - Added 12 unit tests and test infrastructure

## Notes
- All 12 tests pass successfully
- Private member variables remain private (good encapsulation)
- Public methods provide controlled access for testing
- No changes to core program functionality
- Tests cover normal, edge, and guard cases as required
