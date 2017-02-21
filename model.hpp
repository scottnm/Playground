#ifndef __MODEL_H__
#define __MODEL_H__

#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

typedef struct
{
    std::vector<glm::vec3> verts;
    std::vector<glm::ivec3> faces;
} model;

std::unique_ptr<model> load_model(const std::string& file);
void test_print_model(const model& m);

#endif //__MODEL_H__
