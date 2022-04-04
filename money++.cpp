// a program that save & show your past spending 
// name: money++
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <windows.h>

#define ACCOUNT "zard@zardforever"
#define USERNAME "zard"
#define PWD "mTeWAbAb:hXff\aZcjW"
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
    else if(stats=="hightlight"){  // background green output
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
stringstream get_user_input(string message, bool only_integer, int max_size){
     string input;
     stringstream result;   
     bool okay=false;     
     if(only_integer){  
         while(!okay){
             show_message("input", message, false);
             getline(cin, input);             
             for(int i=0;i<input.size();i++){
                 if(!isdigit(input[i])){
                     show_message("failed", input+": invalid input",true);
                     break;
                 }
                 else if(i==input.size()-1)okay=true;
                 else;
             }
             if(input.size()>max_size){
                 show_message("failed", "failed: too much letters",true);
                 okay=false;
             }
         }
         result<<input;
     }
     else{
        while(!okay){
           show_message("input", message, false);
           getline(cin, input);           
           if(input.size()>max_size)show_message("failed", "failed: too much letters",true);
           else okay=true;
        }                 
        result<<input;
     }
     return result;
}

// yes or not
bool yes_or_not(string ques){
    string input;
    while(true){
        show_message("output", ques, false);
        getline(cin, input);
        if(input=="Y"||input=="y")return true;
        else if(input=="n"||input=="N")return false;
        else show_message("output", "failed: Only 'y' or 'n'", true);
    }
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
    show_message("filename", "[money.txt] ", false);   
    show_message("hightlight", "[help.txt]", true);
    return;
}

// list all data saved
bool show_file_content(string filename){
    string line;
    ifstream file;
    file.open(filename, ios::in);
    if(!file.is_open()){
        show_message("failed", filename+": No such file", true);
        return false;
    }
    else while(getline(file, line))show_message("output", line, true);
    file.close();
    return true;
}

// enter password and check
bool pwd_check(string username, string pwd, int pwd_leng){
    string input, real_pwd="";
    for(int i=0;i<pwd_leng;i++)real_pwd.push_back((pwd[i]+13));
    for(int i=1;i<=3;i++){
        show_message("output", "[money++] password for "+username+": ", false);
        getline(cin, input);
        if(real_pwd==input)return true;
        else if(i!=3)show_message("failed", "Sorry, try again.",true);
        else;
    }
    show_message("failed", "failed: No attempt left\n", true);
    return false;
}

// format beautiful
stringstream format_beautiful(string date, string income, string expense, string details, string last_modified){
    stringstream result;
    result<<date;
    int income_size=8-income.length();
    int expense_size=8-expense.length();
    int details_size=2;
    int last_size=50-details.length();
    int size[4]={income_size,expense_size,details_size,last_size};
    string item[4]={income,expense,details,last_modified};
    for(int i=0;i<4;i++){        
        for(int j=0;j<size[i];j++){
            result<<" ";
        }
        result<<item[i];
    }
    return result;    
}

// append data 
void append(string filename){
    show_message("success", "==========APPEND MODE==========", true);
    // get data
    string date, income, expense, details, last_modified;
    date=get_user_input("Date: ", false, 10).str();
    income=get_user_input("Income: ", true, 7).str();
    expense=get_user_input("Expense: ", true, 7).str();
    details=get_user_input("Details: ", false, 45).str();    
    last_modified=get_current_time();
    stringstream ss=format_beautiful(date, income, expense, details, last_modified);
    bool conti=yes_or_not("Do you want to continue? [Y/n] ");
    if(conti==false)return; // no more continuous move
    pwd_check(USERNAME, PWD, 4);
    // append data
    ofstream file;
    file.open(filename, ios_base::app);
    file<<ss.str();
    file.close();
    show_message("success", "======UPDATE SUCCESSFULLY======", true);
    return;
}

// find data in file
void find(string filename, string data){
    ifstream file;
    file.open(filename, ios::in);
    string line;
    if(data==""){
        while(getline(file, line))show_message("output", line, true);
        return;
    }
    while(getline(file, line)){  
        if(line.find(data) == string::npos)show_message("output", line, true);
        else show_message("hightlight", line, true);
    }
    file.close();
    return;
}

// whole operation combination
void money_plus_plus(){
    bool leave=false;
    while(!leave){
        show_message("idle", ACCOUNT, false);
        string command, detail;
        stringstream input=get_user_input("", false, 1000);
        input>>command;
        input>>detail;
        if(command=="clear"&&detail=="")system("cls");
        else if(command=="leave"&&detail=="")leave=true;
        else if(command=="ls"&&detail=="")ls();        
        else if(command=="help"&&detail=="")show_file_content("help.txt");
        else if(command=="append"&&detail=="")append("money.txt");
        else if(command=="find")find("money.txt", detail);
        else if(command=="whoami"&&detail=="")show_message("output", USERNAME, true);
        else if(command=="whoru"&&detail=="")show_message("output", PROGRAMNAME, true);
        else if(command=="money++"){            
            if(detail=="--version")show_message("output", PROGRAMVERSION, true);
            else show_file_content(detail);
        }
        else{ // more instruction are good to add above
            show_message("failed", input.str()+": command not found", true);
        }
    }
    return;
}


int main()
{
    money_plus_plus();
    return 0; 
}
