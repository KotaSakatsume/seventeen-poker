#include "../include/poker.hpp"
#include <algorithm> // std::shuffle
#include <random>    // std::mt19937, std::random_device
#include <iostream>

namespace Poker {

    // コンストラクタ：17枚のカードを生成して山札に入れる
    Deck::Deck() {
        // 1. A, K, Q, J の4ランク × 4スートを生成
        std::vector<Rank> targetRanks = {Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE};
        std::vector<Suit> targetSuits = {Suit::SPADE, Suit::HEART, Suit::DIAMOND, Suit::CLUB};

        for (const auto& s : targetSuits) {
            for (const auto& r : targetRanks) {
                cards.emplace_back(s, r);
            }
        }

        // 2. ジョーカーを1枚追加
        cards.emplace_back(Suit::NONE, Rank::JOKER);
    }

    // シャッフル（メルセンヌ・ツイスタ使用）
    void Deck::shuffle() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);
    }

    // 山札から1枚引く
    Card Deck::draw() {
        if (cards.empty()) {
            throw std::runtime_error("Deck is empty!");
        }
        Card c = cards.back(); // 最後のカードを取得
        cards.pop_back();      // 削除
        return c;
    }

    bool Deck::isEmpty() const {
        return cards.empty();
    }

    int Deck::size() const {
        return cards.size();
    }

    // デバッグ用：山札の中身を全表示
    void Deck::printAll() const {
        for (const auto& c : cards) {
            std::cout << c.toString() << " ";
        }
        std::cout << std::endl;
    }

} // namespace Poker
