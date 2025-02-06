#include "utils.h"
#include <iostream>
#include <curl/curl.h>
#include <optional>

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

class Fetchio
{

public:
    CURL *curl;
    long statusCode;
    struct curl_slist *headers;
    CURLcode response;
    std::string url;

    /**
     * 
     * The body of the HTTP request can be set as a plain string containing JSON content.
     * Example of setting the body with JSON content:
     * body = R"({"Name": "John Doe", "Age": 10, "Geo": [20, 2414012, -321421]})";
     * 
     */
    std::string body;

    Fetchio(std::string url){
        this->headers = nullptr;
        this->body = R"({})"; //defines an empty body as the first value.
        this->url = url;
        this->curl = curl_easy_init();
        this->curl ? this->setup() : FetchioErrors::initialization_error();
    }
    /**
     * Function called automatically after class instantiation.
     * It set's the URL provided and the write function
     */
    void setup(){
        curl_easy_setopt(this->curl, CURLOPT_URL, this->url.c_str());
        curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, write_function);
    }

    /**
     * Set the URL to be fetched
     */
    void set_url(std::string url) {
        if (!this->curl)
            return;
        this->url = url;
        curl_easy_setopt(this->curl, CURLOPT_URL, this->url.c_str());
    }
    /**
     * Returns the URL in the instance
     */
    std::string get_url() const {
        return this->url;
    }

    /**
     * Set the headers to be sent with the request
     */
    void set_headers(std::string KV){
        this->headers = curl_slist_append(this->headers, KV.c_str());
    }

    /**
     * Free the headers
     */
    void free_headers(){

        curl_slist_free_all(this->headers);
    }

    /**
     * Set the headers configuration to be sent with the request
     */
    void grant_headers(){
        curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, this->headers);
    } 
    /**
     * Set the body of the class instance.
     */
    void set_body(std::string B){
        this->body = B;
    }

    std::string get_body() const {
        return this->body;
    }

    /**
     * Get the status code of the request
     */
    long get_status_code(){
        curl_easy_getinfo(this->curl, CURLINFO_RESPONSE_CODE, &this->statusCode);
        return this->statusCode;
    }

    /**
     * Check if the response is OK
     */
    bool response_ok(){
        return this->response == CURLE_OK;
    }

    /**
     * Perform a GET request and return the response
     */
    std::string get(){

        std::string responseString;
        std::string headersString;

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
        this->response = curl_easy_perform(this->curl);

        if (!this->response_ok()){
            FetchioErrors::request_error();
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(this->response) << '\n'; // std::endl has performance issues. LOL.
        }

        curl_easy_cleanup(this->curl);
        return responseString;
    }

    /**
     * 
     * Sends a post request, accepts a body as optional parameter
     * if you don't want to send the body as an parameter, it will
     * consider the body of the class instance. If neither are 
     * initialized or provided, it will consider an empty body.
     */
    std::string post(const std::optional<std::string>& body){

        std::string payload = body.has_value() ? body.value() : this->body;
        std::string responseString;

        curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &responseString);
        curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
        curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, payload.c_str());

        this->response = curl_easy_perform(this->curl);
                if (!this->response_ok()){
            FetchioErrors::request_error();
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(this->response) << '\n'; // std::endl has performance issues. LOL.
        }

        curl_easy_cleanup(this->curl);
        return responseString;


    }
};

int main(int argc, char **argv){


    auto httpInstance = Fetchio("https://jsonplaceholder.typicode.com/posts");
    httpInstance.set_headers("Content-Type: application/json");
    httpInstance.grant_headers();

    auto contents = httpInstance.post(std::nullopt);
    long status = httpInstance.get_status_code();

    std::cout << contents << '\n';
    std::cout << status << '\n';

    return 0;
}