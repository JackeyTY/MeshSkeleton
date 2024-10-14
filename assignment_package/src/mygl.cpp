#include "mygl.h"
#include <glm_includes.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QDateTime>
#include <QDebug>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_worldAxes(this),
      m_progLambert(this), m_progFlat(this), m_progInstanced(this), m_progSky(this), m_progShadow(this), m_progPlayer(this),
      m_terrain(this), m_player(glm::vec3(117.f, 160.f, 197.f), m_terrain),
      m_time(QDateTime::currentMSecsSinceEpoch()), m_shaderTime(0), m_timeElapsed(0.f), m_timeStep(1.f),
      m_texture(nullptr), postFrameBuffer(this, this->width(), this->height(), this->devicePixelRatio()),
      depthFrameBuffer(this, this->width(), this->height(), this->devicePixelRatio()), sunViewProj(),
      m_postprocessShaders(), mp_progPostprocessCurrent(nullptr), quad(this), m_displayedBlock(this), m_displayedPlayer(this, glm::vec3(0, 0, 0))
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    // Tell the timer to redraw 60 times per second
    m_timer.start(16);
    setFocusPolicy(Qt::ClickFocus);

    setMouseTracking(true); // MyGL will track the mouse's movements even if a mouse button is not pressed
    setCursor(Qt::BlankCursor); // Make the cursor invisible
}

MyGL::~MyGL() {
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
}


void MyGL::moveMouseToCenter() {
    QCursor::setPos(this->mapToGlobal(QPoint(width() / 2, height() / 2)));
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instance of the world axes
    m_worldAxes.createVBOdata();


    // Set up all shader programs
    loadProcessShader();

    // Set a color with which to draw geometry.
    // This will ultimately not be used when you change
    // your program to render Chunks with vertex colors
    // and UV coordinates
    m_progLambert.setGeometryColor(glm::vec4(0,1,0,1));


    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    // Initialize frame buffer and quad
    postFrameBuffer.createPost();
    depthFrameBuffer.createDepth();
    quad.createVBOdata();

    m_texture = mkU<Texture>(this);
    m_texture->create(":/textures/minecraft_textures_all.png");
    m_texture->load(0);

    m_terrain.CreateInitialScene(m_player.mcr_position);
    moveMouseToCenter();

    createVoxels();
    m_displayedPlayer.createVBOdata();
    //m_terrain.setBlockAt(100, 200, 100, DIRT);

}

void MyGL::resizeGL(int w, int h) {
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_player.setCameraWidthHeight(static_cast<unsigned int>(w), static_cast<unsigned int>(h));
    glm::mat4 viewproj = m_player.mcr_camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_progSky.setViewProjMatrix(glm::inverse(viewproj));
    m_progSky.setDimension(width(), height());

    for(std::shared_ptr<PostProcessShader> p : m_postprocessShaders)
    {
        p->setDimensions(glm::ivec2(w * this->devicePixelRatio(), h * this->devicePixelRatio()));
    }

    postFrameBuffer.resize(w, h, this->devicePixelRatio());
    postFrameBuffer.destroy();
    postFrameBuffer.createPost();

    printGLErrorLog();
}


