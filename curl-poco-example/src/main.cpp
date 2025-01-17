#include <iomanip>
#include <iostream>
#include <curl/curl.h>
#include <Poco/JSON/Parser.h>

const std::string API_KEY = "&key=fee00b0d52c7cd8f170a09ce4785218a70523396";
const std::string COUNTRIES = "&for=genc+standard+countries+and+areas:US,CN,IN";

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = static_cast<struct MemoryStruct *>(userp);

    char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));
    if(!ptr) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main() {
    CURL *curlHandle;
    CURLcode res;

    struct MemoryStruct chunk = {nullptr, 0};

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curlHandle = curl_easy_init();
    if (curlHandle) {
        std::string urlString = "https://api.census.gov/data/timeseries/idb/5year?get=NAME,POP&YR=2020:2024" +
                                API_KEY + COUNTRIES;
        curl_easy_setopt(curlHandle, CURLOPT_URL, urlString.c_str());

        /* send all data to this function  */
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)&chunk);

        /* some servers do not like requests that are made without a user-agent
           field, so we provide one */
        curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "sheldon-libcurl-agent/1.0");

        /* Perform the request, res gets the return code */
        res = curl_easy_perform(curlHandle);
        /* Check for errors */
        if (res != CURLE_OK) {
            std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Received bytes: " << chunk.size << std::endl;
        }

        /* always cleanup */
        curl_easy_cleanup(curlHandle);
    }

    curl_global_cleanup();

    // Parse the JSON
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(chunk.memory);
    Poco::JSON::Stringifier::stringify(result, std::cout, 4);
    std::cout << std::endl;

    return 0;
}
