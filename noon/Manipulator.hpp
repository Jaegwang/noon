
#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STRINGIFY(s) #s

namespace noon {

class Manipulator
{
    private:

        // hit points;
        glm::vec3 hit_point_yz_red; 
        glm::vec3 hit_point_xz_green;
        glm::vec3 hit_point_xy_blue;

        // check click.
        glm::vec3 clicked_point = glm::vec3(0.f);
        bool clicked_planes[3]; // 0 : yz, 1: xz, 2 : xy
        bool clicked_lines[3];
        bool clicked_circles[3];

        // mode;
        int handle_mode=0; // 0 : translate, 1 : rotate, 2 : scale

    private:

        /* opengl buffer */
        // translate handle vertices;
        GLuint _trans_x_red_vbo, _trans_x_red_vao;
        GLuint _trans_y_green_vbo, _trans_y_green_vao;
        GLuint _trans_z_blue_vbo, _trans_z_blue_vao;

        // rotation handle vertices;
        std::vector< glm::vec3 > _rot_yz_plane_red;
        std::vector< glm::vec3 > _rot_xz_plane_green;
        std::vector< glm::vec3 > _rot_xy_plane_blue;

        GLuint _rot_yz_red_vbo, _rot_yz_red_vao;
        GLuint _rot_xz_green_vbo, _rot_xz_green_vao; 
        GLuint _rot_xy_blue_vbo, _rot_xy_blue_vao;

        // scale handle vertices;
        std::vector< glm::vec3 > _scale_x_red;
        std::vector< glm::vec3 > _scale_y_green;
        std::vector< glm::vec3 > _scale_z_blue;

        GLuint _scale_x_red_vbo, _scale_x_red_vao;
        GLuint _scale_y_green_vbo, _scale_y_green_vao;
        GLuint _scale_z_blue_vbo, _scale_z_blue_vao;        

    private:

        const char* vs_source = STRINGIFY(
            \n#version 400\n
            in vec3 vp;
            uniform mat4 projectionMatrix;
            uniform mat4 modelviewMatrix;
            uniform vec3 uTranslate;

            void main()
            {
                gl_Position = projectionMatrix * modelviewMatrix * vec4( vp+uTranslate, 1.0 );
            }
        );

        const char* fs_source = STRINGIFY(
            \n#version 400\n
            out vec4 out_Color;
            uniform vec3 uColor;

            void main()
            {
                out_Color = vec4( uColor, 1.0 );
            }
        );

        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint shaderProgram;

    public:

        void initialize()
        {
            const float w=0.05f;

            std::vector< glm::vec3 > _trans_x_red;
            _trans_x_red.push_back( glm::vec3( 0.0f, 0.f, 0.0f ) );
            _trans_x_red.push_back( glm::vec3( 1.0f, 0.f, 0.0f ) );
            _trans_x_red.push_back( glm::vec3( 1.0f, 0.f, 0.0f ) );
            _trans_x_red.push_back( glm::vec3( 1.0f-w, 0.f, w  ) );
            _trans_x_red.push_back( glm::vec3( 1.0f-w, 0.f, -w ) );


            glGenBuffers( 1, &_trans_x_red_vbo );
            glBindBuffer( GL_ARRAY_BUFFER, _trans_x_red_vbo );
            glBufferData( GL_ARRAY_BUFFER, _trans_x_red.size() * sizeof(glm::vec3), &(_trans_x_red[0][0]), GL_STATIC_DRAW );

            glGenVertexArrays( 1, &_trans_x_red_vao );
            glBindVertexArray( _trans_x_red_vao );
            glEnableVertexAttribArray(0);
            glBindBuffer( GL_ARRAY_BUFFER, _trans_x_red_vao );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL );


