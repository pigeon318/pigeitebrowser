#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include "html_parser.cpp"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

int main(int argc, char* argv[]) {
    CURLcode g = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (g != CURLE_OK) {
        std::cerr << "curl_global_init failed: " << curl_easy_strerror(g) << '\n';
        return 1;
    }

    std::cout << "=== Pigeite Browser (Engine Test) ===\n";
    std::cout << "Type a URL to fetch or 'exit' to quit.\n\n";

    while (true) {
        std::string url;
        if (argc > 1) {
            url = argv[1];
            argc = 1; // only use argv once
        }
        else {
            std::cout << "URL: ";
            std::getline(std::cin, url);
        }

        if (url.empty()) continue;
        if (url == "exit" || url == "quit") break;

        if (url.find("http://") != 0 && url.find("https://") != 0) {
            url = "https://" + url; // add https automatically if missing
        }

        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "curl_easy_init() failed\n";
            break;
        }

        std::string buf;
        char errbuf[CURL_ERROR_SIZE];
        std::memset(errbuf, 0, sizeof(errbuf));

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "pigeitebrowser/1.0");

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Request failed: " << curl_easy_strerror(res);
            if (errbuf[0]) std::cerr << " (" << errbuf << ")";
            std::cerr << '\n';
        }
        else {
            Node* dom = parseHTML(buf);
            printDOM(dom);
        }


        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    std::cout << "\nExiting Pigeite Browser.\n";
    return 0;
}
