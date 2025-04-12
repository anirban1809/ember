#ifndef __RENDERCONTEXT_H__
#define __RENDERCONTEXT_H__

#include "ShaderProgram.h"
#include "VertexLayout.h"
#include "Mesh.h"
#include <memory>
#include <vector>

/**
 * @class RenderContext
 * @brief Abstract base class for managing rendering operations.
 *
 * The RenderContext class provides an interface for managing rendering
 * operations in a graphics application. It defines methods for setting up
 * scenes, submitting rendering data, and managing global shaders. Derived
 * classes are expected to implement the virtual methods to provide
 * platform-specific rendering functionality.
 *
 * Key responsibilities of the RenderContext include:
 * - Managing the rendering pipeline.
 * - Submitting vertex and mesh data for rendering.
 * - Setting global shaders for consistent rendering configurations.
 * - Handling scene lifecycle operations such as BeginScene and EndScene.
 *
 * This class is designed to be extended by platform-specific implementations
 * (e.g., OpenGL, Vulkan, DirectX) to provide the actual rendering logic.
 */
class RenderContext {
   public:
    /**
     * @brief
     *
     * @return std::shared_ptr<RenderContext>
     */
    static std::shared_ptr<RenderContext> Create();

    /**
     * @brief Destroy the Render Context object
     *
     */
    virtual ~RenderContext() = default;

    /**
     * @brief
     *
     */
    virtual void BeginScene(std::vector<std::shared_ptr<ShaderProgram>>&) = 0;

    /**
     * @brief Sets the global shaders to be used in the rendering context.
     *
     * This method allows the user to specify a collection of shader programs
     * that will be globally applied within the rendering context. These shaders
     * are typically used for rendering operations that require consistent
     * shader configurations across multiple objects or scenes.
     *
     * @param shaders A vector of shared pointers to ShaderProgram objects
     *                representing the global shaders to be set.
     */
    virtual void SetGlobalShaders(
        const std::vector<std::shared_ptr<ShaderProgram>>&) = 0;

    /**
     * @brief Submits a set of vertices and indices for rendering with a
     * specified layout and transformation.
     *
     * @param vertices A vector of floating-point values representing the vertex
     * data.
     * @param indices A vector of unsigned 32-bit integers representing the
     * index data.
     * @param layout The layout of the vertex data, defining how the vertex
     * attributes are organized.
     * @param transform A 4x4 transformation matrix to apply to the vertices.
     */
    virtual void Submit(const std::vector<float>& vertices,
                        const std::vector<uint32>& indices,
                        const VertexLayout& layout,
                        const glm::mat4& transform) = 0;

    /**
     * @brief Submits a mesh to the render context for rendering.
     *
     * This function is responsible for adding the specified mesh to the
     * rendering pipeline. The implementation of this function should handle
     * the necessary steps to prepare the mesh for rendering, such as
     * uploading vertex data, setting up shaders, and managing draw calls.
     *
     * @param mesh The mesh object to be submitted for rendering.
     */
    virtual void SubmitMesh(const Mesh& mesh) = 0;

    /**
     * @brief Submits vertex array data for rendering using the specified shader
     * program.
     *
     * @param vao A shared pointer to the VertexArray object containing the
     * vertex data.
     * @param shader A shared pointer to the ShaderProgram to be used for
     * rendering.
     * @param vertexCount The number of vertices to be rendered.
     */
    virtual void SubmitArrays(std::shared_ptr<VertexArray> vao,
                              std::shared_ptr<ShaderProgram> shader,
                              uint32 vertexCount) = 0;

    /**
     * @brief Submits a vertex array object (VAO) and a shader program for
     * rendering.
     *
     * This function is responsible for queuing the provided vertex array and
     * shader program to be rendered. The actual rendering process is typically
     * handled later in the rendering pipeline.
     *
     * @param vao A shared pointer to the VertexArray object to be submitted.
     * @param shader A shared pointer to the ShaderProgram object to be used for
     * rendering.
     */
    virtual void Submit(std::shared_ptr<VertexArray> vao,
                        std::shared_ptr<ShaderProgram> shader) = 0;

    virtual void EndScene() = 0;

    virtual void Clear() = 0;
};
#endif  // __RENDERCONTEXT_H__