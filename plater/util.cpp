#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#endif
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include "util.h"

#if defined(_WIN32) || defined(_WIN64)
	#define chdir _chdir
#endif

std::string getDirectory(const std::string &filename)
{
    int found;
    found = filename.find_last_of("/\\");
    if (found < 0) {
        return "";
    } else {
        return filename.substr(0, found);
    }
}

std::string getBasename(const std::string &filename)
{
    size_t found;
    found = filename.find_last_of("/\\");
    return filename.substr(found+1);
}

bool chdirFile(const std::string &filename)
{
    std::string targetDirectory = getDirectory(filename);

    
    if (targetDirectory != "") {
        return chdir(targetDirectory.c_str())==0;
    } else {
        return true;
    }
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }   
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::vector<std::string> splitWithEscape(const std::string &s, char delim) {
    std::vector<std::string> elems = split(s, delim);
    std::vector<std::string> escaped;

    std::string current = "";
    for (unsigned int i=0; i<elems.size(); i++) {
        current += elems[i];
        if (elems[i][elems[i].size()-1] == '\\') {
            current[current.size()-1] = ' ';
        } else {
            escaped.push_back(current);
            current = "";
        }
    }
    if (current != "") {
        escaped.push_back(current);
    }

    return escaped;
}

bool isNumeric(std::string str)
{
    for (unsigned int i=0; i<str.size(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

std::string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string trim(std::string s) {
    return ltrim(rtrim(s));
}
