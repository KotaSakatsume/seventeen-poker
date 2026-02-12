#include <emscripten/bind.h>
#include "game.hpp"

using namespace emscripten;
using namespace Poker; // 名前空間を省略できるようにする

// JavaScriptに公開する定義
EMSCRIPTEN_BINDINGS(poker_module) {

    // std::vector<int> をJSの配列として使えるように登録
    register_vector<int>("VectorInt");

    // GamePhase (Enum) の登録
    enum_<GamePhase>("GamePhase")
        .value("INIT", GamePhase::INIT)
        .value("BETTING_1", GamePhase::BETTING_1)
        .value("EXCHANGE", GamePhase::EXCHANGE)
        .value("BETTING_2", GamePhase::BETTING_2)
        .value("SHOWDOWN", GamePhase::SHOWDOWN)
        .value("GAME_OVER", GamePhase::GAME_OVER)
        ;

    // GameState (構造体) の登録
    // JS側ではオブジェクト { playerChips: 1000, ... } として扱える
    value_object<GameState>("GameState")
        .field("playerChips", &GameState::playerChips)
        .field("cpuChips", &GameState::cpuChips)
        .field("pot", &GameState::pot)
        .field("message", &GameState::message)
        .field("phase", &GameState::phase)
        .field("playerHand", &GameState::playerHand) // VectorIntとして渡る
        .field("cpuHand", &GameState::cpuHand)
        ;

    // Game (クラス) の登録
    class_<Game>("Game")
        .constructor<>() // コンストラクタ
        .function("init", &Game::init)
        .function("processAction", &Game::processAction)
        .function("getState", &Game::getState)
        ;
}
