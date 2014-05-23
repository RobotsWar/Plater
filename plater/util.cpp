#include <sstream>
#include "util.h"

std::string getDirectory(const std::string &filename)
{
    size_t found;
    found = filename.find_last_of("/\\");
    return filename.substr(0, found);
}

bool chdirFile(const std::string &filename)
{
    return (chdir(getDirectory(filename).c_str())==0);
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
