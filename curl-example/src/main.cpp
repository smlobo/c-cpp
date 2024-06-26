#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <curl/curl.h>
#include <json/value.h>
#include <json/reader.h>

const std::string API_KEY = "&key=fee00b0d52c7cd8f170a09ce4785218a70523396";
const std::string COUNTRIES = "&for=genc+standard+countries+and+areas:US,CN,IN";

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char *) realloc(mem->memory, mem->size + realsize + 1);
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

    // Parse with json cpp
    Json::Value root;
    Json::Reader reader;
    reader.parse(chunk.memory, root);

    // JSON to our map
    std::unordered_map<unsigned, std::map<std::string, unsigned>> yearCountryMap;
    // Skip the 0 index - this is the title
    for (Json::Value::ArrayIndex i = 1; i != root.size(); i++) {
        Json::Value element = root[i];
        std::cout << "[" << i << "] ";
        for (const Json::Value &j : element) {
            std::cout << j.asString() << ", ";
        }
        std::cout << std::endl;

        // 0 -> Country Name, 1 -> Population, 2 -> Year
        unsigned year = std::stoi(element[2].asString());
        std::map<std::string, unsigned> &countryMap = yearCountryMap[year];
        countryMap[element[0].asString()] = std::stoi(element[1].asString());

    }

    // Pretty print
    std::cout << "Population:\n";
    bool title = false;
    for (const std::pair<const unsigned, std::map<std::string, unsigned>>& yearElement : yearCountryMap) {
        // Print the country title
        if (!title) {
            std::cout << "\t";
            for (std::pair<const std::string, unsigned> countryElement : yearElement.second) {
                std::cout << countryElement.first << "\t\t";
            }
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            title = true;
        }
        std::cout << yearElement.first << "\t";
        for (std::pair<const std::string, unsigned> countryElement : yearElement.second) {
            std::cout << countryElement.second << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}
