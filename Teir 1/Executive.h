#ifndef EXECUTIVE_H
#define EXECUTIVE_H
#include <string.h>
#include <filesystem>
using namespace std;

class Executive{
	private:

	public:
	string path;
        string home;
        string current="";
	    string echo(string input);
	    void cd(string input);
	    void cd();
	    void dotdot(string input);
	    string pwd();
	    string ls();
	    string cleanCom(string input);
	    void slash(string input);
	    void exportCMD(string input);
	    void jobs();
	    void killhandler(string input);
	    void kill(int sigNUM, int pID);
	    bool cmdInputHandler(string input);
        int run();
        
        void checkBros(string input);
        string read(string filename);
        void write(string filename, string input);
        void append(string filename, string input);
};
#endif