// MyGL's constructor links tick() to a timer that fires 60 times per second.
// We're treating MyGL as our game engine class, so we're going to perform
// all per-frame actions here, such as performing physics updates on all
// entities in the scene.
void MyGL::tick() {
    quint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - m_time;
    m_time = QDateTime::currentMSecsSinceEpoch();
    m_player.tick(deltaTime/100.f, this->m_inputs);

    std::vector<int64_t> terrainsNotExpanded = m_terrain.checkExpansion(m_player.mcr_position);

    std::vector<std::vector<Chunk*>> terrainsChunks;
    for (unsigned int i = 0; i < terrainsNotExpanded.size(); i++) {
        std::vector<Chunk*> localChunks;
        for (int x = 0; x < 4; x++) {
            for (int z = 0; z < 4; z++) {
                glm:: ivec2 tz_coords = toCoords(terrainsNotExpanded.at(i));
                Chunk* c = m_terrain.createChunkAt(tz_coords.x + 16 * x, tz_coords.y + 16 * z);
                localChunks.push_back(c);
            }
        }
        terrainsChunks.push_back(localChunks);
    }

    for (unsigned int i = 0; i < terrainsNotExpanded.size(); i++) {
        BlockTypeWorker *bWorker = new BlockTypeWorker(&m_terrain,
                                                       terrainsNotExpanded.at(i),
                                                       terrainsChunks[i],
                                                       &m_terrain.m_chunksWithOnlyBlockData,
                                                       &m_terrain.m_chunksWithOnlyBlockDataLock);
        QThreadPool::globalInstance()->start(bWorker);
    }

    m_terrain.m_chunksWithOnlyBlockDataLock.lock();
    for (Chunk *c : m_terrain.m_chunksWithOnlyBlockData) {
        VBOWorker *vboWorker = new VBOWorker(
                                             &m_terrain.m_chunksWithVBOData,
                                             c,
                                             &m_terrain.m_chunksWithVBODataLock);
        QThreadPool::globalInstance()->start(vboWorker);
    }
    m_terrain.m_chunksWithOnlyBlockData.clear();
    m_terrain.m_chunksWithOnlyBlockDataLock.unlock();

    m_terrain.m_chunksWithVBODataLock.lock();
    for (ChunkVBOData c: m_terrain.m_chunksWithVBOData) {
        c.associated_chunk->sendVBOdata();
    }
    m_terrain.m_chunksWithVBOData.clear();
    m_terrain.m_chunksWithVBODataLock.unlock();

    m_displayedBlock.updateBlock(m_player.getLookAtBlock());
    m_progPlayer.setAnimation(m_player.m_animation.getCurrAnimation());
    m_progPlayer.setCurrFrame(m_player.m_animation.getCurrFrame());

    update(); // Calls paintGL() as part of a larger QOpenGLWidget pipeline
    sendPlayerDataToGUI(); // Updates the info in the secondary window displaying player data

}

void MyGL::sendPlayerDataToGUI() const {
    emit sig_sendPlayerPos(m_player.posAsQString());
    emit sig_sendPlayerVel(m_player.velAsQString());
    emit sig_sendPlayerAcc(m_player.accAsQString());
    emit sig_sendPlayerLook(m_player.lookAsQString());
    glm::vec2 pPos(m_player.mcr_position.x, m_player.mcr_position.z);
    glm::ivec2 chunk(16 * glm::ivec2(glm::floor(pPos / 16.f)));
    glm::ivec2 zone(64 * glm::ivec2(glm::floor(pPos / 64.f)));
    emit sig_sendPlayerChunk(QString::fromStdString("( " + std::to_string(chunk.x) + ", " + std::to_string(chunk.y) + " )"));
    emit sig_sendPlayerTerrainZone(QString::fromStdString("( " + std::to_string(zone.x) + ", " + std::to_string(zone.y) + " )"));
}

// This function is called whenever update() is called.
// MyGL's constructor links update() to a timer that fires 60 times per second,
// so paintGL() called at a rate of 60 frames per second.
void MyGL::paintGL() {
    // shadow map rendering
    depthRendering();
    setDepthFrameBufferTexture();

    // prepare framebuffer
    postFrameBuffer.bindFrameBuffer();
    postFrameBuffer.setViewport();
    postFrameBuffer.clearFramebuffer();

    glm::mat4 viewproj = m_player.mcr_camera.getViewProj();
    // flat shader setup
    m_progFlat.setViewProjMatrix(viewproj);
    // lambert shader setup
    m_progLambert.setViewProjMatrix(viewproj);
    m_progLambert.setShadowViewProjMatrix(sunViewProj);
    m_progLambert.setModelMatrix(glm::mat4());
    m_progLambert.setTime(m_shaderTime);
    m_progLambert.setTimeElp(m_timeElapsed);
    // skybox shader setup
    m_progSky.setViewProjMatrix(glm::inverse(viewproj));
    m_progSky.setEye(m_player.mcr_camera.mcr_position);
    m_progSky.setTime(m_shaderTime);
    m_progSky.setTimeElp(m_timeElapsed);

    m_progLambert.setCameraPos(m_player.mcr_camera.mcr_position);
    mp_progPostprocessCurrent->setTime(m_shaderTime);

    m_shaderTime++;
    m_timeElapsed += m_timeStep;

    m_texture->bind(0);

    m_progSky.draw(quad);
    renderTerrain(&m_progLambert, false);

    renderThirdPersonPlayer();
    performPostprocessRenderPass();

    if (m_displayedBlock.getBlock().display)
    {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.setModelMatrix(glm::mat4());
        m_progFlat.setViewProjMatrix(m_player.mcr_camera.getViewProj());
        m_displayedBlock.createVBOdata();
        m_progFlat.draw(m_displayedBlock);
        glEnable(GL_DEPTH_TEST);
    }

}

