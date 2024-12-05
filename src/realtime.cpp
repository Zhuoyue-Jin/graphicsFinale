#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"

#include "utils/sceneparser.h"
#include "utils/helper.h"
#include "shaderloader.h"
#include <glm/gtc/type_ptr.hpp>
#include "utils/paint.h"
#include "debug.h"


// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Delete shader program
    if (m_shader) {
        glDeleteProgram(m_shader);
        m_shader = 0;
    }

    clearBuffers();
    glDeleteProgram(m_texture_shader);
    glDeleteVertexArrays(1, &m_fullscreen_vao);
    glDeleteBuffers(1, &m_fullscreen_vbo);

    // Task 35: Delete OpenGL memory here
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    doneCurrent();
    // Clear vertex collection
    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_defaultFBO = 2;
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    m_texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/quad.vert", ":/resources/shaders/grayscale.frag");

    glActiveTexture(GL_TEXTURE0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "txture"), 0);
    glUseProgram(0);




    std::vector<GLfloat> fullscreen_quad_data = {
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // Top-left corner
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left corner
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // Top-right corner
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left corner
        1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right corner
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f   // Top-right corner
    };


    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);



    // Task 14: modify the code below to add a second attribute to the vertex attribute array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeFBO();
}

void Realtime::makeFBO()
{
    this->makeCurrent();
    glGenTextures(1, &m_fbo_texture);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);

}

void Realtime::paintGL()
{
    this->makeCurrent();
    paint(this, data, m_shader, m_model, m_view, m_proj, vertices, settings,m_texture_shader,m_fbo,m_fbo_texture, m_fullscreen_vao,m_fbo_width,m_fbo_height,m_defaultFBO);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;
    // Task 34: Regenerate your FBOs
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);
    makeFBO();
    glViewport(0, 0, m_screen_width, m_screen_height);
    // Students: anything requiring OpenGL calls when the program starts should be done here
}

void Realtime::clearBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(GLfloat(0),GLfloat(0),GLfloat(0),GLfloat(0));
    // Clear VAOs
    glDeleteVertexArrays(1, &vertices.cu_vao);
    glDeleteVertexArrays(1, &vertices.s_vao);
    glDeleteVertexArrays(1, &vertices.cy_vao);
    glDeleteVertexArrays(1, &vertices.co_vao);

    // Clear VBOs
    glDeleteBuffers(1, &vertices.cu_vbo);
    glDeleteBuffers(1, &vertices.s_vbo);
    glDeleteBuffers(1, &vertices.cy_vbo);
    glDeleteBuffers(1, &vertices.co_vbo);

    // Clear vertex data containers
    vertices.cube.clear();
    vertices.sphere.clear();
    vertices.cylinder.clear();
    vertices.cone.clear();

    // Reset VAO and VBO handles
    vertices.cu_vao = 0;
    vertices.s_vao = 0;
    vertices.cy_vao = 0;
    vertices.co_vao = 0;
    vertices.cu_vbo = 0;
    vertices.s_vbo = 0;
    vertices.cy_vbo = 0;
    vertices.co_vbo = 0;
}

void Realtime::sceneChanged()
{
    clearBuffers();

    SceneParser::parse(settings.sceneFilePath, data);
    vertices.instance = this;
    figureVertices(data,vertices);
    calcMatrices(data,m_view,m_proj,width(),height());

    if(settings.extraCredit2)
    {
        update();
        return;
    }

    int param_count = data.shapes.size();
    int baseParam1 = settings.shapeParameter1;
    int baseParam2 = settings.shapeParameter2;

    if (settings.extraCredit1)
        extraCredits1(baseParam1,baseParam2,param_count);

    scan_primitives(baseParam1, baseParam2, vertices);
    update();
}

void Realtime::settingsChanged()
{
    if(settings.sceneFilePath!="")
    {
        clearBuffers();
        vertices.instance = this;
        figureVertices(data,vertices);
        calcMatrices(data,m_view,m_proj,width(),height());

        if(settings.extraCredit2)
        {
            update();
            return;
        }

        int param_count = data.shapes.size();


        int baseParam1 = settings.shapeParameter1;
        int baseParam2 = settings.shapeParameter2;

        if (settings.extraCredit1)
            extraCredits1(baseParam1,baseParam2,param_count);


        scan_primitives(baseParam1, baseParam2, vertices);
    }

    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        calcMatrices(data,m_view,m_proj,width(),height(),deltaX,deltaY);

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around
    calcMatrices(data,m_view,m_proj,width(),height(),m_keyMap,deltaTime);

    update(); // asks for a PaintGL() call to occur
}

// DO NOT EDIT
void Realtime::saveViewportImage(std::string filePath) {
    // Make sure we have the right context and everything has been drawn
    makeCurrent();

    int fixedWidth = 1024;
    int fixedHeight = 768;

    // Create Frame Buffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create a color attachment texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fixedWidth, fixedHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Optional: Create a depth buffer if your rendering uses depth testing
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fixedWidth, fixedHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    // Render to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, fixedWidth, fixedHeight);

    // Clear and render your scene here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintGL();

    // Read pixels from framebuffer
    std::vector<unsigned char> pixels(fixedWidth * fixedHeight * 3);
    glReadPixels(0, 0, fixedWidth, fixedHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    // Unbind the framebuffer to return to default rendering to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Convert to QImage
    QImage image(pixels.data(), fixedWidth, fixedHeight, QImage::Format_RGB888);
    QImage flippedImage = image.mirrored(); // Flip the image vertically

    // Save to file using Qt
    QString qFilePath = QString::fromStdString(filePath);
    if (!flippedImage.save(qFilePath)) {
        std::cerr << "Failed to save image to " << filePath << std::endl;
    }

    // Clean up
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}
