#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <filesystem>
#include <random>
#include <regex>
#include <chrono>
#include <thread>
#include <sstream>

#include "json.hpp"
#include "scftl.hpp"
#include "functions.hpp"

using namespace std;
using namespace scftl;
using JSON = nlohmann::json;

int parsingWork_JSON(string &name_of_sub, bool &update_state){

    ifstream json_array("temp.txt");

    int link_count = 0;
    static int copy = 1;

    try{
        JSON data;

        json_array >> data;

        for (auto& config : data){
            
            string remark = config.value("remarks", "Unnamed Server");

            string clean_remark = clear_remark(remark);

            //after filter
            if (update_state = 0){
                if(filesystem::exists("subs/" + name_of_sub + "/" + clean_remark + ".json")){
                    clean_remark.append("_" + to_string(copy));
                    copy++;
                }
            }

            string file_path = "subs/" + name_of_sub + "/" + clean_remark + ".json";

            ofstream config_file(file_path);

            if (config_file.is_open()) {

                config_file << config.dump(4);
                config_file.close();
                link_count++;
            }

            else{
                cout << "Failed to open file (JSON)" << endl; return 0;
            }
        }
        
        return link_count;
    }

    catch(...){
        cout << "Wrong config (JSON catch)" << endl;
        return 0;
    }
}

void vlessWork(string &name_of_sub, string &line, bool &update_state){
    static int copy = 1;

    for (char &c : line) if (c == '\'') c = ' ';
    
    string cmd = "./x2j -u \'" + line + "\' -o \'subs/" + name_of_sub + "/vless_tmp.json\' > /dev/null 2>&1";


   
    this_thread::sleep_for(chrono::milliseconds(5));
    if(system(cmd.c_str()) == 0){
        ifstream f("subs/" + name_of_sub + "/vless_tmp.json");
        JSON single_data;
        f >> single_data;
        f.close();

        string remark = single_data.value("remarks", "Unknown_Name");

        string clean_name = clear_remark(remark);
        if (clean_name.length() > 120) {
            clean_name = clean_name.substr(0, 120);
        }

        if (update_state = 0){
            if(filesystem::exists("subs/" + name_of_sub + "/" + clean_name + ".json")){
                clean_name.append("_" + to_string(copy));
                copy++;
            }
        }

        filesystem::rename("subs/" + name_of_sub + "/vless_tmp.json", "subs/" + name_of_sub + "/" + clean_name + ".json");
    }
    else{cout << "Bad vless!" << endl;}
}

void vmessWork(string &name_of_sub, string &line, bool &update_state){
    static int copy = 1;
    for (char &c : line) if (c == '\'') c = ' ';
    string cmd = "./x2j -u \'" + line + "\' -o \'subs/" + name_of_sub + "/vmess_tmp.json\' > /dev/null 2>&1";
    this_thread::sleep_for(chrono::milliseconds(10));
    if(system(cmd.c_str()) == 0){
        ifstream f("subs/" + name_of_sub + "/vmess_tmp.json");
        JSON single_data;
        f >> single_data;
        f.close();

        string remark = single_data.value("remarks", "Unknown_Name");

        string clean_name = clear_remark(remark);
        if (clean_name.length() > 120) {
            clean_name = clean_name.substr(0, 120);
        }

        if (update_state = 0){
            if(filesystem::exists("subs/" + name_of_sub + "/" + clean_name + ".json")){
                clean_name.append("_" + to_string(copy));
                copy++;
            }
        }

        // cmd = "mv \"subs/" + name_of_sub + "/vmess_tmp.json\" \"subs/" + name_of_sub + "/" + clean_name + ".json\"";
        // system(cmd.c_str());
        filesystem::rename("subs/" + name_of_sub + "/vmess_tmp.json", "subs/" + name_of_sub + "/" + clean_name + ".json");
    }
    else{cout << "Bad vmess!" << endl;}

}

