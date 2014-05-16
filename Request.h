#ifndef _PLATER_REQUEST_H
#define _PLATER_REQUEST_H

#include <map>
#include <iostream>
#include <vector>

namespace Plater
{
    class Request
    {
        public:
            Request();

            void readFromStdin();
            void process();

        protected:
            double plateWidth, plateHeight;
            std::map<std::string, int> parts;
    
            std::string readString();
            double readFloat();
            int readInt();
    };
}

#endif
