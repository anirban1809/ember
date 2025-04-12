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
     * @brief Creates and returns a shared pointer to a new RenderContext
     * instance.
     *
     * This static method is responsible for constructing a new RenderContext
     * object and returning it as a shared pointer. The exact implementation
     * details of the creation process depend on the underlying rendering
     * system and configuration.
     *
     * @return std::shared_ptr<RenderContext> A shared pointer to the newly
     * created RenderContext instance.
     */
    static std::shared_ptr<RenderContext> Create();

    /**
     * @brief Virtual destructor for the RenderContext class.
     *
     * Ensures proper cleanup of resources in derived classes when a
     * RenderContext object is destroyed. Declared as default to use the
     * compiler-generated implementation.
     */
    virtual ~RenderContext() = default;

    /**
     * @brief Begins a new rendering scene.
     *
     * This function initializes the rendering process for a new scene. It is
     * expected to be called before any rendering commands are issued. The
     * provided shaders will be used during the rendering of the scene.
     *
     * @param shaders A vector of shared pointers to ShaderProgram objects
     *                that will be used for rendering the scene.
     */
    virtual void BeginScene(
        std::vector<std::shared_ptr<ShaderProgram>>& shaders) = 0;

    /**
     * @brief Begins the rendering of a new scene.
     *
     * This method is responsible for initializing the rendering context
     * for a new scene. It prepares the necessary resources and state
     * required for rendering.
     *
     * @param shaders A vector of shared pointers to ShaderProgram objects
     *                that will be used during the rendering of the scene.
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

    /**
     * @brief Ends the current rendering scene.
     *
     * This function finalizes the rendering operations for the current scene.
     * It should be called after all rendering commands for the scene have been
     * issued. Implementations may perform cleanup or present the rendered frame
     * to the screen.
     */
    virtual void EndScene() = 0;

    /**
     * @brief Clears the current render context.
     *
     * This function is responsible for resetting or clearing the rendering
     * state, such as clearing the screen or buffer, to prepare for the next
     * rendering operation. It must be implemented by derived classes.
     */
    virtual void Clear() = 0;
};
#endif  // __RENDERCONTEXT_H__