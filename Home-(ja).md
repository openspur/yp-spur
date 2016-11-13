# YP-Spurとは?

YP-Spurは、移動ロボットの走行制御と座標系ベースのコマンドシステムを提供するソフトウェアです。詳細な制御のアーキテクチャについては、[Control Architecture](Control Architecture)を参照して下さい。

# Quick Start Guide

## インストール

* gcc, 標準Cライブラリをインストール
* 下記のコマンドでYP-Spurをビルド、インストール
```
$ git clone https://openspur.org/repos/yp-spur.git
$ cd yp-spur
$ mkdir build
$ cd build
$ ../configure
$ make
$ sudo make install
$ sudo ldconfig
```

## 開発中バージョンを試す

* 下記ページから、試したいブランチ名またはタグ名を探す
https://github.com/at-wat/yp-spur/branches, https://github.com/at-wat/yp-spur/releases.
* インストール手順で git clone した、yp-spurディレクトリに移動
* 所望のブランチまたはタグをチェックアウト
```
$ git fetch origin
$ git checkout -b [ブランチ名] origin/[ブランチ名]
```
* インストール手順と同様、build ディレクトリで make, make install を実行

## 使い方

* 端末(Terminal)を2つ起動
* ypspur-coordinatorを、端末1で実行し、そのまま実行したままに
```
$ ypspur-coordinator -p PARAMETER_FILE.param -d /path/to/device
```
* ユーザプログラムを、端末2で実行
