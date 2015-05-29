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
    input->bind("h", [this](bool down) {
            if (down) setHovered(TileDirection::left); });
    input->bind("u", [this](bool down) {
            if (down) setHovered(TileDirection::top_left); });
    input->bind("i", [this](bool down) {
            if (down) setHovered(TileDirection::top_right); });
    input->bind("k", [this](bool down) {
            if (down) setHovered(TileDirection::right); });
    input->bind("m", [this](bool down) {
            if (down) setHovered(TileDirection::bottom_right); });
    input->bind("n", [this](bool down) {
            if (down) setHovered(TileDirection::bottom_left); });
    input->bind("enter", [this](bool down) {
            if (down) setActive(_hovered); });
    input->bind("v", [this](bool down) {
            if (down) toggleVisible(_hovered); });

    //DEBUG---
    input->bind("j", [this](bool down) {
            // create unit?
            if (down && _hovered!=nullptr)
            {
                auto l_grid = _grid.lock();
                GameLogInfo << "Vision:" << endLog;
                auto visionlist = l_grid->getVision(_hovered);
                GameLogDebug << "The list has " << visionlist.size() << " elements." << endLog;
                for(auto t:visionlist)
                {
                    GameLogInfo << "Tile x:" << t->getX() << " Tile y:" << t->getY() << endLog;
                }
                GameLogInfo << endLog;
            }
        });


    // mouse movement
    input->bindMouseMove(
            [this](int x, int y, int dx, int dy) { 
                if(auto l_grid = _grid.lock()) {
                    // which tile is hovered over:
                    // world_x, world_y
                    auto gr = Arya::Locator::getRoot().getGraphics();
                    auto cam = gr->getCamera();
                    vec3 worldpos = cam->intersectViewRay(gr->normalizeMouseCoordinates(x,y), vec4(0.0f, 0.0f, 1.0f, 0.0f));

                    setHovered(l_grid->getEntity()->worldToBoard(worldpos.x, worldpos.y));
                }
            });

    input->bindMouseButton(
            [this](Arya::MOUSEBUTTON btn, bool down, int, int)
            {
                if (down && btn == Arya::MOUSEBUTTON_LEFT)
                    setActive(_hovered);
            });
}

void GridInput::deactivate()
{
    // unbind keys
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
