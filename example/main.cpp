
/* Ask for an OpenGL Core Context */
#define GLFW_INCLUDE_GLCOREARB
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <noon.hpp>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


int main(int argc, char** argv)
{
    GLFWwindow* window;

    /* Initialize the library */
    if ( !glfwInit() ) return -1;

#ifdef __APPLE__
    /* We need to explicitly ask for a 3.2 context on OS X */
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow( 1280, 720, "Hello World", NULL, NULL );
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    glfwWindowHint(GLFW_DECORATED, false);

    noon::Manipulator handle;
    handle.initialize();

    glm::mat4 handleMatrix(1.f);
        

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init( window, true );
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGuiIO& io = ImGui::GetIO();
    io.RenderDrawListsFn = ImGui_ImplGlfwGL3_RenderDrawData;
    //io.Fonts->AddFontFromFileTTF("MyDataFolder\\MyFontFile.ttf", size_in_pixels);  // CORRECT
    io.Fonts->AddFontFromFileTTF( "/usr/local/include/imgui/misc/fonts/Menlo-Regular.ttf", 16 );

    ImGuiStyle& style = ImGui::GetStyle();

    //style.IndentSpacing = 0;
    //style.WindowPadding.x = 0;


    glClearColor( 0.3f, 0.3f, 0.3f, 1.f );

    noon::TrackBall trackball;

    char buf[1024];
 
    double lastTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double dt = currentTime - lastTime;

        lastTime = currentTime;

       

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers



        //glUseProgram(shader_programme);


        glm::mat4 projection = glm::perspective(45.0f*3.14f/180.f, (float)width/(float)height, 0.1f, 100.0f);

        glm::mat4 transform(1.f);
		transform = glm::translate( transform, glm::vec3(0.f, -0.2f, -2.f) );

        glm::mat4 view = transform * trackball.viewMatrix();


        bool handlecon = handle.update( window, handleMatrix, projection, view );
        handle.draw( handleMatrix, projection, view );




        ImGui_ImplGlfwGL3_NewFrame();
        {

        if( !handlecon )
            {
                trackball.update( window, dt );
            }            

            
            // This creates a window
            ImGui::Begin("Window Title Here");

            ImGui::Text("Hello, world!");

            ImGui::InputText("string", buf, 10 );
            if (ImGui::Button("Save"))
            {
                trackball.reset();
            }
            ImGui::InputTextMultiline( "script", buf, 1024, ImVec2(100, 100 ) );
            ImGui::End();

        }
        ImGui::Render();


        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}

