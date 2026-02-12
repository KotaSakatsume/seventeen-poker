# 17 Poker (Seventeen Poker)

17枚のカードだけで遊ぶ、シンプルかつ奥深いポーカーゲームです。
C++で実装されたゲームロジックをWebAssembly (Emscripten) を使用してブラウザ上で動作させています。

🎮 **[ブラウザで遊ぶ](https://kotasakatsume.github.io/seventeen-poker/)**

## 🃏 ゲームのルール

通常のポーカーとは異なり、以下の**17枚のカードのみ**を使用します。カードカウンティングや確率計算が重要な戦略的ゲームです。

*   **使用カード:** スペード・ハート・ダイヤ・クラブの **A, K, Q, J** (計16枚) + **Joker** (1枚)
*   **手札:** 5枚
*   **ジョーカー:** どのカードの代わりにもなる最強のワイルドカードです。

### 役の強さ (強い順)
1.  **Five of a Kind** (ファイブカード) - ジョーカーを含めて同じランクが5枚
2.  **Royal Straight Flush** (ロイヤルストレートフラッシュ)
3.  **Four of a Kind** (フォーカード)
4.  **Full House** (フルハウス)
5.  **Straight** (ストレート)
6.  **Three of a Kind** (スリーカード)
7.  **Two Pair** (ツーペア)
8.  **One Pair** (ワンペア)
9.  **High Card** (ハイカード)

## 🤖 特徴

*   **思考するCPU:**
    CPUはモンテカルロシミュレーションを実装しています。配られた手札から「どのカードを残せば勝率が最大になるか」をリアルタイムに数百回の試行を行って判断し、最適な交換を行います。
*   **WebAssembly:**
    C++で書かれたロジックをそのままブラウザで動かしているため、複雑な確率計算も高速に処理されます。

## 🛠️ 技術スタック

*   **言語:** C++ (C++17)
*   **ビルド:** Emscripten (WebAssembly)
    *   **Embind:** C++のクラスや関数をJavaScriptから利用するために使用
*   **フロントエンド:** HTML5, JavaScript
*   **アルゴリズム:** モンテカルロ法 (CPUの思考ロジック)
*   **ホスティング:** GitHub Pages

## ビルド方法 (開発者向け)

Emscripten環境にて以下のコマンドでビルドできます。

```bash
emcc src/game.cpp src/deck.cpp src/hand.cpp src/card.cpp src/bindings.cpp \
  -I include \
  -o docs/poker.js \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s MODULARIZE=1 \
  -s EXPORT_NAME="createPokerModule" \
  --bind \
  -O3
```