#ifndef GAME_HPP
#define GAME_HPP

#include "poker.hpp"
#include <vector>

namespace Poker {

    // プレイヤーの状態
    struct Player {
        std::string name;
        int chips;
        std::vector<Card> hand;
        bool isFolded;

        // コンストラクタは2引数（isFoldedは初期化リストでfalseに）
        Player(std::string n, int c) : name(n), chips(c), isFolded(false) {}
    };

    // ゲーム進行クラス
    class Game {
    private:
        Deck deck;
        Player user;
        Player cpu;
        int pot;
        
        // ★追加: ユーザーが主導権（交換の優先権）を持っているか？
        bool userHasInitiative; 

    public:
        Game(int startChips);
        void run();

    private:
        void startRound();
        void bettingPhase(int round);
        void exchangePhase();       // ここで主導権ロジックを使う
        void showdown();
        
        int askCpuAction(int currentBet, int toCall);
        int getUserInput(int min, int max);
    };

} // namespace Poker

#endif