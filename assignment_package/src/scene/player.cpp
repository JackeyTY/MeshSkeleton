#include "player.h"
#include <QString>
#include <iostream>


Player::Player(glm::vec3 pos, Terrain &terrain)
    : Entity(pos), m_velocity(0,0,0), m_acceleration(0,0,0), flightMode(true),
    m_camera(pos + glm::vec3(0.f, 1.5f, 0.f)), mcr_terrain(terrain), lookAt(), rot(glm::mat4()),
    mcr_camera(m_camera), m_animation(),
    inWaterLava(false), thirdPersonMode(false), footstep(new QSoundEffect), jump(new QSoundEffect), flying(new QSoundEffect)
{
    original_x = 0.f;
    original_y = 0.f;
    lookAt.display = false;
    footstep.setSource(QUrl::fromLocalFile(":/sound/footsteps-grass.wav"));
    footstep.setVolume(0.75f);
    //footstep.setLoopCount(1);
    //footstep->play();
    jump.setSource(QUrl::fromLocalFile(":/sound/jump.wav"));
    jump.setVolume(0.75f);
    flying.setSource(QUrl::fromLocalFile(":/sound/flightmode.wav"));
    flying.setVolume(0.75f);
}

Player::~Player()
{}

void Player::tick(float dT, InputBundle &input) {
    processInputs(input);
    rotateCamera(input);
    computePhysics(dT, mcr_terrain);
    updateLookAtBlock();
    m_animation.tick();
}

void Player::processInputs(InputBundle &inputs) {
    // TODO: Update the Player's velocity and acceleration based on the
    // state of the inputs.

    if (inputs.fPressed){
        this->flightMode = !this->flightMode;
        inputs.fPressed = false;
    }

    float amount = 1.0f;
    float impulse = 2.5f;
    if (inputs.shiftPressed) amount *= 1.5f;

    if (flightMode){
        this->m_acceleration = glm::vec3(0.f);

        if (inputs.wPressed){
            this->m_acceleration += this->m_forward * amount;
        }
        if (inputs.sPressed){
            this->m_acceleration += -this->m_forward * amount;
        }
        if (inputs.aPressed){
            this->m_acceleration += -this->m_right * amount;
        }
        if (inputs.dPressed){
            this->m_acceleration += this->m_right * amount;
        }
        if (inputs.ePressed){
            this->m_acceleration += this->m_up * amount;
        }
        if (inputs.qPressed){
            this->m_acceleration += -this->m_up * amount;
        }

    } else {
        amount = 0.3f;
        groundCheck(mcr_terrain);
        this->m_acceleration = glm::vec3(0.f);
        glm::vec3 forward = glm::normalize(glm::vec3(m_forward.x, 0.f, m_forward.z));
        glm::vec3 right = glm::normalize(glm::vec3(m_right.x, 0.f, m_right.z));
        if (inputs.wPressed){
            if(!footstep.isPlaying())footstep.play();
            this->m_acceleration += forward * amount;
        }
        if (inputs.sPressed){
            if(!footstep.isPlaying())footstep.play();
            this->m_acceleration += -forward * amount;
        }
        if (inputs.aPressed){
            if(!footstep.isPlaying())footstep.play();
            this->m_acceleration += -right * amount;
        }
        if (inputs.dPressed){
            if(!footstep.isPlaying())footstep.play();
            this->m_acceleration += right * amount;
        }
        if (!isGrounded){
            glm::vec3 gravity = glm::vec3(0, -1.0f, 0);
            this->m_acceleration += gravity;
        }
        BlockType curBlockType = mcr_terrain.getBlockAt(m_position + glm::vec3(0.f, 1.f, 0.f));
        if (inWaterLava == false && (curBlockType == WATER || curBlockType == LAVA)) {
            inWaterLava = true;
            this->m_acceleration *= (2.f/ 3.f);
            this->m_velocity *= (2.f / 3.f);
        } else if (inWaterLava == true && !(curBlockType == WATER || curBlockType == LAVA)) {
            inWaterLava = false;
            this->m_acceleration /= (2.f / 3.f);
        } else if (inWaterLava == true) {
            this->m_acceleration *= (2.f/ 3.f);
        }
        if (inputs.spacePressed){
            if (inWaterLava) {
                this->m_acceleration += glm::vec3(0, 1, 0) * 1.5f;
            } else if (isGrounded){
                jump.play();
                this->m_velocity += glm::vec3(0, 1, 0) * impulse;
            }
        }
    }


}

