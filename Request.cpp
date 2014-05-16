#include <iostream>
#include "Request.h"

using namespace std;

namespace Plater
{
    Request::Request()
    {
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
            string name = readString();
            int quantity = readInt();
            if (name != "" && quantity != 0) {
                parts[name] = quantity;
            }
        }

        cout << "* Request" << endl;
        cout << "- Plate size: " << plateWidth << "x" << plateHeight << endl;
        for (auto part : parts) {
            cout << "- Part " << part.first << ": " << part.second << " qty" << endl;
        }
    }

    void Request::process()
    {
    }
}
