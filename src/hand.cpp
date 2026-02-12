#include "poker.hpp"
#include <map>
#include <algorithm>
#include <sstream>

namespace Poker {

// 役の名前を文字列で返す
std::string HandResult::toString() const {
    switch(rank) {
        case HandRank::FIVE_OF_A_KIND: return "Five of a Kind";
        case HandRank::ROYAL_STRAIGHT_FLUSH: return "Royal Straight Flush"; // 17ポーカーにはないかもですが念のため
        case HandRank::FOUR_OF_A_KIND: return "Four of a Kind";
        case HandRank::FULL_HOUSE: return "Full House";
        case HandRank::STRAIGHT: return "Straight";
        case HandRank::THREE_OF_A_KIND: return "Three of a Kind";
        case HandRank::TWO_PAIR: return "Two Pair";
        case HandRank::ONE_PAIR: return "One Pair";
        default: return "High Card";
    }
}

// 役判定のメインロジック
HandResult evaluateHand(const std::vector<Card>& hand) {
    HandResult result;
    result.rank = HandRank::HIGH_CARD;

    if (hand.empty()) return result;

    // 数字のカウント用マップ
    std::map<int, int> rankCounts;
    std::map<Suit, int> suitCounts;
    bool hasJoker = false;

    // 手札を解析
    for (const auto& card : hand) {
        if (card.rank == 0 || card.rank == 15) { // Joker (簡易判定)
            hasJoker = true;
        } else {
            rankCounts[card.rank]++;
            suitCounts[card.suit]++;
        }
    }

    // キッカー（役に関係ないカード）を選ぶためにランクを降順ソート
    std::vector<int> ranks;
    for (const auto& card : hand) {
        if (card.rank != 0 && card.rank != 15) {
            ranks.push_back(card.rank);
        }
    }
    std::sort(ranks.rbegin(), ranks.rend());

    // --- 簡易的な役判定ロジック (Joker考慮なしのシンプル版) ---
    // ※本格的な17ポーカーの判定は複雑なので、まずは「動くこと」を優先した基本ロジックです

    bool isFlush = false;
    for (auto const& [suit, count] : suitCounts) {
        if (count >= 5) isFlush = true; // Jokerがあれば4枚でもOKなどの処理が必要
    }

    // ペア判定
    int pairs = 0;
    int threeOfAKind = 0;
    int fourOfAKind = 0;

    for (auto const& [rank, count] : rankCounts) {
        if (count == 4) fourOfAKind++;
        if (count == 3) threeOfAKind++;
        if (count == 2) pairs++;
    }

    // ジョーカーがある場合、一番強い役に昇格させる（簡易実装）
    if (hasJoker) {
        if (fourOfAKind) result.rank = HandRank::FIVE_OF_A_KIND;
        else if (threeOfAKind) result.rank = HandRank::FOUR_OF_A_KIND;
        else if (pairs == 2) result.rank = HandRank::FULL_HOUSE;
        else if (pairs == 1) result.rank = HandRank::THREE_OF_A_KIND;
        else result.rank = HandRank::ONE_PAIR;
    } else {
        if (fourOfAKind) result.rank = HandRank::FOUR_OF_A_KIND;
        else if (threeOfAKind && pairs) result.rank = HandRank::FULL_HOUSE;
        else if (isFlush) result.rank = HandRank::ROYAL_STRAIGHT_FLUSH; // 全カードが10以上ならロイヤル等は省略
        else if (threeOfAKind) result.rank = HandRank::THREE_OF_A_KIND;
        else if (pairs >= 2) result.rank = HandRank::TWO_PAIR;
        else if (pairs == 1) result.rank = HandRank::ONE_PAIR;
    }

    // キッカー設定（本来はもっと詳細に設定が必要）
    result.kickers = ranks;
    
    return result;
}

} // namespace Poker