bool gridMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection, const Terrain &terrain, float *out_dist, glm::ivec3 *out_blockHit) {
    float maxLen = glm::length(rayDirection); // Farthest we search
    glm::ivec3 currCell = glm::ivec3(glm::floor(rayOrigin));
    rayDirection = glm::normalize(rayDirection); // Now all t values represent world dist.

    float curr_t = 0.f;
    while(curr_t < maxLen) {
        float min_t = glm::sqrt(3.f);
        float interfaceAxis = -1; // Track axis for which t is smallest
        for(int i = 0; i < 3; ++i) { // Iterate over the three axes
            if(rayDirection[i] != 0) { // Is ray parallel to axis i?
                float offset = glm::max(0.f, glm::sign(rayDirection[i])); // See slide 5
                // If the player is *exactly* on an interface then
                // they'll never move if they're looking in a negative direction
                if(currCell[i] == rayOrigin[i] && offset == 0.f) {
                    offset = -1.f;
                }
                int nextIntercept = currCell[i] + offset;
                float axis_t = (nextIntercept - rayOrigin[i]) / rayDirection[i];
                axis_t = glm::min(axis_t, maxLen); // Clamp to max len to avoid super out of bounds errors
                if(axis_t < min_t) {
                    min_t = axis_t;
                    interfaceAxis = i;
                }
            }
        }
        if(interfaceAxis == -1) {
            throw std::out_of_range("interfaceAxis was -1 after the for loop in gridMarch!");
        }
        curr_t += min_t; // min_t is declared in slide 7 algorithm
        rayOrigin += rayDirection * min_t;
        glm::ivec3 offset = glm::ivec3(0,0,0);
        // Sets it to 0 if sign is +, -1 if sign is -
        offset[interfaceAxis] = glm::min(0.f, glm::sign(rayDirection[interfaceAxis]));
        currCell = glm::ivec3(glm::floor(rayOrigin)) + offset;
        // If currCell contains something other than EMPTY, return
        // curr_t
        BlockType cellType = terrain.getBlockAt(currCell.x, currCell.y, currCell.z);
        if((cellType != EMPTY) && (cellType != WATER) && (cellType != LAVA)) {
            *out_blockHit = currCell;
            *out_dist = glm::min(maxLen, curr_t);
            return true;
        }
    }
    *out_dist = glm::min(maxLen, curr_t);
    return false;
}


