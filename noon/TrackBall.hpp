
#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace noon {

class TrackBall
{
    private:

        double xpos, ypos;
        double pre_xpos, pre_ypos;

        glm::mat4 rotMatrix = glm::mat4(1.f);
        glm::mat4 transMatrix = glm::mat4(1.f);
        glm::mat4 total = glm::mat4(1.f);

        float scale=1.f;
  
    public:

        void update( GLFWwindow* window, const float dt );

        void reset()
        {
            rotMatrix = glm::mat4(1.f);
            transMatrix = glm::mat4(1.f);
            total = glm::mat4(1.f);

            scale=1.f;
        }

        glm::mat4 viewMatrix();
};

inline void TrackBall::update( GLFWwindow* window, const float dt )
{
    pre_xpos = xpos;
    pre_ypos = ypos;
    glfwGetCursorPos( window, &xpos, &ypos );    

    int alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT );
    int ctl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL );

    int state;

    state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if( state == GLFW_PRESS )
    {
        glm::vec3 dev( xpos-pre_xpos, -(ypos-pre_ypos), 0.f );
        float ang = glm::length( dev ) * dt*5.f;

        if( ang > 1e-5f )
        {
            glm::vec3 axis = normalize( glm::cross( glm::vec3( 0.f, 0.f, 1.f ), dev ) );
            rotMatrix = glm::rotate( ang*3.141592f/180.0f, axis );
        }
    }

    state = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE );
    if( state == GLFW_PRESS )
    {
        glm::vec3 dev( xpos-pre_xpos, -(ypos-pre_ypos), 0.f );
        transMatrix = glm::translate( glm::mat4(1.f), dev * dt*0.3f );
    }

    state = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT );
    if( state == GLFW_PRESS )
    {
        const float dev = (ypos-pre_ypos) * dt*0.2f;

        scale += dev;
        if( scale < 1e-10f ) scale = 10e-10f;
    }

}

inline glm::mat4 TrackBall::viewMatrix()
{
    total = transMatrix * rotMatrix * total;

    total[1][0] = 0.f;
    glm::vec3 x = total[0];
    glm::vec3 y = total[1];
    glm::vec3 z = total[2];

    y = glm::normalize( y );
    z = glm::normalize( glm::cross( x, y ) );
    x = glm::normalize( glm::cross( y, z ) );

    total[0] = glm::vec4( x, 0.f );
    total[1] = glm::vec4( y, 0.f );
    total[2] = glm::vec4( z, 0.f );

    transMatrix = glm::mat4(1.f);
    rotMatrix = glm::mat4(1.f);
  
    glm::mat4 scaleMatrix = glm::scale( glm::mat4(1.f), glm::vec3(scale,scale,scale) );    
    
    return  scaleMatrix * total;
}

}

