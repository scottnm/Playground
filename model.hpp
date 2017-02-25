#ifndef __MODEL_H__
#define __MODEL_H__

#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

typedef struct
{
    glm::ivec3 vi  = {-1, -1, -1}; // vertex indices
    glm::ivec3 vti = {-1, -1, -1}; // vertex texture indices
    glm::ivec3 vni = {-1, -1, -1}; // vertex normal indices
}
face;

typedef struct
{
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> vert_norms;
    std::vector<face> faces;
} model;

std::unique_ptr<model> load_model(const std::string& file);
void test_print_model(const model& m);

#endif //__MODEL_H__
