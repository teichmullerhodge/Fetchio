#include "fetchio.hpp"
#include "helpers.hpp"
#include "logger.hpp"
#include <time.h>
#include <future>
#include <array>

int main(int argc, char **argv){

   Fetchio client = Fetchio("");
   auto urls = std::vector<std::string>(100, "https://jsonplaceholder.typicode.com/todos");
   auto promises = client.get_as_futures(urls);
   

}