            std::vector< glm::vec3 > _trans_y_green;
            _trans_y_green.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
            _trans_y_green.push_back( glm::vec3( 0.0f, 1.0f, 0.0f ) );
            _trans_y_green.push_back( glm::vec3( 0.0f, 1.0f, 0.0f ) );
            _trans_y_green.push_back( glm::vec3( w, 1.0f-w, 0.0f  ) );
            _trans_y_green.push_back( glm::vec3( -w, 1.0f-w, 0.0f ) );            


            glGenBuffers( 1, &_trans_y_green_vbo );
            glBindBuffer( GL_ARRAY_BUFFER, _trans_y_green_vbo );
            glBufferData( GL_ARRAY_BUFFER, _trans_y_green.size() * sizeof(glm::vec3), &(_trans_y_green[0][0]), GL_STATIC_DRAW );

            glGenVertexArrays( 1, &_trans_y_green_vao );
            glBindVertexArray( _trans_y_green_vao );
            glEnableVertexAttribArray(0);
            glBindBuffer( GL_ARRAY_BUFFER, _trans_y_green_vao );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL );


            std::vector< glm::vec3 > _trans_z_blue;
            _trans_z_blue.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
            _trans_z_blue.push_back( glm::vec3( 0.0f, 0.0f, 1.0f ) );
            _trans_z_blue.push_back( glm::vec3( 0.0f, 0.0f, 1.0f ) );
            _trans_z_blue.push_back( glm::vec3( w, 0.0f, 1.0f-w  ) );
            _trans_z_blue.push_back( glm::vec3( -w, 0.0f, 1.0f-w ) );


            glGenBuffers( 1, &_trans_z_blue_vbo );
            glBindBuffer( GL_ARRAY_BUFFER, _trans_z_blue_vbo );
            glBufferData( GL_ARRAY_BUFFER, _trans_z_blue.size() * sizeof(glm::vec3), &(_trans_z_blue[0][0]), GL_STATIC_DRAW );

