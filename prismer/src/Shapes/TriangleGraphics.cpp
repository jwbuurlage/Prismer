#include <Arya.h>

#include "Unit.h"
#include "GridGraphics.h"
#include "Shapes/TriangleGraphics.h"

namespace Prismer {

TriangleEntity::TriangleEntity(shared_ptr<Unit> unit,
        shared_ptr<GridEntity> grid_entity)
    : ShapeEntity(unit, grid_entity)
{
    _entity = Arya::Entity::create();

    _current_position = vec3(_grid_entity->boardToWorld(_unit->getX(), _unit->getY()), 1.0f);
    _entity->setPosition(_current_position);

    auto& root = Arya::Locator::getRoot();
    auto trianglePrototype = root.getModelManager()->getModel("thicktriangle");
    triangle = trianglePrototype->clone();
    triangle->setMaterial(root.getMaterialManager()->createMaterial(vec4(0.6f, 0.6f, 0.6f, 0.5f)));

    _entity->setGraphics(triangle);
    _entity->getGraphics()->setScale(vec3(4.0f, 5.0f, 2.0f));

    _current_yaw = 0.0f;
    _entity->setYaw(0.0f);
}

void TriangleEntity::updateState()
{
    // change colors or whatever
}

void TriangleEntity::setTintColor(vec3 tintColor)
{
    GameLogInfo << "updateee" << endLog;
    auto& root = Arya::Locator::getRoot();
    UnitEntity::setTintColor(tintColor);
    triangle->setMaterial(root.getMaterialManager()->createMaterial(vec4(tintColor, 1.0)));
}

void TriangleEntity::update(float elapsed_time, float)
{
    static float close_enough = 0.2f;
    static float fly_height = 15.0f;

    static float land_take_off_speed = 20.0f;
    static float rotate_speed = 3.0f;

    // check where unit is, and move towards that
    auto world_pos = _grid_entity->boardToWorld(_unit->getX(), _unit->getY());
    auto diff = vec2(world_pos) - vec2(_current_position.x, _current_position.y);

    if (glm::length(diff) > close_enough) {
        // 1) take off vertically
        if (fly_height - _current_position.z
                > close_enough) {
            _current_position.z += elapsed_time * land_take_off_speed;
        } else {
            _current_position.z = fly_height;

            // 2) rotate to goal
            auto cos_alpha = glm::dot(vec2(0.0f, 1.0f),
                glm::normalize(diff));
            auto sin_alpha = -glm::normalize(diff).x;

            auto alpha = glm::atan(sin_alpha, cos_alpha);

            //_current_yaw = alpha;
            //GameLogInfo << alpha << endLog;

            if (glm::abs(alpha - _current_yaw) 
                > close_enough) {
                // is + or - closer
                // two distances
                // DISTANCE (SMALLESR):    SIGN
                // _current_yaw - alpha    +
                // alpha - _current_yaw    -
                //
                // - pi -------- 0 -----|----- pi = - pi ---|------  0

                // ignoring -pi, pi duality:
                auto sign = 1.0f;
                if (glm::abs(_current_yaw - alpha) > M_PI) {
                    if (_current_yaw < alpha)
                        sign = -1.0f;
                    else
                        sign = 1.0f;
                }
                else {
                    if (_current_yaw > alpha)
                        sign = -1.0f;
                    else
                        sign = 1.0f;
                }

                _current_yaw += sign * rotate_speed * elapsed_time;

                if (_current_yaw > M_PI)
                    _current_yaw -= 2.0f * M_PI;
                if (_current_yaw < -M_PI)
                    _current_yaw += 2.0f * M_PI;

            } else {
                // 3) fly to goal
                _current_position.x += 1.0f * glm::normalize(diff).x * _unit->getSpeed() * _grid_entity->getScale() * elapsed_time;
                _current_position.y += 1.0f * glm::normalize(diff).y * _unit->getSpeed() * _grid_entity->getScale() * elapsed_time;
            }
        }
    } else {
        // 4) land
        if (glm::abs(_current_position.z - 1.0f) 
                > close_enough) {
            _current_position.z -= elapsed_time * land_take_off_speed;
        } 
    }

    _entity->setPosition(_current_position);
    _entity->setYaw(_current_yaw);

    return;
}

} // namespace Prismer
