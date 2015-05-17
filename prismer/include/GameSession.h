//GameSession is a base class for GameSessionClient and GameSessionServer
//It serves as a Unit and Faction factory class
//Units and Factions are created with createUnit and createFaction
//and they can be deleted by just calling delete on them.
//Every Unit and Faction has a pointer to this class
//so that it can de-register itself at deconstruction
#pragma once

#include <map>

class Unit;
class Faction;
class Map;

class GameSession
{
    public:
        GameSession();
        virtual ~GameSession();

        //! Check if this session is running on a server
        //! If a Unit performs an action on
        //!  - the server: it should send event packets
        //!  - the client: it should do nothing
        virtual bool isServer() const = 0;

        //! Returns the time since the start of the game in milliseconds
        //! When an event arrives with a certain timestamp, this timer
        //! should be used to correctly predict its curent value
        int getGameTime() const { return gameTimer; }

        //! Get the Map object
        Map* getMap() const { return map; }

        //! Creates a unit
        Unit* createUnit(int id, int type);

        //! Find a unit. ALWAYS check the return value, 0 if not found.
        Unit* getUnitById(int id);

        //! Creates a faction
        Faction* createFaction(int id);
        
        //! Find a faction. ALWAYS check the return value, 0 if not found.
        Faction* getFactionById(int id);
    protected:
        int gameTimer;
        Map* map;

        const std::map<int,Unit*>& getUnitMap() const { return unitMap; }
        const std::map<int,Faction*>& getFactionMap() const { return factionMap; }
    private:
        friend class Unit;
        friend class Faction;
        //We have to use std:: here because we also have a variable called map
        std::map<int,Unit*> unitMap;
        std::map<int,Faction*> factionMap;
        void destroyUnit(int id); //called in Unit deconstructor
        void destroyFaction(int id); //called in Faction deconstructor
};