void trojanWork(string &name_of_sub, string &line, bool &update_state){
    static int copy = 1;
    for (char &c : line) if (c == '\'') c = ' ';
    string cmd = "./x2j -u \'" + line + "\' -o \'subs/" + name_of_sub + "/trojan_tmp.json\' > /dev/null 2>&1";
    this_thread::sleep_for(chrono::milliseconds(10));
    if(system(cmd.c_str()) == 0){
        ifstream f("subs/" + name_of_sub + "/trojan_tmp.json");
        JSON single_data;
        f >> single_data;
        f.close();

        string remark = single_data.value("remarks", "Unknown_Name");

        string clean_name = clear_remark(remark);
        if (clean_name.length() > 120) {
            clean_name = clean_name.substr(0, 120);
        }

        if (update_state = 0){
            if(filesystem::exists("subs/" + name_of_sub + "/" + clean_name + ".json")){
                clean_name.append("_" + to_string(copy));
                copy++;
            }
        }

        filesystem::rename("subs/" + name_of_sub + "/trojan_tmp.json", "subs/" + name_of_sub + "/" + clean_name + ".json");
    }
    else{cout << "Bad trojan!" << endl;}
}

void shadowWork(string &name_of_sub, string &line, bool &update_state){
    static int copy = 1;
    for (char &c : line) if (c == '\'') c = ' ';
    string cmd = "./x2j -u \'" + line + "\' -o \'subs/" + name_of_sub + "/ss_tmp.json\' > /dev/null 2>&1";
    this_thread::sleep_for(chrono::milliseconds(10));
    if(system(cmd.c_str()) == 0){
        ifstream f("subs/" + name_of_sub + "/ss_tmp.json");
        JSON single_data;
        f >> single_data;
        f.close();

        string remark = single_data.value("remarks", "Unknown_Name");

        string clean_name = clear_remark(remark);
        if (clean_name.length() > 120) {
            clean_name = clean_name.substr(0, 120);
        }

        if (update_state = 0){
            if(filesystem::exists("subs/" + name_of_sub + "/" + clean_name + ".json")){
                clean_name.append("_" + to_string(copy));
                copy++;
            }
        }

        filesystem::rename("subs/" + name_of_sub + "/ss_tmp.json", "subs/" + name_of_sub + "/" + clean_name + ".json");
    }
    else{cout << "Bad shadow!" << endl;}
}

void parsingWork(string &sub_link, string &command, string &name_of_sub, bool &update_state){

    int link_count = 0;

    try{
        command.replace(command.find("meow_HWID"), 9, get_fake_hwid());
        command.replace(command.find("another_meow_HWID"), 17, get_fake_hwid());
        command.replace(command.find("LINK"), 4, sub_link);
    }
    catch(...){
        cout << "Stupid niga (Wrong curl link)" << endl << endl;
        cout << "Want to try default link? y/n: ";

        char yn;
        try{
        cin >> yn;
        }
        catch(...){
            cout << "wtf mf" << endl;
            return;
        }
        if (yn == 'y'){
            command = "curl -sL -c cookies.txt -b cookies.txt --max-time 8 "
                    "-A \"Happ/3.1.0 (com.happ.proxy; build:1; iOS 17.4.1) Alamofire/5.8.1\" "
                    "-H \"X-Hwid: " + get_fake_hwid() + "\" "
                    "-H \"X-UI-Request-HWID: " + get_fake_hwid() + "\" "
                    "-H \"Accept: text/plain, */*\" "
                    "-H \"Accept-Language: ru\" "
                    "--compressed "
                    "\"" + sub_link + "\"";    
        }
        else{
            return;
        }   

    }

    cout << "Using command: " << command << endl;

    if (sub_link.find("://") != string::npos) {
        string result = do_cmd(command);
        ofstream pars_file("temp.txt");
        pars_file << result;
        pars_file.close();
    }

    if(filesystem::exists("cookies.txt")) filesystem::remove("cookies.txt");
    
    ifstream check_file("temp.txt");
    string line;
    char firstByte = check_file.get();
    check_file.putback(firstByte); // lol wtf
    while (getline(check_file, line)){

    try{
        //json
        if (firstByte == '[' or firstByte == '{'){

            if (firstByte == '{'){ // one config just copy
                JSON single_data;
                check_file >> single_data;
                check_file.close();

                string remark = single_data.value("remarks", "Unknown_Name");
                
                string clean_remark = clear_remark(remark);

                filesystem::create_directories("subs/" + clean_remark);
                ofstream f("subs/" + clean_remark + "/" + clean_remark + ".json");
                f << single_data;
            }

            else{ 
                check_file.close();
                link_count = parsingWork_JSON(name_of_sub, update_state);
            }
            break;
        }
        
        //others
        else{
            if (line.find("vless://") != string::npos){
                vlessWork(name_of_sub, line, update_state);
                link_count++;
            }

            else if (line.find("vmess://") != string::npos){
                vmessWork(name_of_sub, line, update_state);
                link_count++;
            }

            else if (line.find("trojan://") != string::npos){
                trojanWork(name_of_sub, line, update_state);
                link_count++;
            }
            else if (line.find("ss://") != string::npos){
                shadowWork(name_of_sub, line, update_state);
                link_count++;
            }

            else if (!line.find("/") and !line.find("?") and line.back() == '='){
                    system("mv temp.txt b64temp.txt && base64 -d b64temp.txt > temp.txt"); 
                    filesystem::remove("b64temp.txt");
                    if(filesystem::file_size("temp.txt") > 10){
                        parsingWork(sub_link, command, name_of_sub, update_state);
                    }
                    else{
                        cout << "Wrong link on B64." << endl;
                        continue;
                    }
                }    

                else {cout<< "Unknown type" << endl; continue;}

            }
        }

    catch(...){
        cout << "\nCursed link or not subscription." << endl;
        filesystem::remove_all("subs/" + name_of_sub);
        }
    }
        if (update_state = 0){
            if (link_count == 0){
                filesystem::remove_all("subs/" + name_of_sub);
            }
            cout << "\nAdded " + to_string(link_count) + " servers\n" << endl;
        }
        else{
            cout << "\nUpdated " + to_string(link_count) + " servers\n" << endl;
        }

        filesystem::remove("temp.txt");

        check_file.close();
}

