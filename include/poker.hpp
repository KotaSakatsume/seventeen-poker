#ifndef POKER_HPP
#define POKER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> // for shuffle
#include <random>    // for mt19937

namespace Poker {

    // --- カード関連の定義 ---

    enum class Suit { SPADE, HEART, DIAMOND, CLUB, NONE };

    enum Rank {
        JACK = 11, QUEEN = 12, KING = 13, ACE = 14, JOKER = 15
    };

    struct Card {
        Suit suit;
        int rank;

        Card(Suit s, int r) : suit(s), rank(r) {}
        Card() : suit(Suit::NONE), rank(0) {}

        std::string toString() const;
        bool operator<(const Card& other) const;
        bool operator==(const Card& other) const;
    };

    // --- 山札クラスの定義 (これが抜けていました！) ---

    class Deck {
    private:
        std::vector<Card> cards;
        
    public:
        Deck(); 
        
        void shuffle();                
        Card draw();                   
        bool isEmpty() const;          
        int size() const;              
        
        void printAll() const;
    };

    // --- 役判定関連の定義 ---

    enum class HandRank {
        HIGH_CARD = 0,
        ONE_PAIR = 1,
        TWO_PAIR = 2,
        THREE_OF_A_KIND = 3,
        STRAIGHT = 4,
        FULL_HOUSE = 5,
        FOUR_OF_A_KIND = 6,
        ROYAL_STRAIGHT_FLUSH = 7,
        FIVE_OF_A_KIND = 8
    };

    struct HandResult {
        HandRank rank;
        std::vector<int> kickers;

        bool operator<(const HandResult& other) const {
            if (rank != other.rank) return rank < other.rank;
            return kickers < other.kickers;
        }
        
        bool operator>(const HandResult& other) const {
            return other < *this;
        }

        bool operator==(const HandResult& other) const {
            return rank == other.rank && kickers == other.kickers;
        }

        std::string toString() const {
            switch(rank) {
                case HandRank::FIVE_OF_A_KIND: return "Five of a Kind";
                case HandRank::ROYAL_STRAIGHT_FLUSH: return "Royal Straight Flush";
                case HandRank::FOUR_OF_A_KIND: return "Four of a Kind";
                case HandRank::FULL_HOUSE: return "Full House";
                case HandRank::STRAIGHT: return "Straight";
                case HandRank::THREE_OF_A_KIND: return "Three of a Kind";
                case HandRank::TWO_PAIR: return "Two Pair";
                case HandRank::ONE_PAIR: return "One Pair";
                default: return "High Card";
            }
        }
    };

    HandResult evaluateHand(const std::vector<Card>& hand);

} // namespace Poker

#endif
