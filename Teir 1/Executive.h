#ifndef EXECUTIVE_H
#define EXECUTIVE_H
#include <string.h>
using namespace std;

class Executive{
	private:

	public:
	string path;
        string home;
        string current="";
	string echo(string input);
	string pwd();
	bool cmdInputHandler(string input);
        int run();
};
#endif
