#include "../include/poker.hpp"
#include <cassert>
#include <iostream>

using namespace Poker;

void assertHand(std::vector<Card> cards, HandRank expectedRank, std::string testName) {
    HandResult result = evaluateHand(cards);
    
    // 結果表示（デバッグ用）
    std::cout << "Test: " << testName << " -> " << result.toString() << std::endl;

    if (result.rank == expectedRank) {
        std::cout << "[PASS]" << std::endl;
    } else {
        std::cout << "[FAIL] Expected: " << (int)expectedRank 
                  << " Actual: " << (int)result.rank << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "=== Hand Evaluator Test ===" << std::endl;

    // Test 1: Five of a Kind (AAAA + Joker)
    // JokerがAに化けて、Aが5枚になるパターン
    std::vector<Card> fiveKind = {
        {Suit::SPADE, ACE}, {Suit::HEART, ACE}, 
        {Suit::DIAMOND, ACE}, {Suit::CLUB, ACE}, 
        {Suit::NONE, JOKER}
    };
    assertHand(fiveKind, HandRank::FIVE_OF_A_KIND, "Five of a Kind (4 Aces + Joker)");

    // Test 2: Four of a Kind (AAA + K + Joker)
    // JokerがAに化けて、Aが4枚になるパターン
    std::vector<Card> fourKind = {
        {Suit::SPADE, ACE}, {Suit::HEART, ACE}, 
        {Suit::DIAMOND, ACE}, {Suit::SPADE, KING}, 
        {Suit::NONE, JOKER}
    };
    assertHand(fourKind, HandRank::FOUR_OF_A_KIND, "Four of a Kind (3 Aces + Joker)");

    // Test 3: Full House (JJ + K + Joker) -> JJ K KK (Full House) or JJJ K (Three card)
    // Jokerは最強役を作るので、Kに化けて Full House (JJ KKK) になるはず
    std::vector<Card> fullHouse = {
        {Suit::SPADE, JACK}, {Suit::HEART, JACK}, 
        {Suit::DIAMOND, KING}, {Suit::SPADE, KING}, // Jが2枚、Kが2枚
        {Suit::NONE, JOKER}
    };
    assertHand(fullHouse, HandRank::FULL_HOUSE, "Full House (JJ KK + Joker)");

    // Test 4: Pure Full House (JJJKK)
    std::vector<Card> pureFullHouse = {
        {Suit::SPADE, JACK}, {Suit::HEART, JACK}, 
        {Suit::DIAMOND, JACK}, {Suit::SPADE, KING}, 
        {Suit::HEART, KING}
    };
    assertHand(pureFullHouse, HandRank::FULL_HOUSE, "Pure Full House (JJJKK)");

    std::cout << "\nAll tests passed! Logic is perfect." << std::endl;
    return 0;
}
