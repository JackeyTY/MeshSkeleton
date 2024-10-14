#include "mygl.h"
#include "tinyobj/tiny_obj_loader.h"
#include <iostream>

void project(std::vector<glm::vec3> points, glm::vec3 axis,
             float &min, float &max)
{
    min = INFINITY;
    max = -INFINITY;
    for (glm::vec3 &p : points)
    {
        float val = glm::dot(axis, p);
        if (val < min) min = val;
        if (val > max) max = val;
    }
}

bool voxelTriangleIntersection(int x, int y, int z, std::vector<glm::vec3> triangle)
{
    float triangleMin, triangleMax;
    float boxMin, boxMax;
    std::array<int, 3> boxMins = {x, y, z};
    std::array<int, 3> boxMaxs = {x+1, y+1, z+1};
    std::vector<glm::vec3> boxVertices = {glm::vec3(x, y, z), glm::vec3(x + 1, y, z),
                                          glm::vec3(x, y + 1, z), glm::vec3(x + 1, y + 1, z),
                                          glm::vec3(x, y, z + 1), glm::vec3(x + 1, y, z + 1),
                                          glm::vec3(x, y + 1, z + 1), glm::vec3(x + 1, y + 1, z + 1)};

    // Test the box normals (x-, y- and z-axes)
    std::array<glm::vec3, 3> boxNormals = {glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)};
    for (int i = 0; i < 3; i++)
    {
        project(triangle, boxNormals[i], triangleMin, triangleMax);
        if (triangleMax < boxMins[i]|| triangleMin > boxMaxs[i])
            return false; // No intersection possible.
    }

    // Test the triangle normal
    glm::vec3 triangleNormal = glm::cross(triangle[1] - triangle[2], triangle[1] - triangle[0]);
    float triangleOffset = glm::dot(triangleNormal, triangle[0]);
    project(boxVertices, triangleNormal, boxMin, boxMax);
    if (boxMax < triangleOffset || boxMin > triangleOffset)
        return false; // No intersection possible.

    // Test the nine edge cross-products
    std::array<glm::vec3, 3> triangleEdges = {triangle[0] - triangle[1], triangle[1] - triangle[2], triangle[2] - triangle[0]};
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++)
        {
            // The box normals are the same as it's edge tangents
            glm::vec3 axis = glm::cross(triangleEdges[i], boxNormals[j]);
            project(boxVertices, axis, boxMin, boxMax);
            project(triangle, axis, triangleMin, triangleMax);
            if (boxMax <= triangleMin || boxMin >= triangleMax)
                return false; // No intersection possible
        }
    }
    // No separating axis found.
    return true;
}

void createFromOBJ(const char* filename, Terrain *t)
{
    std::vector<tinyobj::shape_t> shapes; std::vector<tinyobj::material_t> materials;
    std::vector<std::vector<int>> faces;
    std::vector<float> positions;
    std::string errors = tinyobj::LoadObj(shapes, materials, faces, positions, filename);
    std::cout << errors << std::endl;
    if(errors.size() == 0)
    {
        std::map<Chunk*, bool> chunks;
        for(const std::vector<int> &face: faces)
        {
            glm::vec3 initialPos = glm::vec3(117.f, 100.f, 197.f);
            float scale = 1.f;
            glm::vec3 vert1 = glm::vec3(scale * positions[3 * face[0]] + initialPos.x, scale * positions[3 * face[0] + 1] + initialPos.y, scale * positions[3 * face[0] + 2] + initialPos.z);
            glm::vec3 vert2 = glm::vec3(scale * positions[3 * face[1]] + initialPos.x, scale * positions[3 * face[1] + 1] + initialPos.y, scale * positions[3 * face[1] + 2] + initialPos.z);
            glm::vec3 vert3 = glm::vec3(scale * positions[3 * face[2]] + initialPos.x, scale * positions[3 * face[2] + 1] + initialPos.y, scale * positions[3 * face[2] + 2] + initialPos.z);
            int minx = glm::floor(std::min({vert1.x, vert2.x, vert3.x}));
            int maxx = glm::ceil(std::max({vert1.x, vert2.x, vert3.x}));
            int miny = glm::floor(std::min({vert1.y, vert2.y, vert3.y}));
            int maxy = glm::ceil(std::max({vert1.y, vert2.y, vert3.y}));
            int minz = glm::floor(std::min({vert1.z, vert2.z, vert3.z}));
            int maxz = glm::ceil(std::max({vert1.z, vert2.z, vert3.z}));
            for (int z = minz; z <= maxz; z++){
                for (int y = miny; y <= maxy; y++){
                    for (int x = minx; x <= maxx; x++){
                        if (voxelTriangleIntersection(x, y, z, {vert1, vert2, vert3})){
                            Chunk *chunk = t->getChunkAt(x, z).get();
                            if (chunks.find(chunk) == chunks.end()){
                                chunks[chunk] = true;
                            }
                            t->setBlockAt(x, y, z, DUNE);
                        }else{
                            //std::cout << "no intersection" << std::endl;
                        }
                    }
                }
            }
        }
        for (auto const& x : chunks){
            Chunk* chunk = x.first;
            chunk->createVBOdata();
            chunk->sendVBOdata();
        }
    }
    else
    {
        //An error loading the OBJ occurred!
        std::cout << errors << std::endl;
    }
}

void MyGL::createVoxels(){
    createFromOBJ(":/objs/statue.obj", &m_terrain);
}

