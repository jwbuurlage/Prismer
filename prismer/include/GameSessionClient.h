//GameSessionClient is the main (root) class for all ingame related things
#pragma once

#include "GameSession.h"
#include <vector>
#include <memory>

namespace Prismer {

using std::unique_ptr;
using std::make_shared;
using std::vector;

class GameSessionInput;
class GameCamera;
class GridEntity;
class GridInput;
class UnitInfo;

class GameSessionClient : public GameSession
{
    public:
        GameSessionClient();
        ~GameSessionClient();

        //! See GameSession.h
        bool isServer() const override {
            return false;
        }

        //! Initialize the session
        //! Registers keybindings and event handlers
        bool init();

        //! Update all game engine related things like camera movement
        void update(float elapsedTime); //in seconds

        //! Update all units and so on
        //! This timing is the one that should be properly
        //! synchronized with the server
        void updateGameLogic(int elapsedTime); //in ms

        //! Creates a unit
        virtual shared_ptr<Unit> createUnit(int x, int y) override;

     private:
        unique_ptr<GameSessionInput> input;
        vector<int> clients;

        shared_ptr<GridEntity> _grid_entity;
        shared_ptr<GridInput> _grid_input;
        shared_ptr<GameCamera> _camera;

        float total_time = 0.0f;
};

} // namespace Prismer
