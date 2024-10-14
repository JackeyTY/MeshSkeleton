#ifndef BLOCKTYPEWORKER_H
#define BLOCKTYPEWORKER_H

#include <QRunnable>
#include <QMutex>
#include <scene/terrain.h>
using namespace std;

class BlockTypeWorker : public QRunnable
{
private:
    Terrain *mp_terrain;
    int64_t coord;
    std::vector<Chunk*> terrainsChunk;
    std::vector<Chunk*> *mp_chunksWithOnlyBlockData;
    QMutex *mutex;

public:

    BlockTypeWorker(Terrain * terrain,
                    int64_t Coord,
                    std::vector<Chunk*> terrainsChunk,
                    std::vector<Chunk*> *mp_chunksWithOnlyBlockData,
                    QMutex* mutex);
    void run() override;

    // create 4 by 4 chunks and set its neighbors

};
#endif // BLOCKTYPEWORKER_H
