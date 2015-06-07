#pragma once
#include "GraphicsComponent.h"

namespace Arya
{
    using std::shared_ptr;

    class BillboardGraphicsComponent : public GraphicsComponent
    {
        public:
            BillboardGraphicsComponent(){ screenOffset = vec2(0.0f); screenSize = vec2(1.0f); };
            ~BillboardGraphicsComponent(){};
            RenderType getRenderType() const override { return TYPE_BILLBOARD; }

            void setScale(const vec3& _scale) override { scale = vec2(_scale); }
            vec3 getScale() const override { return vec3(scale.x, scale.y, 1.0f); }

            virtual void setScreenOffset(const vec2& offset) override { screenOffset = offset; }
            virtual vec2 getScreenOffset() const override { return screenOffset; }

            virtual void setScreenSize(const vec2& size) { screenSize = size; }
            virtual vec2 getScreenSize() const { return screenSize; }

            Material* getMaterial() const { return material.get(); }
            void setMaterial(shared_ptr<Material> mat) { material = mat; }
        private:
            shared_ptr<Material> material;
            vec2 scale;
            vec2 screenOffset;
            vec2 screenSize;
    };

}
