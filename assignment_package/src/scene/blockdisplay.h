#ifndef BLOCKDISPLAY_H
#define BLOCKDISPLAY_H

#include "drawable.h"

struct Block {
    bool display;
    glm::ivec3 pos;
};

class BlockDisplay : public Drawable {
private:
    Block representedBlock;
public:
    BlockDisplay(OpenGLContext* context);
    // Creates VBO data to make a visual
    void createVBOdata() override;
    void updateBlock(Block);
    GLenum drawMode() override;
    Block getBlock() const;
};

#endif // BLOCKDISPLAY_H
