#pragma once
#include "GraphicsComponent.h"

namespace Arya
{
    using std::shared_ptr;

    class BillboardGraphicsComponent : public GraphicsComponent
    {
        public:
            BillboardGraphicsComponent(){};
            ~BillboardGraphicsComponent(){};
            RenderType getRenderType() const override { return TYPE_BILLBOARD; }

            void setScale(float _scale) override { scale = _scale; }
            float getScale() const override { return scale; }

            virtual void setScreenOffset(const vec2& offset) override { screenOffset = offset; }
            virtual vec2 getScreenOffset() const override { return screenOffset; }

            Material* getMaterial() const { return material.get(); }
            void setMaterial(shared_ptr<Material> mat) { material = mat; }
        private:
            shared_ptr<Material> material;
            float scale;
            vec2 screenOffset;
    };

}
