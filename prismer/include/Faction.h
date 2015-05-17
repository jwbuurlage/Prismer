#pragma once
#include <list>

using std::list;

class Unit;
class Packet;
class GameSession;

class Faction
{
    private:
        friend class GameSession;
        GameSession* const session;

        Faction(int id, GameSession* session);
    public:
        //~Faction deletes all units it has
        //and unregisters itself at factory
        ~Faction();

        //! Adds the unit to this faction
        //! Sets the faction id of the unit to this faction
        void addUnit(Unit* unit);

        //! Set the color index of this faction
        void setColor(int col) { color = col; }

        //! Get a list of units
        list<Unit*>& getUnits() { return units; }

        //! Serialize the faction into a packet
        void serialize(Packet& pk);
        //! Read a faction from a packet
        void deserialize(Packet& pk);

        //! Returns the faction id
        int getId() const { return id; }

        //! Returns the id of the client that owns this faction
        int getClientId() const { return clientId; }

        //! Set the id of the client that owns this faction
        void setClientId(int id) { clientId = id; }

    private:
        const int id;
        int clientId;

        //!! It is important that a linked list is used here
        //because during the iteration over this container,
        //new units will be created and existing ones deleted
        //and iterators should not be invalidated after
        //such operations
        list<Unit*> units;
        int color;
};
