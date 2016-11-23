#include "raster_renderer.h"
#include <string>

namespace graphics
{

    static void ToInverseTranspose (glm::mat4& m)
    {
        glm::mat3 tmp = glm::mat3 (m);
        tmp = glm::inverse (tmp);
        tmp = glm::transpose (tmp);

        m[0][0] = tmp[0][0]; m[0][1] = tmp[0][1]; m[0][2] = tmp[0][2];
        m[1][0] = tmp[1][0]; m[1][1] = tmp[1][1]; m[1][2] = tmp[1][2];
        m[2][0] = tmp[2][0]; m[2][1] = tmp[2][1]; m[2][2] = tmp[2][2];
    }

    void RasterRenderer::render (const Scene& scene)
    {
        Camera* camera = scene.p_active_camera;
        glm::mat4 projection = camera->projection ();

        Light lights[scene.num_lights];
        for (uint n=0; n<scene.num_lights; ++n) {
            lights[n] = scene.lights[n];
            lights[n].position = vec3 (camera->view () * vec4 (lights[n].position, 1.f));
        }

        for (uint i=0; i<scene.models.size (); ++i) {

            Model* model = scene.models[i];
            //std::cout << model->GetType () << "\n";

            glm::mat4 modelview = camera->view () * model->transform.transform;
            glm::mat4 modelviewinvtrans = modelview;
            ToInverseTranspose (modelviewinvtrans);

            GLuint u_ProjectionMatrix = model->p_shader->GetUniform ("ProjectionMatrix");
            GLuint u_ModelViewMatrix = model->p_shader->GetUniform ("ModelViewMatrix");
            GLuint u_ModelViewMatrixInverseTranspose = model->p_shader->GetUniform ("ModelViewMatrixInverseTranspose");
            GLuint u_ambient_color = model->p_shader->GetUniform ("ambient_color");
            GLuint u_diffuse_color = model->p_shader->GetUniform ("diffuse_color");
            GLuint u_specular_color = model->p_shader->GetUniform ("specular_color");
            GLuint u_emission_color = model->p_shader->GetUniform ("emission_color");
            GLuint u_shininess = model->p_shader->GetUniform ("shininess");

            GLuint u_enablelighting = model->p_shader->GetUniform ("enablelighting");
            GLuint u_num_lights = model->p_shader->GetUniform ("num_lights");

            model->p_shader->Bind ();
            glUniformMatrix4fv (u_ProjectionMatrix, 1, GL_FALSE, glm::value_ptr (projection));
            glUniformMatrix4fv (u_ModelViewMatrix, 1, GL_FALSE, glm::value_ptr (modelview));
            glUniformMatrix4fv (u_ModelViewMatrixInverseTranspose, 1, GL_FALSE, glm::value_ptr (modelviewinvtrans));
            glUniform4fv (u_ambient_color, 1, glm::value_ptr (scene.ambient));
            glUniform4fv (u_diffuse_color, 1, glm::value_ptr (model->material.diffuse));
            glUniform4fv (u_specular_color, 1, glm::value_ptr (model->material.specular));
            glUniform4fv (u_emission_color, 1, glm::value_ptr (model->material.emission));
            glUniform1f (u_shininess, model->material.shininess);

            glUniform1i (u_enablelighting, true);
            glUniform1i (u_num_lights, scene.num_lights);

            for (uint n=0; n<scene.num_lights; ++n) {
                std::string number = std::to_string (n);
                GLuint u_lights_pos = model->p_shader->GetUniform (("lights["+number+"].position").c_str());
                GLuint u_lights_col = model->p_shader->GetUniform (("lights["+number+"].color").c_str());

                glUniform4fv (u_lights_pos, 1, glm::value_ptr (lights[n].position));
                glUniform4fv (u_lights_col, 1, glm::value_ptr (lights[n].color));
            }

            model->gldata.Draw ();
        }
    }

}
