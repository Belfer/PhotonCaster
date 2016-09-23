#include "mesh_data.h"

namespace graphics
{

    void OpenGLMeshData::GenBuffers (uint VAOsize, uint VBOsize, uint EBOsize)
    {
        DeleteBuffers ();

        this->VAOsize = VAOsize;
        this->VBOsize = VBOsize;
        this->EBOsize = EBOsize;

        VAOs = new GLuint[VAOsize];
        VBOs = new GLuint[VBOsize];
        EBOs = new GLuint[EBOsize];

        glGenVertexArrays (VAOsize, VAOs);
        glGenBuffers (VBOsize, VBOs);
        glGenBuffers (EBOsize, EBOs);
    }

    void OpenGLMeshData::DeleteBuffers ()
    {
        if (VAOsize > 0) {
            glDeleteVertexArrays (VAOsize, VAOs);
            delete [] VAOs;
            VAOs = nullptr;
            VAOsize = 0;
        }
        if (VBOsize > 0) {
            glDeleteBuffers (VBOsize, VBOs);
            delete [] VBOs;
            VBOs = nullptr;
            VBOsize = 0;
        }
        if (EBOsize > 0) {
            glDeleteBuffers (EBOsize, EBOs);
            delete [] EBOs;
            EBOs = nullptr;
            EBOsize = 0;
        }
    }

    void OpenGLMeshData::ArraySubData (uint loc, uint size, GLfloat* data)
    {
        glBindBuffer (GL_ARRAY_BUFFER, VBOs[loc]);
        glBufferSubData (GL_ARRAY_BUFFER, 0, size * sizeof (data[0]), data);
        glBindBuffer (GL_ARRAY_BUFFER, 0);
    }

    void OpenGLMeshData::ElementSubData (uint loc, uint size, GLuint* data)
    {
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBOs[loc]);
        glBufferSubData (GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof (data[0]), data);
    }

    void OpenGLMeshData::ArrayBuffer (uint loc, uint size, GLfloat* data, GLuint flag, GLuint type, uint count)
    {
        // Put array data in GPU memory
        glBindBuffer (GL_ARRAY_BUFFER, VBOs[loc]);
        glBufferData (GL_ARRAY_BUFFER, size, data, flag);

        glEnableVertexAttribArray (loc);
        glVertexAttribPointer (loc, count, type, GL_FALSE, count * sizeof (GLfloat), NULL);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLMeshData::ArrayBuffer (uint loc, uint size, GLfloat* data, GLuint flag, GLuint type, uint count, const GLvoid* offset)
    {
        // Put array data in GPU memory
        glBindBuffer (GL_ARRAY_BUFFER, VBOs[loc]);
        glBufferData (GL_ARRAY_BUFFER, size, data, flag);

        glEnableVertexAttribArray (loc);
        glVertexAttribPointer (loc, count, type, GL_FALSE, count * sizeof (GLfloat), offset);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLMeshData::ElementBuffer (uint loc, uint size, GLuint* data, GLuint flag)
    {
        // Put element data in GPU memory
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBOs[loc]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, size, data, flag);
    }

    void OpenGLMeshData::Draw ()
    {
        OpenGLMeshData::Draw (drawMode, drawCount, useIndices);
    }

    void OpenGLMeshData::Draw (GLuint flag, GLuint drawCount, bool useIndices)
    {
        for (uint i=0; i<VAOsize; ++i) {
            glBindVertexArray (VAOs[i]);
            if (useIndices) {
                glDrawElements (flag, drawCount, GL_UNSIGNED_INT, 0);
            } else {
                glDrawArrays (flag, 0, drawCount);
            }
        }
        glBindVertexArray (0);
    }

}