void showSubs(){
    string name;
    int amount;

    cout << "\n";
    cout << "= = = = = = = = = = = = = = = = = = =" << endl;

    for(auto entry : filesystem::directory_iterator("subs")){
        name = entry.path().filename().string();
        amount = distance(filesystem::directory_iterator(entry), filesystem::directory_iterator{}) - 1;
        cout << name << " (" << amount << ")" << endl;
        cout << "= = = = = = = = = = = = = = = = = = =" << endl;
    }
    cout << "\n";
}

void setSettingsValue(string key, string new_value){
    ifstream in("settings.txt");
    ostringstream buf;
    string line;

    while(getline(in, line)){
        if (line.rfind(key+"=", 0) == 0){
            buf << key << "=" << new_value << "\n";
        }
        else{
            buf << line << '\n';
        }
    }

    in.close();
    ofstream f("settings.txt");
    f << buf.str();
}

void advanced(){
    
    int a_choice;

    do{
        cout << "\nAdvanced Settings\n\n1) Change URL (curl)\n2) Change HWID\n3) Edit current config\n4) Edit settings.txt manually\n\n0) Back";
        cout << "\nOption: ";
        cin >> a_choice;

        switch(a_choice){
            case 1:{
                cout << endl;
                string cmd_a = "";
                cout << endl;
                cout << "\nCreator hopes that you are aware of how to build this command.\nYou always can edit file yourself." << endl;

                cout << "\nEnter your link or write 0 to set default: ";

                cin >> cmd_a;

                if (cmd_a != "0"){
                    setSettingsValue("cmd_a", cmd_a);
                }
                else{
                    setSettingsValue("cmd_a", R"(cmd_a=curl -sL -c cookies.txt -b cookies.txt --max-time 8 -A "Happ/3.1.0 (com.happ.proxy; build:1; iOS 17.4.1) Alamofire/5.8.1" -H "X-Hwid: meow_HWID" -H "X-UI-Request-HWID: another_meow_HWID" -H "Accept: text/plain, */*" -H "Accept-Language: ru" --compressed "LINK")");
                    cout << "Default curl command restored\n";
                }
                break;
            }
            case 2:{
                string fake_hwid;
                cout << "\nWrite your new HWID: ";
                cin >> fake_hwid;
                setSettingsValue("fake_hwid", fake_hwid);
                break;
            }
            case 3:{
                fstream f("current.conf");
                string conf_path;
                f >> conf_path;
                string command = "nano subs/" + conf_path;
                system(command.c_str());
                break;
            }
            case 4:
                system("nano settings.txt");
                break;

        }
        
    } while (a_choice != 0);

    cout << "\033[2J\033[1;1H";
}

void deleteSub(){

    cout << "\n";
    int deleting_choice;

    while (deleting_choice != 0){

        vector<filesystem::path> subs;
        int i = 1;

        for (const auto& entry : filesystem::directory_iterator("subs")) {

            if (entry.is_directory()) {
                subs.push_back(entry.path());
                cout << i << ") " << entry.path().filename().string() << endl;
                i++;
            }
        }

        cout << "\n";

        if (subs.empty()) {
            cout << "Nothing to delete.\n";
            deleting_choice = 0;
        }
        else{
        
            cout << "Choose a subscription to delete";
            cout << "\n0) Cancel\nOption: ";

            cin >> deleting_choice;
        }
        
        if (deleting_choice > 0 && deleting_choice <= (int)subs.size()){
            try{
                filesystem::remove_all(subs[deleting_choice - 1]);
                cout << "\033[2J\033[1;1H";
                cout << "Deleted: " << subs[deleting_choice - 1].filename().string() << endl << endl;
                return;
            }
            catch(...){
                cout << "Unknown option" << endl;
            }
        }
        else{
            return;
        }
    }
    
}

void addSub(string sub_link_or_file, string name_of_sub){

    bool update_state;

    if (filesystem::exists("temp.txt")){filesystem::remove("temp.txt");} // if dumbass used ^C (I'm lazy to add SIGINT)

    if (sub_link_or_file.empty()){
        cout << "Enter subsribtion link or path to file: ";
        cin >> sub_link_or_file;
        update_state = 1;
    }

    string fake_hwid = get_fake_hwid();

    string curl_command = get_curl(sub_link_or_file, fake_hwid);
    
    if (!filesystem::exists(sub_link_or_file)){
        if (name_of_sub.empty()){
            cout << "Enter a name for your sub (you can write anything): ";
            cin >> name_of_sub;
        }

        filesystem::create_directories("subs/" + name_of_sub);
        ofstream f("subs/" + name_of_sub + "/" + name_of_sub + "_link");
        f << sub_link_or_file;
        f.close();
                
        parsingWork(sub_link_or_file, curl_command, name_of_sub, update_state);
    }
    else if(filesystem::exists(sub_link_or_file)){

        // 1. Сразу копируем конфиг во временный файл
        filesystem::copy_file(sub_link_or_file, "temp.txt", filesystem::copy_options::overwrite_existing);
        
        // 2. Если имени нет (первый запуск) — берем его у файла
        if (name_of_sub.empty()){
            name_of_sub = filesystem::path(sub_link_or_file).stem().string();
        }

        // 3. Эти действия нужны ВСЕГДА (и для создания, и для апдейта)
        filesystem::create_directories("subs/" + name_of_sub);
        ofstream f("subs/" + name_of_sub + "/" + name_of_sub + "_path");
        f << sub_link_or_file; // Обновляем путь, если вдруг файл переехал
        f.close();

        // 4. Парсим
        parsingWork(sub_link_or_file, curl_command, name_of_sub, update_state);
    }
    else{cout << "Pizdec" << endl; return;}
}

void updateSub(){
    cout << "\n";
    int update_choice = -1;

    while (update_choice != 0){

        vector<filesystem::path> subs;
        int i = 1;

        for (const auto& entry : filesystem::directory_iterator("subs")) {

            if (entry.is_directory()) {
                subs.push_back(entry.path());
                cout << i << ") " << entry.path().filename().string() << endl;
                i++;
            }
        }

        cout << "\n";

        if (subs.empty()) {
            cout << "No subs. Nothing to update.\n";
            update_choice = 0;
            
        }
        else{
        
            cout << "Choose a subscription to update";
            cout << "\n0) Cancel\nOption: ";

            cin >> update_choice;
        }

        if (update_choice > 0 && update_choice <= (int)subs.size()){
            try{
                string line;
                string name = subs[update_choice - 1].filename().string();
                string link = "subs/" + name + "/" + name + "_link";
                string path = "subs/" + name + "/" + name + "_path";
                ifstream f(link);
                try{
                    if(f.is_open()){
                        cout << "\033[2J\033[1;1H";
                        getline(f, line);
                        addSub(line, name);
                    }

                    else{
                        ifstream f(path);
                        if (f.is_open()){
                            cout << "\033[2J\033[1;1H";
                            getline(f, line);
                            addSub(line, name);
                        }
                    }
                }
                catch(...){
                    cout << "Lox ebat'" << endl;
                }

            }
            catch(...){
                cout << "Unknown option" << endl;
            }
        }
        break;

    }

}
