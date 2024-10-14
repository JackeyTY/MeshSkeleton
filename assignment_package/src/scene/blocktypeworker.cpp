#include "blocktypeworker.h"
#include "iostream"

BlockTypeWorker::BlockTypeWorker(Terrain * terrain,
                                 int64_t Coord,
                                 std::vector<Chunk*> terrainsChunk,
                                 std::vector<Chunk*> *mp_chunksWithOnlyBlockData,
                                 QMutex* mutex)
    :mp_terrain(terrain), coord(Coord), terrainsChunk(terrainsChunk), mp_chunksWithOnlyBlockData(mp_chunksWithOnlyBlockData), mutex(mutex)
{
}

void BlockTypeWorker::run() {

    int x = toCoords(coord).x;
    int z = toCoords(coord).y;

    try {
        mp_terrain->createTerrainZone(x, z, false);
    }
    catch(std::out_of_range &e) {
        std::cout << "BlockTypeWorkerOutRange";
    }

    River river = River(mp_terrain, x, z);

    double random = ((double) rand() / (RAND_MAX));
    if (random < 0.15)
        river.draw();


    mutex->lock();
    for (Chunk *c : terrainsChunk) {
        mp_terrain->m_chunksWithOnlyBlockData.push_back(c);
    }
    mutex->unlock();
}