// TODO: Change this so it renders the nine zones of generated
// terrain that surround the player (refer to Terrain::m_generatedTerrain
// for more info)
void MyGL::renderTerrain(ShaderProgram* shader, bool isShadow) {
    int xFloor = static_cast<int>(glm::floor(m_player.mcr_position.x / 16.f));
    int zFloor = static_cast<int>(glm::floor(m_player.mcr_position.z / 16.f));
    int x = 16 * xFloor;
    int z = 16 * zFloor;

    m_terrain.draw(x - 64 * NUMZONETODRAW, x + 64 * NUMZONETODRAW, z - 64 * NUMZONETODRAW, z + 64 * NUMZONETODRAW, shader, isShadow);
}

void MyGL::performPostprocessRenderPass() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
    glViewport(0, 0, this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    postFrameBuffer.bindToTextureSlot(1);
    setCurPostProcessShader();
    mp_progPostprocessCurrent->draw(quad, 1);
}

void MyGL::keyPressEvent(QKeyEvent *e) {
    //float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        //amount = 10.0f;
        m_inputs.shiftPressed = true;
    }

    if(e->modifiers() & Qt::Key_Space){
        //amount = 10.0f;
        m_inputs.spacePressed = true;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        //m_player.rotateOnUpGlobal(-amount);
    } else if (e->key() == Qt::Key_Left) {
        //m_player.rotateOnUpGlobal(amount);
    } else if (e->key() == Qt::Key_Up) {
        //m_player.rotateOnRightLocal(-amount);
    } else if (e->key() == Qt::Key_Down) {
        //m_player.rotateOnRightLocal(amount);
    } else if (e->key() == Qt::Key_W) {
        //m_player.moveForwardLocal(amount);
        m_inputs.wPressed = true;
    } else if (e->key() == Qt::Key_S) {
        //m_player.moveForwardLocal(-amount);
        m_inputs.sPressed = true;
    } else if (e->key() == Qt::Key_D) {
        //m_player.moveRightLocal(amount);
        m_inputs.dPressed = true;
    } else if (e->key() == Qt::Key_A) {
        //m_player.moveRightLocal(-amount);
        m_inputs.aPressed = true;
    } else if (e->key() == Qt::Key_Q) {
        //m_player.moveUpGlobal(-amount);
        m_inputs.qPressed = true;
    } else if (e->key() == Qt::Key_E) {
        //m_player.moveUpGlobal(amount);
        m_inputs.ePressed = true;
    } else if (e->key() == Qt::Key_F) {
        m_inputs.fPressed = true;
    } else if (e->key() == Qt::Key_Space){
        m_inputs.spacePressed = true;
    } else if (e->key() == Qt::Key_P) {
        m_timeStep = 60.f / m_timeStep;
    } else if (e->key() == Qt::Key_M){
        m_player.toggleFirstPersonOnOff();
    } else if (e->key() == Qt::Key_F1){
        m_player.playEmote(2);
    } else if (e->key() == Qt::Key_F2){
        m_player.playEmote(3);
    } else if (e->key() == Qt::Key_F3){
        m_player.playEmote(4);
    } else if (e->key() == Qt::Key_F4){
        m_player.playEmote(5);
    } else if (e->key() == Qt::Key_F5){
        m_player.playEmote(6);
    }
}

void MyGL::keyReleaseEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Shift){
        m_inputs.shiftPressed = false;
    }

    if (e->key() == Qt::Key_W){
        m_inputs.wPressed = false;
    } else if (e->key() == Qt::Key_S) {
        m_inputs.sPressed = false;
    } else if (e->key() == Qt::Key_D) {
        m_inputs.dPressed = false;
    } else if (e->key() == Qt::Key_A) {
        m_inputs.aPressed = false;
    } else if (e->key() == Qt::Key_Q) {
        m_inputs.qPressed = false;
    } else if (e->key() == Qt::Key_E) {
        m_inputs.ePressed = false;
    } else if (e->key() == Qt::Key_Space){
        m_inputs.spacePressed = false;
    }
}

void MyGL::mouseMoveEvent(QMouseEvent *e) {
    // TODO
    m_inputs.mouseX = e->pos().x() - width()/2.f;
    m_inputs.mouseY = e->pos().y() - height()/2.f;
    moveMouseToCenter();
}

