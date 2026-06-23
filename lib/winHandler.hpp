#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 600

class win{
    public:
        inline void pullWindow(){
            glfwInit();
            const char* title = "ECML";
            GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr);
        
            glfwMakeContextCurrent(window);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Sets Background color (R, G, B, Alpha)
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        
            while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
            {
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        
            glfwDestroyWindow(window);
            glfwTerminate();
        }
};