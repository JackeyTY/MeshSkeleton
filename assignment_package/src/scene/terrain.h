#pragma once
#include "smartpointerhelp.h"
#include "chunk.h"
#include <unordered_map>
#include <unordered_set>
#include "shaderprogram.h"
#include "progen.h"
#include "texture.h"
#include "QMutex"
#include "river.h"

// per side, that is -NUMZONETODRAW to NUMZONETODRAW will be drawn
#define NUMZONETOWORK 3
#define NUMZONETODRAW 2

class Player;
class River;
// Helper functions to convert (x, z) to and from hash map key
int64_t toKey(int x, int z);
glm::ivec2 toCoords(int64_t k);

struct ChunkVBOData {
    Chunk *associated_chunk;
    std::vector<glm::vec4> vertex_data;
    std::vector<GLuint> idx_data;
    std::vector<glm::vec4> trans_vertex_data;
    std::vector<GLuint> trans_idx_data;
};

// The container class for all of the Chunks in the game.
// Ultimately, while Terrain will always store all Chunks,
// not all Chunks will be drawn at any given time as the world
// expands.
class Terrain {
private:
    // Stores every Chunk according to the location of its lower-left corner
    // in world space.
    // We combine the X and Z coordinates of the Chunk's corner into one 64-bit int
    // so that we can use them as a key for the map, as objects like std::pairs or
    // glm::ivec2s are not hashable by default, so they cannot be used as keys.
    std::unordered_map<int64_t, uPtr<Chunk>> m_chunks;

    // We will designate every 64 x 64 area of the world's x-z plane
    // as one "terrain generation zone". Every time the player moves
    // near a portion of the world that has not yet been generated
    // (i.e. its lower-left coordinates are not in this set), a new
    // 4 x 4 collection of Chunks is created to represent that area
    // of the world.
    // The world that exists when the base code is run consists of exactly
    // one 64 x 64 area with its lower-left corner at (0, 0).
    // When milestone 1 has been implemented, the Player can move around the
    // world to add more "terrain generation zone" IDs to this set.
    // While only the 3 x 3 collection of terrain generation zones
    // surrounding the Player should be rendered, the Chunks
    // in the Terrain will never be deleted until the program is terminated.
    std::unordered_set<int64_t> m_generatedTerrain;

    // TODO: DELETE ALL REFERENCES TO m_geomCube AS YOU WILL NOT USE
    // IT IN YOUR FINAL PROGRAM!
    // The instance of a unit cube we can use to render any cube.
    // Presently, Terrain::draw renders one instance of this cube
    // for every non-EMPTY block within its Chunks. This is horribly
    // inefficient, and will cause your game to run very slowly until
    // milestone 1's Chunk VBO setup is completed.
    OpenGLContext* mp_context;
    ProGen progen;
    uPtr<Texture> mp_texture;

    //void createTerrainZone(int x, int z);
    BlockType getBlockTypeAtHeight(int x, int y, int z, int blockType, bool top);

public:
    Terrain(OpenGLContext *context);
    ~Terrain();

    // threads manipulation data
    std::vector<Chunk*> m_chunksWithOnlyBlockData;
    QMutex m_chunksWithOnlyBlockDataLock;
    std::vector<ChunkVBOData> m_chunksWithVBOData;
    QMutex m_chunksWithVBODataLock;

    // Instantiates a new Chunk and stores it in
    // our chunk map at the given coordinates.
    // Returns a pointer to the created Chunk.
    Chunk* instantiateChunkAt(int x, int z);
    // Do these world-space coordinates lie within
    // a Chunk that exists?
    bool hasChunkAt(int x, int z) const;
    // Assuming a Chunk exists at these coords,
    // return a mutable reference to it
    uPtr<Chunk>& getChunkAt(int x, int z);
    // Assuming a Chunk exists at these coords,
    // return a const reference to it
    const uPtr<Chunk>& getChunkAt(int x, int z) const;
    // Given a world-space coordinate (which may have negative
    // values) return the block stored at that point in space.
    BlockType getBlockAt(int x, int y, int z) const;
    BlockType getBlockAt(glm::vec3 p) const;
    // Given a world-space coordinate (which may have negative
    // values) set the block at that point in space to the
    // given type.
    void setBlockAt(int x, int y, int z, BlockType t);

    // Draws every Chunk that falls within the bounding box
    // described by the min and max coords, using the provided
    // ShaderProgram
    void draw(int minX, int maxX, int minZ, int maxZ, ShaderProgram *shaderProgram, bool shader);

    void CreateInitialScene(glm::vec3);

    // void updateScene(const glm::vec3 pos, ShaderProgram *shaderProgram, Player *p);
    Chunk* createChunkAt(int x, int z);
    std::vector<int64_t> checkExpansion(glm::vec3 position);

    void createTerrainZone(int x, int z, bool createvbo);
};
