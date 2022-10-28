#include <iostream>
#include <string.h>
#include <filesystem>
#include "Executive.h"
using namespace std;

//Define pre-existing Environmental Variables.
string currDirect;//$PWD
string homeDirect;//$HOME
string pathDirect;//$PATH

string Executive::echo(string input){
    //cout<<input;
    
    bool inQuotes = 0;
    char quotesChar;
    bool inEnviromentVarName = 0;
    string enviromentVarName;
    string output;
    
    for(int i=0; i<(int)input.length(); i++){
        
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
                
                //Print the current path of the Environmental Variable 
                //
                // echo $PATH -> Print the value of path
                output = output + getenv(enviromentVarName.c_str());
                //Remove temp EV name
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
    if(inEnviromentVarName){
        output = output + getenv(enviromentVarName.c_str());
    }
    return output; //returns output string
    
}

void Executive::cd(string input){
    //cout<<input;
    if(input == ".."){      
        dotdot(pwd());
    }
}

void Executive::dotdot(string input){
    int finalslash=-1;
    
    for(int i=0; i<(int)input.length(); i++){
        if(input.at(i) == '/'){
            finalslash=i;
        }
    }
    
    if(finalslash!=-1){
        //cout<<input.substr(0,finalslash);
        if(finalslash == 0){
            std::filesystem::current_path("/");
        }
        else{
            std::filesystem::current_path(input.substr(0,finalslash));
        }
       
    }
    else{
        cout<<"you should not be here";
    }
        
    
}

string Executive::pwd(){
    return std::filesystem::current_path().generic_string();
}

string Executive::ls(){
    string output;
    string temp;
    string path=pwd();
    for (const auto & entry : filesystem::directory_iterator(path)){
        temp= entry.path().filename().generic_string();
        output = output + ' ' + temp;
    }
    return output;
}

void Executive::slash(string input){
    system(input.c_str());
    return;
}

string Executive::cleanCom(string input){ //cleans the input of comments
    string output;
    bool inQuotes=0;
    char quotesChar;
    
    for(int i=0; i<(int)input.length(); i++){
        
        if(inQuotes == 1){  //checks if we exit the curent string in quotes before looking for other special chars
            if(input.at(i) == quotesChar){
                inQuotes = 0;
            }
            output=output+input.at(i);
        }
        else if((input.at(i) == '\"')||(input.at(i) == '\'')){  //enteres an element surrounded in quotes
            inQuotes=1;
            quotesChar= input.at(i);
            output=output+input.at(i);
        }
        else if(input.at(i) == '#'){    //recgnizes that the rest of the input is a commet and breaks from the loop
            break;
        }
        else{   //adds current char sideto the output string
            output=output+input.at(i);
        }
    }
    
    return output;
}

void Executive::exportCMD(string input){//Sets the value of an envirnomental variable.
       string LHS="";
       string RHS="";
       bool RHSVar=0;
       bool side=1;
       
    for(int i=0; i<(int)input.length(); i++){ //parse input and return the elements to be exported
        if(input.at(i) == '='){
            side=0;
            if(input.at(i+1) == '$'){
                RHSVar=1;
                i++;
            }
        }
        else if(side){
            LHS=LHS+input.at(i);
        }
        else{
            RHS=RHS+input.at(i);
        }
    }
    
    //
    if(side){//If invalid syntax
    return;
    }
    if(RHSVar) {//If the RHS is an existing ENV
    
        //Get and Set the new ENV to the existing ENV's value
        setenv(LHS.c_str(), getenv(RHS.c_str()), 1);
        
        if(LHS == "PWD") {//Rewriting the existing PWD with existing ENVIRONMENTAL VAR
            currDirect = getenv("PWD");
        }
        else if(LHS == "HOME"){//Rewriting the existing HOME with existing ENVIRONMENTAL VAR
            homeDirect = getenv("HOME");
        }
        else if(LHS == "PATH"){//Rewriting the existing PATH with existing ENVIRONMENTAL VAR
            pathDirect = getenv("PATH");
        }
    }
    else{//Being given the directory
        
        
        
        if(LHS == "PWD") {//Rewriting the existing PWD with existing ENVIRONMENTAL VAR
            currDirect = setenv(LHS.c_str(), RHS.c_str(), 1);;
        }
        else if(LHS == "HOME"){//Rewriting the existing HOME with existing ENVIRONMENTAL VAR
            homeDirect = setenv(LHS.c_str(), RHS.c_str(), 1);
        }
        else if(LHS == "PATH"){//Rewriting the existing PATH with existing ENVIRONMENTAL VAR
            pathDirect = setenv(LHS.c_str(), RHS.c_str(), 1);
        }
        else{//All other inputs
            setenv(LHS.c_str(), RHS.c_str(), 1);
        }
    }
    
    
    cout<<LHS<<"\t"<<RHS<<"\t"<<RHSVar;
}

void Executive::jobs(){

}

void Executive::kill(int sigNUM, int pID){


}

bool Executive::cmdInputHandler(string input){
    string cmd;
    
    input=cleanCom(input);
    
    for(int i=0; i<(int)input.length(); i++){ //checks for first valid command then runs corrisponding fn
        cmd=cmd + input.at(i); 
        if(cmd == "echo"){
            if(i+2<=(int)input.length()){
                cout<< echo(input.substr(i+2));
            }
        }
        //SLASH command (run program)
        if(cmd == "./"){
            slash(input);
        }
        //Prints the working directory
        if(cmd == "pwd"){
            cout<<pwd();
        }
        //Lists the files within the current directory
        if(cmd == "ls"){
            cout<<ls();
        }
        //Change working directory
        if(cmd == "cd"){
            if(i+2<=(int)input.length()){
                cd(input.substr(i+2));
            }
            else{
                cd("");
            }
        }
        //Create a new ENV
        if(cmd == "export"){
            if(i+2<=(int)input.length()){
                exportCMD(input.substr(i+2));
                }
        }
        //Prints all currently running background processes
        if(cmd == "jobs"){
        }
        //Send a POSIX signal and PID (both ints), send the signal to the given process.
        if(cmd == "kill") {
        }
        //Exit command(s)
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
    
    //Init existing Environmental Variables.
    currDirect = getenv("PWD");
    homeDirect = getenv("HOME");
    pathDirect = getenv("PATH");
    
    cout<<"\n\n Welcome...";
    
    while(quit == 0){ //runs shell waiting for user input
        cout<<"\n[QUASH]$ "; 
        getline(cin,input);
        //cout<<input<<"\n\n";
        quit=cmdInputHandler(input);
    }
    
    return 0;
}
