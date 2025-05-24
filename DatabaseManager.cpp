#include "DatabaseManager.h"
#include <iostream>
#include "GameExceptions.h"

namespace GameNamespace
{
    DatabaseManager::DatabaseManager(const std::string& databasePath)
        : databaseConnection(nullptr)
    {
        const int successCode = SQLITE_OK;

        int resultCode
        {
            sqlite3_open(
                databasePath.c_str(),
                &databaseConnection
            )
        };

        if (resultCode != successCode) 
        {
            throw DatabaseOpenException();
        }
        else 
        {
            initializeDatabase();
        }
    }

    DatabaseManager::~DatabaseManager() {

        if (databaseConnection) 
        {
            sqlite3_close(databaseConnection);
        }
    }

    void DatabaseManager::initializeDatabase() {
        const char* createUsersTableSql
        {
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "username TEXT NOT NULL, "
            "start_time TEXT NOT NULL, "
            "score INTEGER NOT NULL"
                ");"
        };

        char* errorMessage{};

        const int executionResult
        {
            sqlite3_exec(
                databaseConnection,
                createUsersTableSql,
                nullptr,
                nullptr,
                &errorMessage
            )
        };

        if (executionResult != SQLITE_OK) 
        {
            sqlite3_free(errorMessage);

            throw DatabaseCreateTableException();
        }
    }

    bool DatabaseManager::addUser(
        const std::string& username,
        const std::string& sessionStartTime,
        int userScore
    ) {
        const char* insertUserSql
        {
            "INSERT INTO users (username, start_time, score) "
            "VALUES (?, ?, ?);"
        };

        sqlite3_stmt* insertStatement{};

        int preparationResult
        {
            sqlite3_prepare_v2(
                databaseConnection,
                insertUserSql,
                -1,
                &insertStatement,
                nullptr
            )
        };

        if (preparationResult != SQLITE_OK) 
        {
            return false;
        }

        sqlite3_bind_text(
            insertStatement,
            1,
            username.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_text(
            insertStatement,
            2,
            sessionStartTime.c_str(),
            -1,
            SQLITE_TRANSIENT
        );

        sqlite3_bind_int(
            insertStatement,
            3,
            userScore
        );

        const bool isSuccess
        {
            (sqlite3_step(insertStatement) == SQLITE_DONE)
        };

        sqlite3_finalize(insertStatement);

        return isSuccess;
    }

    std::vector<ScoreEntry> DatabaseManager::getTopScores(int scoreLimit) {
        std::vector<ScoreEntry> scoreEntries{};

        const char* topScoresSql
        {
            "SELECT username, MAX(score) AS max_score "
            "FROM users "
            "GROUP BY username "
            "ORDER BY max_score DESC, username ASC "
            "LIMIT ?;"
        };

        sqlite3_stmt* topScoresStatement{};

        int preparationResult
        {
            sqlite3_prepare_v2(
                databaseConnection,
                topScoresSql,
                -1,
                &topScoresStatement,
                nullptr
            )
        };

        if (preparationResult != SQLITE_OK) 
        {
            return scoreEntries;
        }

        sqlite3_bind_int(
            topScoresStatement,
            1,
            scoreLimit
        );

        while (sqlite3_step(topScoresStatement) == SQLITE_ROW) 
        {
            ScoreEntry entry;

            entry.username = reinterpret_cast<const char*>(
                    sqlite3_column_text(topScoresStatement, 0)
                );

            entry.score = sqlite3_column_int(topScoresStatement, 1);

            scoreEntries.push_back(entry);
        }

        sqlite3_finalize(topScoresStatement);

        return scoreEntries;
    }
}
