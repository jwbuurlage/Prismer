#include "Tile.h"
#include "Grid.h"
#include "GridInput.h"
#include "GameLogger.h"

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
            if (down) setActive(hovered); });
}

void GridInput::deactivate()
{
    // unbind keys
}

void GridInput::setActive(shared_ptr<Tile> tile) {
    if (!tile)
        return;

    if (active)
        active->setActive(false);

    active = tile;
    active->setActive(true);
}

void GridInput::setHovered(shared_ptr<Tile> tile) {
    if (!tile)
        return;

    if (hovered)
        hovered->setHovered(false);

    hovered = tile;
    hovered->setHovered(true);
}

void GridInput::setHovered(TileDirection dir) {
    if (!hovered)
        return;

    auto nb = hovered->getNeighbor(dir);
    if (nb)
        setHovered(nb);
}

void GridInput::setActive(TileDirection dir) {
    if (!hovered)
        return;

    auto nb = hovered->getNeighbor(dir);
    if (nb)
        setHovered(nb);
}

} // namespace Prismer
