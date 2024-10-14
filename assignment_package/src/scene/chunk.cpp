#include "chunk.h"
#include <iostream>


Chunk::Chunk(OpenGLContext* context, int x, int z) : Drawable(context),m_blocks(), minX(x), minZ(z), m_neighbors{{XPOS, nullptr}, {XNEG, nullptr}, {ZPOS, nullptr}, {ZNEG, nullptr}}
{
    std::fill_n(m_blocks.begin(), 65536, EMPTY);
}
Chunk::~Chunk(){}
// Does bounds checking with at()
BlockType Chunk::getBlockAt(unsigned int x, unsigned int y, unsigned int z) const {
    return m_blocks.at(x + 16 * y + 16 * 256 * z);
}

// Exists to get rid of compiler warnings about int -> unsigned int implicit conversion
BlockType Chunk::getBlockAt(int x, int y, int z) const {
    return getBlockAt(static_cast<unsigned int>(x), static_cast<unsigned int>(y), static_cast<unsigned int>(z));
}

// Does bounds checking with at()
void Chunk::setBlockAt(unsigned int x, unsigned int y, unsigned int z, BlockType t) {
    m_blocks.at(x + 16 * y + 16 * 256 * z) = t;
}


const static std::unordered_map<Direction, Direction, EnumHash> oppositeDirection {
    {XPOS, XNEG},
    {XNEG, XPOS},
    {YPOS, YNEG},
    {YNEG, YPOS},
    {ZPOS, ZNEG},
    {ZNEG, ZPOS}
};

void Chunk::linkNeighbor(uPtr<Chunk> &neighbor, Direction dir) {
    if(neighbor != nullptr) {
        this->m_neighbors[dir] = neighbor.get();
        neighbor->m_neighbors[oppositeDirection.at(dir)] = this;
    }
}
GLenum Chunk::drawMode(){
    return GL_TRIANGLES;
}

const static std::unordered_map<BlockType, bool> transBlocks = {{EMPTY, 1}, {WATER, 1}};
const static std::unordered_map<BlockType, bool> animatableBlocks = {{WATER, 1}, {LAVA, 1}};

