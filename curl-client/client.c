#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>

#define ASIA "/country/asia"
#define AMERICA "/country/america"
#define URL "http://localhost:8080"

#define MAX_BODY_LENGTH 100

// Globals
curl_version_info_data *version_data;
char version_string[MAX_BODY_LENGTH];

size_t write_callback(void* contents, size_t size, size_t nmemb, char ubuf[MAX_BODY_LENGTH]) {
    strncpy(ubuf, contents, MAX_BODY_LENGTH-1);
    return size * nmemb;
}

void make_request(CURL *curl, char *path) {
    char cbuf[100];
    long lbuf;

    if (!curl) {
        printf("Null curl\n");
        exit(1);
    }

    curl_easy_setopt(curl, CURLOPT_URL, path);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, version_string);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, cbuf);

    /* Perform the request */
    curl_easy_perform(curl);

    // Extract info
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lbuf);
    printf("Got: %ld {} : %s\n", lbuf, cbuf);

    /* reset */
    curl_easy_reset(curl);

    /* Sleep */
    sleep(2);
}

int main(int argc, char **argv) {
    CURL *curl;

    // Version info
    version_data = curl_version_info(CURLVERSION_NOW);
    printf("Client using libcurl version: %s\n", version_data->version);
    sprintf(version_string, "C-libcurl-%s", version_data->version);

    // Initialize
    curl = curl_easy_init();

    // Setup paths
    char *asia_path = (char *) malloc(strlen(URL) + strlen(ASIA) + 1);
    sprintf(asia_path, "%s%s", URL, ASIA);
    char *america_path = (char *) malloc(strlen(URL) + strlen(AMERICA) + 1);
    sprintf(america_path, "%s%s", URL, AMERICA);

    while (1) {
        make_request(curl, asia_path);
        make_request(curl, america_path);
    }

}
