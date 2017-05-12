#ifndef __VIEWMATRIX_H__
#define __VIEWMATRIX_H__

#include <glm/mat4x4.hpp>
glm::mat4 perspective_proj_xform(float camera_distance);
glm::mat4 screenspace_xform(int w, int h);

#endif // __VIEWMATRIX_H__
