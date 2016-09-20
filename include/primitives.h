#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include "mesh_data.h"

using namespace std;
using namespace graphics;

namespace util
{
    namespace gl
    {
        static void triangle (OpenGLMeshData& data, const vec3& p1, const vec3& p2, const vec3& p3)
        {
            GLfloat vertices[9];
            vertices[0] = p1.x; vertices[1] = p1.y; vertices[2] = p1.z;
            vertices[3] = p2.x; vertices[4] = p2.y; vertices[5] = p2.z;
            vertices[6] = p3.x; vertices[7] = p3.y; vertices[8] = p3.z;

            vec3 normal = cross (normalize (p2-p1), normalize (p3-p1));
            GLfloat normals[9];
            for (uint i=0; i<9;) {
                normals[i++] = normal.x;
                normals[i++] = normal.y;
                normals[i++] = normal.z;
            }

            GLuint indices[] = {
                0, 1, 2,
                0, 2, 1
            };

            data.GenBuffers (1, 2, 1);
            data.BindVertexArray (0);
            data.ArrayBuffer (0, sizeof (vertices), vertices, GL_STATIC_DRAW, GL_FLOAT, 3);
            data.ArrayBuffer (1, sizeof (normals), normals, GL_STATIC_DRAW, GL_FLOAT, 3);
            data.ElementBuffer (0, sizeof (indices), indices, GL_STATIC_DRAW);
            data.UnbindVertexArray ();
            data.SetDrawCount (6);
            data.SetUseIndices (true);
        }

        static void screen (OpenGLMeshData& data, const float& size)
        {
            GLfloat vertices[] = {
                -1.0f,-1.0f, 0.f,
                -1.0f, 1.0f, 0.f,
                 1.0f,-1.0f, 0.f,
                 1.0f, 1.0f, 0.f
            };

            GLfloat normals[] = {
                 0.f, 0.f, 1.f,
                 0.f, 0.f, 1.f,
                 0.f, 0.f, 1.f,
                 0.f, 0.f, 1.f
            };

            GLfloat texcoords[] = {
                 0.f, 0.f,
                 0.f, 1.f,
                 1.f, 0.f,
                 1.f, 1.f
            };

            GLuint indices[] = {
                0, 1, 2,
                2, 1, 3
            };

            data.GenBuffers (1, 3, 1);
            data.BindVertexArray (0);
            data.ArrayBuffer (0, sizeof (vertices), vertices, GL_STATIC_DRAW, GL_FLOAT, 3);
            data.ArrayBuffer (1, sizeof (normals), normals, GL_STATIC_DRAW, GL_FLOAT, 3);
            data.ArrayBuffer (2, sizeof (texcoords), texcoords, GL_STATIC_DRAW, GL_FLOAT, 2);
            data.ElementBuffer (0, sizeof (indices), indices, GL_STATIC_DRAW);
            data.UnbindVertexArray ();
            data.SetDrawCount (6);
            data.SetUseIndices (true);
        }

        static void plane (OpenGLMeshData& data, const float& size)
        {
            GLfloat vertices[12] = {
                -0.5f,-0.5f, 0.f,
                -0.5f, 0.5f, 0.f,
                 0.5f,-0.5f, 0.f,
                 0.5f, 0.5f, 0.f
            };

            GLfloat texcoords[8] = {
                 0.f, 0.f,
                 0.f, 1.f,
                 1.f, 0.f,
                 1.f, 1.f
            };

            GLuint indices[6] = {
                0, 2, 1,
                0, 3, 2
            };

            for (uint i=0; i<36; ++i) {
                vertices[i] *= size;
            }

            data.GenBuffers (1, 3, 1);
            data.BindVertexArray (0);
            data.ArrayBuffer (0, sizeof (vertices), vertices, GL_STATIC_DRAW, GL_FLOAT, 3);
            //data.ArrayBuffer (1, sizeof (vertices), normals, GL_STATIC_DRAW, GL_FLOAT, 3);
            data.ArrayBuffer (2, sizeof (texcoords), texcoords, GL_STATIC_DRAW, GL_FLOAT, 2);
            data.ElementBuffer (0, sizeof (indices), indices, GL_STATIC_DRAW);
            data.UnbindVertexArray ();
            data.SetDrawCount (6);
            data.SetUseIndices (true);
        }

        static void icoshpere (OpenGLMeshData& data, const float& radius, const uint& recursion)
        {
            GLfloat vertices[] = {
                 0.000f, 1.000f, 0.000f,    // Top-most point.
                 0.894f, 0.447f, 0.000f,
                 0.276f, 0.447f, 0.851f,
                -0.724f, 0.447f, 0.526f,
                -0.724f, 0.447f,-0.526f,
                 0.276f, 0.447f,-0.851f,
                 0.724f,-0.447f, 0.526f,
                -0.276f,-0.447f, 0.851f,
                -0.894f,-0.447f, 0.000f,
                -0.276f,-0.447f,-0.851f,
                 0.724f,-0.447f,-0.526f,
                 0.000f,-1.000f, 0.000f    // Bottom-most point.
            };

            GLfloat* normals = vertices;

            GLuint indices[] = {
                0, 2, 1,
                0, 3, 2,
                0, 4, 3,
                0, 5, 4,
                0, 1, 5,
                1, 2, 6,
                6, 2, 7,
                7, 2, 3,
                3, 8, 7,
                8, 3, 4,
                4, 9, 8,
                9, 4, 5,
                5, 10, 9,
                10, 5, 1,
                1, 6, 10,
                11, 9, 10,
                11, 8, 9,
                11, 7, 8,
                11, 6, 7,
                11, 10, 6
            };

            for (uint i=0; i<36; ++i) {
                vertices[i] *= radius;
            }

            data.GenBuffers (1, 2, 1);
            data.BindVertexArray (0);
            data.ArrayBuffer (0, sizeof (vertices), vertices, GL_STATIC_DRAW, GL_FLOAT, 3);
            data.ArrayBuffer (1, sizeof (vertices), normals, GL_STATIC_DRAW, GL_FLOAT, 3);
            data.ElementBuffer (0, sizeof (indices), indices, GL_STATIC_DRAW);
            data.UnbindVertexArray ();
            data.SetDrawCount (60);
            data.SetUseIndices (true);
        }
    }
}

#endif // PRIMITIVES_H
