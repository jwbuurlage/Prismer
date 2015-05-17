//GameSessionClient is the main (root) class for all ingame related things
#pragma once
#include "GameSession.h"

#include <vector>
using std::vector;

class GameSessionInput;
class Faction;
struct CellList;

class GameSessionClient : public GameSession
{
    public:
        GameSessionClient();
        ~GameSessionClient();

        //! See GameSession.h
        bool isServer() const override { return false; }

        //! Initialize the session
        //! Registers keybindings and event handlers
        bool init();

        //! Completely rebuilds cell lists
        void rebuildCellList();

        //! Return the faction of the local player
        //! TODO: What if the player could control multiple factions
        Faction* getLocalFaction() const { return localFaction; } ;

        //! Update all game engine related things like camera movement
        void update(float elapsedTime); //in seconds

        //! Update all units and so on
        //! This timing is the one that should be properly
        //! synchronized with the server
        void updateGameLogic(int elapsedTime); //in ms

        //TODO
        //void handleEvent(Packet& packet);

     private:
        GameSessionInput* input;
        Faction* localFaction;
        vector<int> clients;

        CellList* unitCells;
};
