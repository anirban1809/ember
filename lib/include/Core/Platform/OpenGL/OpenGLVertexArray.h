#ifndef __OPENGLVERTEXARRAY_H__
#define __OPENGLVERTEXARRAY_H__

#include "../../VertexArray.h"
#include "../../VertexLayout.h"

class OpenGLVertexArray : public VertexArray {
   public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
    void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

    const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers()
        const override;
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

    void SetVertexLayout(const VertexLayout& layout);

   private:
    uint32 rendererID;
    uint32 attributeIndex = 0;

    std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
    std::shared_ptr<IndexBuffer> indexBuffer;
};
#endif  // __OPENGLVERTEXARRAY_H__