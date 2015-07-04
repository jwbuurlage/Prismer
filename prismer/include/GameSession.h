// GameSession is a base class for GameSessionClient and GameSessionServer
// It serves as a Unit and Faction factory class
// Units and Factions are created with createUnit and createFaction
// and they can be deleted by just calling delete on them.
// Every Unit and Faction has a pointer to this class
// so that it can de-register itself at deconstruction
#pragma once

#include <map>
#include <list>
#include <memory>

namespace Prismer {

using std::shared_ptr;
using std::unique_ptr;

class Unit;
class UnitInfo;
class Faction;
class Grid;

class GameSession
    : public std::enable_shared_from_this<GameSession>
{
    public:
        GameSession();
        virtual ~GameSession();

        void init();

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
        //FIXME: prototype? just circles for now..
        virtual shared_ptr<Unit> createUnit(int x, int y);

        /** Generate a unique id */
        int generateId() const {
            static int id = 1;
            return id++;
        }

        /** Return unit by id */
        shared_ptr<Unit> getUnitById(int id);

        /** Debug function */
        void _listUnits() const;

        /** Debug function */
        void _getVision() const;

        shared_ptr<Grid> getGrid() {
            return _grid;
        }

        void startMatch();

        void nextFaction();

    protected:
        int gameTimer;

        const std::map<int, shared_ptr<Unit>>& getUnitMap() const {
            return unitMap;
        }

        shared_ptr<Grid> _grid;

        std::list<shared_ptr<Faction>> _factions;
        decltype(_factions)::iterator _currentFactionIt;

        int _turn = 0;

    private:
        friend class Unit;

        std::map<int,shared_ptr<Unit>> unitMap;

        // Called in Unit deconstructor
        void destroyUnit(int id);
};

} // namespace Prismer
