#ifndef UTILS_H 
#define UTILS_H


#define FAILED_FETCHIO_INITIALIZATION "Error initializing Fetchio."
#define FAILED_PERFORMING_REQUEST "Error making request."

#include <iostream>


class FetchioErrors {

    public:

        static void initialization_error(){
            std::cout << FAILED_FETCHIO_INITIALIZATION << '\n';            
        }

        static void request_error(){
            std::cout << FAILED_PERFORMING_REQUEST << '\n';            
        }

};




#endif 