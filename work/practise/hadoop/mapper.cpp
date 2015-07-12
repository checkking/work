#include <iostream>
#include <fstream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main()
{
    string buff = "";
    string jobid = "";
    string jobinfo = "";
    int pos = string::npos;
    while (getline(cin, buff))
    {
        if (buff.size() == 0)
        { // filter blank line.
            continue;
        }
        // split the jobid from other values.
        pos = buff.find("\t");
        if (pos == string::npos) 
        {
            continue;
        }
        jobid = buff.substr(0, pos);
        jobinfo = buff.substr(pos + 1); 
        cout << jobid << "\t" << jobinfo << endl;
    }

    return 0;
}

