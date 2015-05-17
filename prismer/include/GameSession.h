// GameSession is a base class for GameSessionClient and GameSessionServer
// It serves as a Unit and Faction factory class
// Units and Factions are created with createUnit and createFaction
// and they can be deleted by just calling delete on them.
// Every Unit and Faction has a pointer to this class
// so that it can de-register itself at deconstruction
#pragma once

#include <map>
#include <memory>

namespace Prismer {

class Unit;
class UnitInfo;

class GameSession
    : public std::enable_shared_from_this<GameSession>
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
        int getGameTime() const {
            return gameTimer;
        }

        //! Creates a unit
        Unit* createUnit(UnitInfo info);

        /** Generate a unique id */
        int generateId() const { return 1; }

        /** Return unit by id */
        Unit* getUnitById(int id);

    protected:
        int gameTimer;

        const std::map<int, Unit*>& getUnitMap() const {
            return unitMap;
        }

    private:
        friend class Unit;

        std::map<int,Unit*> unitMap;

        // Called in Unit deconstructor
        void destroyUnit(int id);
};

} // namespace Prismer
