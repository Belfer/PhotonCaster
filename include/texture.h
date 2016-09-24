#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <assert.h>
#include <glm/glm.hpp>

namespace graphics
{

    class Texture
    {
    public:
        Texture () : texture (0), width (0), height (0), bound (false) {}
        virtual ~Texture () { DeleteTexture (); }

        void GenTexture ();
        void DeleteTexture ();

        void CreateImage (GLint format, GLsizei width, GLsizei height, const GLvoid* data, bool mipmap = false);
        void CreateSubImage (GLint format, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const GLvoid* data);

        void Bind (uint unit);
        inline void Unbind () { glBindTexture (GL_TEXTURE_2D, 0); bound = false; }

        void SetPixelAlignment (GLuint alignment);
        void SetFiltering (GLuint filter);
        void SetMipMapping (GLuint mipmap);
        void SetWrapping (GLuint wrap);
        void SetBorder (GLfloat* borderColor);

        inline uint GetWidth () const { return width; }
        inline uint GetHeight () const { return height; }
        inline bool IsBounded () const { return bound; }

    private:
        Texture(const Texture& other);
        void operator=(const Texture& other);

        GLuint texture;
        GLsizei width, height;
        bool bound;
    };

}

#endif // TEXTURE_H
