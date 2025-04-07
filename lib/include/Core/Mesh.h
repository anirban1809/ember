#ifndef __MESH_H__
#define __MESH_H__

#include "Types.h"
#include "Material.h"
#include "VertexLayout.h"
#include <vector>
#include <memory>
#include "../../../vendor/glm/glm.hpp"

struct Mesh {
    std::vector<float> vertices;
    std::vector<uint32> indices;
    VertexLayout layout;
    glm::mat4 transform;
    std::shared_ptr<Material> material;

    Mesh() = default;

    Mesh(const std::vector<float>& vertices, const std::vector<uint32>& indices,
         const VertexLayout& layout, const glm::mat4& transform,
         std::shared_ptr<Material> material)
        : vertices(vertices),
          indices(indices),
          layout(layout),
          transform(transform),
          material(material) {}
};
#endif  // __MESH_H__