#include <iostream>
#include <fstream>
#include <string>
#include <map>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main()
{
    string jobid;
    string jobinfo;
    string buff;
    string out_buff;
    int pos = string::npos;
    map<string, string> reduce_map;

    while (getline(cin, buff)) 
    {
        if (buff.size() == 0) 
        {
            continue;
        }
        pos = buff.find("\t"); 
        if (pos == string::npos) 
        {
            continue;
        }
        jobid = buff.substr(0, pos);
        jobinfo = buff.substr(pos + 1);
        if (reduce_map.find(jobid) == reduce_map.end()) 
        {
            //if jobid count first time, store it in the map
            reduce_map[jobid] = jobinfo;
        }
        else 
        {
            // if jobid count second time, process it. 
            pos = jobinfo.find("submit_time");
            if (pos == string::npos) 
            {
                // the jobfinish file line
                out_buff = reduce_map[jobid] + "\t" + jobinfo; 
            }
            else 
            {
                // the jobsubmit file line
                out_buff = jobinfo + "\t" + reduce_map[jobid];
            }
            reduce_map.erase(jobid);
            cout << jobid << "\t" << out_buff << endl;
        }
    }

    return 0;
}