void MyGL::mousePressEvent(QMouseEvent *e) {
    // TODO
    if (e->button() == Qt::LeftButton){
        m_player.removeBlock();
    }else if (e->button() == Qt::RightButton){
        m_player.addBlock();
    }
}

void MyGL::loadProcessShader() {
    // render shaders
    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    //m_progInstanced.create(":/glsl/instanced.vert.glsl", ":/glsl/lambert.frag.glsl");
    // skybox shader
    m_progSky.create(":/glsl/sky.vert.glsl", ":/glsl/sky.frag.glsl");
    // shadow map shader
    m_progShadow.create(":/glsl/shadow.vert.glsl", ":/glsl/shadow.frag.glsl");
    m_progPlayer.create(":/glsl/player.vert.glsl", ":/glsl/player.frag.glsl");

    // post-process shaders
    std::shared_ptr<PostProcessShader> noOp = std::make_shared<PostProcessShader>(this);
    noOp->create(":/glsl/passthrough.vert.glsl", ":/glsl/noOp.frag.glsl");
    m_postprocessShaders.push_back(noOp);

    std::shared_ptr<PostProcessShader> water = std::make_shared<PostProcessShader>(this);
    water->create(":/glsl/passthrough.vert.glsl", ":/glsl/water.frag.glsl");
    m_postprocessShaders.push_back(water);

    std::shared_ptr<PostProcessShader> lava = std::make_shared<PostProcessShader>(this);
    lava->create(":/glsl/passthrough.vert.glsl", ":/glsl/lava.frag.glsl");
    m_postprocessShaders.push_back(lava);

    std::shared_ptr<PostProcessShader> sobel = std::make_shared<PostProcessShader>(this);
    sobel->create(":/glsl/passthrough.vert.glsl", ":/glsl/sobel.frag.glsl");
    m_postprocessShaders.push_back(sobel);

    mp_progPostprocessCurrent = noOp.get();
}

void MyGL::setCurPostProcessShader() {
    glm::vec3 pos = m_player.mcr_camera.mcr_position;
    if (!m_terrain.hasChunkAt(pos[0], pos[2])) {
        mp_progPostprocessCurrent = m_postprocessShaders[0].get();
        return;
    }
    BlockType curBlock = m_terrain.getBlockAt(pos);
    if (curBlock == WATER) {
        mp_progPostprocessCurrent = m_postprocessShaders[1].get();
    } else if (curBlock == LAVA) {
        mp_progPostprocessCurrent = m_postprocessShaders[2].get();
    } else if (m_player.m_animation.getCurrAnimation() == 3){
        if (m_player.m_animation.getCurrFrame() >= 20 && m_player.m_animation.getCurrFrame() <= 48){
            mp_progPostprocessCurrent = m_postprocessShaders[3].get();
        }else{
            mp_progPostprocessCurrent = m_postprocessShaders[0].get();
        }
    }else
    {
        mp_progPostprocessCurrent = m_postprocessShaders[0].get();
    }
}

void MyGL::depthRendering() {
    depthFrameBuffer.bindFrameBuffer();
    glViewport(0, 0, 1024, 1024);
    depthFrameBuffer.clearFramebuffer();
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-1000,1000,-1000,1000,300,10000);
    glm::mat4 depthViewMatrix = glm::lookAt(getSunLocation(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 depthViewProj = depthProjectionMatrix * depthViewMatrix;
    sunViewProj = depthViewProj;
    m_progShadow.setViewProjMatrix(depthViewProj);
    m_progShadow.setModelMatrix(glm::mat4());
    renderTerrain(&m_progShadow, true);
    renderThirdPersonPlayerShadow(&m_progShadow);
}

void MyGL::setDepthFrameBufferTexture() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
    glViewport(0, 0, this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    depthFrameBuffer.bindToTextureSlot(1);
}

glm::vec3 MyGL::getSunLocation() {
    float time = glm::mod(m_timeElapsed, 216000.f) / 216000.f;
    glm::vec3 p = glm::vec3(0.f, 5000.f, 0.f);
    glm::vec3 up = glm::vec3(1.f, 0.f, 0.f);
    float rot = 6.28318530718f * time;
    glm::vec3 curLocation = glm::vec3(cos(rot) * p.x - sin(rot) * p.y,
                                      sin(rot) * p.x + cos(rot) * p.y,
                                      p.z);
    return curLocation;
}
