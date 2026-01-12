#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include <glm/glm.hpp>

namespace Engine {

    class SubTexture2D {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
        
        const Ref<Texture2D>& GetTexture() const { return m_Texture; }
        const glm::vec2* GetTexCoords() const { return m_TexCoords; }
        
        static Ref<SubTexture2D> CreateFromCoords(
            const Ref<Texture2D>& texture,
            const glm::vec2& coords,        // Grid coordinates (e.g., {2, 3})
            const glm::vec2& cellSize,      // Size of each cell in pixels (e.g., {32, 32})
            const glm::vec2& spriteSize = {1, 1} // Number of cells for this sprite (default 1x1)
        );
        
    private:
        Ref<Texture2D> m_Texture;
        glm::vec2 m_TexCoords[4];
    };

}

