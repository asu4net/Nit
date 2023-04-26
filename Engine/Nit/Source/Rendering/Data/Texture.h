#pragma once

namespace Nit
{
    class Texture
    {
    public:
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetTextureID() const = 0;
        virtual void Bind(uint32_t slot = 0) const = 0;
    };
}