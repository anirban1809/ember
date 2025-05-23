#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Core/Material.h"
#include "Core/Mesh.h"
#include "Core/Types.h"
#include "glm/fwd.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <exception>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

struct Vertex {
    aiVector3D position;
    aiVector3D normal;
    aiVector3D texCoords;
    aiVector3D tangent;
    aiVector3D bitangent;

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal &&
               texCoords == other.texCoords && tangent == other.tangent &&
               bitangent == other.bitangent;
    }
};

struct VertexHasher {
    std::size_t operator()(const Vertex& v) const {
        auto h1 = std::hash<float>{}(v.position.x) ^
                  std::hash<float>{}(v.position.y) ^
                  std::hash<float>{}(v.position.z);
        auto h2 = std::hash<float>{}(v.normal.x) ^
                  std::hash<float>{}(v.normal.y) ^
                  std::hash<float>{}(v.normal.z);
        auto h3 = std::hash<float>{}(v.texCoords.x) ^
                  std::hash<float>{}(v.texCoords.y);
        return h1 ^ h2 ^ h3;
    }
};

class Importer {
   public:
    static void LoadFile(const std::string& path,
                         std::vector<float>& vertexBuffer,
                         std::vector<uint32>& indexBuffer) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            path, aiProcess_Triangulate | aiProcess_GenNormals |
                      aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode) {
            std::cerr << "Assimp error: " << importer.GetErrorString()
                      << std::endl;
        }

        std::unordered_map<Vertex, unsigned int, VertexHasher> uniqueVertices;

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];

            for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
                const aiFace& face = mesh->mFaces[i];
                if (face.mNumIndices != 3) continue;  // skip non-triangle faces

                for (unsigned int j = 0; j < 3; ++j) {
                    unsigned int idx = face.mIndices[j];

                    Vertex v;
                    v.position = mesh->mVertices[idx];
                    v.normal = mesh->HasNormals() ? mesh->mNormals[idx]
                                                  : aiVector3D(0, 0, 0);
                    v.texCoords = mesh->HasTextureCoords(0)
                                      ? mesh->mTextureCoords[0][idx]
                                      : aiVector3D(0, 0, 0);

                    v.tangent = mesh->HasTangentsAndBitangents()
                                    ? mesh->mTangents[idx]
                                    : aiVector3D(1, 0, 0);
                    v.bitangent = mesh->HasTangentsAndBitangents()
                                      ? mesh->mBitangents[idx]
                                      : aiVector3D(0, 1, 0);

                    if (uniqueVertices.count(v) == 0) {
                        uniqueVertices[v] =
                            static_cast<unsigned int>(vertexBuffer.size() / 14);

                        vertexBuffer.push_back(v.position.x);
                        vertexBuffer.push_back(v.position.y);
                        vertexBuffer.push_back(v.position.z);

                        vertexBuffer.push_back(v.texCoords.x);
                        vertexBuffer.push_back(v.texCoords.y);

                        vertexBuffer.push_back(v.normal.x);
                        vertexBuffer.push_back(v.normal.y);
                        vertexBuffer.push_back(v.normal.z);

                        // add tangents
                        vertexBuffer.push_back(v.tangent.x);
                        vertexBuffer.push_back(v.tangent.y);
                        vertexBuffer.push_back(v.tangent.z);

                        // add bitangents
                        vertexBuffer.push_back(v.bitangent.x);
                        vertexBuffer.push_back(v.bitangent.y);
                        vertexBuffer.push_back(v.bitangent.z);
                    }

                    indexBuffer.push_back(uniqueVertices[v]);
                }
            }
        }
    }

    static Mesh Load(const std::string& path,
                     std::shared_ptr<Material> material) {
        std::vector<float> vertexBuffer;  // [x, y, z, u, v, nx, ny, nz]
        std::vector<uint32> indexBuffer;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            path,
            aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode) {
            std::cerr << "Assimp error: " << importer.GetErrorString()
                      << std::endl;
        }

        std::unordered_map<Vertex, unsigned int, VertexHasher> uniqueVertices;

        for (unsigned int m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];

            for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
                const aiFace& face = mesh->mFaces[i];
                if (face.mNumIndices != 3) continue;  // skip non-triangle faces

                for (unsigned int j = 0; j < 3; ++j) {
                    unsigned int idx = face.mIndices[j];

                    Vertex v;
                    v.position = mesh->mVertices[idx];
                    v.normal = mesh->HasNormals() ? mesh->mNormals[idx]
                                                  : aiVector3D(0, 0, 0);
                    v.texCoords = mesh->HasTextureCoords(0)
                                      ? mesh->mTextureCoords[0][idx]
                                      : aiVector3D(0, 0, 0);

                    if (uniqueVertices.count(v) == 0) {
                        uniqueVertices[v] =
                            static_cast<unsigned int>(vertexBuffer.size() / 8);

                        vertexBuffer.push_back(v.position.x);
                        vertexBuffer.push_back(v.position.y);
                        vertexBuffer.push_back(v.position.z);

                        vertexBuffer.push_back(v.texCoords.x);
                        vertexBuffer.push_back(v.texCoords.y);

                        vertexBuffer.push_back(v.normal.x);
                        vertexBuffer.push_back(v.normal.y);
                        vertexBuffer.push_back(v.normal.z);

                        // zero out the tangents
                        vertexBuffer.push_back(0.0f);
                        vertexBuffer.push_back(0.0f);
                        vertexBuffer.push_back(0.0f);

                        // zero out the bitangents
                        vertexBuffer.push_back(0.0f);
                        vertexBuffer.push_back(0.0f);
                        vertexBuffer.push_back(0.0f);
                    }

                    indexBuffer.push_back(uniqueVertices[v]);
                }
            }
        }

        return Mesh(vertexBuffer, indexBuffer,
                    VertexLayout{
                        {ShaderType::Float3, 0 * sizeof(float), 0},  // position
                        {ShaderType::Float2, 3 * sizeof(float), 1},  // texcoord
                        {ShaderType::Float3, 5 * sizeof(float), 2},  // normal
                    },
                    glm::mat4(1.0f), material);
    }
};

#endif  // __IMPORTER_H__