#ifndef PLAYERDISPLAY_H
#define PLAYERDISPLAY_H

#include "drawable.h"

class PlayerDisplay : public Drawable {
public:
    PlayerDisplay(OpenGLContext* context, glm::vec3);
    // Creates VBO data to make a visual
    void createVBOdata() override;
    GLenum drawMode() override;
    const glm::vec3 m_position;
};

#endif // PLAYERDISPLAY_H
