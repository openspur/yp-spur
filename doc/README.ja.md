# YP-Spurとは?

YP-Spurは、移動ロボットの走行制御と座標系ベースのコマンドシステムを提供するソフトウェアです。詳細な制御のアーキテクチャについては、[Control Architecture](./Control-Architecture.md)を参照して下さい。

[![Build Status](https://travis-ci.org/openspur/yp-spur.svg?branch=master)](https://travis-ci.org/openspur/yp-spur)


# Index

[English version is here.](README.md)

* [Home](README.ja.md)
* [Parameter File](Parameter-File.ja.md)
* [Control Architecture (en)](Control-Architecture.md)
* [Manpage (en)](Manpage.md)
* [Embedding Parameters](Embedding-Parameters.ja.md)
* [Auxiliary Files](Auxiliary-Files.md)


# Quick Start Guide

## インストール

* gcc, 標準Cライブラリ, cmakeをインストール
* 下記のコマンドでYP-Spurをビルド、インストール
```
$ git clone https://github.com/openspur/yp-spur
$ cd yp-spur
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig
```

## ブランチしているバージョンを試す

(Gitの使い方を理解している前提です)

* 下記ページから、試したいブランチ名またはタグ名を探す
https://github.com/openspur/yp-spur/branches, https://github.com/openspur/yp-spur/releases.
* インストール手順で git clone した、yp-spurディレクトリに移動
* 所望のブランチまたはタグをチェックアウト
```
$ git fetch origin
$ git checkout -b [ブランチ名] origin/[ブランチ名]
```
* インストール手順と同様、build ディレクトリで make, make install を実行

## 使い方

* 端末(Terminal)を2つ起動
* ypspur-coordinatorを、端末1で実行
```
$ ypspur-coordinator -p PARAMETER_FILE.param -d /path/to/device
```
* ユーザプログラムを、端末2で実行
