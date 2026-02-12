#include "poker.hpp"

namespace Poker {

Deck::Deck() {
    // 17ポーカー仕様: J(11)〜A(14) + Joker(15)
    for (int s = 0; s < 4; ++s) {
        for (int r = 11; r <= 14; ++r) {
            cards.emplace_back(static_cast<Suit>(s), r);
        }
    }
    cards.emplace_back(Suit::NONE, Rank::JOKER);
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card Deck::draw() {
    if (cards.empty()) {
        // カード切れの場合はダミー(NONE, 0)を返すが、
        // 受け取り側でチェックが必要
        return Card(Suit::NONE, 0);
    }
    Card c = cards.back();
    cards.pop_back();
    return c;
}

} // namespace Poker
