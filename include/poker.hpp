#ifndef POKER_HPP
#define POKER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> // for shuffle
#include <random>    // for mt19937

namespace Poker {

    // カードのスート（マーク）
    enum class Suit { SPADE, HEART, DIAMOND, CLUB, NONE };

    // カードのランク（数字）。強さ比較のため数値化
    enum Rank {
        JACK = 11, QUEEN = 12, KING = 13, ACE = 14, JOKER = 15
    };

    // カードクラス
    struct Card {
        Suit suit;
        int rank;

        // コンストラクタ
        Card(Suit s, int r) : suit(s), rank(r) {}
        
        // デフォルトコンストラクタ（配列作成用）
        Card() : suit(Suit::NONE), rank(0) {}

        // デバッグ用表示メソッド
        std::string toString() const;

        // 比較演算子（ソートや強弱判定で必須）
        bool operator<(const Card& other) const;
        bool operator==(const Card& other) const;
    };

    // 山札クラス
    class Deck {
    private:
        std::vector<Card> cards;
        
    public:
        Deck(); // コンストラクタで17枚生成
        
        void shuffle();                // シャッフル
        Card draw();                   // 1枚引く
        bool isEmpty() const;          // 空かチェック
        int size() const;              // 残り枚数
        
        // デバッグ用：中身を全部表示
        void printAll() const;
    };

} // namespace Poker

#endif
