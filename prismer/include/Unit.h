struct UnitInfo
{
    int type_id;
    // ...
};

class GameSession;

class Unit
{
    public:
        Unit(int id,
            UnitInfo info,
            GameSession* session) {
            _id = id;
            _info = info;
            _session = session;
        }
        ~Unit() { }

        int getId() const {
            return _id;
        }

        void update(int gameTimer) const;

    private:
        int _id;
        UnitInfo _info;
        GameSession* _session;
};
