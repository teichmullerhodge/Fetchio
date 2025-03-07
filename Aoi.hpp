#ifndef AOI_HPP
#define AOI_HPP


#include <curl/curl.h>
#include <future>
#include <vector>
#include <string>
#include "logger.hpp"


typedef std::vector<std::string> aoiheader;
typedef std::future<std::string> aoiresponse;
typedef std::string              aoibody; 
typedef std::string              aoiurl;
typedef CURL*                    aoiengine;


#define ASYNC true
#define SYNC false

#define DEFAULT_HEADERS aoiheader{"Content-Type : application/json"}

static size_t write_function(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



class Aoi {

    private:

        constexpr static bool response_ok(long status){
            // response_ok:
            // sub rdi, 200
            // cmp rdi, 99
            // setle al
            // ret
            return (status - 200) <= 99;
        
    }

      
    public:

        static aoiengine start(){
            curl_global_init(CURL_GLOBAL_ALL);   
            return curl_easy_init();
        }
        static void release(aoiengine engine){
            curl_easy_cleanup(engine);
            curl_global_cleanup();
         
        }

        

        static std::string perform_get(CURL *c, std::string url, aoiheader __HEADERS, bool isAsync){


            std::string READ_BUFFER;
            if(isAsync){

                c = curl_easy_init();
            }
            if(c){
                curl_easy_setopt(c, CURLOPT_URL, url.c_str());
                struct curl_slist *headers = NULL;

                for(auto& v : __HEADERS){
                    
                    headers = curl_slist_append(headers, v.c_str()); 

                }

                curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_function);
                curl_easy_setopt(c, CURLOPT_WRITEDATA, &READ_BUFFER);
                CURLcode res = curl_easy_perform(c);
                if(res != CURLE_OK) {
                    Logger::log_error("CURL_PERFORM_FAILED: ");
                    Logger::log_error(curl_easy_strerror(res));
                }

                long HTPPCODE = 0;
                curl_easy_getinfo (c, CURLINFO_RESPONSE_CODE, &HTPPCODE);

                if(response_ok(HTPPCODE)){

                        curl_slist_free_all(headers);
                        return READ_BUFFER;

                }

                curl_slist_free_all(headers);
                return READ_BUFFER;
            }
            Logger::log_error("Failed initializing curl!\n");
            return "";
        }

        static std::string perform_post(CURL *c, std::string url, aoiheader __HEADERS, std::string body, bool isAsync){
            std::string payload = body;
            std::string READ_BUFFER;
            if(isAsync){

                c = curl_easy_init();
            }
            if(c){

                curl_easy_setopt(c, CURLOPT_URL, url.c_str());
                struct curl_slist *headers = NULL;

                for(auto& v : __HEADERS){
                    
                    headers = curl_slist_append(headers, v.c_str()); 

                }

                curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_function);

                curl_easy_setopt(c, CURLOPT_WRITEDATA, &READ_BUFFER);
                curl_easy_setopt(c, CURLOPT_POST, 1L);
                curl_easy_setopt(c, CURLOPT_POSTFIELDS, payload.c_str());
                CURLcode res = curl_easy_perform(c);
                if(res != CURLE_OK) {
                    Logger::log_error("CURL_PERFORM_FAILED: ");
                    Logger::log_error(curl_easy_strerror(res));
                }

                long HTPPCODE = 0;
                curl_easy_getinfo (c, CURLINFO_RESPONSE_CODE, &HTPPCODE);

                if(response_ok(HTPPCODE)){

                        curl_slist_free_all(headers);
                        return READ_BUFFER;

                }

                curl_slist_free_all(headers);
                return READ_BUFFER;

            }

            return "";

        }


        static std::vector<aoiresponse> get_as_batch(CURL* curl, std::vector<aoiurl> urls, std::vector<aoiheader> __HEADERS, bool isAsync){

            std::vector<aoiresponse> promises{};
            promises.reserve(urls.size());

            for(size_t i = 0; i < urls.size(); ++i){
            

                std::cout << urls[i] << "\n";
                promises.emplace_back(std::async(std::launch::async, &Aoi::get, curl, urls[i], __HEADERS[i], isAsync));


            }

            return promises;

        }
        static std::vector<aoiresponse> post_as_batch(CURL* curl, std::vector<std::string> urls, std::vector<aoiheader> __HEADERS, std::vector<std::string> __PAYLOADS, bool isAsync){

            std::vector<aoiresponse> promises{};
            promises.reserve(urls.size());

            for(size_t i = 0; i < urls.size(); ++i){

                promises.emplace_back(std::async(std::launch::async, &Aoi::perform_post, curl, urls[i], __HEADERS[i], __PAYLOADS[i], isAsync));

            }

            return promises;

        }

        static std::string get(CURL* c, std::string url, aoiheader headers, bool isAsync){

            return Aoi::perform_get(c, url, headers, isAsync);
        }

        static std::string post(CURL* c, std::string url, aoiheader headers, std::string payload, bool isAsync){

            return Aoi::perform_post(c, url, headers, payload, isAsync);

        }

};

#endif