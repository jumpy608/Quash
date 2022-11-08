#include <iostream>
#include <string.h>
#include <filesystem>
#include "Executive.h"
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
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
    else if(input.at(0) == '$'){
        if(input.length()>1){
            std::filesystem::current_path(getenv(input.substr(1,input.length()).c_str()));
        }
    }
}

void Executive::cd(){
    std::filesystem::current_path(getenv("HOME"));
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
    bool wsc=1;
    
    for(int i=0; i<(int)input.length(); i++){
        if((input.at(i) == ' ')&&(wsc)){
        }
        else{
            wsc=0;
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
            else{   //adds current char to the output string
                output=output+input.at(i);
            }
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

void Executive::killhandler(string input){
    string LHS="";
    string RHS="";
    bool side=1;
       
    for(int i=0; i<(int)input.length(); i++){ //parse input and return the elements to be exported
        if(input.at(i) == ' '){
            side=0;
        }
        else if(side){
            LHS=LHS+input.at(i);
        }
        else{
            RHS=RHS+input.at(i);
        }
    }
    if(side){//If invalid syntax
    return;
    }
    kill(stoi(LHS),stoi(RHS));
}

void Executive::kill(int sigNUM, int pID){


}

void Executive::execute(string input) {
    pid_t child_pid = fork();
    
    if (child_pid == -1) {
    
    perror("fork");
    }
    else if (child_pid == 0){
    
        //This part builds the array of args
    
        char* args[64];
        int elements=0;
        string temp;
        bool whitespace=0;
        bool first=1;
        string bin="";
        
        for(int i=0; i<(int)input.length(); i++){ //split first element from string
            if(input.at(i) == ' '){
                if(!whitespace){
                    whitespace=1;
                    args[elements]=(char*)temp.c_str();
                    cout<<args[elements];
                    elements++;
                    if(first){
                        first=0;
                        bin="/bin/" +temp;
                    }
                    temp="";
                }
                
            }
            else{
                whitespace=0;
                temp=temp+input.at(i);
            }
        }
        
        args[elements]=(char*)temp.c_str();
        cout<<args[elements];
        elements++;
        
        for(int i=elements; i< 64; i++){
            args[i]=NULL;
        }
    
        execvp(bin.c_str(), args);
        return;
    }
    else {
    wait(NULL);
    
    }
    
    
}

string Executive::cmdInputHandler(string input){
    string cmd;
    
    input=cleanCom(input);
    
    for(int i=0; i<(int)input.length(); i++){ //checks for first valid command then runs corrisponding fn
        cmd=cmd + input.at(i); 
        if(cmd == "echo"){
            if(i+2<=(int)input.length()){
                return echo(input.substr(i+2));
            }
            return "";
        }
        //SLASH command (run program)
        if(cmd == "./"){
            slash(input);
            return "";
        }
        //Prints the working directory
        if(cmd == "pwd"){
            return pwd();
        }
        //Lists the files within the current directory
        if(cmd == "lss"){
            return ls();
        }
        //Change working directory
        if(cmd == "cd"){
            if(i+2<=(int)input.length()){
                cd(input.substr(i+1));
            }
            else{
                cd();
            }
            return "";
        }
        //Create a new ENV
        if(cmd == "export"){
            if(i+2<=(int)input.length()){
                exportCMD(input.substr(i+2));
                }
            return "";
        }
        //messes with files with cat
        if(cmd == "cat"){
            if(i+2<=(int)input.length()){
                cat(input.substr(i+2));
                }
            return "";
        }
        //Prints all currently running ba	    void execute(string input);ckground processes
        if(cmd == "jobs"){
            return "";
        }
        //Send a POSIX signal and PID (both ints), send the signal to the given process.
        if(cmd == "kill") {
            if(i+2<=(int)input.length()){
                killhandler(input.substr(i+1));
            }
            return "";
        }
        //Exit command(s)
        if((cmd == "quit") || (cmd == "exit")){
            exit(0);
            return "";
        }
    }
    execute(input);
    return "";
}


void Executive::cat(string input){
    bool side=1;
    int cmd=0;
    int cmdindex=0;
    string cmdtext="";
    string LHS;
    string RHS;
    
    // < file.txt read from file
    // file1.txt >> file2.txt append file1 output to file2 
    // input.txt > file.txt
    // | pipeing
    // lhs cmd rhs cmd2 in
    // if cmd2 != null cat rhs cmd2 in
    
    for(int i=0; i<(int)input.length(); i++){ //parse input and return the elements to be exported
        if((input.at(i) == '<')||(input.at(i) == '|')||(input.at(i) == '>')){
            
            if(cmd==0){
                side=0;
                cmd++;
                
                if(input.at(i) == '<'){
                    cmdtext="<";
                }
                if(input.at(i) == '>'){
                    if((i+1)<(int)input.length()){
                        if(input.at(i+1) == '>'){
                            i++;
                            cmdtext=">>";
                        }
                        else{
                            cmdtext=">";
                        }
                    }
                    else{
                        cmdtext=">";
                    }
                }
                if(input.at(i) == '|'){
                    cmdtext="|"; //we dont have pipeing yet but If we did we we know to do it here
                }
                
            }
            else if(cmd==1){
                cmd++;
                cmdindex=i;
                break;
                
            }
            
        }
        else if(side){
            LHS=LHS+input.at(i);
        }
        else{
            RHS=RHS+input.at(i);
        }
    }
    

    
    LHS=removews(LHS);
    RHS=removews(RHS);
    
    
    //cout<<"\n\n"<<side<<" "<<cmd<<" "<<cmdindex<<" "<<cmdtext<<" "<<LHS<<" "<<RHS<<" "<<"\n\n";
    
    if(cmdtext==">>"){
        //cout<<"append";
        if(istextfile(LHS)){
            append(RHS,read(LHS));
        }
        else{
            append(RHS,LHS);
        }
    }
    if(cmdtext==">"){
        //cout<<"Write";
        if(istextfile(LHS)){
            write(RHS,read(LHS));
        }
        else{
            write(RHS,LHS);
        }
    }
    if(cmdtext=="<"){
        //cout<<"Read";
        cout<<read(RHS);
    }
    
    if(cmd==0){
        cout<<read(input);
    }
    if(cmd==2){
        cat(RHS+input.substr(cmdindex));
    }

    
}

bool Executive::istextfile(string filename){
    return((int)filename.find(".txt") != -1);
}

string Executive::removews(string input){
    string output="";
    for(int i=0; i<(int)input.length(); i++){
        if(input.at(i) == ' '){
        }
        else{
            output=output + input.at(i);
        }
    }
    return output;
}

string Executive::read(string filename){
    string tempRead;
    string output="";
    bool first=1;
    
    ifstream f(filename);  //prints file 
    if(f.is_open()){
        while(getline(f, tempRead)){
            if(first){
                first=0;
                output=output+tempRead;
            }
            else{
                output=output+"\n"+tempRead;
            }
        }
        f.close();
        
    }
    return output;
}

void Executive::write(string filename, string input){
    string tempRead;
    string output="";
    ofstream f(filename);  //prints file 
    if(f.is_open()){
        f << input;
        f.close();
        
    }
    return;
}

void Executive::append(string filename, string input){
    string tempRead;
    string output="";
    ofstream f(filename,ios::app);  //prints file 
    if(f.is_open()){
        f << input;
        f.close();
        
    }
    return;
}

int Executive::run()
{
    /*
    string temp = "hello \"world\" # this should print hello world";
    cout<<temp<<"\n\n";
    cout<<echo(temp);
    cout<<pwd();
    execute("ls");
    */
    /*
    cout<<read("file.txt");
    write("a.txt", read("file.txt"));
    append("b.txt", read("file.txt"));
    */
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
        cout<<cmdInputHandler(input);
    }
    
    return 0;
}
