#ifndef UTIL_H
#define UTIL_H

#include <functional>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace util
{

    static hash<string> str_hashfn;

    static void split (const string& s, const char& delim, vector<string>& elems) {
        stringstream ss;
        ss.str (s);
        string item;
        while (getline (ss, item, delim)) {
            elems.emplace_back (item);
        }
    }


    static vector<string> split (const string& s, const char& delim) {
        vector<string> elems;
        split (s, delim, elems);
        return elems;
    }

    static std::string at (const std::string& src, const char& delim, const size_t& index)
    {
        vector<string> strlist;
        split (src, delim, strlist);
        return strlist[index];
    }
}

#endif // UTIL_H
