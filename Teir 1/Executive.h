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
	    void execute(string input);
	    void killhandler(string input);
	    void kill(int sigNUM, int pID);
	    string cmdInputHandler(string input);
        int run();
        
        void cat(string input);
        string read(string filename);
        void write(string filename, string input);
        void append(string filename, string input);
        bool istextfile(string input);
        string removews(string input);
        
        void toprintornottoprint(string input);
        bool iscat(string input);
        
};
#endif
