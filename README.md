# seventeen-poker# 17 Poker Engine 🃏

![Build Status](https://img.shields.io/badge/build-pending-yellow) ![License](https://img.shields.io/badge/license-MIT-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B17-orange)

## 📖 Overview (概要)
**「17ポーカー（Seventeen Poker）」** のための高速なゲームエンジンおよび対戦シミュレーターです。
C++17を用いた厳密なクラス設計、ビット演算を考慮した役判定アルゴリズム、および拡張性のあるアーキテクチャを採用しています。

本プロジェクトは、ソフトウェアエンジニアリングの実践（Design Patterns, TDD, CI/CD）を目的として開発されています。

## 🚀 Key Features (特徴)
* **Optimized Hand Evaluator:** ジョーカー（ワイルドカード）を含む全パターンの役判定を高速に処理。
* **Strategy Engine:** ミニマックス法や期待値計算に基づいたCPU思考ルーチン（実装予定）。
* **Modern C++:** スマートポインタや`std::optional`などを活用した安全なメモリ管理。

## 🛠 Requirements (必須環境)
* C++ Compiler (Create/Clang/GCC) supporting **C++17**
* Make

## 📦 Build & Run (ビルドと実行)

### 1. クローンと移動
```bash
git clone [https://github.com/your-username/17poker-engine.git](https://github.com/your-username/17poker-engine.git)
cd 17poker-engine