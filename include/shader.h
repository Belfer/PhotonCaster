#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>

namespace graphics
{

    class Shader
    {
    public:
        Shader () : program (0) {}
        virtual ~Shader () { DeleteProgram (); }

        void CreateProgram ();

        void DeleteProgram ();

        void AddShader (GLuint type, const std::string& source);

        void Compile ();

        inline void Bind () { glUseProgram (program); }

        inline void BindAttribute (const std::string& name, GLuint loc) { glBindAttribLocation (program, loc, name.c_str ()); }

        inline GLuint GetUniform (const std::string& name) { return glGetUniformLocation (program, name.c_str ()); }

        inline GLuint GetProgram () const { return program; }

    private:
        GLuint program;
        std::vector<GLuint> shaders;
    };

}

#endif // SHADER_H
