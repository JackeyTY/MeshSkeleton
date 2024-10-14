#include "blockdisplay.h"

BlockDisplay::BlockDisplay(OpenGLContext* context)
    :Drawable(context), representedBlock()
{
    representedBlock.display = false;
}

GLenum BlockDisplay::drawMode(){
    return GL_LINES;
}

void BlockDisplay::updateBlock(Block b){
    representedBlock = b;
}

Block BlockDisplay::getBlock() const{
    return representedBlock;
}

void BlockDisplay::createVBOdata(){
    std::vector<GLuint> idx  = {0, 1, 1, 2, 2, 3, 3, 0, 2, 4, 4, 5, 5, 3, 4, 6, 6, 7, 7, 5, 1, 6, 0, 7};
    std::vector<glm::vec4> pos = {glm::vec4(representedBlock.pos[0], representedBlock.pos[1], representedBlock.pos[2], 1),
                                  glm::vec4(representedBlock.pos[0], representedBlock.pos[1] + 1, representedBlock.pos[2], 1),
                                  glm::vec4(representedBlock.pos[0] + 1, representedBlock.pos[1] + 1, representedBlock.pos[2], 1),
                                  glm::vec4(representedBlock.pos[0] + 1, representedBlock.pos[1], representedBlock.pos[2], 1),
                                  glm::vec4(representedBlock.pos[0] + 1, representedBlock.pos[1] + 1, representedBlock.pos[2] + 1, 1),
                                  glm::vec4(representedBlock.pos[0] + 1, representedBlock.pos[1], representedBlock.pos[2] + 1, 1),
                                  glm::vec4(representedBlock.pos[0], representedBlock.pos[1] + 1, representedBlock.pos[2] + 1, 1),
                                  glm::vec4(representedBlock.pos[0], representedBlock.pos[1], representedBlock.pos[2] + 1, 1)};
    std::vector<glm::vec4> col;
    for (int i = 0; i <= 7; i++){
        col.push_back(glm::vec4(1.f));
    }
    m_count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, m_bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);
}
