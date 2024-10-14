#include "terrain.h"
#include <iostream>
#include <stdexcept>

Terrain::Terrain(OpenGLContext *context)
    : m_chunks(), m_generatedTerrain(), mp_context(context), progen(), mp_texture(nullptr)
{}

Terrain::~Terrain() {
}

// Combine two 32-bit ints into one 64-bit int
// where the upper 32 bits are X and the lower 32 bits are Z
int64_t toKey(int x, int z) {
    int64_t xz = 0xffffffffffffffff;
    int64_t x64 = x;
    int64_t z64 = z;

    // Set all lower 32 bits to 1 so we can & with Z later
    xz = (xz & (x64 << 32)) | 0x00000000ffffffff;

    // Set all upper 32 bits to 1 so we can & with XZ
    z64 = z64 | 0xffffffff00000000;

    // Combine
    xz = xz & z64;
    return xz;
}

glm::ivec2 toCoords(int64_t k) {
    // Z is lower 32 bits
    int64_t z = k & 0x00000000ffffffff;
    // If the most significant bit of Z is 1, then it's a negative number
    // so we have to set all the upper 32 bits to 1.
    // Note the 8    V
    if(z & 0x0000000080000000) {
        z = z | 0xffffffff00000000;
    }
    int64_t x = (k >> 32);

    return glm::ivec2(x, z);
}

// Surround calls to this with try-catch if you don't know whether
// the coordinates at x, y, z have a corresponding Chunk
BlockType Terrain::getBlockAt(int x, int y, int z) const
{
    if(hasChunkAt(x, z)) {
        // Just disallow action below or above min/max height,
        // but don't crash the game over it.
        if(y < 0 || y >= 256) {
            return EMPTY;
        }
        const uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        return c->getBlockAt(static_cast<unsigned int>(x - chunkOrigin.x),
                             static_cast<unsigned int>(y),
                             static_cast<unsigned int>(z - chunkOrigin.y));
    }
    else {
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

BlockType Terrain::getBlockAt(glm::vec3 p) const {
    return getBlockAt(p.x, p.y, p.z);
}

void Terrain::setBlockAt(int x, int y, int z, BlockType t)
{
    if(hasChunkAt(x, z)) {
        uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        c->setBlockAt(static_cast<unsigned int>(x - chunkOrigin.x),
                      static_cast<unsigned int>(y),
                      static_cast<unsigned int>(z - chunkOrigin.y),
                      t);
    }
    else {
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

bool Terrain::hasChunkAt(int x, int z) const {
    // Map x and z to their nearest Chunk corner
    // By flooring x and z, then multiplying by 16,
    // we clamp (x, z) to its nearest Chunk-space corner,
    // then scale back to a world-space location.
    // Note that floor() lets us handle negative numbers
    // correctly, as floor(-1 / 16.f) gives us -1, as
    // opposed to (int)(-1 / 16.f) giving us 0 (incorrect!).
    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));
    return m_chunks.find(toKey(16 * xFloor, 16 * zFloor)) != m_chunks.end();
}


uPtr<Chunk>& Terrain::getChunkAt(int x, int z) {
    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));
    return m_chunks[toKey(16 * xFloor, 16 * zFloor)];
}


const uPtr<Chunk>& Terrain::getChunkAt(int x, int z) const {
    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));
    return m_chunks.at(toKey(16 * xFloor, 16 * zFloor));
}

Chunk* Terrain::instantiateChunkAt(int x, int z) {
    uPtr<Chunk> chunk = mkU<Chunk>(this->mp_context,x, z);
    Chunk *cPtr = chunk.get();
    m_chunks[toKey(x, z)] = std::move(chunk);
    // Set the neighbor pointers of itself and its neighbors
    if(hasChunkAt(x, z + 16)) {
        auto &chunkNorth = m_chunks[toKey(x, z + 16)];
        cPtr->linkNeighbor(chunkNorth, ZPOS);
    }
    if(hasChunkAt(x, z - 16)) {
        auto &chunkSouth = m_chunks[toKey(x, z - 16)];
        cPtr->linkNeighbor(chunkSouth, ZNEG);
    }
    if(hasChunkAt(x + 16, z)) {
        auto &chunkEast = m_chunks[toKey(x + 16, z)];
        cPtr->linkNeighbor(chunkEast, XPOS);
    }
    if(hasChunkAt(x - 16, z)) {
        auto &chunkWest = m_chunks[toKey(x - 16, z)];
        cPtr->linkNeighbor(chunkWest, XNEG);
    }
    return cPtr;
}

