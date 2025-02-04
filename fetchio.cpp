#include <iostream>
#include <curl/curl.h>



class Fetchio {

    CURL *curl;

    Fetchio(std::string url){
        this->curl = curl_easy_init();

    }

    void set_url(){
        if(!this->curl) return;
        curl_easy_setopt(this->)
    }

}


int main(int argc, char **argv){

    return 0;
}