#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <random>
#include <regex>
#include <chrono>
#include <thread>
#include <csignal>

#include "scftl.hpp"
#include "json.hpp"

using namespace std;
using namespace scftl;
using JSON = nlohmann::json;

string get_curl(string &sub_link_or_file, string &fake_hwid){
    //кастом ссылка для curl
    string default_curl = "curl -sL -c cookies.txt -b cookies.txt --max-time 8 "
                    "-A \"Happ/3.1.0 (com.happ.proxy; build:1; iOS 17.4.1) Alamofire/5.8.1\" "
                    "-H \"X-Hwid: " + fake_hwid + "\" "
                    "-H \"X-UI-Request-HWID: " + fake_hwid + "\" "
                    "-H \"Accept: text/plain, */*\" "
                    "-H \"Accept-Language: ru\" "
                    "--compressed "
                    "\"" + sub_link_or_file + "\"";
    string curl_command = "";
    string line;
    ifstream f("settings.txt");
    while(getline(f, line)){
        if(line.find("cmd_a") != string::npos){
            size_t pos = line.find('=');
            curl_command = line.substr(pos+1);
            break;
        }
    }

    if (curl_command == "" or curl_command == "default"){
        curl_command = default_curl;
    }
    return curl_command;
}

string get_fake_hwid(){
    
    string hwid;
    string line;

    ifstream f("settings.txt");
    while(getline(f, line)){
        if(line.find("fake_hwid") != string::npos){
            size_t pos = line.find('=');
            hwid = line.substr(pos+1);
            break;
        }
    }

    return hwid;
}

string create_fake_hwid(){

    string hwid;
    size_t length = 16;
    const string characters = "0123456789abcdefghijklmnopqrstuvwxyz";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, characters.size() - 1);

    hwid = "TC-";
    for (size_t i = 0; i < length; ++i) {
        hwid += characters[distribution(generator)];
    }
    
    ofstream outfile("fake.hwid");
    if (outfile.is_open()) {
        outfile << hwid;
        outfile.close();
    }

    return hwid;
}

string url_decode(string text) {
    string decoded;

    for (size_t i = 0; i < text.length(); ++i) {
        if (text[i] == '%' && i + 2 < text.length()) {
            // Берем две цифры после %, переводим из Hex в int
            int value;
            stringstream ss;
            ss << hex << text.substr(i + 1, 2);
            ss >> value;
            decoded += static_cast<char>(value);
            i += 2; // Пропускаем обработанные цифры
        } else if (text[i] == '+') {
            decoded += ' '; // В URL плюс часто означает пробел
        } else {
            decoded += text[i];
        }
    }

    return decoded;
}

string clear_remark(string dirty_remark) {
    // 1. флаги → коды
    dirty_remark = replaceFlagsToCode(dirty_remark);

    // 2. убираем эмодзи, кириллицу оставляем
    auto cp = utf8ToCodepoints(dirty_remark);
    string result;
    for (uint32_t c : cp) {
        if (c >= 0x1F000 && c <= 0x1FFFF) continue;
        if (c >= 0x2600  && c <= 0x27BF)  continue;
        if (c == 0x200D)                   continue;
        if (c >= 0xFE00  && c <= 0xFE0F)  continue;
        result += codepointToUtf8(c);
    }

    // 3. оставляем только нужные символы, остальное → _
    string clean;
    for (unsigned char c : result) {
        if (isalnum(c) || c == '.' || c == '-' || c == '_') {
            clean += c;
        } else if (c >= 0x80) {
            clean += c; // кириллица и прочие многобайтные
        } else {
            if (!clean.empty() && clean.back() != '_')
                clean += '_';
        }
    }

    // 5. несколько _ подряд → один _
    static regex multi_underscore("_+");
    clean = regex_replace(clean, multi_underscore, "_");

    // 6. убрать _ в начале и конце
    while (!clean.empty() && clean.front() == '_') clean.erase(0, 1);
    while (!clean.empty() && clean.back() == '_')  clean.pop_back();

    return clean;
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\v\f");
    if (start == string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r\v\f");
    return str.substr(start, end - start + 1);
}

string do_cmd(string cmd){
    
    FILE* commandeus = popen(cmd.c_str(), "r");
    if (!commandeus){return "";}

    char buf[256];
    string result;

    while (fgets(buf, sizeof(buf), commandeus)){
        result += buf;
    }

    pclose(commandeus);

    result = trim(result);

    return result;
}

string get_active_conf(){
    string line;

    if (filesystem::exists("./current.conf")){
        ifstream f("./current.conf");
        getline(f, line);
        f.close();
        return line;
    }

    else{
        cout << "No active config." << endl;
        return "None";
    }
}

string get_ip(int port){


    string curl_response = do_cmd("curl --connect-timeout 5 -s -x 127.0.0.1:" + to_string(port) + " https://ipinfo.io/json | jq -r '.ip, .country'");    
    
    if (!curl_response.empty()){
        
        size_t pos;

        while ((pos = curl_response.find('\n')) != string::npos) {
            curl_response.replace(pos, 1, " ");
        }
        
        curl_response = replaceCodeToWord(curl_response);
    } 
    else{
        curl_response = "Unknown (timeout)";
    }
    
    return curl_response;
}

int get_port_from_conf() {
    string path = get_active_conf(); // получаем путь к текущему конфигу
    if (path == "None") return 1080; // дефолт

    // Команда вытащит первое число после слова "port"
    string cmd = "grep '\"port\"' subs/" + path + " | head -n 1 | grep -oE '[0-9]+'";    
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (pipe) {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) result += buffer;
        pclose(pipe);
    }

    return result.empty() ? 1080 : stoi(result);
}

