#include "../include/game.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>
#include <limits> // numeric_limits
#include <set>    // 重複チェック用

namespace Poker {

    // コンストラクタ
    Game::Game(int startChips) 
        : user("You", startChips), cpu("CPU", startChips), pot(0), userHasInitiative(true) {}

    // ユーザー入力のヘルパー関数
    int Game::getUserInput(int min, int max) {
        int choice;
        while (true) {
            std::cout << "> ";
            std::cin >> choice;
            if (std::cin.fail() || choice < min || choice > max) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter " << min << "-" << max << "." << std::endl;
            } else {
                return choice;
            }
        }
    }

    // CPUの思考ルーチン
    int Game::askCpuAction(int currentBet, int toCall) {
        (void)currentBet;
        (void)toCall;

        HandResult res = evaluateHand(cpu.hand);
        
        // 役がワンペア以上なら強気
        if (res.rank >= HandRank::ONE_PAIR) {
            return 2; 
        }
        
        // ブラフ
        if (rand() % 5 == 0) {
            return 2;
        }

        return 1; 
    }

    void Game::startRound() {
        std::cout << "\n========================================" << std::endl;
        std::cout << " NEW ROUND START! (Chips: You=" << user.chips << ", CPU=" << cpu.chips << ")" << std::endl;
        std::cout << "========================================" << std::endl;

        deck = Deck(); 
        deck.shuffle();
        user.hand.clear();
        cpu.hand.clear();
        user.isFolded = false;
        cpu.isFolded = false;
        pot = 0;
        
        // ラウンド開始時はUserが親とする
        userHasInitiative = true;

        int ante = 10;
        user.chips -= ante;
        cpu.chips -= ante;
        pot += ante * 2;

        for (int i = 0; i < 5; ++i) {
            user.hand.push_back(deck.draw());
            cpu.hand.push_back(deck.draw());
        }

        std::sort(user.hand.begin(), user.hand.end());
        std::cout << "Your Hand: ";
        for (const auto& c : user.hand) std::cout << c.toString() << " ";
        std::cout << std::endl;
    }

    void Game::bettingPhase(int round) {
        if (user.isFolded || cpu.isFolded) return;

        std::cout << "\n--- Betting Round " << round << " ---" << std::endl;
        
        std::cout << "[1] Check/Call  [2] Bet/Raise (50)  [0] Fold" << std::endl;
        int action = getUserInput(0, 2);

        if (action == 0) {
            user.isFolded = true;
            std::cout << "You folded." << std::endl;
            return;
        } else if (action == 2) {
            user.chips -= 50;
            pot += 50;
            userHasInitiative = true; // 主導権奪取
            std::cout << "You bet 50. (Initiative Taken!)" << std::endl;
        } else {
            std::cout << "You checked/called." << std::endl;
        }

        int cpuAction = askCpuAction(0, 0);
        if (cpuAction == 0) { 
            std::cout << "CPU calls." << std::endl;
        } else if (cpuAction == 2) {
            cpu.chips -= 50;
            pot += 50;
            userHasInitiative = false; // 主導権喪失
            std::cout << "CPU raises 50! (Initiative Lost...)" << std::endl;
            
            user.chips -= 50;
            pot += 50;
            std::cout << "(Auto-Call to match CPU)" << std::endl;
        } else {
            std::cout << "CPU checks/calls." << std::endl;
        }
        
        std::cout << "Current Pot: " << pot << std::endl;
    }

    // ★ここが修正された交換フェーズ（カード指定版）
    void Game::exchangePhase() {
        if (user.isFolded || cpu.isFolded) return;

        std::cout << "\n--- Card Exchange ---" << std::endl;
        
        if (userHasInitiative) {
            std::cout << ">> You have the initiative! (Exchange First)" << std::endl;
        } else {
            std::cout << ">> CPU has the initiative! (CPU Exchanges First)" << std::endl;
        }

        // 交換処理ロジック
        auto performExchange = [&](bool isUser) {
            int currentDeckSize = deck.size();
            int limit = (currentDeckSize < 5) ? currentDeckSize : 5;

            if (isUser) {
                std::cout << "\n[Your Turn] Deck remaining: " << currentDeckSize << std::endl;
                
                if (limit == 0) {
                    std::cout << "Deck is empty! You cannot exchange cards." << std::endl;
                    return;
                }

                // 番号付きで手札を表示
                std::cout << "Your Hand: ";
                for(size_t i=0; i<user.hand.size(); ++i) {
                    std::cout << "[" << i+1 << "]" << user.hand[i].toString() << " ";
                }
                std::cout << std::endl;

                std::cout << "Select cards to DISCARD (e.g. '1 3' to discard 1st and 3rd card)." << std::endl;
                std::cout << "Input '0' to keep all cards." << std::endl;

                // 入力バッファのクリア（前のcinの改行ゴミを消す）
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::vector<int> discardIndices;
                std::string line;

                while (true) {
                    std::cout << "> ";
                    std::getline(std::cin, line);

                    // 空入力や0は「交換なし」
                    if (line.empty() || line == "0") {
                        discardIndices.clear();
                        break;
                    }

                    std::stringstream ss(line);
                    int idx;
                    discardIndices.clear();
                    std::set<int> uniqueCheck;
                    bool valid = true;

                    while (ss >> idx) {
                        if (idx < 1 || idx > (int)user.hand.size()) {
                            std::cout << "Invalid index: " << idx << std::endl;
                            valid = false;
                            break;
                        }
                        if (uniqueCheck.count(idx)) {
                            // 重複は無視
                            continue;
                        }
                        uniqueCheck.insert(idx);
                        discardIndices.push_back(idx - 1); // 内部では0-indexed
                    }

                    if (!valid) continue; // 再入力

                    if ((int)discardIndices.size() > limit) {
                        std::cout << "Too many cards! Max allowed is " << limit << "." << std::endl;
                        continue;
                    }

                    break; // 入力OK
                }

                if (!discardIndices.empty()) {
                    std::cout << "Discarding " << discardIndices.size() << " cards..." << std::endl;
                    
                    // 新しい手札を作る
                    std::vector<Card> newHand;
                    // 捨てないカードを追加
                    for(size_t i=0; i<user.hand.size(); ++i) {
                        bool isDiscarded = false;
                        for(int d : discardIndices) {
                            if ((int)i == d) isDiscarded = true;
                        }
                        if (!isDiscarded) newHand.push_back(user.hand[i]);
                    }
                    // 新しいカードをドロー
                    for(size_t i=0; i<discardIndices.size(); ++i) {
                        newHand.push_back(deck.draw());
                    }

                    user.hand = newHand;
                    std::sort(user.hand.begin(), user.hand.end());
                    
                    std::cout << "New Hand: ";
                    for (const auto& c : user.hand) std::cout << c.toString() << " ";
                    std::cout << std::endl;
                } else {
                    std::cout << "You kept all cards." << std::endl;
                }

            } else {
                // CPUロジック
                std::cout << "\n[CPU Turn] Deck remaining: " << currentDeckSize << std::endl;
                if (limit == 0) {
                    std::cout << "CPU cannot exchange (Deck empty)." << std::endl;
                    return;
                }

                int cpuMax = (limit >= 3) ? 3 : limit;
                int cpuSwap = 0;
                if (cpuMax > 0) cpuSwap = rand() % (cpuMax + 1);

                for(int i=0; i<cpuSwap; ++i) {
                    if (!cpu.hand.empty()) cpu.hand.erase(cpu.hand.begin());
                    cpu.hand.push_back(deck.draw());
                }
                
                if (cpuSwap > 0) std::cout << "CPU exchanged " << cpuSwap << " cards." << std::endl;
                else std::cout << "CPU stands pat." << std::endl;
            }
        };

        // 順番制御
        if (userHasInitiative) {
            performExchange(true);
            performExchange(false);
        } else {
            performExchange(false);
            performExchange(true);
        }
    }

    void Game::showdown() {
        if (user.isFolded) {
            std::cout << "You folded. CPU wins " << pot << " chips!" << std::endl;
            cpu.chips += pot;
            return;
        }
        if (cpu.isFolded) { 
            std::cout << "CPU folded. You win " << pot << " chips!" << std::endl;
            user.chips += pot;
            return;
        }

        std::cout << "\n--- SHOWDOWN ---" << std::endl;
        
        HandResult userRes = evaluateHand(user.hand);
        HandResult cpuRes = evaluateHand(cpu.hand);

        std::cout << "Your Hand: ";
        for (const auto& c : user.hand) std::cout << c.toString() << " ";
        std::cout << " -> " << userRes.toString() << std::endl;

        std::cout << "CPU Hand : ";
        for (const auto& c : cpu.hand) std::cout << c.toString() << " ";
        std::cout << " -> " << cpuRes.toString() << std::endl;

        if (userRes > cpuRes) {
            std::cout << "YOU WIN! (+" << pot << ")" << std::endl;
            user.chips += pot;
        } else if (cpuRes > userRes) {
            std::cout << "CPU WINS! (+" << pot << ")" << std::endl;
            cpu.chips += pot;
        } else {
            std::cout << "DRAW! Pot split." << std::endl;
            user.chips += pot / 2;
            cpu.chips += pot / 2;
        }
    }

    void Game::run() {
        while (user.chips > 0 && cpu.chips > 0) {
            startRound();
            bettingPhase(1);
            exchangePhase();
            bettingPhase(2);
            showdown();

            std::cout << "\nNext round? [1] Yes [0] Quit" << std::endl;
            if (getUserInput(0, 1) == 0) break;
        }
        std::cout << "Game Over." << std::endl;
    }

} // namespace Poker
