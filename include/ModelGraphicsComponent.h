#pragma once
#include "AnimationBase.h"
#include "GraphicsComponent.h"

namespace Arya
{
    using std::shared_ptr;

    class Model;

    class ModelGraphicsComponent : public GraphicsComponent
    {
        public:
            ModelGraphicsComponent();
            ~ModelGraphicsComponent();
            RenderType getRenderType() const override { return TYPE_MODEL; }

            Model* getModel() const { return model.get(); }

            void setScale(float _scale) { scale = _scale; }
            float getScale() const { return scale; }
            AnimationState* getAnimationState() const override { return animState.get(); }

            void setAnimation(const char* name) override;
            void updateAnimation(float elapsedTime) override;
            void setAnimationTime(float time) override;

            //! setModel releases the old model and animationstate.
            //! If the new model is nonzero,
            //! it creates a new AnimationState object
            void setModel(shared_ptr<Model> model);

        private:
            shared_ptr<Model> model;
            unique_ptr<AnimationState> animState;
            float scale;
    };

}
