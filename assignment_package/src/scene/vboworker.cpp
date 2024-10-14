#include "vboworker.h"
#include "iostream"

VBOWorker::VBOWorker(
                     std::vector<ChunkVBOData>* mp_chunksWithVBOData,
                     Chunk *c,
                     QMutex *mutex)
    :
    mp_chunksWithVBOData(mp_chunksWithVBOData),
    mp_mutex(mutex),
    mp_chunk(c)
{
}
void VBOWorker::run() {
    mp_chunk->createVBOdata();

    ChunkVBOData vboData;
    vboData.associated_chunk = mp_chunk;
    vboData.idx_data = mp_chunk->m_VBOdataIdx;
    vboData.vertex_data = mp_chunk->m_VBOdataAll;
    vboData.trans_idx_data = mp_chunk->m_VBOdataTransIdx;
    vboData.trans_vertex_data = mp_chunk->m_VBOdataTransAll;

    mp_mutex->lock();
    mp_chunksWithVBOData->push_back(vboData);
    mp_mutex->unlock();
}