            glGenVertexArrays( 1, &_trans_z_blue_vao );
            glBindVertexArray( _trans_z_blue_vao );
            glEnableVertexAttribArray(0);
            glBindBuffer( GL_ARRAY_BUFFER, _trans_z_blue_vao );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL );


            for( int i=0; i<360; ++i )
            {
                const float a = sin((float)i*3.1416f/180.f);
                const float b = cos((float)i*3.1416f/180.f);

                _rot_yz_plane_red.push_back( glm::vec3( 0.f, a, b ) );
                _rot_xz_plane_green.push_back( glm::vec3( a, 0.f, b ) );
                _rot_xy_plane_blue.push_back( glm::vec3( a, b, 0.f ) );
            }

            glGenBuffers( 1, &_rot_yz_red_vbo );
            glBindBuffer( GL_ARRAY_BUFFER, _rot_yz_red_vbo );
            glBufferData( GL_ARRAY_BUFFER, _rot_yz_plane_red.size() * sizeof(glm::vec3), &(_rot_yz_plane_red[0][0]), GL_STATIC_DRAW );

            glGenVertexArrays( 1, &_rot_yz_red_vao );
            glBindVertexArray( _rot_yz_red_vao );
            glEnableVertexAttribArray(0);
            glBindBuffer( GL_ARRAY_BUFFER, _rot_yz_red_vao );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL );



            glGenBuffers( 1, &_rot_xz_green_vbo );
            glBindBuffer( GL_ARRAY_BUFFER, _rot_xz_green_vbo );
            glBufferData( GL_ARRAY_BUFFER, _rot_xz_plane_green.size() * sizeof(glm::vec3), &(_rot_xz_plane_green[0][0]), GL_STATIC_DRAW );

            glGenVertexArrays( 1, &_rot_xz_green_vao );
            glBindVertexArray( _rot_xz_green_vao );
            glEnableVertexAttribArray(0);
            glBindBuffer( GL_ARRAY_BUFFER, _rot_xz_green_vao );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL );


            glGenBuffers( 1, &_rot_xy_blue_vbo );
            glBindBuffer( GL_ARRAY_BUFFER, _rot_xy_blue_vbo );
            glBufferData( GL_ARRAY_BUFFER, _rot_xy_plane_blue.size() * sizeof(glm::vec3), &(_rot_xy_plane_blue[0][0]), GL_STATIC_DRAW );

            glGenVertexArrays( 1, &_rot_xy_blue_vao );
            glBindVertexArray( _rot_xy_blue_vao );
            glEnableVertexAttribArray(0);
            glBindBuffer( GL_ARRAY_BUFFER, _rot_xy_blue_vao );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL );



            ////

            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource( vertexShader, 1, &vs_source, NULL );
            glCompileShader( vertexShader );

            GLint success = 0;
            glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
            if (success == GL_FALSE) {    
                std::cout<<"fail"<<std::endl;

                GLchar infoLog[1024];
                glGetShaderInfoLog( vertexShader, 1024, NULL, infoLog);

                std::cout<< infoLog << std::endl;
            }

            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource( fragmentShader, 1, &fs_source, NULL );
            glCompileShader( fragmentShader );

            shaderProgram = glCreateProgram();
            glAttachShader( shaderProgram, vertexShader );
            glAttachShader( shaderProgram, fragmentShader );
            glLinkProgram( shaderProgram );

        }

        void draw( const glm::mat4& originMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelViewMatrix )
        {
            glUseProgram( shaderProgram );

            GLuint projectionMatrixId = glGetUniformLocation( shaderProgram, "projectionMatrix" );
            GLuint modelviewMatrixId = glGetUniformLocation( shaderProgram, "modelviewMatrix" ); 
            GLuint translateId = glGetUniformLocation( shaderProgram, "uTranslate" );
            GLuint colorId = glGetUniformLocation( shaderProgram, "uColor" );

            glm::mat4 modelViewOrigin = modelViewMatrix * originMatrix;

            glUniformMatrix4fv( projectionMatrixId, 1, GL_FALSE, glm::value_ptr(projectionMatrix) );
            glUniformMatrix4fv( modelviewMatrixId, 1, GL_FALSE, glm::value_ptr(modelViewOrigin) );

            glm::vec3 trans(0.f);
            glm::vec3 color;

            glUniform3fv( translateId, 1, glm::value_ptr(trans) );

            if( !clicked_lines[0] ) color = glm::vec3( 1.0f, 0.f, 0.f );
            else color = glm::vec3( 1.f, 1.f, 1.f );
            glUniform3fv( colorId, 1, ( &color[0] ) );

            glBindVertexArray( _trans_x_red_vao );
            glDrawArrays( GL_LINES, 0, 2 );
            glDrawArrays( GL_TRIANGLES, 2, 3 );


            if( !clicked_lines[1] ) color = glm::vec3( 0.0f, 1.f, 0.f );
            else color = glm::vec3( 1.f, 1.f, 1.f );
            glUniform3fv( colorId, 1, ( &color[0] ) );

            glBindVertexArray( _trans_y_green_vao );
            glDrawArrays( GL_LINES, 0, 2 );
            glDrawArrays( GL_TRIANGLES, 2, 3 );

            if( !clicked_lines[2] ) color = glm::vec3( 0.0f, 0.f, 1.f );
            else color = glm::vec3( 1.f, 1.f, 1.f );
            glUniform3fv( colorId, 1, ( &color[0] ) );

            glBindVertexArray( _trans_z_blue_vao );
            glDrawArrays( GL_LINES, 0, 2 );
            glDrawArrays( GL_TRIANGLES, 2, 3 );


            if( !clicked_circles[0] ) color = glm::vec3( 1.0f, 0.f, 0.f );
            else color = glm::vec3( 1.f, 1.f, 1.f );
            glUniform3fv( colorId, 1, ( &color[0] ) );

            glBindVertexArray( _rot_yz_red_vao );
            glDrawArrays( GL_LINE_LOOP, 0, _rot_yz_plane_red.size() );


            if( !clicked_circles[1] ) color = glm::vec3( 0.f, 1.f, 0.f );
            else color = glm::vec3( 1.f, 1.f, 1.f );
            glUniform3fv( colorId, 1, ( &color[0] ) );

            glBindVertexArray( _rot_xz_green_vao );
            glDrawArrays( GL_LINE_LOOP, 0, _rot_xz_plane_green.size() );    

            if( !clicked_circles[2] ) color = glm::vec3( 0.f, 0.f, 1.f );
            else color = glm::vec3( 1.f, 1.f, 1.f );
            glUniform3fv( colorId, 1, ( &color[0] ) );

            glBindVertexArray( _rot_xy_blue_vao );
            glDrawArrays( GL_LINE_LOOP, 0, _rot_xy_plane_blue.size() );


            // Draw clicked point
            color = glm::vec3( 1.f, 1.f, 0.f );

            glUniform3fv( colorId, 1, ( &color[0] ) );
            glUniform3fv( translateId, 1, glm::value_ptr(clicked_point) );            

            glBindVertexArray( _trans_x_red_vao );
            glDrawArrays( GL_POINTS, 0, 1 );            


            glUseProgram( 0 );
        }

        glm::vec3 rayIntersection( const glm::vec3& origin, const glm::vec3& dir, const glm::vec3& nor, const glm::vec3& center )
        {
            const float denom = glm::dot( nor, dir );
            if( abs(denom) > 1e-10f )
            {
                float t = glm::dot( (center-origin), nor ) / denom;
                if( t >= 0.f )
                {
                    return origin + dir * t;
                }
            }

            return glm::vec3( 1e+30f );
        }

        bool update( GLFWwindow* window, glm::mat4& originMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelViewMatrix )
        {
            int w, h;
            glfwGetWindowSize( window, &w, &h );

            double xpos, ypos;
            glfwGetCursorPos( window, &xpos, &ypos );

            glm::vec2 point = glm::vec2( (xpos/(float)w)*2.f-1.f, (ypos/(float)h)*2.f-1.f );

            // 1. Projection to view
            glm::vec4 ray = glm::vec4(point.x, point.y, -1.f, 1.0f);

            ray = glm::inverse(projectionMatrix) * ray;
            ray = glm::vec4(ray.x, -ray.y, -1.f, 0.f);

            // 2. View to world
            ray = glm::inverse( modelViewMatrix ) * ray;
            ray.w = 0.f;
            ray = glm::normalize(ray);        

            glm::vec4 camPos = glm::inverse( modelViewMatrix * originMatrix ) * glm::vec4(0.f,0.f,0.f,1.f);


            hit_point_yz_red   = rayIntersection( glm::vec3(camPos), glm::vec3(ray), glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,0.f,0.f) );
            hit_point_xz_green = rayIntersection( glm::vec3(camPos), glm::vec3(ray), glm::vec3(0.f,1.f,0.f), glm::vec3(0.f,0.f,0.f) );
            hit_point_xy_blue  = rayIntersection( glm::vec3(camPos), glm::vec3(ray), glm::vec3(0.f,0.f,1.f), glm::vec3(0.f,0.f,0.f) );


            int state = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT );
            if( state == GLFW_PRESS )
            {
                if( hit_point_yz_red.x   >= 1e+30f ) hit_point_yz_red   = clicked_point;
                if( hit_point_xz_green.x >= 1e+30f ) hit_point_xz_green = clicked_point;
                if( hit_point_xy_blue.x  >= 1e+30f ) hit_point_xy_blue  = clicked_point;

                if( clicked_lines[0] == true )
                {
                    const float px = clicked_planes[1] ? hit_point_xz_green.x : clicked_planes[2] ? hit_point_xy_blue.x : clicked_point.x;
                    originMatrix[3].x += px-clicked_point.x;

                    return true;
                }                
                else if( clicked_lines[1] == true )
                {
                    const float py = clicked_planes[0] ? hit_point_yz_red.y : clicked_planes[2] ? hit_point_xy_blue.y : clicked_point.y;
                    originMatrix[3].y += py-clicked_point.y;

                    return true;
                }
                else if( clicked_lines[2] == true )
                {
                    const float pz = clicked_planes[0] ? hit_point_yz_red.z : clicked_planes[1] ? hit_point_xz_green.z : clicked_point.z;
                    originMatrix[3].z += pz-clicked_point.z;

                    return true;
                }
            }
            else
            {
                clicked_lines[0]=clicked_lines[1]=clicked_lines[2] = false;
                clicked_planes[0]=clicked_planes[1]=clicked_planes[2] = false;

                // on yz red plane;
                if( hit_point_yz_red.y > 0.f && hit_point_yz_red.y < 1.f && abs(hit_point_yz_red.z) < 0.05f )
                {
                    clicked_lines[1] = true;
                    clicked_planes[0] = true;
                    clicked_point = glm::vec3( 0.f, hit_point_yz_red.y, 0.f );
                }
                else if( hit_point_yz_red.z > 0.f && hit_point_yz_red.z < 1.f && abs(hit_point_yz_red.y) < 0.05f )
                {
                    clicked_lines[2] = true;
                    clicked_planes[0] = true;
                    clicked_point = glm::vec3( 0.f, 0.f, hit_point_yz_red.z );                    
                }
                // on xz green plan;
                else if( hit_point_xz_green.x > 0.f && hit_point_xz_green.x < 1.f && abs(hit_point_xz_green.z) < 0.05f )
                {
                    clicked_lines[0] = true;
                    clicked_planes[1] = true;
                    clicked_point = glm::vec3( hit_point_xz_green.x, 0.f, 0.f );
                }
                else if( hit_point_xz_green.z > 0.f && hit_point_xz_green.z < 1.f && abs(hit_point_xz_green.x) < 0.05f )
                {
                    clicked_lines[2] = true;
                    clicked_planes[1] = true;
                    clicked_point = glm::vec3( 0.f, 0.f, hit_point_xz_green.z );
                }
                // on xy blue plane;
                else if( hit_point_xy_blue.x > 0.f && hit_point_xy_blue.x < 1.f && abs(hit_point_xy_blue.y) < 0.05f )
                {
                    clicked_lines[0] = true;
                    clicked_planes[2] = true;
                    clicked_point = glm::vec3( hit_point_xy_blue.x, 0.f, 0.f );
                }
                else if( hit_point_xy_blue.y > 0.f && hit_point_xy_blue.y < 1.f && abs(hit_point_xy_blue.x) < 0.05f )
                {
                    clicked_lines[1] = true;
                    clicked_planes[2] = true;
                    clicked_point = glm::vec3( 0.f, hit_point_xy_blue.y, 0.f );
                }


                clicked_circles[0]=clicked_circles[1]=clicked_circles[2] = false;
                if( abs( glm::length(hit_point_yz_red-glm::vec3(0.f))-1.f ) < 0.05f )
                {
                    clicked_circles[0] = true;
                    clicked_point = glm::normalize( hit_point_yz_red );
                    clicked_planes[0] = true;
                }
                else if( abs( glm::length(hit_point_xz_green-glm::vec3(0.f))-1.f ) < 0.05f )
                {
                    clicked_circles[1] = true;
                    clicked_point = glm::normalize( hit_point_xz_green );
                    clicked_planes[1] = true;
                }        
                else if( abs( glm::length(hit_point_xy_blue-glm::vec3(0.f))-1.f ) < 0.05f )
                {
                    clicked_circles[2] = true;
                    clicked_point = glm::normalize( hit_point_xy_blue );
                    clicked_planes[2] = true;
                }
            }

            return false;            
        }

};

}
// http://antoniak.in/blog/2017/04/24/quick-tip-mouse-picking/