void Player::computePhysics(float dT, const Terrain &terrain) {
    // TODO: Update the Player's position based on its acceleration
    // and velocity, and also perform collision detection.
    //friction
    this->m_velocity *= (inWaterLava ? 0.2 : 0.85);
    this->m_velocity += this->m_acceleration * dT;
    if (flightMode){
        moveAlongVector(m_velocity * dT);
    } else{
        if(glm::length(m_velocity)<0.3f){
            if(footstep.isPlaying()) footstep.stop();
        }
        if (thirdPersonMode){
            if (isGrounded && (glm::length(m_velocity) > 0.3f)){
                m_animation.play(0);
            }else{
                m_animation.stop(0);
            }
        }
        std::array<glm::vec3, 12> verts = {glm::vec3(m_position.x + 0.5f, m_position.y, m_position.z + 0.5f),
                                           glm::vec3(m_position.x + 0.5f, m_position.y, m_position.z - 0.5f),
                                           glm::vec3(m_position.x - 0.5f, m_position.y, m_position.z + 0.5f),
                                           glm::vec3(m_position.x - 0.5f, m_position.y, m_position.z - 0.5f),
                                           glm::vec3(m_position.x + 0.5f, m_position.y + 1.f, m_position.z + 0.5f),
                                           glm::vec3(m_position.x + 0.5f, m_position.y + 1.f, m_position.z - 0.5f),
                                           glm::vec3(m_position.x - 0.5f, m_position.y + 1.f, m_position.z + 0.5f),
                                           glm::vec3(m_position.x - 0.5f, m_position.y + 1.f, m_position.z - 0.5f),
                                           glm::vec3(m_position.x + 0.5f, m_position.y + 2.f, m_position.z + 0.5f),
                                           glm::vec3(m_position.x + 0.5f, m_position.y + 2.f, m_position.z - 0.5f),
                                           glm::vec3(m_position.x - 0.5f, m_position.y + 2.f, m_position.z + 0.5f),
                                           glm::vec3(m_position.x - 0.5f, m_position.y + 2.f, m_position.z - 0.5f)};
        float minDistX = INFINITY;
        float minDistY = INFINITY;
        float minDistZ = INFINITY;
        glm::ivec3 outBlock;
        //gridmarch x-axis
        for (int i = 0 ; i < 12; i++) {
            float currDist;
            if (gridMarch(verts[i], glm::vec3(m_velocity.x, 0, 0), terrain, &currDist, &outBlock) && currDist < minDistX){
                BlockType type = mcr_terrain.getBlockAt(outBlock.x, outBlock.y, outBlock.z);
                if ((type != WATER) && (type != LAVA)) minDistX = currDist;
            }
        }

        //gridmarch Y-axis
        for (int i = 0 ; i < 12; i++) {
            float currDist;
            if (gridMarch(verts[i], glm::vec3(0, m_velocity.y, 0), terrain, &currDist, &outBlock) && currDist < minDistY){
                BlockType type = mcr_terrain.getBlockAt(outBlock.x, outBlock.y, outBlock.z);
                if ((type != WATER) && (type != LAVA)) minDistY = currDist;
            }
        }
        //gridmarch z-axis
        for (int i = 0 ; i < 12; i++) {
            float currDist;
            if (gridMarch(verts[i], glm::vec3(0, 0, m_velocity.z), terrain, &currDist, &outBlock) && currDist < minDistZ){
                BlockType type = mcr_terrain.getBlockAt(outBlock.x, outBlock.y, outBlock.z);
                if ((type != WATER) && (type != LAVA)) minDistZ = currDist;
            }
        }
        if (glm::length(m_velocity) > 0.000001f){
            glm::vec3 dir = glm::normalize(m_velocity);
            float vX = glm::min(glm::abs(m_velocity.x * dT), minDistX - 0.0001f);
            float vY = glm::min(glm::abs(m_velocity.y * dT), minDistY - 0.0001f);
            float vZ = glm::min(glm::abs(m_velocity.z * dT), minDistZ - 0.0001f);
            if (vX!= 0) moveAlongVector(glm::vec3(dir.x, 0.f, 0.f) * vX);
            if (vY!= 0) moveAlongVector(glm::vec3(0.f, dir.y, 0.f) * vY);
            if (vZ!= 0) moveAlongVector(glm::vec3(0.f, 0.f, dir.z) * vZ);
        }
        //groundCheck(terrain);
    }
}

void Player::setCameraWidthHeight(unsigned int w, unsigned int h) {
    m_camera.setWidthHeight(w, h);
}

void Player::moveAlongVector(glm::vec3 dir) {
    Entity::moveAlongVector(dir);
    m_camera.moveAlongVector(dir);
}
void Player::moveForwardLocal(float amount) {
    Entity::moveForwardLocal(amount);
    m_camera.moveForwardLocal(amount);
}
void Player::moveRightLocal(float amount) {
    Entity::moveRightLocal(amount);
    m_camera.moveRightLocal(amount);
}
void Player::moveUpLocal(float amount) {
    Entity::moveUpLocal(amount);
    m_camera.moveUpLocal(amount);
}
void Player::moveForwardGlobal(float amount) {
    Entity::moveForwardGlobal(amount);
    m_camera.moveForwardGlobal(amount);
}
void Player::moveRightGlobal(float amount) {
    Entity::moveRightGlobal(amount);
    m_camera.moveRightGlobal(amount);
}
void Player::moveUpGlobal(float amount) {
    Entity::moveUpGlobal(amount);
    m_camera.moveUpGlobal(amount);
}
void Player::rotateOnForwardLocal(float degrees) {
    Entity::rotateOnForwardLocal(degrees);
    m_camera.rotateOnForwardLocal(degrees);
}
void Player::rotateOnRightLocal(float degrees) {
    Entity::rotateOnRightLocal(degrees);
    m_camera.rotateOnRightLocal(degrees);
}
void Player::rotateOnUpLocal(float degrees) {
    Entity::rotateOnUpLocal(degrees);
    m_camera.rotateOnUpLocal(degrees);
}
void Player::rotateOnForwardGlobal(float degrees) {
    Entity::rotateOnForwardGlobal(degrees);
    m_camera.rotateOnForwardGlobal(degrees);
}
void Player::rotateOnRightGlobal(float degrees) {
    Entity::rotateOnRightGlobal(degrees);
    m_camera.rotateOnRightGlobal(degrees);
}
void Player::rotateOnUpGlobal(float degrees) {
    Entity::rotateOnUpGlobal(degrees);
    m_camera.rotateOnUpGlobal(degrees);
}

