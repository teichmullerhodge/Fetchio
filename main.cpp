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

   std::vector<aoiurl> urls;
   urls.reserve(100);
   std::vector<aoiheader> headers;
   headers.reserve(100);
   for (int i = 0; i < 100; ++i) {

      urls.push_back("https://jsonplaceholder.typicode.com/posts");
      headers.push_back({"Content-Type: application/json"});

   }
   
   auto promises = Aoi::get_as_batch(aoi, urls, headers, ASYNC);
   for(auto &p : promises){

      std::cout << p.get() << "\n";      

   }

   Aoi::release(aoi);  
   return 0;

}




int main(int argc, char **argv){

   // auto response = Aoi::get(Aoi::start(), aoiurl{"https://jsonplaceholder.typicode.com/posts"}, DEFAULT_HEADERS, SYNC);
   // std::cout << response << "\n";
   batch();

}


