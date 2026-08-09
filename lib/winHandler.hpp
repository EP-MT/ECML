#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <windows.h>
#include <string>
#include <cmath>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 600

class win{
    private:
        int CurrSize = 0;
        std::string charQ;
        std::vector<std::string> Commands;
        int Index = 0;

        inline void Getch(GLFWwindow* window, unsigned int AlphaNum){
        charQ.push_back(static_cast<char>(AlphaNum));
        }

        inline void SubmitCommand(GLFWwindow* window, int key, int scancode, int action, int mods){
            if (action == GLFW_PRESS && key == GLFW_KEY_ENTER){
                if (!charQ.empty()){
                    Commands.push_back(charQ);
                    Index = static_cast<int>(Commands.size()) - 1;
                    charQ.clear();
                }
            }
        }

        inline void Checkgetch(GLFWwindow* window){
            if(!charQ.empty()){
                if(charQ.size() != CurrSize){
                    std::cout << charQ << std::endl;
                    CurrSize = charQ.size();
                }
            }
        }

        inline void ScrollCommands(GLFWwindow* window){
            if (Commands.empty()){
                return;
            }

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
                if (Index > 0){
                    Sleep(665);
                    --Index;
                }
                std::cout << Commands.at(Index) << std::endl;
            }

            else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
                if (Index + 1 < static_cast<int>(Commands.size())){
                    Sleep(665);
                    ++Index;
                }
                std::cout << Commands.at(Index) << std::endl;
            }
        }

        inline void textMain(GLFWwindow* window){
            Checkgetch(window);
            ScrollCommands(window);
        }

        inline void DrawChars(){
            std::cout << "Drawing" << std::endl;
        }

        inline void LoadFont(const char* filename){
            std::cout << filename << std::endl;
        }
        
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