QString Player::posAsQString() const {
    std::string str("( " + std::to_string(m_position.x) + ", " + std::to_string(m_position.y) + ", " + std::to_string(m_position.z) + ")");
    return QString::fromStdString(str);
}
QString Player::velAsQString() const {
    std::string str("( " + std::to_string(m_velocity.x) + ", " + std::to_string(m_velocity.y) + ", " + std::to_string(m_velocity.z) + ")");
    return QString::fromStdString(str);
}
QString Player::accAsQString() const {
    std::string str("( " + std::to_string(m_acceleration.x) + ", " + std::to_string(m_acceleration.y) + ", " + std::to_string(m_acceleration.z) + ")");
    return QString::fromStdString(str);
}
QString Player::lookAsQString() const {
    std::string str("( " + std::to_string(m_forward.x) + ", " + std::to_string(m_forward.y) + ", " + std::to_string(m_forward.z) + ")");
    return QString::fromStdString(str);
}

void Player::rotateCamera(InputBundle &inputs){
    if (thirdPersonMode){
        m_camera.moveForwardLocal(12.f);
        m_camera.moveUpLocal(0.f);
    }
    if (inputs.mouseX != 0 || inputs.mouseY != 0){
        float sensitivity = 0.4f;
        float deltaX = sensitivity * inputs.mouseX;
        float deltaY = sensitivity * inputs.mouseY;
        rotateOnUpGlobal(-deltaX);
        rotateOnRightLocal(-deltaY);
        inputs.mouseX = 0.f;
        inputs.mouseY = 0.f;
    }
    if (thirdPersonMode){
        m_camera.moveForwardLocal(-12.f);
        m_camera.moveUpLocal(0.f);
    }
}


void Player::groundCheck(const Terrain &terrain){
    this->isGrounded  = false;
    std::array<glm::vec3, 4> verts = {glm::vec3(m_position.x + 0.5f, m_position.y - 0.01f, m_position.z + 0.5f),
                                      glm::vec3(m_position.x + 0.5f, m_position.y - 0.01f, m_position.z - 0.5f),
                                      glm::vec3(m_position.x - 0.5f, m_position.y - 0.01f, m_position.z + 0.5f),
                                      glm::vec3(m_position.x - 0.5f, m_position.y - 0.01f, m_position.z - 0.5f)};

    for (int i = 0; i < 4; i++){
        BlockType curBlockType = terrain.getBlockAt(verts[i]);
        if(!(curBlockType == EMPTY || curBlockType == WATER || curBlockType == LAVA)){
            this->isGrounded = true;
            m_velocity.y = 0.f;
            break;
        }
    }
}

void Player::removeBlock(){
    float currDist;
    glm::ivec3 outBlock;
    glm::vec3 pos;
    if (thirdPersonMode){
        pos = m_position + glm::vec3(0.f, 1.f, 0.f);
    }else{
        pos = m_camera.mcr_position;
    }
    if(gridMarch(pos, this->m_forward * 3.0f, mcr_terrain, &currDist, &outBlock)){
        if (mcr_terrain.getBlockAt(outBlock[0], outBlock[1], outBlock[2]) == BEDROCK) {
            return;
        }
        m_animation.play(1);
        mcr_terrain.setBlockAt(outBlock.x, outBlock.y, outBlock.z, EMPTY);
        Chunk *c = mcr_terrain.getChunkAt(outBlock.x, outBlock.z).get();
        if (c) {
            c->createVBOdata();
            c->sendVBOdata();
        }
    }

    if (mcr_terrain.getChunkAt(outBlock.x - 16, outBlock.z).get())
    {
        mcr_terrain.getChunkAt(outBlock.x - 16, outBlock.z)->createVBOdata();
        mcr_terrain.getChunkAt(outBlock.x - 16, outBlock.z)->sendVBOdata();
    }
    if (mcr_terrain.getChunkAt(outBlock.x, outBlock.z - 16).get())
    {
        mcr_terrain.getChunkAt(outBlock.x, outBlock.z - 16)->createVBOdata();
        mcr_terrain.getChunkAt(outBlock.x, outBlock.z - 16)->sendVBOdata();
    }
    if (mcr_terrain.getChunkAt(outBlock.x + 16, outBlock.z).get())
    {
        mcr_terrain.getChunkAt(outBlock.x + 16, outBlock.z)->createVBOdata();
        mcr_terrain.getChunkAt(outBlock.x + 16, outBlock.z)->sendVBOdata();
    }
    if (mcr_terrain.getChunkAt(outBlock.x, outBlock.z + 16).get())
    {
        mcr_terrain.getChunkAt(outBlock.x, outBlock.z + 16)->createVBOdata();
        mcr_terrain.getChunkAt(outBlock.x, outBlock.z + 16)->sendVBOdata();
    }

}

