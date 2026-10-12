#include "winLib.hpp"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 600

class win{
    private:
        int CurrSize = 0;
        std::string charQ;
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
                    CurrSize = charQ.size();
                }
            }
        }
        
        inline void ScrollCommands(GLFWwindow* window, std::string pwd){
            float startY = 50.0f;
            std::string cmd = Commands.at(Index);
            
            if (Commands.empty()){
                return;
            }
            
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
                if (Index > 0){
                    Sleep(665);
                    --Index;
                    std::string lineToDraw = pwd + " " + cmd;
                    printText(20.0f, startY, lineToDraw.c_str());
                    startY += 20.0f;
                }
            }
            
            else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
                if (Index + 1 < static_cast<int>(Commands.size())){
                    Sleep(665);
                    ++Index;
                    std::string lineToDraw = pwd + " " + cmd;
                    printText(20.0f, startY, lineToDraw.c_str());
                    startY += 20.0f;
                }
            }
        }
        
    public:
        std::vector<std::string> Commands;
        inline void pullWindow(std::string cur_dir){
            glfwInit();
            const char* title = "ECML";
            GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr);
            
            glfwMakeContextCurrent(window);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Sets Background color (R, G, B, Alpha)
            glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
            
            // --- SETUP 2D PROJECTION MATRIX FOR TEXT ---
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            // Top-left is (0,0), bottom-right is (WINDOW_WIDTH, WINDOW_HEIGHT)
            glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
            {
                glClear(GL_COLOR_BUFFER_BIT);
                Checkgetch(window);
                ScrollCommands(window, cur_dir);                
                DrawChars(window,cur_dir);
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        //Function to push font on screen
        inline void printText(float x, float y, const char *text) {
            char buffer[99999]; // Buffer for quad vertices
            int num_quads;
            
            // Generates vertex data for the string
            num_quads = stb_easy_font_print(x, y, (char*)text, NULL, buffer, sizeof(buffer));
            
            // Render using legacy OpenGL vertex arrays
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 16, buffer);
            glDrawArrays(GL_QUADS, 0, num_quads * 4);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        
        //Function to determine what font to push on screen
        inline void DrawChars(GLFWwindow* window, std::string pwd, std::string input = "") {
            // Set text color to White
            glColor3f(1.0f, 1.0f, 1.0f);
    
            // Print active input queue at position (20, 20)
            if (!charQ.empty()) {
                std::string activeLine = pwd + " " + charQ;
                printText(20.0f, 20.0f, activeLine.c_str());
            }

            // Print Commands List
            ScrollCommands(window, pwd);
        }
    };