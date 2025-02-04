#include <iostream>
#include <curl/curl.h>

size_t write_function(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
    
    }


class Fetchio {

public:
    CURL *curl;        
    long statusCode;
    struct curl_slist* headers;
    CURLcode response;

    Fetchio(std::string url){
        this->curl = curl_easy_init();
        if(this->curl){
            curl_easy_setopt(this->curl, CURLOPT_URL, url);
        }

        else {
            std::cout << "Error initializing curl!" << '\n';

        }
    }


    void set_url(std::string url){
        if(!this->curl) return;
        curl_easy_setopt(this->curl, CURLOPT_URL, url);
    }

    void set_headers(std::string KV) {
        curl_slist_append(this->headers, KV.c_str());

    }

    void grant_headers(){
        if(!this->headers) return;
        curl_easy_setopt(this->curl, CURLOPT_HEADER, this->headers);

    }
    

    void free_headers(){
        curl_slist_free_all(this->headers);
    }



    bool response_ok(){
        return this->response == CURLE_OK;
    }

    void get(){

        std::string responseString;
        std::string headersString;

        curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, write_function);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headersString);
        curl_easy_perform(this->curl);
        std::cout << responseString;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        this->curl = NULL;

    }



};


int main(int argc, char **argv){

    auto httpInstance = Fetchio("https://www.google.com");
    httpInstance.get();


    return 0;
}