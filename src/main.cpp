#include <iostream>
#include "../include/poker.hpp"

int main() {
    std::cout << "=== 17 Poker Engine Test ===" << std::endl;

    // 1. 山札の生成
    Poker::Deck deck;
    std::cout << "[Init] Deck size: " << deck.size() << " (Expected: 17)" << std::endl;

    // 2. 生成直後の並びを表示（ソートされているはず）
    std::cout << "Original: ";
    deck.printAll();

    // 3. シャッフル
    deck.shuffle();
    std::cout << "Shuffled: ";
    deck.printAll();

    // 4. 全部引いてみる
    std::cout << "\nDrawing all cards..." << std::endl;
    int count = 0;
    while (!deck.isEmpty()) {
        Poker::Card c = deck.draw();
        std::cout << "[" << count + 1 << "] " << c.toString() << std::endl;
        count++;
    }

    return 0;
}
