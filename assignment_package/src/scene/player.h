#pragma once
#include "entity.h"
#include "camera.h"
#include "terrain.h"
#include "scene\blockdisplay.h"
#include "scene\animationmanager.h"
#include <QtMultimedia/QSoundEffect>

class Player : public Entity {
private:
    glm::vec3 m_velocity, m_acceleration;
    bool flightMode;
    bool isGrounded;
    Camera m_camera;
    Terrain &mcr_terrain;
    void processInputs(InputBundle &inputs);
    void computePhysics(float dT, const Terrain &terrain);
    void rotateCamera(InputBundle &inputs);
    void groundCheck(const Terrain &terrain);
    void moveToGround(float dT);
    float original_x;
    float original_y;
    Block lookAt;
    glm::mat4 rot;


public:
    QSoundEffect footstep;
    QSoundEffect jump;
    QSoundEffect flying;
    // Readonly public reference to our camera
    // for easy access from MyGL
    const Camera& mcr_camera;
    AnimationManager m_animation;
    bool inWaterLava;
    bool thirdPersonMode;

    Player(glm::vec3 pos, Terrain &terrain);
    virtual ~Player() override;

    void setCameraWidthHeight(unsigned int w, unsigned int h);

    void tick(float dT, InputBundle &input) override;

    // Player overrides all of Entity's movement
    // functions so that it transforms its camera
    // by the same amount as it transforms itself.
    void moveAlongVector(glm::vec3 dir) override;
    void moveForwardLocal(float amount) override;
    void moveRightLocal(float amount) override;
    void moveUpLocal(float amount) override;
    void moveForwardGlobal(float amount) override;
    void moveRightGlobal(float amount) override;
    void moveUpGlobal(float amount) override;
    void rotateOnForwardLocal(float degrees) override;
    void rotateOnRightLocal(float degrees) override;
    void rotateOnUpLocal(float degrees) override;
    void rotateOnForwardGlobal(float degrees) override;
    void rotateOnRightGlobal(float degrees) override;
    void rotateOnUpGlobal(float degrees) override;

    // For sending the Player's data to the GUI
    // for display
    QString posAsQString() const;
    QString velAsQString() const;
    QString accAsQString() const;
    QString lookAsQString() const;

    void removeBlock();
    void addBlock();

    void updateLookAtBlock();
    Block getLookAtBlock() const;

    glm::mat4 getRotationMatrix();
    void toggleFirstPersonOnOff();
    void playEmote(int index);
};
