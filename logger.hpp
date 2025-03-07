#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <ctime>
///  log messages with different colors on the terminal.
class Logger {
    
    private:

        static std::string get_current_time(){

                std::time_t rawtime;
                std::tm* timeinfo;
                char buffer [80];
            
                std::time(&rawtime);
                timeinfo = std::localtime(&rawtime);
            
                std::strftime(buffer,80,"[%d/%m/%Y %H:%M:%S]",timeinfo);
                return buffer;            
        }

    public:
        static void log_sucess(std::string message){
            std::cout << "\033[1;32m" << get_current_time() << " [SUCCESS] - " << message << "\033[0m";
        }

        static void log_error(std::string message){
            std::cout << "\033[1;31m" << get_current_time()  << " [ERROR] - " << message << "\033[0m";
        }

        static void log_warning(std::string message){
            std::cout << "\033[1;35m" << get_current_time() << " [WARNING] - " << message << "\033[0m";
        }

        static void log_info(std::string message){
            std::cout << "\033[1;34m" << get_current_time() << " [INFO] - " << message << "\033[0m";
        }

};

#endif 