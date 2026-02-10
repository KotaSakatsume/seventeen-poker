#include "../include/poker.hpp"
#include <sstream>

namespace Poker {

    // カードを文字列に変換（デバッグ・表示用）
    // 例: SPADE, ACE -> "♠A"
    std::string Card::toString() const {
        if (rank == Rank::JOKER) {
            return "JOKER";
        }

        std::stringstream ss;
        
        // スートの表示
        switch (suit) {
            case Suit::SPADE:   ss << "♠"; break;
            case Suit::HEART:   ss << "♥"; break;
            case Suit::DIAMOND: ss << "♦"; break;
            case Suit::CLUB:    ss << "♣"; break;
            default:            ss << "?"; break;
        }

        // ランクの表示
        switch (rank) {
            case Rank::JACK:    ss << "J"; break;
            case Rank::QUEEN:   ss << "Q"; break;
            case Rank::KING:    ss << "K"; break;
            case Rank::ACE:     ss << "A"; break;
            default:            ss << rank; break;
        }

        return ss.str();
    }

    // 比較演算子（ソート用）
    // ランクが強い順（A > K ...）に並べたい場合はここを調整
    // 今回は単純に「ランクの大きさ」で比較します
    bool Card::operator<(const Card& other) const {
        if (rank != other.rank) {
            return rank < other.rank; // ランクが違うならランクで比較
        }
        return suit < other.suit;     // ランクが同じならスートで比較
    }

    // 等価演算子
    bool Card::operator==(const Card& other) const {
        return rank == other.rank && suit == other.suit;
    }

} // namespace Poker
