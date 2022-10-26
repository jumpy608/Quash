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
	    string pwd();
	    string ls();
	    string cleanCom(string input);
	    void slash(string input);
	    void exportCMD(string input);
	    bool cmdInputHandler(string input);
        int run();
};
#endif
