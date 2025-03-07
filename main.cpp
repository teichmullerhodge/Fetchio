#include "Aoi.hpp"
#include "logger.hpp"
#include <time.h>
#include <future>
#include <array>
#include <tuple>
#include <vector>
#include <optional>


int batch(){

   auto aoi = Aoi::start();

   std::vector<aoiurl> urls{100, "https://jsonplaceholder.typicode.com/posts"};
   std::vector<aoiheader> headers{100, {"Content-Type : application/json"}};
   auto promises = Aoi::get_as_batch(aoi, urls, headers, ASYNC);
   for(auto &p : promises){

      std::cout << p.get() << "\n";      

   }

   Aoi::release(aoi);  
   return 0;

}




int main(int argc, char **argv){

   auto response = Aoi::get(Aoi::start(), aoiurl{"https://jsonplaceholder.typicode.com/posts"}, DEFAULT_HEADERS, SYNC);
   std::cout << response << "\n";
   // batch();

}


