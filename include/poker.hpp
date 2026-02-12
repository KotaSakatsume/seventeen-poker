#ifndef POKER_HPP
#define POKER_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <random>

namespace Poker {

    enum class Suit { SPADE, HEART, DIAMOND, CLUB, NONE };

    // src/card.cpp で Rank::JACK のように使うための定義
    namespace Rank {
        const int JACK = 11;
        const int QUEEN = 12;
        const int KING = 13;
        const int ACE = 14;
        const int JOKER = 15;
    }

    struct Card {
        Suit suit;
        int rank;

        Card(Suit s, int r) : suit(s), rank(r) {}
        Card() : suit(Suit::NONE), rank(0) {}

        int getId() const { return static_cast<int>(suit) * 100 + rank; }
        
        std::string toString() const;
        
        // ★ これらが抜けていました！復活！
        bool operator<(const Card& other) const;
        bool operator==(const Card& other) const;
    };

    class Deck {
    private:
        std::vector<Card> cards;
    public:
        Deck();
        void shuffle();
        Card draw();
    };

    enum class HandRank { HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT, FULL_HOUSE, FOUR_OF_A_KIND, ROYAL_STRAIGHT_FLUSH, FIVE_OF_A_KIND };

    struct HandResult {
        HandRank rank;
        std::vector<int> kickers;
        bool operator>(const HandResult& other) const {
            if (rank != other.rank) return rank > other.rank;
            return kickers > other.kickers;
        }
        bool operator<(const HandResult& other) const {
            if (rank != other.rank) return rank < other.rank;
            return kickers < other.kickers;
        }
        std::string toString() const;
    };

    HandResult evaluateHand(const std::vector<Card>& hand);

} // namespace Poker

#endif
