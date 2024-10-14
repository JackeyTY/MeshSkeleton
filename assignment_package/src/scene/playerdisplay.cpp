#include "playerdisplay.h"
#include "texturehelp.h"
#include "mygl.h"
#include <iostream>

PlayerDisplay::PlayerDisplay(OpenGLContext* context, glm::vec3 p)
    : Drawable(context), m_position(p)
{}

GLenum PlayerDisplay::drawMode(){
    return GL_TRIANGLES;
}

void PlayerDisplay::createVBOdata(){
    std::vector<GLuint> idx;
    std::vector<glm::vec4> interleaved;
    //head top
    glm::vec4 normal(0.f, 1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 2.0f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(YPOS)->second, 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 2.0f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(YPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 2.0f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(YPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 5.f));
    interleaved.push_back(glm::vec4(-0.25f, 2.0f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(YPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 5.f));
    //head bot
    normal = glm::vec4(0.f, -1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(YNEG)->second, 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(YNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(YNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 5.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(YNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 5.f));
    //head right
    normal = glm::vec4(1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(XPOS)->second, 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(XPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 2.0f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(XPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 2.0f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(XPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 5.f));
    //head left
    normal = glm::vec4(-1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(XNEG)->second, 0.f, 5.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(XNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 5.f));
    interleaved.push_back(glm::vec4(-0.25f, 2.0f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(XNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 5.f));
    interleaved.push_back(glm::vec4(-0.25f, 2.0f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(XNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 5.f));
    //head front
    normal = glm::vec4(0.f, 0.f, 1.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(ZPOS)->second, 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(ZPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 2.0f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(ZPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 5.f));
    interleaved.push_back(glm::vec4(-0.25f, 2.0f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(ZPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 5.f));
    //head back
    normal = glm::vec4(0.f, 0.f, -1.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(ZNEG)->second, 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(ZNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 5.f));
    interleaved.push_back(glm::vec4(0.25f, 2.0f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(ZNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 5.f));
    interleaved.push_back(glm::vec4(-0.25f, 2.0f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(HEAD)->second.find(ZNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 5.f));

    //body
    normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(YPOS)->second, 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(YPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(YPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 0.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(YPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 0.f));
    //head bot
    normal = glm::vec4(0.f, -1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(YNEG)->second, 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(YNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(YNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 0.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(YNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 0.f));
    //head right
    normal = glm::vec4(1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(XPOS)->second, 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(XPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(XPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(XPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 0.f));
    //head left
    normal = glm::vec4(-1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(XNEG)->second, 0.f, 0.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(XNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 0.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(XNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 0.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(XNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 0.f));
    //head front
    normal = glm::vec4(0.f, 0.f, 1.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(ZPOS)->second, 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(ZPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(ZPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 0.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(ZPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 0.f));
    //head back
    normal = glm::vec4(0.f, 0.f, -1.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(ZNEG)->second, 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(ZNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 0.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(ZNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 0.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.25f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(BODY)->second.find(ZNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 0.f));

    //leftArm
    normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.5f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YPOS)->second, 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.5f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 1.f));
    //head bot
    normal = glm::vec4(0.f, -1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.5f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YNEG)->second, 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.5f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 1.f));
    //head right
    normal = glm::vec4(1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XPOS)->second, 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 1.f));
    //head left
    normal = glm::vec4(-1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.5f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XNEG)->second, 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.5f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.5f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.5f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 1.f));
    //head front
    normal = glm::vec4(0.f, 0.f, 1.f, 0.f);
    interleaved.push_back(glm::vec4(-0.5f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZPOS)->second, 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.5f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 1.f));
    //head back
    normal = glm::vec4(0.f, 0.f, -1.f, 0.f);
    interleaved.push_back(glm::vec4(-0.5f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZNEG)->second, 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.25f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 1.f));
    interleaved.push_back(glm::vec4(-0.5f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 1.f));

    //rightArm
    normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YPOS)->second, 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 2.f));
    //head bot
    normal = glm::vec4(0.f, -1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YNEG)->second, 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(YNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 2.f));
    //head right
    normal = glm::vec4(1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.5f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XPOS)->second, 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 2.f));
    //head left
    normal = glm::vec4(-1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XNEG)->second, 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(XNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 2.f));
    //head front
    normal = glm::vec4(0.f, 0.f, 1.f, 0.f);
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZPOS)->second, 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 2.f));
    //head back
    normal = glm::vec4(0.f, 0.f, -1.f, 0.f);
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZNEG)->second, 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.5f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 2.f));
    interleaved.push_back(glm::vec4(0.25f, 1.5f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(ARM)->second.find(ZNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 2.f));

    //leftLeg
    normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YPOS)->second, 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 3.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 3.f));
    //head bot
    normal = glm::vec4(0.f, -1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YNEG)->second, 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 3.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 3.f));
    //head right
    normal = glm::vec4(1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XPOS)->second, 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 3.f));
    //head left
    normal = glm::vec4(-1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XNEG)->second, 0.f, 3.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 3.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 3.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 3.f));
    //head front
    normal = glm::vec4(0.f, 0.f, 1.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZPOS)->second, 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 3.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 3.f));
    //head back
    normal = glm::vec4(0.f, 0.f, -1.f, 0.f);
    interleaved.push_back(glm::vec4(-0.25f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZNEG)->second, 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 3.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 3.f));
    interleaved.push_back(glm::vec4(-0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 3.f));

    //rightLeg
    normal = glm::vec4(0.f, 1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YPOS)->second, 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 4.f));
    //head bot
    normal = glm::vec4(0.f, -1.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YNEG)->second, 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(YNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 4.f));
    //head right
    normal = glm::vec4(1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.25f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XPOS)->second, 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 4.f));
    //head left
    normal = glm::vec4(-1.f, 0.f, 0.f, 0.f);
    interleaved.push_back(glm::vec4(0.f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XNEG)->second, 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(XNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 4.f));
    //head front
    normal = glm::vec4(0.f, 0.f, 1.f, 0.f);
    interleaved.push_back(glm::vec4(0.f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZPOS)->second, 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, 0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, 4.f));
    //head back
    normal = glm::vec4(0.f, 0.f, -1.f, 0.f);
    interleaved.push_back(glm::vec4(0.f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZNEG)->second, 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.25f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, 4.f));
    interleaved.push_back(glm::vec4(0.f, 0.75f, -0.125f, 1.f));
    interleaved.push_back(normal);
    interleaved.push_back(glm::vec4(blockFaceUVs.find(LEG)->second.find(ZNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, 4.f));


    int numFace = 36;
    for (int i = 0; i < numFace * 4; i+=4) {
        idx.push_back(i);
        idx.push_back(i + 1);
        idx.push_back(i + 2);
        idx.push_back(i);
        idx.push_back(i + 2);
        idx.push_back(i + 3);
    }
    m_count = idx.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generateAll();
    bindAll();
    mp_context->glBufferData(GL_ARRAY_BUFFER, interleaved.size() * sizeof(glm::vec4), interleaved.data(), GL_STATIC_DRAW);
}

void MyGL::renderThirdPersonPlayer()
{
    if (m_player.thirdPersonMode){
        glm::mat4 model = glm::mat4();
        //std::cout << glm::to_string(model) << std::endl;
        model = glm::translate(model, m_player.mcr_position - m_displayedPlayer.m_position);
        model = model * m_player.getRotationMatrix();
        m_progPlayer.setModelMatrix(model);
        m_progPlayer.setViewProjMatrix(m_player.mcr_camera.getViewProj());
        m_progPlayer.setTimeElp(m_timeElapsed);
        m_progPlayer.drawInterleaved(m_displayedPlayer, 0, 1);
    }
}

void MyGL::renderThirdPersonPlayerShadow(ShaderProgram *shader) {
    if (m_player.thirdPersonMode){
        glm::mat4 model = glm::mat4();
        model = glm::translate(model, m_player.mcr_position - m_displayedPlayer.m_position);
        model = model * m_player.getRotationMatrix();
        shader->setModelMatrix(model);
        shader->drawInterleaved(m_displayedPlayer, 0, 1);
    }
}
