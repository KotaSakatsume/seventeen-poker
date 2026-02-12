#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>
#include "poker.hpp"

namespace Poker {

enum class GamePhase { INIT, BETTING_1, EXCHANGE, BETTING_2, SHOWDOWN, GAME_OVER };

struct GameState {
    int playerChips;
    int cpuChips;
    int pot;
    std::string message;
    GamePhase phase;
    std::vector<int> playerHand;
    std::vector<int> cpuHand;
};

class Game {
public:
    Game();
    void init();
    void processAction(int input, const std::vector<int>& input_arr = {});
    GameState getState() const;

private:
    Deck deck;
    std::vector<Card> playerHand;
    std::vector<Card> cpuHand;
    
    int playerChips;
    int cpuChips;
    int pot;
    GamePhase phase;
    std::string currentMessage;

    void dealInitialCards();
    void determineWinner();

    // ★追加: CPUの頭脳
    void cpuExchangeTurn(); // 最適な交換を行う
    double calculateWinRate(const std::vector<Card>& currentHand, const std::vector<int>& keepIndices);
};

} // namespace Poker

#endif
