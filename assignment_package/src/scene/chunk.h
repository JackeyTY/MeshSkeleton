#pragma once
#include "smartpointerhelp.h"
#include "glm_includes.h"
#include <array>
#include <unordered_map>
#include <cstddef>
#include "drawable.h"
#include "texturehelp.h"


//using namespace std;

// One Chunk is a 16 x 256 x 16 section of the world,
// containing all the Minecraft blocks in that area.
// We divide the world into Chunks in order to make
// recomputing its VBO data faster by not having to
// render all the world at once, while also not having
// to render the world block by block.

// TODO have Chunk inherit from Drawable
class Chunk : public Drawable {
private:
    // All of the blocks contained within this Chunk
    std::array<BlockType, 65536> m_blocks;
    int minX, minZ;
    // This Chunk's four neighbors to the north, south, east, and west
    // The third input to this map just lets us use a Direction as
    // a key for this map.
    // These allow us to properly determine
    std::unordered_map<Direction, Chunk*, EnumHash> m_neighbors;

public:
    Chunk(OpenGLContext* context, int x, int z);
    ~Chunk();
    void createVBOdata() override;

    GLenum drawMode() override;

    BlockType getBlockAt(unsigned int x, unsigned int y, unsigned int z) const;
    BlockType getBlockAt(int x, int y, int z) const;
    void setBlockAt(unsigned int x, unsigned int y, unsigned int z, BlockType t);
    void linkNeighbor(uPtr<Chunk>& neighbor, Direction dir);
    void sendVBOdata();
    void sendTransVBOdata();

    std::vector<GLuint> m_VBOdataIdx;
    std::vector<glm::vec4> m_VBOdataAll;
    std::vector<GLuint> m_VBOdataTransIdx;
    std::vector<glm::vec4> m_VBOdataTransAll;

    friend class Terrain;
};
