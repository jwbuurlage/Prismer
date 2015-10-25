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


            void setScale(const vec3& _scale) override { scale = _scale; }
            vec3 getScale() const override { return scale; }

            void setAnimation(const char* name) override;
            void updateAnimation(float elapsedTime) override;
            void setAnimationTime(float time) override;

            //! setModel releases the old model and animationstate.
            //! If the new model is nonzero,
            //! it creates a new AnimationState object
            void setModel(shared_ptr<Model> model);

            Model* getModel() const { return model.get(); }
            AnimationState* getAnimationState() const { return animState.get(); }
        private:
            shared_ptr<Model> model;
            unique_ptr<AnimationState> animState;
            vec3 scale;
    };

}
