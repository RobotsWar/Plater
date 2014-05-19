#include <iostream>
#include "Request.h"

using namespace std;

namespace Plater
{
    Request::Request()
    {
    }

    Request::~Request()
    {
        for (auto part : parts) {
            delete part.second;
        }
    }

    std::string Request::readString()
    {
        std::string data;
        cin >> data;
        return data;
    }

    double Request::readFloat()
    {
        return atof(readString().c_str());
    }

    int Request::readInt()
    {
        return atoi(readString().c_str());
    }

    void Request::readFromStdin()
    {
        plateWidth = readFloat();
        plateHeight = readFloat();

        while (!cin.eof()) {
            string filename = readString();
            int quantity = readInt();
            if (filename != "" && quantity != 0) {
                parts[filename] = new Part;
                parts[filename]->load(filename);
                quantities[filename] = quantity;
            }
        }

        cout << "* Request" << endl;
        cout << "- Plate size: " << plateWidth << "x" << plateHeight << endl;
        for (auto part : quantities) {
            cout << "- Part " << part.first << ": " << part.second << " qty" << endl;
        }
    }

    void Request::process()
    {
    }
}
