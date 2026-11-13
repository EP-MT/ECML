#include "winLib.hpp"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 600
class win{
    private:
        std::string charQ;
        int historyIndex = -1;
        
        inline void Getch(GLFWwindow*, unsigned int codepoint){
            if (codepoint >= 32 && codepoint <= 126) {
                charQ.push_back(static_cast<char>(codepoint));
                historyIndex = -1;
            }
        }
        
        inline void SubmitCommand(GLFWwindow* handle, int key, int, int action, int){
            if (action != GLFW_PRESS) {
                return;
            }

            if (key == GLFW_KEY_ENTER) {
                if (!charQ.empty()) {
                    Commands.push_back(charQ);
                    pendingCommands.push_back(charQ);
                    charQ.clear();
                    historyIndex = -1;
                }
            }

            else if (key == GLFW_KEY_BACKSPACE && !charQ.empty()) {
                charQ.pop_back();
                historyIndex = -1;
            }

            else if (key == GLFW_KEY_UP && !Commands.empty()) {
                if (historyIndex < static_cast<int>(Commands.size()) - 1) {
                    ++historyIndex;
                    charQ = Commands[Commands.size() - 1 - historyIndex];
                }
            }
            
            else if (key == GLFW_KEY_DOWN && historyIndex >= 0) {
                --historyIndex;
                if (historyIndex < 0) {
                    charQ.clear();
                }

                else {
                    charQ = Commands[Commands.size() - 1 - historyIndex];
                }
            }
        }
        
    public:
        std::vector<std::string> Commands;
        
        inline void pullWindow(std::string& cur_dir, const std::function<bool(const std::string&)>& onCommand){
            if (!glfwInit()) {
                std::cerr << "Unable to initialize GLFW\n";
                return;
            }

            const char* title = "ECML";
            GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr);
            if (window == nullptr) {
                std::cerr << "Unable to create the ECML window\n";
                glfwTerminate();
                return;
            }

            glfwSetWindowUserPointer(window, this);
            glfwSetCharCallback(window, [](GLFWwindow* handle, unsigned int codepoint) {
                static_cast<win*>(glfwGetWindowUserPointer(handle))->Getch(handle, codepoint);
            });
            glfwSetKeyCallback(window, [](GLFWwindow* handle, int key, int scancode, int action, int mods) {
                static_cast<win*>(glfwGetWindowUserPointer(handle))->SubmitCommand(
                    handle, key, scancode, action, mods
                );
            });

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
                DrawChars(window,cur_dir);
                glfwSwapBuffers(window);
                glfwPollEvents();

                for (const std::string& command : pendingCommands) {
                    if (!onCommand(command)) {
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                        break;
                    }
                }
                pendingCommands.clear();
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
            std::string activeLine = pwd + "> " + charQ;
            printText(20.0f, 20.0f, activeLine.c_str());

            const std::size_t firstCommand =
                Commands.size() > 25 ? Commands.size() - 25 : 0;
            float y = 50.0f;
            for (std::size_t i = firstCommand; i < Commands.size(); ++i) {
                printText(20.0f, y, Commands[i].c_str());
                y += 20.0f;
            }
        }

    private:
        std::vector<std::string> pendingCommands;
    };