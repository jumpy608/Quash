#include <iostream>
#include <string.h>
#include <filesystem>
#include "Executive.h"
using namespace std;

string Executive::echo(string input){
    bool inQuotes = 0;
    char quotesChar;
    bool inEnviromentVarName = 0;
    string enviromentVarName;
    string output;
    
    for(int i=0; i<input.length(); i++){
        
        if(inQuotes == 1){  //checks if we exit the curent string in quotes before looking for other special chars
            if(input.at(i) == quotesChar){
                inQuotes = 0;
            }
            else{
                output=output+input.at(i);
            }
        }
        else if(inEnviromentVarName == 1){  //copys down enviroment var name
            if(input.at(i) == ' '){
                inEnviromentVarName = 0; 
                
                
                //handle enviroment var here once we get to this point
                enviromentVarName ="";
                
            }
            else{
                enviromentVarName=enviromentVarName+input.at(i);
            }
        }
        else if((input.at(i) == '\"')||(input.at(i) == '\'')){  //enteres an element surrounded in quotes
            inQuotes=1;
            quotesChar= input.at(i);
        }
        else if(input.at(i) == '$'){    //switches mode to copy down enviroment var name
            inEnviromentVarName=1;
        }
        else if(input.at(i) == '#'){    //recgnizes that the rest of the input is a commet and breaks from the loop
            break;
        }
        else{   //adds current char to the output string
            output=output+input.at(i);
        }
    }
    return output; //returns output string
    
}

string Executive::pwd(){
    return std::filesystem::current_path().generic_string();
}

bool Executive::cmdInputHandler(string input){
    string cmd;
    
    for(int i=0; i<input.length(); i++){ //checks for first valid command then runs corrisponding fn
        cmd=cmd + input.at(i); 
        if(cmd == "echo"){
            if(i+2<=input.length()){
                cout<< echo(input.substr(i+2));
            }
        }
        if(cmd == "pwd"){
            cout<<pwd();
        }
        if((cmd == "quit") || (cmd == "exit")){
            return 1;
        }
    }
    return 0;
}

int Executive::run()
{
    
    string temp = "hello \"world\" # this should print hello world";
    cout<<temp<<"\n\n";
    cout<<echo(temp);
    cout<<pwd();
    bool quit=0;
    string input;
    
    cout<<"\n\n Welcome...";
    
    while(quit == 0){ //runs shell waiting for user input
        cout<<"\n[QUASH]$ "; 
        getline(cin,input);
        //cout<<input<<"\n\n";
        quit=cmdInputHandler(input);
    }
    
    return 0;
}
