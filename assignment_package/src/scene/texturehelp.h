#ifndef TEXTUREHELP_H
#define TEXTUREHELP_H

#include <unordered_map>
#include <cstddef>
#include "glm_includes.h"

//using namespace std;

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.
enum BlockType : unsigned char
{
    EMPTY, GRASS, DIRT, STONE, WATER, SNOW, LAVA, SAND, BEDROCK, DUNE, HEAD, BODY, ARM, LEG
};

// The six cardinal directions in 3D space
enum Direction : unsigned char
{
    XPOS, XNEG, YPOS, YNEG, ZPOS, ZNEG
};

// Lets us use any enum class as the key of a
// std::unordered_map
struct EnumHash {
    template <typename T>
    size_t operator()(T t) const {
        return static_cast<size_t>(t);
    }
};

#define BLK_UVX * 0.0625f
#define BLK_UVY * 0.0625f
#define BLK_UV 0.0625f
const static std::unordered_map<BlockType, std::unordered_map<Direction, glm::vec2, EnumHash>, EnumHash> blockFaceUVs {

    {GRASS, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(3.f BLK_UVX, 15.f BLK_UVY)},
                                                                {XNEG, glm::vec2(3.f BLK_UVX, 15.f BLK_UVY)},
                                                                {YPOS, glm::vec2(8.f BLK_UVX, 13.f BLK_UVY)},
                                                                {YNEG, glm::vec2(2.f BLK_UVX, 15.f BLK_UVY)},
                                                                {ZPOS, glm::vec2(3.f BLK_UVX, 15.f BLK_UVY)},
                                                                {ZNEG, glm::vec2(3.f BLK_UVX, 15.f BLK_UVY)}}},
    {STONE, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(1.f BLK_UVX, 15.f BLK_UVY)},
                                                               {XNEG, glm::vec2(1.f BLK_UVX, 15.f BLK_UVY)},
                                                               {YPOS, glm::vec2(1.f BLK_UVX, 15.f BLK_UVY)},
                                                               {YNEG, glm::vec2(1.f BLK_UVX, 15.f BLK_UVY)},
                                                               {ZPOS, glm::vec2(1.f BLK_UVX, 15.f BLK_UVY)},
                                                               {ZNEG, glm::vec2(1.f BLK_UVX, 15.f BLK_UVY)}}},
    {DIRT, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(2.f BLK_UVX, 15.f BLK_UVY)},
                                                               {XNEG, glm::vec2(2.f BLK_UVX, 15.f BLK_UVY)},
                                                               {YPOS, glm::vec2(2.f BLK_UVX, 15.f BLK_UVY)},
                                                               {YNEG, glm::vec2(2.f BLK_UVX, 15.f BLK_UVY)},
                                                               {ZPOS, glm::vec2(2.f BLK_UVX, 15.f BLK_UVY)},
                                                               {ZNEG, glm::vec2(2.f BLK_UVX, 15.f BLK_UVY)}}},
    {SNOW, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(2.f BLK_UVX, 11.f BLK_UVY)},
                                                               {XNEG, glm::vec2(2.f BLK_UVX, 11.f BLK_UVY)},
                                                               {YPOS, glm::vec2(2.f BLK_UVX, 11.f BLK_UVY)},
                                                               {YNEG, glm::vec2(2.f BLK_UVX, 11.f BLK_UVY)},
                                                               {ZPOS, glm::vec2(2.f BLK_UVX, 11.f BLK_UVY)},
                                                               {ZNEG, glm::vec2(2.f BLK_UVX, 11.f BLK_UVY)}}},
    {LAVA, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(13.f BLK_UVX, 1.f BLK_UVY)},
                                                              {XNEG, glm::vec2(13.f BLK_UVX, 1.f BLK_UVY)},
                                                              {YPOS, glm::vec2(13.f BLK_UVX, 1.f BLK_UVY)},
                                                              {YNEG, glm::vec2(13.f BLK_UVX, 1.f BLK_UVY)},
                                                              {ZPOS, glm::vec2(13.f BLK_UVX, 1.f BLK_UVY)},
                                                              {ZNEG, glm::vec2(13.f BLK_UVX, 1.f BLK_UVY)}}},
    {WATER, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(13.f BLK_UVX, 3.f BLK_UVY)},
                                                              {XNEG, glm::vec2(13.f BLK_UVX, 3.f BLK_UVY)},
                                                              {YPOS, glm::vec2(13.f BLK_UVX, 3.f BLK_UVY)},
                                                              {YNEG, glm::vec2(13.f BLK_UVX, 3.f BLK_UVY)},
                                                              {ZPOS, glm::vec2(13.f BLK_UVX, 3.f BLK_UVY)},
                                                              {ZNEG, glm::vec2(13.f BLK_UVX, 3.f BLK_UVY)}}},
    {BEDROCK, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(1.f BLK_UVX, 14.f BLK_UVY)},
                                                               {XNEG, glm::vec2(1.f BLK_UVX, 14.f BLK_UVY)},
                                                               {YPOS, glm::vec2(1.f BLK_UVX, 14.f BLK_UVY)},
                                                               {YNEG, glm::vec2(1.f BLK_UVX, 14.f BLK_UVY)},
                                                               {ZPOS, glm::vec2(1.f BLK_UVX, 14.f BLK_UVY)},
                                                               {ZNEG, glm::vec2(1.f BLK_UVX, 14.f BLK_UVY)}}},
    {SAND, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(2.f BLK_UVX, 14.f BLK_UVY)},
                                                              {XNEG, glm::vec2(2.f BLK_UVX, 14.f BLK_UVY)},
                                                              {YPOS, glm::vec2(2.f BLK_UVX, 14.f BLK_UVY)},
                                                              {YNEG, glm::vec2(2.f BLK_UVX, 14.f BLK_UVY)},
                                                              {ZPOS, glm::vec2(2.f BLK_UVX, 14.f BLK_UVY)},
                                                              {ZNEG, glm::vec2(2.f BLK_UVX, 14.f BLK_UVY)}}},
    {DUNE, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(1.f BLK_UVX, 7.f BLK_UVY)},
                                                              {XNEG, glm::vec2(1.f BLK_UVX, 7.f BLK_UVY)},
                                                              {YPOS, glm::vec2(1.f BLK_UVX, 7.f BLK_UVY)},
                                                              {YNEG, glm::vec2(1.f BLK_UVX, 7.f BLK_UVY)},
                                                              {ZPOS, glm::vec2(1.f BLK_UVX, 7.f BLK_UVY)},
                                                              {ZNEG, glm::vec2(1.f BLK_UVX, 7.f BLK_UVY)}}},
    {HEAD, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(10.f BLK_UVX, 4.f BLK_UVY)},
                                                              {XNEG, glm::vec2(8.f BLK_UVX, 4.f BLK_UVY)},
                                                              {YPOS, glm::vec2(9.f BLK_UVX, 5.f BLK_UVY)},
                                                              {YNEG, glm::vec2(9.f BLK_UVX, 6.f BLK_UVY)},
                                                              {ZPOS, glm::vec2(11.f BLK_UVX, 4.f BLK_UVY)},
                                                              {ZNEG, glm::vec2(9.f BLK_UVX, 4.f BLK_UVY)}}},
    {BODY, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(8.f BLK_UVX, 3.f BLK_UVY)},
                                                              {XNEG, glm::vec2(8.f BLK_UVX, 3.f BLK_UVY)},
                                                              {YPOS, glm::vec2(11.f BLK_UVX, 3.f BLK_UVY)},
                                                              {YNEG, glm::vec2(12.f BLK_UVX, 3.f BLK_UVY)},
                                                              {ZPOS, glm::vec2(10.f BLK_UVX, 3.f BLK_UVY)},
                                                              {ZNEG, glm::vec2(9.f BLK_UVX, 3.f BLK_UVY)}}},
    {ARM, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(12.f BLK_UVX, 4.f BLK_UVY)},
                                                              {XNEG, glm::vec2(12.f BLK_UVX, 4.f BLK_UVY)},
                                                              {YPOS, glm::vec2(13.f BLK_UVX, 4.f BLK_UVY)},
                                                              {YNEG, glm::vec2(14.f BLK_UVX, 4.f BLK_UVY)},
                                                              {ZPOS, glm::vec2(12.f BLK_UVX, 4.f BLK_UVY)},
                                                              {ZNEG, glm::vec2(12.f BLK_UVX, 4.f BLK_UVY)}}},
    {LEG, std::unordered_map<Direction, glm::vec2, EnumHash> {{XPOS, glm::vec2(7.f BLK_UVX, 2.f BLK_UVY)},
                                                             {XNEG, glm::vec2(9.f BLK_UVX, 2.f BLK_UVY)},
                                                             {YPOS, glm::vec2(11.f BLK_UVX, 2.f BLK_UVY)},
                                                             {YNEG, glm::vec2(11.f BLK_UVX, 2.f BLK_UVY)},
                                                             {ZPOS, glm::vec2(10.f BLK_UVX, 2.f BLK_UVY)},
                                                             {ZNEG, glm::vec2(8.f BLK_UVX, 2.f BLK_UVY)}}}


};

#endif // TEXTUREHELP_H
