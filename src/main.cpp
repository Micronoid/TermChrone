#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>

#include "../include/settings.hpp"
#include "../include/functions.hpp"

using namespace std;

void put_exoskelet(){
 // TODO
}

void startXray() { 

    string active_sub_and_config = get_active_conf();

    put_exoskelet();

    if (active_sub_and_config == "None"){
        cout << "No config choosed. Xray not started." << endl;
    }
    
    cout << "Starting Xray...\n";
    
    // Команда для запуска Xray с тестовым конфигом
    string cmd = "xray -config subs/" + active_sub_and_config + " >> xray.log 2>&1 &";
    int result = system(cmd.c_str());
    this_thread::sleep_for(chrono::milliseconds(100));

    if (result == 0) {
        cout << "Xray started!\n";
    } else {
        cout << "Error starting Xray\n";
    }
}

bool isXrayRunning() {
    int port = get_port_from_conf();
    string cmd = "ss -tlnp | grep -q :" + to_string(port);
    return system(cmd.c_str()) == 0;
}

void stopXray() {   
    cout << "Stopping Xray...\n";
    system("pkill xray");
    cout << "Xray killed.\n";
}

void showMenu() {

    cout << "\033[2J\033[1;1H";
    cout << "     === TermChrone v0.7 ===\n";
    cout << "----------------------------------\n";

    if (isXrayRunning()) {
        cout << "       Status:  [ RUNNING ]\n";
        string conf = get_active_conf(); 
        cout << " Config:  " << conf << "\n";
        int port = get_port_from_conf();
        cout << " Inbounds(port): " << port << endl;

        cout << " IP: " << get_ip(port) << endl; 

        cout << "\n";
    } else {
        cout << "       Status:  [ STOPPED ]\n";
    }

    cout << "----------------------------------\n";
    cout << "1. Start/Stop Xray (" + get_active_conf() + ")\n";
    cout << "2. Check Logs (xray.log)\n"; 
    cout << "3. Choose Config (Subscriptions)\n";
    cout << "4. Settings\n";
    cout << "5. Exit\n";
    cout << "----------------------------------\n";
    cout << "Option: ";
}

void settingsTermChrone(){

    int choice_in_settings;
    
    string sub_link;
    cout << "\033[2J\033[1;1H";

    do {
        cout << " === TermChrone v0.7 ===\n    --- Settings ---\n\n"
        "-------------------\n"
        "1. Show subs\n"
        "2. Update subs\n"
        "3. Add sub\n"
        "4. Delete sub\n"
        "5. Advanced\n"
        "-------------------\n"

        "0. Back" << endl;

        cout << "Option: ";
        cin >> choice_in_settings;
        
        switch(choice_in_settings) {
            case 1:
                cout << "\033[2J\033[1;1H";
                showSubs();
                break;
            case 2:
                cout << "\033[2J\033[1;1H";
                updateSub();
                break;
            case 3:{
                cout << "\033[2J\033[1;1H";
                string kostil, lol;
                addSub(kostil, lol);
                break;
            }
            case 4:
                cout << "\033[2J\033[1;1H";
                deleteSub();
                break;
            case 5:
                cout << "\033[2J\033[1;1H";
                advanced();
                break;
            case 0:
                break;            
            default:
                cout << "Unknown option.\n";
        }
    } while (choice_in_settings != 0);

}

void ui(){
    int choice;
    string active_sub, active_server = "None";
    
    do {
        showMenu();
        cin >> choice;
        switch(choice) {
            case 1:
                if (isXrayRunning()) {
                    stopXray();
                } else {
                    if (get_active_conf() == "None") {
                        cout << "\a\n[!] Error: Please choose config first (Option 3)\n";
                        cin.ignore(); cin.get();
                    } else {
                        startXray();
                    }
                }
                break;
            case 2:
                if (isXrayRunning()) {
                    cout << "Xray is running on port: " << get_port_from_conf() << endl;
                    system("tail -50 xray.log");
                } else {
                    cout << "Xray not started\n";
                }
                cout << "Press Enter to close...    ";
                cin.ignore(); 
                cin.get(); 
                break;
            case 3:
                choosing_conf();
                break;
            case 4:
                settingsTermChrone();
                break;
            case 5:
                stopXray();
                cout << "Bye bye!\n";
                filesystem::remove("xray.log");
                break;
            default:
                cout << "Unknown option.\n";
        
        } 
    } while (choice != 5);
}

int main(int argc, char* argv[]) {

    filesystem::path build_path = filesystem::canonical(argv[0]).parent_path();
    
    filesystem::current_path(build_path);

    
    if (!filesystem::exists("settings.txt")){
        make_settings_file();
    }
    if (!filesystem::exists("subs")){
        filesystem::create_directory("subs");
    }

    ui();
    
    return 0;
}