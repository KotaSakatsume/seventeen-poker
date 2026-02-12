#include <iostream>
#include <ctime>
#include "../include/game.hpp"

int main() {
    // 乱数初期化（これをしないと毎回同じカードになる）
    std::srand(std::time(nullptr));

    std::cout << "==================================" << std::endl;
    std::cout << "      WELCOME TO 17 POKER         " << std::endl;
    std::cout << "==================================" << std::endl;

    // 所持金1000チップでスタート
    Poker::Game game(1000);
    game.run();

    return 0;
}
