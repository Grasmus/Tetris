#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>

namespace GameNamespace
{
    struct ScoreEntry 
    {
        std::string username;
        int score;
    };

    class DatabaseManager 
    {
    public:
        DatabaseManager(const std::string& databasePath);

        ~DatabaseManager();

        bool addUser(
            const std::string& username,
            const std::string& sessionStartTime,
            int userScore
        );

            std::vector<ScoreEntry> getTopScores(int scoreLimit);

    private:
        void initializeDatabase();

    private:
        sqlite3* databaseConnection{};
    };
}
