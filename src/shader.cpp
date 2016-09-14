#include "shader.h"

namespace graphics
{

    static void CheckShaderError (GLuint shader, GLuint flag, bool program, const std::string& msg);

    Shader::Shader ()
    {
        program = glCreateProgram ();
    }

    Shader::~Shader ()
    {
        glDeleteProgram (program);
    }

    void Shader::AddShader (GLuint type, const std::string& source)
    {
        GLuint shader = glCreateShader (type);

        if (shader == 0) {
            std::cerr << "Shader could not be created!\n";
        }

        const GLchar* srcs[1];
        GLint srcsLengths[1];

        srcs[0] = source.c_str ();
        srcsLengths[0] = source.length ();

        glShaderSource (shader, 1, srcs, srcsLengths);
        glCompileShader (shader);

        switch (type) {
        case GL_VERTEX_SHADER:
            CheckShaderError (shader, GL_COMPILE_STATUS, false, "Vertex shader compilation failed!");
            break;
        case GL_FRAGMENT_SHADER:
            CheckShaderError (shader, GL_COMPILE_STATUS, false, "Fragment shader compilation failed!");
            break;
        case GL_GEOMETRY_SHADER:
            CheckShaderError (shader, GL_COMPILE_STATUS, false, "Geometry shader compilation failed!");
            break;
        default:
            CheckShaderError (shader, GL_COMPILE_STATUS, false, "Unknown Shader compilation failed!");
            break;
        }

        glAttachShader (program, shader);
        shaders.push_back (shader);
    }

    void Shader::Compile ()
    {
        glLinkProgram (program);
        CheckShaderError (program, GL_LINK_STATUS, true, "Could not link shader program!");

        glValidateProgram (program);
        CheckShaderError (program, GL_LINK_STATUS, true, "Shader program is invalid!");

        for (auto it = shaders.begin (); it != shaders.end (); it++) {
            glDeleteShader ((*it));
        }
    }

    static void CheckShaderError (GLuint shader, GLuint flag, bool program, const std::string& msg)
    {
        GLint success = 0;
        GLchar error[1024] = {0};

        if (program) {
            glGetProgramiv (shader, flag, &success);
        } else {
            glGetShaderiv (shader, flag, &success);
        }

        if (success == GL_FALSE) {
            if (program) {
                glGetProgramInfoLog (shader, sizeof (error), NULL, error);
            } else {
                glGetShaderInfoLog (shader, sizeof (error), NULL, error);
            }

            std::cerr << msg << " - " << error << std::endl;
        }
    }

}
