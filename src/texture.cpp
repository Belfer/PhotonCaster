#include "texture.h"

namespace graphics
{

    void Texture::GenTexture ()
    {
        DeleteTexture ();
        glGenTextures (1, &texture);
        glBindTexture (GL_TEXTURE_2D, texture);
    }

    void Texture::DeleteTexture ()
    {
        if (texture != 0) {
            glDeleteTextures (1, &texture);
            texture = 0;
        }
    }

    void Texture::CreateImage (GLint format, GLsizei width, GLsizei height, const GLvoid* data)
    {
        this->width = width;
        this->height = height;
        if (!bound) glBindTexture (GL_TEXTURE_2D, texture);
        glTexImage2D (GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if (!bound) glBindTexture (GL_TEXTURE_2D, 0);
    }

    void Texture::CreateSubImage (GLint format, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const GLvoid* data)
    {
        if (!bound) glBindTexture (GL_TEXTURE_2D, texture);
        glTexSubImage2D (GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, format, GL_UNSIGNED_BYTE, data);
        if (!bound) glBindTexture (GL_TEXTURE_2D, 0);
    }

    void Texture::Bind (uint unit)
    {
        assert (unit >= 0 && unit <= 15);

        glActiveTexture (GL_TEXTURE0 + unit);
        glBindTexture (GL_TEXTURE_2D, texture);
        bound = true;
    }

    void Texture::SetPixelAlignment (GLuint alignment)
    {
        if (!bound) glBindTexture (GL_TEXTURE_2D, texture);
        glPixelStorei (GL_UNPACK_ALIGNMENT, alignment);
        if (!bound) glBindTexture (GL_TEXTURE_2D, 0);
    }

    void Texture::SetFiltering (GLuint filter)
    {
        if (!bound) glBindTexture (GL_TEXTURE_2D, texture);

        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter); // GL_NEAREST, GL_LINEAR
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        if (!bound) glBindTexture (GL_TEXTURE_2D, 0);
    }

    void Texture::SetMipMapping (GLuint mipmap)
    {
        if (!bound) glBindTexture (GL_TEXTURE_2D, texture);

        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap); // GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap); // GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR

        if (!bound) glBindTexture (GL_TEXTURE_2D, 0);
    }

    void Texture::SetWrapping (GLuint wrap)
    {
        if (!bound) glBindTexture (GL_TEXTURE_2D, texture);

        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap); // GL_REPEAT, GL_MIRRORED_REPEAT,
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap); // GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER

        if (!bound) glBindTexture (GL_TEXTURE_2D, 0);
    }

    void Texture::SetBorder (GLfloat* borderColor)
    {
        if (!bound) glBindTexture (GL_TEXTURE_2D, texture);
        glTexParameterfv (GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        if (!bound) glBindTexture (GL_TEXTURE_2D, 0);
    }

}
