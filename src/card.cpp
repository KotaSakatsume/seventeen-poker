#include "poker.hpp"
#include <sstream>

namespace Poker {

std::string Card::toString() const {
    if (rank == 0) return "JOKER"; // 簡易実装
    
    std::stringstream ss;
    switch (suit) {
        case Suit::SPADE:   ss << "S"; break;
        case Suit::HEART:   ss << "H"; break;
        case Suit::DIAMOND: ss << "D"; break;
        case Suit::CLUB:    ss << "C"; break;
        default: break;
    }

    switch (rank) {
        case Rank::JACK:  ss << "J"; break;
        case Rank::QUEEN: ss << "Q"; break;
        case Rank::KING:  ss << "K"; break;
        case Rank::ACE:   ss << "A"; break;
        default:          ss << rank; break;
    }
    return ss.str();
}

bool Card::operator<(const Card& other) const {
    if (rank != other.rank) return rank < other.rank;
    return suit < other.suit;
}

bool Card::operator==(const Card& other) const {
    return rank == other.rank && suit == other.suit;
}

} // namespace Poker
