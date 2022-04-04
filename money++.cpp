// a program that save & show your past spending 
// name: money++
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <windows.h>

#define ACCOUNT "zard@zardforever"
#define USERNAME "zard"
#define PROGRAMNAME "money++"
#define PROGRAMVERSION "Name: money++ 1.0.1\nPurpose: money tracking\nCreator: zardforever\nLink: https://github.com/ZardForever1009/money-plus-plus\n"  

using namespace std;

// FOREGROUND_RED|BACKGROUND_BLUE| ......
void change_font_color(WORD c) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    return;
}

// show message (no newline character)
void show_message(string stats, string line, bool new_line){
    if(stats=="success"){                // green output
        change_font_color(FOREGROUND_GREEN);
        cout<<line;
        change_font_color(7);
    }
    else if(stats=="input"||stats=="filename"){  // yellow output
        change_font_color(FOREGROUND_RED|FOREGROUND_GREEN);
        cout<<line;
        change_font_color(7);
    }
    else if(stats=="hightlight"){  // backgroung green output
        change_font_color(BACKGROUND_GREEN);
        cout<<line;
        change_font_color(7);
    }
    else if(stats=="idle"){         // green color
        change_font_color(FOREGROUND_GREEN);
        cout<<line;
        change_font_color(7);
        cout<<": ";        
    }                               // normal white color
    else if(stats=="failed"||stats=="output")cout<<line;
    else{                     
        change_font_color(FOREGROUND_RED);
        cout<<"No matched stats instruction"<<endl;
        change_font_color(7);
        return;
    }
    if(new_line)cout<<endl;
    return;
}

// get user input & analyze
string get_user_input(bool only_integer, string message){
    string input;
    if(only_integer){ // need to check validity
        bool okay=false;        
        while(!okay){
            show_message("input", message, false);
            getline(cin, input);
            for(int i=0;i<input.size();i++){
                if(!isdigit(input[i])){
                    show_message("failed", input+": invalid input", false);                    
                    break;
                }
                else if(i==input.size()-1)okay=true;
            }            
        }
    }
    else{
        show_message("input", message, false);
        cin>>input;
    }
    return input;        
}

// get current time to store in file
string get_current_time(){
    auto start = std::chrono::system_clock::now();
    // Some computation here
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    return ctime(&end_time);
}

// get all file name output
void ls(){
    show_message("filename", "[earn.txt] ", false);
    show_message("filename", "[spent.txt] ", false);
    show_message("filename", "[total.txt]", true);
    return;
}

// list all data saved
void show_file_content(string filename){
    string line;
    ifstream file;
    file.open(filename, ios::in);
    if(!file.is_open())show_message("failed", filename+": No such file", true);
    else while(getline(file, line))show_message("output", line, true);
    file.close();
    return;
}

// whole operation combination
void money_plus_plus(){
    bool leave=false;
    while(!leave){
        show_message("idle", ACCOUNT, false);
        string action=get_user_input(false, "");
        if(action=="clear")system("cls");
        else if(action=="leave")leave=true;
        else if(action=="ls")ls();        
        else if(action=="help")show_file_content("help.txt");
        else if(action=="insert");
        else if(action=="find");
        else if(action=="whoami")show_message("output", USERNAME, true);
        else if(action=="whoru")show_message("output", PROGRAMNAME, true);
        else if(action=="money++"){
            string detail;
            cin>>detail;
            if(detail=="-v")show_message("output", PROGRAMVERSION, true);
            else show_file_content(detail);
        }
        else{ // more instruction are good to add above
            show_message("failed", action+": command not found", true);
        }
    }
    return;
}


int main()
{
    money_plus_plus();
    
    return 0; 
}