void Player::addBlock(){
    float currDist;
    glm::ivec3 outBlock;
    glm::vec3 pos;
    if (thirdPersonMode){
        pos = m_position + glm::vec3(0.f, 1.f, 0.f);
    }else{
        pos = m_camera.mcr_position;
    }
    if(gridMarch(pos, this->m_forward * 3.0f, mcr_terrain, &currDist, &outBlock)){
        m_animation.play(1);
        BlockType type = mcr_terrain.getBlockAt(outBlock.x, outBlock.y, outBlock.z);
        glm::vec3 intersection = pos + this->m_forward * currDist;
        glm::vec3 offset = glm::vec3(intersection.x - outBlock.x, intersection.y - outBlock.y, intersection.z - outBlock.z);
        Chunk *c = nullptr;
        if (fabs(offset.z) < 0.001f){
            mcr_terrain.setBlockAt(outBlock.x, outBlock.y, outBlock.z - 1, type);
            c = mcr_terrain.getChunkAt(outBlock.x, outBlock.z - 1).get();
        } else if (fabs(offset.z - 1.f) < 0.001f){
            mcr_terrain.setBlockAt(outBlock.x, outBlock.y, outBlock.z + 1, type);
            c = mcr_terrain.getChunkAt(outBlock.x, outBlock.z + 1).get();
        } else if (fabs(offset.x) < 0.001f) {
            mcr_terrain.setBlockAt(outBlock.x - 1, outBlock.y, outBlock.z, type);
            c = mcr_terrain.getChunkAt(outBlock.x - 1, outBlock.z).get();
        }else if (fabs(offset.x - 1.f) < 0.001f) {
            mcr_terrain.setBlockAt(outBlock.x + 1, outBlock.y, outBlock.z, type);
            c = mcr_terrain.getChunkAt(outBlock.x + 1, outBlock.z).get();
        } else if (fabs(offset.y) < 0.001f) {
            mcr_terrain.setBlockAt(outBlock.x, outBlock.y - 1, outBlock.z, type);
            c = mcr_terrain.getChunkAt(outBlock.x, outBlock.z).get();
        } else if (fabs(offset.y - 1.f) < 0.001f){
            mcr_terrain.setBlockAt(outBlock.x, outBlock.y + 1, outBlock.z, type);
            c = mcr_terrain.getChunkAt(outBlock.x, outBlock.z).get();
        }
        if (c){
            c->createVBOdata();
            c->sendVBOdata();
        }
    }
}

void Player::updateLookAtBlock()
{
    float currDist;
    glm::ivec3 outBlock;
    if (!thirdPersonMode){
        if(gridMarch(m_camera.mcr_position, this->m_forward * 3.0f, mcr_terrain, &currDist, &outBlock))
        {
            lookAt.pos[0] = outBlock[0];
            lookAt.pos[1] = outBlock[1];
            lookAt.pos[2] = outBlock[2];
            lookAt.display = true;
        }else{
            lookAt.display = false;
        }
    }else{
        lookAt.display = false;
    }
}

Block Player::getLookAtBlock() const
{
    return lookAt;
}

glm::mat4 Player::getRotationMatrix()
{
    glm::mat4 model = glm::mat4();
    if (m_animation.getCurrAnimation() < 2 || m_animation.getCurrFrame() <= 1){
        float angle = acosf(glm::dot(glm::vec3(1, 0, 0), m_right));
        if (glm::dot(m_forward, glm::vec3(-1, 0, 0)) < 0.f){
            angle *= -1;
        }
        model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
        rot = model;
    }
    return rot;
}

void Player::toggleFirstPersonOnOff()
{
    //toggle off
    if (thirdPersonMode){
        thirdPersonMode = false;
        m_camera.moveForwardLocal(12.f);
        m_camera.moveUpLocal(0.f);
    }
    //toggle on
    else{
        thirdPersonMode = true;
        m_camera.moveForwardLocal(-12.f);
        m_camera.moveUpLocal(0.f);
    }
}

void Player::playEmote(int index)
{
    if (thirdPersonMode)
    {
        m_animation.play(index);
    }
}
