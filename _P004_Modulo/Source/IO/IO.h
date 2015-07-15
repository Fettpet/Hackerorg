#ifndef IO_H
#define IO_H
#include "../Modulo/game.h"
#include <string>

namespace IO {
// read
    Game::Game readGameFromFile(const std::string& filename);
    Game::Game readGameFromHackerOrg(const std::string& user,
                               const std::string& password);
// write
    void writeGameToFile(const Game::Game& game, const std::string filename);
    void writeSolutionToHacker(const std::string& user, const std::string& password, const std::string solution);

// Goto Level
    void gotoLevel(const std::string& user, const std::string& password, uint level);
}

#endif // GAME_H
