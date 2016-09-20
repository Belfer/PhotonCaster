#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

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

    static void load_image (const std::string& filename,
                            const GLuint& format, const GLuint& wrap,
                            const GLuint& filter, const GLuint& mipmap)
    {
        /*unsigned char* image = SOIL_load_image (filename.c_str (), &width, &height, 0,
                                                format == GL_RGB ? SOIL_LOAD_RGB : SOIL_LOAD_RGBA);
        glBindTexture (GL_TEXTURE_2D, texture);

        // Wrapping
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        // Filtering
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap);

        glTexImage2D (GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap (GL_TEXTURE_2D);

        SOIL_free_image_data (image);
        glBindTexture (GL_TEXTURE_2D, 0);*/
    }

}

#endif // LOADER_H
