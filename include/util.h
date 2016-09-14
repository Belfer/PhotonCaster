#ifndef UTIL_H
#define UTIL_H

#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include "mesh_data.h"

using namespace std;
using namespace graphics;

namespace util
{
    static void gl_triangle (OpenGLMeshData& data, const vec3& p1, const vec3& p2, const vec3& p3)
    {
        GLfloat vertices[9];
        vertices[0] = p1.x; vertices[1] = p1.y; vertices[2] = p1.z;
        vertices[3] = p2.x; vertices[4] = p2.y; vertices[5] = p2.z;
        vertices[6] = p3.x; vertices[7] = p3.y; vertices[8] = p3.z;

        vec3 normal = glm::cross (glm::normalize (p2-p1), glm::normalize (p3-p1));
        GLfloat normals[9];
        for (uint i=0; i<9;) {
            normals[i++] = normal.x;
            normals[i++] = normal.y;
            normals[i++] = normal.z;
        }

        data.GenBuffers (1, 1, 0);
        data.BindVertexArray (0);
        data.AddArrayBuffer (0, sizeof (vertices), vertices, GL_STATIC_DRAW, GL_FLOAT, 3);
        data.AddArrayBuffer (1, sizeof (normals), normals, GL_STATIC_DRAW, GL_FLOAT, 3);
        //data.AddElementBuffer (0, 6, indices, GL_STATIC_DRAW);
        data.UnbindVertexArray ();
        data.SetDrawCount (3);
        data.SetUseIndices (false);
    }

    //str.erase (std::remove (str.begin (), str.end (), '.'), str.end ());

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
