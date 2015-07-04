#include "GridInput.h"

#include "Grid.h"
#include "GridGraphics.h"
#include "GameLogger.h"
#include "Tile.h"
#include "Unit.h"

namespace Prismer {

GridInput::GridInput(weak_ptr<Grid> grid)
    : _grid(grid)
{
    auto l_grid = _grid.lock();

    if (l_grid->getTiles().empty()) {
        GameLogError << "GridInput() called with empty grid" << endLog;
    }
    else {
        auto first_tile = l_grid->getTile(l_grid->getWidth() / 2, l_grid->getHeight() / 2);

        //setActive(first_tile);
        setHovered(first_tile);
    }

    activate();
}

GridInput::~GridInput()
{
    deactivate();
}

void GridInput::activate()
{
    // bind keys
    auto input = Arya::Locator::getRoot().getInputSystem();

    keyBindings.push_back(input->bind("h", [this](bool down, const Arya::MousePos&) {
            if (down) setHovered(TileDirection::left); return down; }));
    keyBindings.push_back(input->bind("u", [this](bool down, const Arya::MousePos&) {
            if (down) setHovered(TileDirection::top_left); return down; }));
    keyBindings.push_back(input->bind("i", [this](bool down, const Arya::MousePos&) {
            if (down) setHovered(TileDirection::top_right); return down; }));
    keyBindings.push_back(input->bind("k", [this](bool down, const Arya::MousePos&) {
            if (down) setHovered(TileDirection::right); return down; }));
    keyBindings.push_back(input->bind("m", [this](bool down, const Arya::MousePos&) {
            if (down) setHovered(TileDirection::bottom_right); return down; }));
    keyBindings.push_back(input->bind("n", [this](bool down, const Arya::MousePos&) {
            if (down) setHovered(TileDirection::bottom_left); return down; }));
    keyBindings.push_back(input->bind("enter", [this](bool down, const Arya::MousePos&) {
            if (down) setActive(_hovered); return down; }));
    keyBindings.push_back(input->bind("v", [this](bool down, const Arya::MousePos&) {
                if (down) toggleVisible(_hovered); return down; }));

    if (input->controllerEnabled()) {
        keyBindings.push_back(input->bindControllerButton("a", [this](bool down) {
                if (down)
                    setActive(_hovered);
                return down;
            }));
    }

    // mouse movement
    keyBindings.push_back(input->bindMouseMove(
            [this](const Arya::MousePos& position, int dx, int dy) { 
                if(auto l_grid = _grid.lock()) {
                    // which tile is hovered over:
                    // world_x, world_y
                    auto gr = Arya::Locator::getRoot().getGraphics();
                    auto cam = gr->getCamera();
                    vec3 worldpos = cam->intersectViewRay(vec2(position.nX, position.nY), vec4(0.0f, 0.0f, 1.0f, 0.0f));

                    setHovered(l_grid->getEntity()->worldToBoard(worldpos.x, worldpos.y));
                }
                return false;
            }, Arya::CHAIN_LAST
            ));

    keyBindings.push_back(input->bindMouseButton(
            [this](Arya::MOUSEBUTTON btn, bool down, const Arya::MousePos&)
            {
                if (down && btn == Arya::MOUSEBUTTON_LEFT)
                {
                    setActive(_hovered);
                    return true;
                }
                return false;
            }));
}

void GridInput::deactivate()
{
    keyBindings.clear();
}

void GridInput::setActive(shared_ptr<Tile> tile) {
    if (_active) {
        _active->setActive(false);
        if (_active->getInfo()->hasUnit()) {
            _active->getInfo()->getUnit()->deactivate();
        }
    }

    _active = tile;

    if (!_active)
        return;

    _active->setActive(true);

    if (_active->getInfo()->hasUnit())
        _active->getInfo()->getUnit()->activate(shared_from_this());
}

void GridInput::toggleVisible(shared_ptr<Tile> tile)
{
    if (!tile)
    {
        return;
    }
    tile->setVisible(!tile->getInfo()->isVisible());
}

void GridInput::setHovered(shared_ptr<Tile> tile) {
    if (!tile)
        return;

    if (_hovered)
        _hovered->setHovered(false);

    _hovered = tile;
    _hovered->setHovered(true);
}

void GridInput::setHovered(TileDirection dir) {
    if (!_hovered)
        return;

    auto nb = _hovered->getNeighbor(dir);
    if (nb)
        setHovered(nb);
}

} // namespace Prismer
