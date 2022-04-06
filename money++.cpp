// a program that save & show your past spending 
// name: money++
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <sys\stat.h>
#include <dirent.h>
#include <conio.h>
#include <chrono>
#include <ctime>
#include <windows.h>

#define ACCOUNT "zard@zardforever"
#define USERNAME "zard"
#define PWD "mTeWAbAb:hXff\aZcjW"
#define PROGRAM_NAME "money++"
#define PROGRAM_VERSION "Name: money++ 1.0.1\nPurpose: money tracking\nCreator: zardforever\nLink: https://github.com/ZardForever1009/money-plus-plus\n"  

using namespace std;

// FOREGROUND_RED|BACKGROUND_BLUE| ......
void change_font_color(WORD c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    return;
}

// show message (no newline character)
void show_message(const string stats, string line, bool new_line, string current_path){
    if(stats=="success"){                // green output
        change_font_color(FOREGROUND_GREEN);
        cout<<line;
        change_font_color(7);
    }
    else if(stats=="input"||stats=="file"){  // yellow output
        change_font_color(FOREGROUND_RED|FOREGROUND_GREEN);
        cout<<line;
        change_font_color(7);
    }
    else if(stats=="highlight"){  // background green output
        change_font_color(BACKGROUND_GREEN);
        cout<<line;
        change_font_color(7);
    }
    else if(stats=="idle"){         // green color
        change_font_color(FOREGROUND_INTENSITY|FOREGROUND_GREEN);
        cout<<line;
        change_font_color(7);
        cout<<":";
		change_font_color(FOREGROUND_INTENSITY|FOREGROUND_BLUE);
		if(current_path=="")cout<<"~";
		else cout<<current_path;
		change_font_color(7);
        cout<<"$ ";
    }                               // normal white color
    else if(stats=="failed"||stats=="output"){
		cout<<line;
	}
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
stringstream get_user_input(const string message, bool only_integer, int min_size, int max_size){
     string input;
     stringstream result;   
     bool okay=false;     
     if(only_integer){  
         while(!okay){
			 REINPUT:
             show_message("input", message, false, "");
             getline(cin, input);             
             for(int i=0;i<input.size();i++){
                 if(!isdigit(input[i])){
                     show_message("failed", input+": invalid input",true, "");
                     goto REINPUT;
                 }
                 else if(i==input.size()-1)okay=true;
                 else;
             }
             if(input.size()>max_size||input.size()<min_size){
                 show_message("failed", "failed: invalid input",true, "");
                 okay=false;
             }
         }
         result<<input;
     }
     else{
        while(!okay){
           show_message("input", message, false, "");
           getline(cin, input);           
           if(input.size()>max_size||input.size()<min_size)show_message("failed", "failed: invalid input",true, "");
           else okay=true;
        }                 
        result<<input;
     }
     return result;
}

// yes or not
bool yes_or_not(const string ques){
    string input;
    while(true){
        show_message("output", ques, false, "");
        getline(cin, input);
        if(input=="Y"||input=="y")return true;
        else if(input=="n"||input=="N")return false;
        else show_message("output", "failed: Only 'y' or 'n'", true, "");
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
stringstream ls(bool show){
    struct dirent *d;
    struct stat dst;    
    DIR *dr;    
    string path = ".\\";    
    dr = opendir(path.c_str());
	stringstream ss;    
    if (dr != nullptr){
        for (d = readdir(dr); d != nullptr; d = readdir(dr)){
            string type = d->d_name;
            if (stat(type.c_str(), &dst) == 0){                       
                if(dst.st_mode & S_IFREG){
					ss<<type<<" ";
					if(show){
						show_message("file", type, false, "");
						cout<<" ";
					}
				}
            }
        }
		if(show)cout<<endl;
        closedir(dr);
    }
    else show_message("failed", "failed: 'ls' operation error", true, "");
    return ss;
}

// list all data saved
bool show_file_content(const string filename, bool with_num){
    string line;
    ifstream file;
    file.open(filename, ios::in);
    if(!file.is_open()){
        show_message("failed", filename+": No such file", true, "");
        return false;
    }
    else {
		if(with_num){
			int num=1;
			getline(file, line);
			getline(file, line);
			while(getline(file, line)){
				show_message("output", line+" --> "+to_string(num), true, "");
				num++;
			}
		}
		else{
			while(getline(file, line)){
				show_message("output", line, true, "");
			}
		}
	}
    file.close();
    return true;
}

// enter password and check
bool pword_check(string username, string pwd, int pwd_leng){
    string input, real_pwd="";
    for(int i=0;i<pwd_leng;i++)real_pwd.push_back((pwd[i]+13));
    for(int i=1;i<=3;i++){
        show_message("output", "[money++] password for "+username+": ", false, "");
        HANDLE hStdInput=GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode = 0;
        // Create a restore point Mode
        // is know 503
        GetConsoleMode(hStdInput, &mode);
        // Enable echo input
        // set to 499
        SetConsoleMode(hStdInput,mode & (~ENABLE_ECHO_INPUT));
        getline(cin, input);
		SetConsoleMode(hStdInput, mode);
        cout<<endl;		
        if(real_pwd==input)return true;
        else if(i!=3)show_message("failed", "Sorry, try again.",true, "");
        else;
    }
    show_message("failed", "failed: No attempt left\n", true, "");
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
    show_message("success", "==========APPEND MODE==========", true, "");
    // get data
    string date, income, expense, details, last_modified;
    date=get_user_input("Date: ", false, 10, 10).str();
    income=get_user_input("Income: ", true, 0, 7).str();
    expense=get_user_input("Expense: ", true, 0, 7).str();
    details=get_user_input("Details: ", false, 0, 45).str();    
    last_modified=get_current_time();
    stringstream ss=format_beautiful(date, income, expense, details, last_modified);
    bool conti=yes_or_not("Do you want to continue? [Y/n] ");
    if(conti==false)return; // no more continuous move
    if(pword_check(USERNAME, PWD, 4)){
		// append data
		ofstream file;
		file.open(filename, ios_base::app);
		file<<ss.str();
		file.close();
		show_message("success", "======UPDATE SUCCESSFULLY======", true, "");
	}
    return;
}

// find data in file
void find(string filename, string data){
    ifstream file;
    file.open(filename, ios::in);
    string line;
    if(data==""){
        show_message("failed", "failed: Need specified data", true, "");
        return;
    }
    bool matched=false;
    while(file>>line){
        if(line.find(data) != string::npos){
            matched=true;
            break;
        }
    }
    file.close();
    file.open(filename, ios::in);
    if(matched){
        while(getline(file, line)){      
            if(line.find(data) == string::npos)show_message("output", line, true, "");
            else show_message("highlight", line, true,"");
        }
    }
    else show_message("failed", "failed: No matched case", true, "");
    file.close();
    return;
}

// whole operation combination
void money_plus_plus(){
    bool leave=false;
	string current_path;
    while(!leave){
        show_message("idle", ACCOUNT, false, current_path);
        string command, detail;
        stringstream input=get_user_input("", false, 0, 1000);
        input>>command;
        input>>detail;
        if(command=="clear"&&detail=="")system("cls");
        else if(command=="leave"&&detail=="")leave=true;
        else if(command=="ls"&&detail=="")ls(true);        
        else if(command=="help"&&detail=="")show_file_content("help.txt", false);
        else if(command=="append"&&detail=="")append("money.txt");		
        else if(command=="find")find("money.txt", detail);
        else if(command=="whoami"&&detail=="")show_message("output", USERNAME, true, "");
        else if(command=="whoru"&&detail=="")show_message("output", PROGRAM_NAME, true, "");
        else if(command=="money++"&&detail=="--version")show_message("output", PROGRAM_VERSION, true, "");
        else if(command=="money++"&&detail!="")show_file_content(detail, false);
        else{ // more instruction are good to add above
            show_message("failed", input.str()+": command not found", true, "");
        }
    }
    return;
}

int main()
{
    money_plus_plus();
    return 0; 
}
