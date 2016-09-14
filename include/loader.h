#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <iostream>
#include <fstream>

namespace util
{

    static std::string load_file (const std::string& filename)
    {
        std::ifstream file;
        file.open (filename.c_str ());

        std::string out;
        std::string line;

        if (file.is_open ()) {
            while (file.good ()) {
                getline (file, line);
                out.append (line+"\n");
            }
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }

        return out;
    }

}

#endif // LOADER_H
