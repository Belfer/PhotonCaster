#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

namespace graphics
{

    class PolygonMeshData
    {
    public:
        PolygonMeshData () {}
        virtual ~PolygonMeshData () {}

        void AddVertices (const size_t& size, float* vertices);
        void AddTriangle (const uint& a, const uint& b, const uint& c);

        inline uint GetTriCount () const { return tri_count; }

    private:
        uint tri_count;
        vector<uint> indices;
        vector<vec3> vertices;
    };

    class OpenGLMeshData
    {
    public:
        OpenGLMeshData () : VAOsize (0), VBOsize (0), EBOsize (0),
            drawMode (GL_TRIANGLES), drawCount (0), useIndices (false) {}
        virtual ~OpenGLMeshData () { DeleteBuffers (); }

        void GenBuffers (uint VAOsize, uint VBOsize, uint EBOsize);

        void DeleteBuffers ();

        void ArraySubData (uint loc, uint size, GLfloat* data);

        void ElementSubData (uint loc, uint size, GLuint* data);

        void ArrayBuffer (uint loc, uint size, GLfloat* data, GLuint flag, GLuint type, uint count);

        void ArrayBuffer (uint loc, uint size, GLfloat* data, GLuint flag, GLuint type, uint count, const GLvoid* offset);

        void ElementBuffer (uint loc, uint size, GLuint* data, GLuint flag);

        void Draw ();

        void Draw (GLuint flag, GLuint drawCount, bool useIndices);

        inline void SetDrawMode (GLuint drawMode) { this->drawMode = drawMode; }

        inline void SetDrawCount (GLuint drawCount) { this->drawCount = drawCount; }

        inline void SetUseIndices (bool useIndices) { this->useIndices = useIndices; }

        inline void BindVertexArray (GLuint VAO) { glBindVertexArray (VAOs[VAO]); }

        inline void BindArrayBuffer (GLuint VBO) { glBindBuffer (GL_ARRAY_BUFFER, VBOs[VBO]); }

        inline void BindElementBuffer (GLuint EBO) { glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBOs[EBO]); }

        inline void UnbindVertexArray () { glBindVertexArray (0); }

        inline void UnbindArrayBuffer () { glBindBuffer (GL_ARRAY_BUFFER, 0); }

    private:
        GLuint* VAOs;
        GLuint* VBOs;
        GLuint* EBOs;
        uint VAOsize;
        uint VBOsize;
        uint EBOsize;

        GLuint drawMode;
        GLuint drawCount;
        bool useIndices;
    };

}

#endif // MESH_DATA_H
