#include "game.hpp"
#include <iostream>
#include <map>

namespace Poker {

Game::Game() : playerChips(1000), cpuChips(1000), pot(0), phase(GamePhase::INIT) {
    init();
}

void Game::init() {
    deck = Deck();
    deck.shuffle();
    playerHand.clear();
    cpuHand.clear();
    pot = 0;
    phase = GamePhase::BETTING_1;
    currentMessage = "ゲーム開始。あなたのターンです。";
    dealInitialCards();
}

void Game::dealInitialCards() {
    for (int i = 0; i < 5; ++i) {
        playerHand.push_back(deck.draw());
        cpuHand.push_back(deck.draw());
    }
}

// ★最強CPUの思考回路
// 現在の手札で「どのカードを残せば勝率が最大になるか」をシミュレーションする
void Game::cpuExchangeTurn() {
    double bestWinRate = -1.0;
    std::vector<int> bestKeepIndices;

    // 5枚のカードの「残す/捨てる」の全組み合わせは 2^5 = 32通り
    // ビットマスクを使って全探索する (00000 〜 11111)
    for (int i = 0; i < 32; ++i) {
        std::vector<int> keepIndices;
        for (int j = 0; j < 5; ++j) {
            if ((i >> j) & 1) { // jビット目が1なら「残す」
                keepIndices.push_back(j);
            }
        }

        // その組み合わせでの勝率を計算
        double rate = calculateWinRate(cpuHand, keepIndices);
        
        if (rate > bestWinRate) {
            bestWinRate = rate;
            bestKeepIndices = keepIndices;
        }
    }

    // 最良の選択を実行
    // まず、残さないカードを特定
    std::vector<Card> newHand;
    for (int idx : bestKeepIndices) {
        newHand.push_back(cpuHand[idx]);
    }
    
    // 足りない分をドロー
    while (newHand.size() < 5) {
        newHand.push_back(deck.draw());
    }
    
    cpuHand = newHand;
    
    // 勝率に応じてメッセージを変える（ちょっと人間味を出す）
    if (bestWinRate > 0.8) currentMessage = "CPU: (ニヤリ...) 準備完了だ。";
    else if (bestWinRate > 0.5) currentMessage = "CPU: まあまあの手だな。";
    else currentMessage = "CPU: くっ...勝負だ！";
}

// モンテカルロ・シミュレーション
double Game::calculateWinRate(const std::vector<Card>& currentHand, const std::vector<int>& keepIndices) {
    int wins = 0;
    int simulations = 500; // 500回の未来予知を行う (17枚デッキなら十分な精度)

    for (int i = 0; i < simulations; ++i) {
        // 1. シミュレーション用のデッキを作成
        // (本来は「今見えていないカード」だけで作るべきだが、
        //  簡易的に「新品のデッキから自分の手持ちを除いたもの」とする)
        Deck simDeck;
        simDeck.shuffle();
        
        // 2. 自分の手札を構築
        std::vector<Card> mySimHand;
        for (int idx : keepIndices) {
            mySimHand.push_back(currentHand[idx]);
        }
        
        // シミュレーション用デッキから、すでに持っているカードを抜く（重複防止）
        // ※Deckクラスにremove機能がないので、drawした時に捨てる方式で簡易実装
        // 本来はDeckクラスにremoveを作るべきだが、ここではロジックでカバー
        
        // 3. 自分（CPU）が不足分をドロー
        while (mySimHand.size() < 5) {
            Card c = simDeck.draw();
            // 自分の持っているカードと同じなら引き直し（簡易的な重複排除）
            bool isDuplicate = false;
            for(const auto& existing : currentHand) { // 元の手札にあるカードは出ないはず
                if(existing == c) isDuplicate = true;
            }
            if (!isDuplicate) mySimHand.push_back(c);
        }

        // 4. 相手（プレイヤー）の手札をランダムに5枚ドロー
        std::vector<Card> playerSimHand;
        while (playerSimHand.size() < 5) {
            Card c = simDeck.draw();
            // 自分の元の手札に含まれているカードは、プレイヤーには行かない
             bool isDuplicate = false;
            for(const auto& existing : currentHand) {
                if(existing == c) isDuplicate = true;
            }
            // 自分のシミュレーション手札に含まれているカードも行かない
            for(const auto& existing : mySimHand) {
                if(existing == c) isDuplicate = true;
            }
            
            if (!isDuplicate && c.rank != 0) { // Rank0はdraw失敗
                playerSimHand.push_back(c);
            }
        }

        // 5. 勝敗判定
        if (evaluateHand(mySimHand) > evaluateHand(playerSimHand)) {
            wins++;
        }
    }

    return static_cast<double>(wins) / simulations;
}

void Game::processAction(int input, const std::vector<int>& input_arr) {
    switch (phase) {
        case GamePhase::BETTING_1:
            if (input == 0) { // Fold
                cpuChips += pot;
                currentMessage = "あなたがフォールドしました。";
                phase = GamePhase::GAME_OVER;
            } else {
                int bet = (input == 2) ? 100 : 50;
                playerChips -= bet;
                cpuChips -= bet;
                pot += (bet * 2);
                currentMessage = "交換するカードを選んでください。";
                phase = GamePhase::EXCHANGE;
            }
            break;

        case GamePhase::EXCHANGE:
            // プレイヤーの交換処理
            if (!input_arr.empty()) {
                // 選択されたインデックス（捨てるカード）以外を残す方式に変更
                // input_arr には「残すカード」が入ってくる？ それとも「捨てるカード」？
                // shell.html の実装では「selectedCards」を送ってくる。
                // 普通は「交換したいカードを選ぶ」ので、送られてきたものを捨てる。
                
                // ここでは「送られてきたインデックスのカードを捨てる（新しく引く）」と解釈
                for (int idx : input_arr) {
                    if (idx >= 0 && idx < 5) {
                        playerHand[idx] = deck.draw();
                    }
                }
            }
            
            // ★CPUの交換ターン（思考時間）
            cpuExchangeTurn();
            
            currentMessage = "最終ベットを行ってください。";
            phase = GamePhase::BETTING_2;
            break;

        case GamePhase::BETTING_2:
            if (input == 0) {
                cpuChips += pot;
                currentMessage = "フォールドしました。";
                phase = GamePhase::GAME_OVER;
            } else {
                determineWinner();
                phase = GamePhase::SHOWDOWN;
            }
            break;

        case GamePhase::SHOWDOWN:
            init();
            break;

        default:
            break;
    }
}

void Game::determineWinner() {
    auto pRes = evaluateHand(playerHand);
    auto cRes = evaluateHand(cpuHand);

    if (pRes > cRes) {
        playerChips += pot;
        currentMessage = "あなたの勝ち！ (" + pRes.toString() + ")";
    } else if (pRes < cRes) {
        cpuChips += pot;
        currentMessage = "CPUの勝ち... (" + cRes.toString() + ")";
    } else {
        playerChips += pot / 2;
        cpuChips += pot / 2;
        currentMessage = "引き分けです。";
    }
}

GameState Game::getState() const {
    GameState s;
    s.playerChips = playerChips;
    s.cpuChips = cpuChips;
    s.pot = pot;
    s.message = currentMessage;
    s.phase = phase;

    for (const auto& c : playerHand) s.playerHand.push_back(c.getId());
    for (const auto& c : cpuHand) {
        if (phase == GamePhase::SHOWDOWN) {
            s.cpuHand.push_back(c.getId());
        } else {
            s.cpuHand.push_back(-1);
        }
    }
    return s;
}

} // namespace Poker
