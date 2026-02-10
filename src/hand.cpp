#include "../include/poker.hpp"
#include <algorithm>
#include <map>

namespace Poker {

    // 内部ヘルパー: Jokerなしの5枚の手札を判定する
    HandResult evaluatePureHand(const std::vector<Card>& hand) {
        std::map<int, int> rankCounts;
        std::map<Suit, int> suitCounts;
        
        for (const auto& card : hand) {
            rankCounts[card.rank]++;
            suitCounts[card.suit]++;
        }

        bool isFlush = false;
        for (auto const& [suit, count] : suitCounts) {
            if (count == 5) isFlush = true;
        }

        std::vector<int> ranks;
        for (const auto& card : hand) ranks.push_back(card.rank);
        std::sort(ranks.begin(), ranks.end());
        
        bool isStraight = true;
        for (size_t i = 0; i < ranks.size() - 1; ++i) {
            if (ranks[i+1] != ranks[i] + 1) {
                isStraight = false;
                break;
            }
        }

        int pairCount = 0;
        int threeCount = 0;
        int fourCount = 0;
        
        for (auto const& [rank, count] : rankCounts) {
            if (count == 2) pairCount++;
            if (count == 3) threeCount++;
            if (count == 4) fourCount++;
        }

        HandRank rank = HandRank::HIGH_CARD;
        
        if (fourCount == 1) rank = HandRank::FOUR_OF_A_KIND;
        else if (threeCount == 1 && pairCount == 1) rank = HandRank::FULL_HOUSE;
        else if (isFlush && isStraight) rank = HandRank::ROYAL_STRAIGHT_FLUSH;
        else if (isStraight) rank = HandRank::STRAIGHT;
        else if (threeCount == 1) rank = HandRank::THREE_OF_A_KIND;
        else if (pairCount == 2) rank = HandRank::TWO_PAIR;
        else if (pairCount == 1) rank = HandRank::ONE_PAIR;

        return {rank, {}}; 
    }

    // メイン関数
    HandResult evaluateHand(const std::vector<Card>& originalHand) {
        bool hasJoker = false;
        for (const auto& card : originalHand) {
            if (card.rank == Rank::JOKER) hasJoker = true;
        }

        if (!hasJoker) {
            return evaluatePureHand(originalHand);
        }

        HandResult bestResult = {HandRank::HIGH_CARD, {}};
        std::vector<Suit> suits = {Suit::SPADE, Suit::HEART, Suit::DIAMOND, Suit::CLUB};
        std::vector<int> ranks = {Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE};

        for (const auto& s : suits) {
            for (const auto& r : ranks) {
                std::vector<Card> tempHand;
                for (const auto& card : originalHand) {
                    if (card.rank == Rank::JOKER) {
                        tempHand.push_back(Card(s, r));
                    } else {
                        tempHand.push_back(card);
                    }
                }
                
                // PureHand判定だと5カードを判定できないのでここで特別チェック
                std::map<int, int> counts;
                for(const auto& c : tempHand) counts[c.rank]++;
                bool isFive = false;
                for(auto const& [val, count] : counts) if(count == 5) isFive = true;

                HandResult currentRes;
                if (isFive) {
                    currentRes = {HandRank::FIVE_OF_A_KIND, {}};
                } else {
                    currentRes = evaluatePureHand(tempHand);
                }

                if (currentRes.rank > bestResult.rank) {
                    bestResult = currentRes;
                }
            }
        }
        return bestResult;
    }
}