string pingServers(string sub, string config){

    string ping_ms;
    string ready_server;
    string ready_address;
    bool isIpForced = false;


    ifstream f("subs/" + sub + "/" + config);
    string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());

        if (content.find("IPIfNonMatch") != string::npos || 
            content.find("IPOnDemand") != string::npos) {
            isIpForced = true;
        }
        size_t pos = content.rfind("\"address\"");
        if(pos != string::npos){
            size_t start = content.find("\"", pos + 10) + 1;
            size_t end = content.find("\"", start);
            ready_server = content.substr(start, end - start);
            }
        else{
            return "error: no address";
        }

    if(isIpForced == true){
        ready_address = do_cmd("dig +short "+ ready_server +" 2>/dev/null | head -1");
        if(ready_address.empty()){
            return "err | dns";
        }
    }
    else{
        ready_address = ready_server;
    }

    ping_ms = do_cmd("timeout 4 curl -o /dev/null -s -w '%{time_total}' -k https://" + ready_address + "/ 2>/dev/null || echo 'TIMEOUT'");

    try {
        if (ping_ms != "TIMEOUT"){
            float seconds = stof(ping_ms);
            if (seconds < 0.0001){return "dead?";}
            if (seconds >= 3){return "T/O";}
            ping_ms = to_string(static_cast<int>(seconds * 1000));
        }
        else{
            return "T/O";
        }
    } catch (...) {
        return "err | pizdec";
    }

    return ping_ms;
}

static sig_atomic_t volatile keep_pinging = 1;
void sig_h(int sig){
    cout << "\nStopping wait a sec...\n";
    keep_pinging = 0;
}

void choosing_conf() {
    vector<string> subs;
    
    // get subs
    for (auto &entry : filesystem::directory_iterator("subs")) {
        if (entry.is_directory()) {
            subs.push_back(entry.path().filename().string());
        }
    }
    
    // showing subs
    int i = 1;
    for (string sub_c : subs) {
        cout << i << ") " << sub_c << endl;
        i++;
    }
    cout << "\n0) Cancel\nChoose sub: ";
    
    int sub_choice;
    cin >> sub_choice;
    
    if (sub_choice == 0) return;
    if (sub_choice < 1 || sub_choice > (int)subs.size()) {
        cout << "Wrong option\n";
        choosing_conf();
        return;
    }
    
    string sub = subs[sub_choice - 1];
    
    vector<string> configs;
    for (auto &entry : filesystem::directory_iterator("subs/" + sub)) {
        string filename = entry.path().filename().string();
        if (filename.find("_link") == string::npos && filename.find("_path") == string::npos) {
            configs.push_back(filename);
        }
    }
    
    int choice;
    cout << "\n--- Configs in [" << sub << "] ---\n";
    for (size_t j = 0; j < configs.size(); j++) {
        cout << j + 1 << ") " << configs[j] << endl;
    }
    cout << "\n-1) Ping all servers\n0) Exit\nChoose conf: ";
    cin >> choice;
        
    while (true) {
        
        if (choice == 0) {
            return;
        }
        else if (choice == -1) {

            bool pizdatiy_kostile_user_interrupt = 0;

            keep_pinging = 1;

            signal(SIGINT, sig_h); // TODO xray падает сука
            signal(SIGTERM, sig_h);
            
            cout << "\033[2J\033[1;1H";

            cout << "\nPinging servers... (press ^C to stop)\n" << endl;

            for (size_t j = 0; j < configs.size(); j++) {
                if (!keep_pinging){
                    pizdatiy_kostile_user_interrupt = 1;
                    signal(SIGINT, SIG_DFL);  // SIG_DFL = Default
                    signal(SIGTERM, SIG_DFL);
                    break;
                }
                string ping = pingServers(sub, configs[j]);
                cout << j + 1 << ") " << configs[j] << " _| " << (ping=="T/O" ? "T/O" : ping + " ms") << " |_ "<< endl;
            }
            keep_pinging = 0;
            signal(SIGINT, SIG_DFL);  // SIG_DFL = Default
            signal(SIGTERM, SIG_DFL);

            if(pizdatiy_kostile_user_interrupt){
                cout << "\n--- Configs in [" << sub << "] ---\n";
                for (size_t j = 0; j < configs.size(); j++) {
                    cout << j + 1 << ") " << configs[j] << endl;
                }

            }

            cout << "\n-1) Ping all servers\n0) Exit\nChoose conf: ";
            cin >> choice;
            continue;
        }
        else if (choice >= 1 && choice <= (int)configs.size()) {

            string config_path = sub + "/" + configs[choice - 1];
            ofstream f("./current.conf");
            f << config_path;
            f.close();
            cout << "Selected: " << config_path << endl;
            return;
        }
        else {
            cout << "Wrong option\n";
        }
    }
}

void make_settings_file(){
    ofstream f("settings.txt");
            
            f << "# Your beautiful hwid.\n";
            f << "fake_hwid=" + create_fake_hwid() + "\n";
            f << "# Curl url, idk what else to say read man curl :P\n";
            f << "# Example: curl https://funny_link/sub/bruh\n";
            f << "# Editing url gives you opportunity to show or give the service whatever it wants \n# to return you a valid config or link to configs\n";
            f << R"(# Default command is: cmd_a=curl -sL -c cookies.txt -b cookies.txt --max-time 8 -A "Happ/3.1.0 (com.happ.proxy; build:1; iOS 17.4.1) Alamofire/5.8.1" -H "X-Hwid: meow_HWID" -H "X-UI-Request-HWID: another_meow_HWID" -H "Accept: text/plain, */*" -H "Accept-Language: ru" --compressed "LINK")" << "\n";
            f << R"(cmd_a=default)";            
            f << "\n";
            f << "\n"; // ping_type here
            f << "# Easter egg";

            f.close();
}


