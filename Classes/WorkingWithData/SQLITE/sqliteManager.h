//#ifndef __SQLITE_MANAGER_H__
//#define __SQLITE_MANAGER_H__
//
//#include "cocos2d.h"
//#include "wxsqlite/sqlite3.h"
//#include <vector>
//
//class SQLiteManager {
//public:
//    static SQLiteManager* getInstance();
//    static void destroyInstance();
//    //ismine INTEGER,  movement_speed REAL, life_spawn INTEGER, block_speed REAL, skill_duration REAL
//    struct PlayerInfo {
//        int id;
//        int money;
//        int movement_speed;
//        int life_spawn;
//        int block_speed;
//        int skill_duration;
//        int x2CoinActive;
//        int x2JumpActive;
//        int shieldActive;
//    };
//    struct ScoreInfo {
//        int id;
//        int score;
//        int playerId;
//    };
//
//    bool init();
//    void closeDatabase();
//    void executeQuery(const std::string& query);
//    /*void setPlayerInfo(const int playerId, int movement_speed, int money, int life_spawn, int block_speed, int skill_duration, int x_2_coin, int x_2_jump, int shield);
//    PlayerInfo getPlayerById(int playerId);
//    void addScore(int playerId, int score);
//    std::vector<ScoreInfo> getTop10Highscore();*/
//private:
//    static SQLiteManager* s_instance;
//    sqlite3* _database;
//
//    SQLiteManager() : _database(nullptr) {}
//    ~SQLiteManager() { closeDatabase(); }
//    SQLiteManager(const SQLiteManager&) = delete;
//    SQLiteManager& operator=(const SQLiteManager&) = delete;
//};
//
//#endif // __SQLITE_MANAGER_H__