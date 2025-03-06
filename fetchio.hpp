#include <iostream>
#include <curl/curl.h>
#include <optional>
#include <future>
#include <vector>
#include <array>
#include "helpers.hpp"
#include "logger.hpp"




size_t write_function(void *contents, size_t size, size_t nmemb, std::string *s){
    size_t newLength = size * nmemb;
    try{
        s->append((char *)contents, newLength);
    }
    catch (std::bad_alloc &e){
        return 0;
    }
    return newLength;
}



class Fetchio {


    private:


    bool keepAlive;

     // ? Function called automatically after class instantiation.
     // ? It set's the URL provided and the write function
     void setup(){
        curl_easy_setopt(this->curl, CURLOPT_URL, this->url.c_str());
        curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, write_function);
        this->headers = curl_slist_append(this->headers, "Content-Type: application/json");
        this->grant_headers();
    }
    // ? release the resources, automatically called by the destructor.
    void release(){
        this->free_headers();
        if (this->curl) {
            curl_easy_cleanup(this->curl);
            this->curl = nullptr;
        }
    }
    

public:

    long statusCode;
    struct curl_slist *headers;
    CURLcode response;
    std::string url;
    CURL *curl;

     // ? The body of the HTTP request can be set as a plain string containing JSON content.
     // ? Example of setting the body with JSON content:
     // ? body = R"({"Name": "John Doe", "Age": 10, "Geo": [20, 2414012, -321421]})";      
    std::string body;

    Fetchio(std::string url){
        curl_global_init(CURL_GLOBAL_ALL);
        this->headers = nullptr;
        this->body = R"({})"; 
        this->url = url;
        this->keepAlive = true;
        this->curl = curl_easy_init();
        if (this->curl) {
            this->setup();
        } else {
            FetchioErrors::initialization_error();
            std::cerr << "Erro ao inicializar cURL\n";
        }
    }

    ~Fetchio(){ // ! I love you RAII creator.
            this->release();
    }

    static std::string get(CURL *c, CURLcode response){

        std::string responseString;
        std::string headersString;
        curl_easy_setopt(c, CURLOPT_WRITEDATA, &responseString);
        response = curl_easy_perform(c);

        if (!Fetchio::response_ok(response)){
            FetchioErrors::request_error();
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(response) << '\n'; // ? std::endl has performance issues. LOL.
        }

        return responseString;
    }

    std::future<std::string> __fetchio__(){

        return std::async(std::launch::async, [this]() { return this->get(this->curl, this->response); });
    }

    void set_keepalive(bool keepAlive){
        this->keepAlive = keepAlive;
    }
        
     // ? Set the URL to be fetched
    void set_url(std::string url) {
        if (!this->curl){
            Logger::log_error("Curl is not initialized!\n");
            return;
        }
        this->url = url;
        curl_easy_setopt(this->curl, CURLOPT_URL, this->url.c_str());
    }
    

    // ? overloading 
    std::vector<std::future<std::string>> get_as_futures(std::vector<std::string> urls){

        std::vector<std::future<std::string>> result = {};
        std::vector<std::unique_ptr<Fetchio>> clients; 
        for(auto url : urls) {

            std::cout << url << "\n";                                    
            clients.push_back(std::make_unique<Fetchio>(url)); 
            result.emplace_back(clients.back()->__fetchio__()); 

        }

        return result;
    }
    
     // ? Set the headers to be sent with the request
    void set_headers(std::string KV){
        if(!this->curl){
            return;
        }
        this->headers = curl_slist_append(this->headers, KV.c_str());
        this->grant_headers();
    }

    
     // ? Free the headers
    void free_headers(){

        curl_slist_free_all(this->headers);
    }

    
     // ? Set the headers configuration to be sent with the request
    void grant_headers(){
        curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, this->headers);
    } 
    
     // ? Set the body of the class instance.
    void set_body(std::string B){
        this->body = B;
    }

    std::string get_body() const {
        return this->body;
    }

    
     // ? Get the status code of the request
    constexpr long get_status_code(){
        curl_easy_getinfo(this->curl, CURLINFO_RESPONSE_CODE, &this->statusCode);
        return this->statusCode;
    }

    
     //  ? Check if the response is OK
    static constexpr bool response_ok(CURLcode response){
        return response == CURLE_OK;
    }

    
     // ^ Sends a post request, accepts a body as optional parameter
     // ^ if you don't want to send the body as an parameter, it will
     // ^ consider the body of the class instance. If neither are 
     // ^ initialized or provided, it will consider an empty body.
    std::string post(const std::optional<std::string>& body){

        std::string payload = body.has_value() ? body.value() : this->body;
        std::string responseString;

        curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &responseString);
        curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
        curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, payload.c_str());

        this->response = curl_easy_perform(this->curl);
                if (!Fetchio::response_ok(this->response)){
            FetchioErrors::request_error();
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(this->response) << '\n'; // std::endl has performance issues. LOL.
        }

        if(!this->keepAlive){

            curl_easy_cleanup(this->curl);
        }

    
        return responseString;

    }

    
};
