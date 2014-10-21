#include "JavaScriptView.h"

namespace berry {

JavaScriptView::JavaScriptView(int width, int height, const char *title):
    width(width),
    height(height)
{
    // Create the global JS context
    this->jsGlobalContext = duk_create_heap_default();

    // Create a window
    if (!glfwInit()) {
        printf("[JavaScriptView]: unable to init GLFW\n");
    }
    this->window = glfwCreateWindow(width, height, title, /*glfwGetPrimaryMonitor()*/NULL, NULL);
    if (!this->window) {
        printf("[JavaScriptView]: unable to create window\n");
    }

    // Setup OpenGL context
    glfwMakeContextCurrent(this->window);

    // Setup event callbacks
    theInput.setDelegate(this);
    glfwSetKeyCallback(this->window, &Input::keyCallback);

    // Load boot script
    this->loadScriptAtPath(BERRY_BOOT_JS);
}
JavaScriptView::~JavaScriptView()
{
    // Cleanup JavaScript engine
    duk_destroy_heap(this->jsGlobalContext);

    // Cleanup GLFW
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void JavaScriptView::loadScriptAtPath(const char *path)
{
    duk_eval_file(this->jsGlobalContext, path);
}

void JavaScriptView::startRunning()
{
    while (!glfwWindowShouldClose(this->window)) {
        this->run();
    }
}

void JavaScriptView::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void JavaScriptView::run()
{
    float ratio;
    int width, height;

    glfwGetFramebufferSize(this->window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.0f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.6f, -0.4f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.6f, -0.4f, 0.0f);
        glColor3f(0.0f, 0.f, 1.0f);
        glVertex3f(0.f, 0.6f, 0.0f);
    glEnd();

    // RAF
    duk_eval_string(this->jsGlobalContext, "berry.tickAnimFrame();");

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

}
