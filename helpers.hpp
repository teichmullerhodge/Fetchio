#ifndef UTILS_HPP
#define UTILS_HPP


#define FAILED_FETCHIO_INITIALIZATION "Error initializing Fetchio."
#define FAILED_PERFORMING_REQUEST "Error making request."

#define DEFAULT_HEADERS "Content-Type: application/json"

#include <iostream>
#include <nlohmann/json.hpp>


class FetchioErrors {

    public:

        static void initialization_error(){
            std::cout << FAILED_FETCHIO_INITIALIZATION << '\n';            
        }

        static void request_error(){
            std::cout << FAILED_PERFORMING_REQUEST << '\n';            
        }

};


class FetchioLogger {

    public:
        static void log_response(std::string response, long statusCode){
            std::cout << "[StatusCode] -> " << statusCode << '\n';
            std::cout << "[Response] -> " << response << '\n';
        }

        static void log_json_response(nlohmann::json response, long statusCode){

            response["StatusCode"] = statusCode;
            std::cout << response.dump(4) << '\n';

        }  
};




#endif 