Chunk* Terrain::createChunkAt(int x, int z) {
    uPtr<Chunk> chunk = mkU<Chunk>(mp_context,x,z);
    Chunk *cPtr = chunk.get();
    chunk->m_count = 0;
    chunk->m_transCount = 0;

    m_chunks[toKey(x, z)] = std::move(chunk);
    if(hasChunkAt(x, z + 16)) {
        auto &chunkNorth = m_chunks[toKey(x, z + 16)];
        cPtr->linkNeighbor(chunkNorth, ZPOS);
    }
    if(hasChunkAt(x, z - 16)) {
        auto &chunkSouth = m_chunks[toKey(x, z - 16)];
        cPtr->linkNeighbor(chunkSouth, ZNEG);
    }
    if(hasChunkAt(x + 16, z)) {
        auto &chunkEast = m_chunks[toKey(x + 16, z)];
        cPtr->linkNeighbor(chunkEast, XPOS);
    }
    if(hasChunkAt(x - 16, z)) {
        auto &chunkWest = m_chunks[toKey(x - 16, z)];
        cPtr->linkNeighbor(chunkWest, XNEG);
    }
    return cPtr;
}

void Terrain::createTerrainZone(int x, int z, bool createvbo) {
    std::vector<Chunk*> newChunks;
    for (int i = 0; i < 64; i += 16) {
        for (int j = 0; j < 64; j += 16) {
            if (!hasChunkAt(x + i, z + j)){
                newChunks.push_back(instantiateChunkAt(x + i, z + j));
            }
        }
    }

    m_generatedTerrain.insert(toKey(x, z));

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            int curX = x + i;
            int curZ = z + j;
            std::vector<int> blockInfo = progen.getBlockHeight(curX, curZ);
            int curY = blockInfo[0];
            for (int k = 0; k <= curY; k++) {
                setBlockAt(curX, k, curZ, getBlockTypeAtHeight(curX, k, curZ, blockInfo[1], curY));
            }
            if (curY < 132) {
                for (int t = curY + 1; t < 132; t++) {
                    setBlockAt(curX, t, curZ, WATER);
                }
            }
        }
    }
//    for (int i = 0; i < 64; i+=16) {
//        for (int j = 0; j < 64; j+=16) {
//            River river = River(this, x+i, z+j);
//            river.draw();
//        }
//    }
    if (createvbo) {
        for (Chunk* c : newChunks){
            c ->createVBOdata();
            c->sendVBOdata();
        }
    }
}

void Terrain::CreateInitialScene(glm::vec3 pos) {
    int xFloor = static_cast<int>(glm::floor(pos.x / 64.f));
    int zFloor = static_cast<int>(glm::floor(pos.z / 64.f));
    int x = 64 * xFloor;
    int z = 64 * zFloor; //x and z represent the chunk's position (lower left corner)
    for (int i = x - 64; i < x + 65; i += 64) {
        for (int j = z - 64; j < z + 65; j += 64) {
            createTerrainZone(i, j, true);
        }
    }
}

BlockType Terrain::getBlockTypeAtHeight(int x, int y, int z, int blockType, bool top) {
    if (y == 0) {
        return BEDROCK;
    } else if (y < 25) {
        return LAVA;
    } else if (y < 118) {
        if (progen.keepCave(x, y, z)) {
            return STONE;
        } else {
            return EMPTY;
        }
    } else if (y < 120) {
        return STONE;
    } else if (y < 128) {
        return SAND;
    } else if (y < 160) {
        if (blockType == 0) {
            return DUNE;
        }
        return top ? GRASS : DIRT;
    } else if (y < 200) {
        return STONE;
    } else {
        return top ? SNOW : STONE;
    }
}

std::vector<int64_t> Terrain::checkExpansion(glm::vec3 position) {

    std::vector<int64_t> output;
    int lowerLeftX = glm::floor(position.x / 64.0f);
    int lowerLeftZ = glm::floor(position.z / 64.0f);

    for (int z = -NUMZONETOWORK; z <= NUMZONETOWORK; z++) {
        for (int x = -NUMZONETOWORK; x <= NUMZONETOWORK; x++) {
            int64_t currTerrain = toKey((lowerLeftX + x) * 64, (lowerLeftZ + z) * 64);
            if (m_generatedTerrain.find(currTerrain) == m_generatedTerrain.end()) {
                m_generatedTerrain.insert(currTerrain);
                output.push_back(currTerrain);
            }
        }
    }
    return output;
}

// TODO: When you make Chunk inherit from Drawable, change this code so
// it draws each Chunk with the given ShaderProgram, remembering to set the
// model matrix to the proper X and Z translation!
void Terrain::draw(int minX, int maxX, int minZ, int maxZ, ShaderProgram *shaderProgram, bool shadow) {
    for(int x = minX; x < maxX; x += 16) {
        for(int z = minZ; z < maxZ; z += 16) {
            if (hasChunkAt(x, z)) {
                const uPtr<Chunk> &chunk = getChunkAt(x, z);
                shaderProgram->drawInterleaved(*chunk, 0, 1);
            }
        }
    }
    if (shadow) {
        return;
    }
    for(int x = minX; x < maxX; x += 16) {
        for(int z = minZ; z < maxZ; z += 16) {
            if (hasChunkAt(x, z)) {
                const uPtr<Chunk> &chunk = getChunkAt(x, z);
                shaderProgram->drawTransInterleaved(*chunk, 0, 1);
            }
        }
    }
}
