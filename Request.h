#ifndef _PLATER_REQUEST_H
#define _PLATER_REQUEST_H

#include <map>
#include <iostream>
#include <vector>
#include "Part.h"

namespace Plater
{
    class Request
    {
        public:
            Request();
            virtual ~Request();

            void readFromStdin();
            void process();

        protected:
            double plateWidth, plateHeight;
            std::map<std::string, int> quantities;
            std::map<std::string, Part*> parts;
    
            std::string readString();
            double readFloat();
            int readInt();
    };
}

#endif