void Chunk::createVBOdata() {
    int num_count = 0; //num increased by 4 every time for index vbo
    int num_countTrans = 0; //num increased by 4 every time for index vbo

    std::vector<glm::vec4> all;
    std::vector<glm::vec4> transAll;
    std::vector<GLuint> idx;
    std::vector<GLuint> transIdx;

    std::vector<glm::vec4> *used;

    for (int z = 0; z < 16; ++z) {
        for (int y = 0; y < 256; ++y) {
            for (int x = 0; x < 16; ++x) {

                int num = 0;
                int transNum = 0;
                BlockType t = getBlockAt(x, y, z);
                if (t == EMPTY) {
                    continue;
                }

                // create vbo data for 6 faces
                BlockType topBlock = EMPTY;
                if (y == 255) {
                    topBlock = EMPTY;
                } else {
                    topBlock = getBlockAt(x, y+1, z);
                }

                BlockType botBlock = EMPTY;
                if (y == 0) {
                    botBlock = EMPTY;
                } else {
                    botBlock = getBlockAt(x, y-1, z);
                }

                BlockType rightBlock = EMPTY;
                if (x == 15) {
                    Chunk *rightNeighbor = m_neighbors.at(XPOS);
                    if (rightNeighbor != nullptr) {
                        rightBlock = rightNeighbor->getBlockAt((x+1) % 16, y, z);
                    }
                } else {
                    rightBlock = getBlockAt(x + 1, y, z);
                }

                BlockType leftBlock = EMPTY;
                if (x == 0) {
                    Chunk *leftNeighbor = m_neighbors.at(XNEG);
                    if (leftNeighbor != nullptr) {
                        leftBlock = leftNeighbor->getBlockAt(15, y, z);
                    }
                } else {
                    leftBlock = getBlockAt(x - 1, y, z);
                }

                BlockType frontBlock = EMPTY;
                if (z == 15) {
                    Chunk *frontNeighbor = m_neighbors.at(ZPOS);
                    if (frontNeighbor != nullptr) {
                        frontBlock = frontNeighbor->getBlockAt(x, y, 0);
                    }
                } else {
                    frontBlock = getBlockAt(x, y, z+1);
                }

                BlockType backBlock = EMPTY;
                if (z == 0) {
                    Chunk *backNeighbor = m_neighbors.at(ZNEG);
                    if (backNeighbor != nullptr) {
                        backBlock = backNeighbor->getBlockAt(x, y, 15);
                    }
                } else {
                    backBlock = getBlockAt(x, y, z-1);
                }

                if (transBlocks.find(topBlock) != transBlocks.end()) {
                    if (transBlocks.find(t)== transBlocks.end()){
                        num++;
                        used = &all;
                    }else{
                        if (topBlock == EMPTY){
                            transNum++;
                            used = &transAll;
                        }else{
                            continue;
                        }
                    }
                    bool animatable;
                    if (animatableBlocks.find(t) == transBlocks.end()){
                        animatable = false;
                    }else{
                        animatable = true;
                    }
                    glm::vec4 normal(0.f, 1.f, 0.f, 0.f);

                    used->push_back(glm::vec4(x + minX, y+1, z + minZ+1, 1.f));
                    used->push_back(normal);
                    used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(YPOS)->second, 0.f, animatable));

                    used->push_back(glm::vec4(x + minX + 1, y+1, z + minZ+1, 1.f));
                    used->push_back(normal);
                    used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(YPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, animatable));

                    used->push_back(glm::vec4(x + minX + 1, y+1, z + minZ, 1.f));
                    used->push_back(normal);
                    used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(YPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, animatable));

                    used->push_back(glm::vec4(x + minX, y+1, z + minZ, 1.f));
                    used->push_back(normal);
                    used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(YPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, animatable));
                    /**
                    glm::vec4 normal(0.f, 1.f, 0.f, 0.f);
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);

                    pos.push_back(glm::vec4(x + minX, y+1, z + minZ+1, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y+1, z + minZ+1, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y+1, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX, y+1, z + minZ, 1.f));

                    uv.push_back(blockFaceUVs.find(t)->second.find(YPOS)->second);
                    uv.push_back(blockFaceUVs.find(t)->second.find(YPOS)->second + glm::vec2(BLK_UV, 0.f));
                    uv.push_back(blockFaceUVs.find(t)->second.find(YPOS)->second + glm::vec2(BLK_UV, BLK_UV));
                    uv.push_back(blockFaceUVs.find(t)->second.find(YPOS)->second + glm::vec2(0.f, BLK_UV));
                    **/
                }
                if (transBlocks.find(botBlock) != transBlocks.end()) {
                    if (transBlocks.find(t)== transBlocks.end()){
                        num++;
                        used = &all;
                        bool animatable;
                        if (animatableBlocks.find(t) == transBlocks.end()){
                            animatable = false;
                        }else{
                            animatable = true;
                        }
                        glm::vec4 normal(0.f, -1.f, 0.f, 0.f);

                        used->push_back(glm::vec4(x + minX, y, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(YNEG)->second, 0.f, animatable));

                        used->push_back(glm::vec4(x + minX + 1, y, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(YNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, animatable));

                        used->push_back(glm::vec4(x + minX + 1, y, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(YNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, animatable));

                        used->push_back(glm::vec4(x + minX, y, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(YNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, animatable));
                    }

                    /**
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);

                    pos.push_back(glm::vec4(x + minX, y, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y, z + minZ+1, 1.f));
                    pos.push_back(glm::vec4(x + minX, y, z + minZ+1, 1.f));

                    uv.push_back(blockFaceUVs.find(t)->second.find(YNEG)->second);
                    uv.push_back(blockFaceUVs.find(t)->second.find(YNEG)->second + glm::vec2(BLK_UV, 0.f));
                    uv.push_back(blockFaceUVs.find(t)->second.find(YNEG)->second + glm::vec2(BLK_UV, BLK_UV));
                    uv.push_back(blockFaceUVs.find(t)->second.find(YNEG)->second + glm::vec2(0.f, BLK_UV));
                    **/
                }
                if (transBlocks.find(rightBlock) != transBlocks.end()) {
                    if (transBlocks.find(t)== transBlocks.end()){
                        num++;
                        used = &all;
                        bool animatable;
                        if (animatableBlocks.find(t) == transBlocks.end()){
                            animatable = false;
                        }else{
                            animatable = true;
                        }
                        glm::vec4 normal(1.f, 0.f, 0.f, 0.f);

                        used->push_back(glm::vec4(x + minX + 1, y, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(XPOS)->second, 0.f, animatable));

                        used->push_back(glm::vec4(x + minX + 1, y, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(XPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, animatable));

                        used->push_back(glm::vec4(x + minX + 1, y+1, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(XPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, animatable));


                        used->push_back(glm::vec4(x + minX + 1, y+1, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(XPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, animatable));
                    }
                    /**
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);

                    pos.push_back(glm::vec4(x + minX + 1, y, z + minZ+1, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y+1, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y+1, z + minZ+1, 1.f));

                    uv.push_back(blockFaceUVs.find(t)->second.find(XPOS)->second);
                    uv.push_back(blockFaceUVs.find(t)->second.find(XPOS)->second + glm::vec2(BLK_UV, 0.f));
                    uv.push_back(blockFaceUVs.find(t)->second.find(XPOS)->second + glm::vec2(BLK_UV, BLK_UV));
                    uv.push_back(blockFaceUVs.find(t)->second.find(XPOS)->second + glm::vec2(0.f, BLK_UV));
                    **/
                }
                if (transBlocks.find(leftBlock) != transBlocks.end()) {
                    if (transBlocks.find(t)== transBlocks.end()){
                        num++;
                        used = &all;
                        bool animatable;
                        if (animatableBlocks.find(t) == transBlocks.end()){
                            animatable = false;
                        }else{
                            animatable = true;
                        }
                        glm::vec4 normal(-1.f, 0.f, 0.f, 0.f);

                        used->push_back(glm::vec4(x + minX, y, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(XNEG)->second, 0.f, animatable));

                        used->push_back(glm::vec4(x + minX, y, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(XNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, animatable));

                        used->push_back(glm::vec4(x + minX, y+1, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(XNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, animatable));

                        used->push_back(glm::vec4(x + minX, y+1, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(XNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, animatable));
                    }

                    /**
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);

                    pos.push_back(glm::vec4(x + minX, y, z + minZ+1, 1.f));
                    pos.push_back(glm::vec4(x + minX, y, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX, y+1, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX, y+1, z + minZ+1, 1.f));

                    uv.push_back(blockFaceUVs.find(t)->second.find(XNEG)->second);
                    uv.push_back(blockFaceUVs.find(t)->second.find(XNEG)->second + glm::vec2(BLK_UV, 0.f));
                    uv.push_back(blockFaceUVs.find(t)->second.find(XNEG)->second + glm::vec2(BLK_UV, BLK_UV));
                    uv.push_back(blockFaceUVs.find(t)->second.find(XNEG)->second + glm::vec2(0.f, BLK_UV));
                    **/
                }
                if (transBlocks.find(frontBlock) != transBlocks.end()) {
                    if (transBlocks.find(t)== transBlocks.end()){
                        num++;
                        used = &all;
                        bool animatable;
                        if (animatableBlocks.find(t) == transBlocks.end()){
                            animatable = false;
                        }else{
                            animatable = true;
                        }
                        glm::vec4 normal(0.f, 0.f, 1.f, 0.f);
                        used->push_back(glm::vec4(x + minX, y, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(ZPOS)->second, 0.f, animatable));
                        used->push_back(glm::vec4(x + minX + 1, y, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(ZPOS)->second + glm::vec2(BLK_UV, 0.f), 0.f, animatable));
                        used->push_back(glm::vec4(x + minX + 1, y+1, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(ZPOS)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, animatable));
                        used->push_back(glm::vec4(x + minX, y+1, z + minZ+1, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(ZPOS)->second + glm::vec2(0.f, BLK_UV), 0.f, animatable));
                    }
                    /**
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);

                    pos.push_back(glm::vec4(x + minX, y, z + minZ+1, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y, z + minZ+1, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y+1, z + minZ+1, 1.f));
                    pos.push_back(glm::vec4(x + minX, y+1, z + minZ+1, 1.f));

                    uv.push_back(blockFaceUVs.find(t)->second.find(ZPOS)->second);
                    uv.push_back(blockFaceUVs.find(t)->second.find(ZPOS)->second + glm::vec2(BLK_UV, 0.f));
                    uv.push_back(blockFaceUVs.find(t)->second.find(ZPOS)->second + glm::vec2(BLK_UV, BLK_UV));
                    uv.push_back(blockFaceUVs.find(t)->second.find(ZPOS)->second + glm::vec2(0.f, BLK_UV));
                    **/
                }
                if (transBlocks.find(backBlock) != transBlocks.end()) {
                    if (transBlocks.find(t)== transBlocks.end()){
                        num++;
                        used = &all;
                        bool animatable;
                        if (animatableBlocks.find(t) == transBlocks.end()){
                            animatable = false;
                        }else{
                            animatable = true;
                        }
                        glm::vec4 normal(0.f, 0.f, -1.f, 0.f);

                        used->push_back(glm::vec4(x + minX, y, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(ZNEG)->second, 0.f, animatable));

                        used->push_back(glm::vec4(x + minX + 1, y, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(ZNEG)->second + glm::vec2(BLK_UV, 0.f), 0.f, animatable));

                        used->push_back(glm::vec4(x + minX + 1, y+1, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(ZNEG)->second + glm::vec2(BLK_UV, BLK_UV), 0.f, animatable));

                        used->push_back(glm::vec4(x + minX, y+1, z + minZ, 1.f));
                        used->push_back(normal);
                        used->push_back(glm::vec4(blockFaceUVs.find(t)->second.find(ZNEG)->second + glm::vec2(0.f, BLK_UV), 0.f, animatable));
                    }

                    /**
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);
                    nor.push_back(normal);

                    pos.push_back(glm::vec4(x + minX, y, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX + 1, y+1, z + minZ, 1.f));
                    pos.push_back(glm::vec4(x + minX, y+1, z + minZ, 1.f));

                    uv.push_back(blockFaceUVs.find(t)->second.find(ZNEG)->second);
                    uv.push_back(blockFaceUVs.find(t)->second.find(ZNEG)->second + glm::vec2(BLK_UV, 0.f));
                    uv.push_back(blockFaceUVs.find(t)->second.find(ZNEG)->second + glm::vec2(BLK_UV, BLK_UV));
                    uv.push_back(blockFaceUVs.find(t)->second.find(ZNEG)->second + glm::vec2(0.f, BLK_UV));
                    **/
                }

                /*
                for (int i = 0; i < num * 4; ++i) {
                    switch(t) {
                    case GRASS:
                        col.push_back(glm::vec4(95.f, 159.f, 53.f, 255.f) / 255.f);
                        break;
                    case DIRT:
                        col.push_back(glm::vec4(121.f, 85.f, 58.f, 255.f) / 255.f);
                        break;
                    case STONE:
                        col.push_back(glm::vec4(0.5f));
                        break;
                    case WATER:
                        col.push_back(glm::vec4(0.f, 0.f, 0.75f, 1.f));
                        break;
                    case SNOW:
                        col.push_back(glm::vec4(1.f, 1.f, 1.f, 1.f));
                        break;
                    default:
                        col.push_back(glm::vec4(1.f, 0.f, 1.f, 255.f));
                        break;
                    }
                }
                **/
                for (int i = 0; i < num; i++) {
                    idx.push_back(num_count);
                    idx.push_back(num_count + 1);
                    idx.push_back(num_count + 2);
                    idx.push_back(num_count);
                    idx.push_back(num_count + 2);
                    idx.push_back(num_count + 3);

                    num_count = num_count + 4;
                }

                for (int i = 0; i < transNum; i++) {
                    transIdx.push_back(num_countTrans);
                    transIdx.push_back(num_countTrans + 1);
                    transIdx.push_back(num_countTrans + 2);
                    transIdx.push_back(num_countTrans);
                    transIdx.push_back(num_countTrans + 2);
                    transIdx.push_back(num_countTrans + 3);

                    num_countTrans = num_countTrans + 4;
                }

            }
        }


    //interleave
    /**
    for (int i = 0; i < pos.size(); ++i) {
        all.push_back(pos.at(i));
        all.push_back(nor.at(i));
        //all.push_back(col.at(i));
        all.push_back(glm::vec4(uv.at(i), 0.f, 0.f));
    }
    **/
    }
    this->m_VBOdataIdx = idx;
    this->m_VBOdataAll = all;
    this->m_VBOdataTransIdx = transIdx;
    this->m_VBOdataTransAll = transAll;

}



void Chunk::sendVBOdata() {
    m_count = m_VBOdataIdx.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_VBOdataIdx.size() * sizeof(GLuint), m_VBOdataIdx.data(), GL_STATIC_DRAW);

    generateAll();
    bindAll();
    mp_context->glBufferData(GL_ARRAY_BUFFER, m_VBOdataAll.size() * sizeof(glm::vec4), m_VBOdataAll.data(), GL_STATIC_DRAW);

    m_transCount = m_VBOdataTransAll.size();

    generateTransIdx();
    bindTransIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_VBOdataTransIdx.size() * sizeof(GLuint), m_VBOdataTransIdx.data(), GL_STATIC_DRAW);

    generateTransAll();
    bindTransAll();
    mp_context->glBufferData(GL_ARRAY_BUFFER, m_VBOdataTransAll.size() * sizeof(glm::vec4), m_VBOdataTransAll.data(), GL_STATIC_DRAW);
}
