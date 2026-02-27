//#pragma once
//
//// Include statements
//// #include <iostream>
//// #include <stdexcept>
//// #include "InvalidIndexExceptions.h"
//
//// Conceptual demonstration of exception handling
//
//// CUSTOM EXCEPTION CLASS - Demonstrates overriding what()
//class CustomException : public std::exception
//{
//private:
//    std::string message;
//    
//public:
//    CustomException(const std::string& msg) : message(msg) {}
//    
//    // Override what() to return custom message
//    const char* what() const throw()  // C++14 compatible (noexcept in C++11+)
//    {
//        return message.c_str();
//    }
//};
//
//int main()
//{
//    // TRY BLOCK - Code that might throw an exception
//    try
//    {
//        // Call a function that throws an exception
//        // someFunction();
//        
//        // throw runtime_error("message");
//        // throw InvalidIndexException("message");
//        // throw out_of_range("message");
//        // throw CustomException("custom message via what()");
//    }
//    
//    // CATCH BLOCKS - Handle different exception types
//    // Order matters: most specific to most general
//    
//    catch (const CustomException& e)  // Custom exception with overridden what()
//    {
//        // Handle custom exception
//        // cout << e.what() << endl;  // Calls our overridden what()
//    }
//    
//    catch (const InvalidIndexException& e)  // Custom exception (most specific)
//    {
//        // Handle custom exception
//        // cout << e.what() << endl;
//    }
//    
//    catch (const out_of_range& e)  // Standard specific exception
//    {
//        // Handle out_of_range
//        // cout << e.what() << endl;
//    }
//    
//    catch (const runtime_error& e)  // Standard base exception (less specific)
//    {
//        // Handle runtime_error
//        // cout << e.what() << endl;
//    }
//    
//    catch (...)  // Catch-all for any other exception type
//    {
//        // Handle unknown exceptions
//        // cout << "Unknown exception caught" << endl;
//    }
//    
//    return 0;
//}

