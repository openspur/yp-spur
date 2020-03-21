^^^^^^^^^^^^^^^^^^^^^
Changelog for YP-Spur
^^^^^^^^^^^^^^^^^^^^^

1.18.0 (2020-03-21)
-------------------
* Accumulate packet lost count (`#132 <https://github.com/openspur/yp-spur/issues/132>`_)
* Fix torque offset gear factor (`#131 <https://github.com/openspur/yp-spur/issues/131>`_)
* Fix typo in error message (`#130 <https://github.com/openspur/yp-spur/issues/130>`_)
* Add DEVICE_TIMEOUT parameter and show packet lost error (`#129 <https://github.com/openspur/yp-spur/issues/129>`_)
* Fix --no-yp-protocol mode (`#128 <https://github.com/openspur/yp-spur/issues/128>`_)
* Add option to ping chained devices (`#127 <https://github.com/openspur/yp-spur/issues/127>`_)
* Update assets to v0.0.7 (`#126 <https://github.com/openspur/yp-spur/issues/126>`_)
* Update assets to v0.0.6 (`#125 <https://github.com/openspur/yp-spur/issues/125>`_)
* Migrate changelog format to rst (`#124 <https://github.com/openspur/yp-spur/issues/124>`_)
* Contributors: Atsushi Watanabe

1.17.1
------
- Show driver error only if the state is changed (`#122 <https://github.com/openspur/yp-spur/issues/122>`_)
- Trap SIGINT only in odometry receive loop (`#120 <https://github.com/openspur/yp-spur/issues/120>`_)

1.17.0
------
- Fix Spur_freeze command (`#117 <https://github.com/openspur/yp-spur/issues/117>`_)
- Bump protocol version to 11:05 (`#116 <https://github.com/openspur/yp-spur/issues/116>`_)
- Add LR cutoff frequence parameter (`#115 <https://github.com/openspur/yp-spur/issues/115>`_)
- Add hall signal delay parameter (`#114 <https://github.com/openspur/yp-spur/issues/114>`_)

1.16.0
------
- Fix deployment (`#108 <https://github.com/openspur/yp-spur/issues/108>`_)
- Fix serial communication error handling (`#107 <https://github.com/openspur/yp-spur/issues/107>`_)
- Fix ENCODER_DENOMINATOR handling to make param units corresponding to hardware (`#104 <https://github.com/openspur/yp-spur/issues/104>`_)
- Reorganize console output (`#105 <https://github.com/openspur/yp-spur/issues/105>`_)
- Generate and install pkg-config file (`#101 <https://github.com/openspur/yp-spur/issues/101>`_)
- Fix protocol version send target (`#106 <https://github.com/openspur/yp-spur/issues/106>`_)
- Fix initial absolute encoder value handling (`#103 <https://github.com/openspur/yp-spur/issues/103>`_)
- Correct invalid links (`#99 <https://github.com/openspur/yp-spur/issues/99>`_)
- Expose the communication functions (`#97 <https://github.com/openspur/yp-spur/issues/97>`_)
- Fix deceleration factor during over speed (`#95 <https://github.com/openspur/yp-spur/issues/95>`_)
- Update motor error ID (`#93 <https://github.com/openspur/yp-spur/issues/93>`_)
- Add flag to check availability of get_device_error_state (`#92 <https://github.com/openspur/yp-spur/issues/92>`_)
- Fix joint state initialization (`#91 <https://github.com/openspur/yp-spur/issues/91>`_)
- Receive error status of connected device (`#89 <https://github.com/openspur/yp-spur/issues/89>`_)
- Stricten parameter error check (`#88 <https://github.com/openspur/yp-spur/issues/88>`_)
- Add encoder denominator parameter (`#86 <https://github.com/openspur/yp-spur/issues/86>`_)
- Enable build on semantic version tag (`#83 <https://github.com/openspur/yp-spur/issues/83>`_)
- Modernize coding styles (`#10 <https://github.com/openspur/yp-spur/issues/10>`_)

1.15.3
------
- Fix finding readline in cmake (`#75 <https://github.com/openspur/yp-spur/issues/75>`_)
- Enable matrix build on Travis (`#77 <https://github.com/openspur/yp-spur/issues/77>`_)
- Run test on Ubuntu Bionic (`#76 <https://github.com/openspur/yp-spur/issues/76>`_)
- Fix reading last parameter line without newline (`#74 <https://github.com/openspur/yp-spur/issues/74>`_)
- Fix joint command names in manpage (`#71 <https://github.com/openspur/yp-spur/issues/71>`_)
- Update manpage of libypspur (`#70 <https://github.com/openspur/yp-spur/issues/70>`_)

1.15.2
------
- Add exec_depend to catkin for ROS build (`#64 <https://github.com/openspur/yp-spur/issues/64>`_)

1.15.1
------
- Fix buildtool_depend in ROS manifest (`#62 <https://github.com/openspur/yp-spur/issues/62>`_)
- Fix ROS package manifest (`#61 <https://github.com/openspur/yp-spur/issues/61>`_)
- Add run_depend to catkin in ROS package manifest (`#60 <https://github.com/openspur/yp-spur/issues/60>`_)
- Fix deployment (`#59 <https://github.com/openspur/yp-spur/issues/59>`_)
- Workaround for CERT_UNTRUSTED error in npm (`#58 <https://github.com/openspur/yp-spur/issues/58>`_)

1.15.0
------
- Fix project version handling both on cmake 2 and 3 (`#55 <https://github.com/openspur/yp-spur/issues/55>`_)
- Fix odometry timestamp (`#54 <https://github.com/openspur/yp-spur/issues/54>`_)
- Find libreadline on cmake (`#52 <https://github.com/openspur/yp-spur/issues/52>`_)
- Update install document for CMake version. (`#51 <https://github.com/openspur/yp-spur/issues/51>`_)
- Move wiki into doc directory. (`#50 <https://github.com/openspur/yp-spur/issues/50>`_)
- Support catkin build system. (`#47 <https://github.com/openspur/yp-spur/issues/47>`_)
- Fix process termination. (`#49 <https://github.com/openspur/yp-spur/issues/49>`_)
- Fix behavior on CMake3 (CMP0048) (`#48 <https://github.com/openspur/yp-spur/issues/48>`_)
- Reduce minimum cmake version to 2.8.3. (`#44 <https://github.com/openspur/yp-spur/issues/44>`_)
- Add build test on xenial and trusty. (`#46 <https://github.com/openspur/yp-spur/issues/46>`_)
- Define INCLUDE_DIRS in package config file. (`#41 <https://github.com/openspur/yp-spur/issues/41>`_)
- Add manifest and update cmake config. (`#40 <https://github.com/openspur/yp-spur/issues/40>`_)
- Use CMake. (`#39 <https://github.com/openspur/yp-spur/issues/39>`_)
- Remove B-Loco firmware. (`#38 <https://github.com/openspur/yp-spur/issues/38>`_)
- Support encoder index signal. (`#37 <https://github.com/openspur/yp-spur/issues/37>`_)
- fixes serial output to be raw mode (`#36 <https://github.com/openspur/yp-spur/issues/36>`_)
- fixes joint control feature switch definitions (`#34 <https://github.com/openspur/yp-spur/issues/34>`_)
- adds simultaneous joint angle and velocity control command (`#33 <https://github.com/openspur/yp-spur/issues/33>`_)
- bundles latest ypspur-gui on win32 binary deployment (`#32 <https://github.com/openspur/yp-spur/issues/32>`_)
- adds get_joint_torque command (`#29 <https://github.com/openspur/yp-spur/issues/29>`_)
- deploys windows binary on release (`#31 <https://github.com/openspur/yp-spur/issues/31>`_)
- adds MinGW build test (`#30 <https://github.com/openspur/yp-spur/issues/30>`_)
- adds travis setting (`#28 <https://github.com/openspur/yp-spur/issues/28>`_)
- fixes TORQUE_FINENESS related error messages (`#25 <https://github.com/openspur/yp-spur/issues/25>`_)
- updates readme (`#24 <https://github.com/openspur/yp-spur/issues/24>`_)
- Supported negative gear ratio (this changes kinematics without affecting motor control)
- Use long long int instead of int64_t
- Added parameter to divide encoder count for high resolution encoder
- Retry odometry receive during parameter update
- Added parameter range validation
- Fixed a bug that the motion_control bypasses vel/acc limit
- Added sleep in motor parameter transmission for old devices
- Changed order of the board version warning message
- Increased protocol version
- Removed debug output of inertia parameters
- Updated warnings about board version
- Added 64-bit windows support
- Fixed a bug that the robot sometimes doesn't move by stop_line
- Fixed a bug that ignores L_C1 parameter
- Fixed motor id of vehicle control using VEHICLE_CONTROL parameter
- Fixed update flags of default parameters
- Fixed COUNT_REV parameter update flag
- Support more than two motors
- Add encoder type and velocity control cycle parameter
- Added AC motor phase offset parameter
- Fixed thread termination problem
- Fixed return value of Spur_get_pos
- Fixed include path for sh2 firmware build
- configure.acのSSM有効化に関するバグを修正
- Fixed build problems on mingw32
- Merge updates from formula-calc project
- 表示されるメッセージのスペルミスを修正
- Added error handling to aviod compiler warning
- Cleaned link settings and dependencies
- Autoreconf using automake 1.13.4
- configureにSSM使用の無効化オプションを追加 (automake 1.13)
- コンパイルError/Warningの修正
- デフォルトで --high-resolution オプションを有効化

1.14.0
------
- パラメータの説明文の配列の誤りを修正
- freeモードから駆動モード切り替え時に速度指令が不連続になるバグを修正
- 摩擦補償パラメータを回転方向ごとに設定できるように変更
- 摩擦補償速度比例項の単位変換の間違いを修正
- fork()が提供されない環境でコンパイルエラーになる問題を修正
- sh-velの速度制御指令の分解能向上を無効化
- プロセス間通信をsocketで行うモードを追加(--socket)
- トルク推定の符号間違いを修正
- adjust_posが正しく働いていなかったのを修正
- Spur_md_init_socketマクロ定義の誤りを修正
- socketによるプロセス間通信の初期化の誤りを修正
- Merge branch 'socket_ipc'
- パラメータ取得・設定コマンドで左右輪それぞれ設定可能に
- 目標速度取得コマンドの追加
- 最短時間制御の時間遅れによる振動を改善
- キネマティクス計算を定義どおりに修正
- spin/orientコマンドで目標角度が正規化されるように修正
- オドメトリ取得時刻推定のデバッグ出力追加
- ソケットによるプロセス間通信をWindows環境に対応
- Windows環境でのコンパイルを改善
- ssmの同期ずれのバグ修正
- ypspur-coordinator: get_wheelvel/angのverbose表示バグを修正
- ypspur-interpreter: 一行での複数コマンド指定
- ypspur-interpreter: にデジタルIO制御関数を追加
- ypspur-interpreter: コマンドライン引数での複数コマンド実行に対応
- samples/run-test: 位置制御を用いてオーバーシュートを削減

1.13.5
------
- sh-velでトルク指令モードから速度制御モードへの移行時に加速度制限がかからない問題を修正
- 摩擦パラメータの単位変換の間違いを修正
- ロボット速度制御、車輪速度制御、トルク制御の切り替え時に加速度制限などが正しくかかるように修正

1.13.4
------
- [AWD] コンパイル時にライブラリとユーティリティ・サンプルの依存関係を解決
- multi-deviceブランチをマージ(複数デバイスの同時利用機能)
- different-motor-supportブランチをマージ(異なる種類のモータ・ギア混在環境のサポート)
- モータの番号と左右車輪の対応を修正
- 制御開始時の振動を抑制
- トルク指令制御コマンドの動作を整理
- Locoボードへのエンコーダ分解能送信(ブラシレスモータ対応用)
- パラメータ名とモータ番号の対応付けを間違えるdifferent-motor-supportブランチのバグを修正
- libformula-calcの更新をsubtreeマージ
- sh-velのウォッチドッグタイマが働かないバグを修正
- high-resolutionブランチをマージ(速度制御指令の分解能向上機能)
- パラメータファイルにモータの種類の項目を追加
- ypspur-interpreterの引数で速度等を指定しなかった場合に速度等の設定コマンドを発行しないように修正
- デーモンプロセスで起動するオプション追加
- wheel_velコマンドでホイール角加速度制限がかかるように修正
- デバイス依存のパラメータ(固定小数桁数、PWM分解能)をデバイスから取得するように変更
- デバイスからパラメータファイルを取得する機能の追加
- ダイナミクス補償のバグフィックス
- wheel_velの線形フィードバック切り替え動作を修正
- ダイナミクス補償の目標加速度計算にLPFを追加
- 最短時間制御が0付近で振動しないよう、線形フィードバックに切り替え

1.13.3
------
- [AWD] sh-velをelf形式のコンパイラに対応
- sh_velに変数サイズ確認コマンド追加(コンパイラ変更時のデバッグ用)
- sh-velのスタートアップルーチンで変数初期化が正しく行われていなかったバグを修正
- sh-velでモータからインパルス状のノイズ(カリカリ音)が発生する現象を改善
- ypspur-interpreterに初期速度設定オプション、コマンド実行オプション追加
- シリアル通信切断時に詳細なエラーを表示
- Windows環境で生成される実行ファイル類の拡張子を.gitignoreに追加
- pthread_tが単なるポインタでない環境でコンパイルエラーになる問題を修正
- pkg-configが無い環境でpkg-configを使おうとしないように変更
- 制御モード移行時に加速度制限がかからない場合がある問題を修正
- ypspur-interpreterをEOF入力に対応
- パラメータファイルの更新を監視して自動的に再読み込みする隠しオプションを追加
- sh-velがsh-coff-gccでコンパイルできないバグを修正
- sh-velコンパイル時の不適切な最適化を抑制
- sh-velが高ボーレートでも正しく設定されるように修正
- スレッド終了処理を修正、ボーレート不適合時の通信エラー表示を修正
- SH開発環境がない場合にもsh-vel.motを生成しようとする問題を修正
- 出力レベル毎のエラー表示用関数を追加
- [GND] シリアル通信のタイムアウト時のエラーを表示(コーディングルール:インデントを修正)

1.13.2 (2012.4.5)
-----------------
- [AWD] Freeモードから抜ける際に加速度制限が正しく働かないバグを修正
- [AWD] B-Loco通信無効モードのCPU使用率を低減
- [AWD] パラメータファイルの説明を出力する起動オプションを追加(--param-help)

1.13.1 (2011.12.14)
-------------------
- [AWD] sh-velのスタートアップルーチン、リンカスクリプトをオリジナルのファイルに

1.13.0 (2011.12.11)
-------------------
- [AWD] パラメータファイル中にロボットのサイズを記述できるように
- [AWD] B-Locoとの通信を一切しないモードを追加(--without-device)
- [AWD] 制御しないモードのコマンドライン引数を変更(--without-control)
- [AWD] Windows環境で新しいgccに対応
- [AWD] Windows環境で共有メモリとMutexの名前競合を解決
- [AWD] Windows環境でシリアル通信のOS上のバッファをクリアできるように

1.12.3 (2011.12.6)
------------------
- [GND] sh-velのリセットタイマの仕様変更.YPSpur拡張コマンドの通信時はタイムアウトを長めに設定

1.12.2 (2011.12.5)
------------------
- [YOK] Mac OS X環境でシリアル通信ができるようにした

1.12.1 (2011.12.4)
------------------
- [GND] A/Dの値をssmに書き込む際のバッファリングのバグ修正
- [GND] ビットレート(ボーレート)の設定のバグ修正
- [GND] ypspur-coordinaterの初期化に失敗するとsh-velがリセットされないバグを修正(※sh-velを入れ直す必要あり)

1.12.0 (2011.11.14)
-------------------
- [AWD] Passiveモード(押して進むモード作成)

1.11.2 (2011.11.3)
------------------
- [AWD] トルク推定の式をパラメータの単位系の修正に対応

1.11.1 (2011.10.31)
-------------------
- [STK] モータ制御ゲインの計算で時間を考慮していないバグを修正
- [STK] libodmssm.cで戻り値を使用していないバグを修正
- [STK] パラーメータのバージョンが新しすぎても動作するバグを修正
- [STK] MOTOR_VTCを読み込まないように修正
- [STK] パラメータの単位系を修正（GAIN_KP, GAIN_KI, INTEGRAL_MAX, TORQUE_VISCOS）

1.11.0 (2011.10.30)
-------------------
- [AWD] 並進力[N],トルク[Nm]の推定値取得コマンド追加(Spur_get_force)
- [AWD] タイヤの出力トルク[Nm]の推定値取得コマンド追加(YP_get_wheel_torque)
- [AWD] 出力トルクの推定値からロボットの慣性モーメントを推定するサンプル sample/MOI-estimate を追加
- [AWD] 64bit環境用での、32bitコンパイルしたときに共有ライブラリが生成されないバグを修正
- [AWD] 32/64bitの切り替え方法を変更、CFLAGS="-m32" ./configure のように (これまで configure --host=i686 としていたが、本来このオプションは i686-gcc というファイル名のコンパイラを使うという指定)
- [AWD] ypspur-interpreterのコマンド解析部分のバグを修正

1.10.1 (2011.9.22)
------------------
- [STK] odometry_receiveで同じデータを何度も処理するバグを修正
- [STK] odometry_receiveで受信したデータパケット数が正しいかを確認するようにした

1.10.0 (2011.7.28)
------------------
- [STK] 一部関数の名称変更
- [STK] msg関連の定義をypspur.hからypparam.hへ移動
- [STK] ypspur-coordinatorの戻り値を修正
- [STK] ssmのadjustするSNAMEをSNAME_ADJUSTに修正
- [STK] PWS補償の項が間違っていたので修正(モータ制御PIゲインの値を変える必要があります)
- [STK] PWS補償のゲインを質量・慣性モーメントより自動的に計算するようにした。(慣性モーメントパラメータの追加)

1.9.0 (2011.7.22)
-----------------
- [GND] パラメータ取得コマンドの追加(YPSpur_parameter_get)

1.8.6 (2011.6.17)
-----------------
- [AWD] タイヤの回転数取得コマンドの追加(YP_get_wheel_vel) 
- [FWR] タイヤの角度取得コマンドの追加(YP_get_wheel_ang) 

1.8.5 (2011.6.1)
----------------
- [STK] ssmの終了処理を追加
- [AWD] トルク指令コマンドの追加(YP_wheel_torque) 

1.8.4 (2011.4.28)
-----------------
- [STK] sample/run-testを四角形を描くように改造、高速化
- [STK] src/odometry.c odometry_receive()のad変換周りを修正
- [STK] Spur_wheel_vel()をYP_wheel_vel()に変更
- [STK] set_adjust_com()を修正
- [STK] bitレートを用いたタイムスタンプの計算式を修正

1.8.3 (2011.2.19)
-----------------
- [AWD] Spur_init時にメッセージキューが存在しない場合に-1を返すように修正
- [AWD] Windows環境でメッセージ通信の破棄に対応
- [AWD] Windows環境のシリアル通信のBaudRate型をDWORD型に修正
- [AWD] Windows環境のメッセージ通信実装にmsgctlを追加
- [AWD] Windows環境でpexportsが存在しないとき警告メッセージを表示するように変更
- [AWD] siglonglmpが利用不可なとき可能ならlongjmpを利用するように修正(Ctrl+C処理を改善)

1.8.2 (2010.11.18)
------------------
- [AWD] sh-velでPWM値のリミット処理の間違いを修正

1.8.1 (2010.11.18)
------------------
- [AWD] 再接続時に、コマンド系が初期化されないように修正
- [AWD] 再接続時に、デバイスが存在しかつ通信が成立しない場合に再試行するように修正

1.8.0 (2010.11.4)
-----------------
- [AWD] デジタルIOを利用可能に

1.7.4 (2010.11.2)
-----------------
- [AWD] SIGINTが送られたときの処理をちゃんと
- [AWD] 終了時にメッセージキューを破棄するように変更
- [AWD] すべての関数で、メッセージキューが破棄されているとき-1を返すように変更
- [AWD] YPSpur_get_error_stateで、メッセージキューが破棄されていることによるエラーが発生したことがあるかチェックする(再度Spur_initをするとエラー情報がクリアされる)

1.7.3 (2010.10.13)
------------------
- [STK] SIGINTが送られたときの処理がなされていなかったので、追加（とりあえずexit(0）)
- [STK] オドメトリなどをSSMに書き込まない"--without-ssm"モードを追加

1.7.2 (2010.10.2)
-----------------
- [STK] YPSpur_orientの定義がypspur.hになかったので追加
- [STK] ssmを使用すると、成功しても失敗したとメッセージがでるのを修正
- [STK] ADを使うとSSMまわりの時間推定が間違えるのを修正
- [STK] reconnect処理のtryconnectの引数を入れ忘れてたので追加
- [STK] MacOS Xなどのldconfigが無い環境ではldconfigをしないように変更
- [STK] LONG HELPを追加

1.7.1 (2010.9.26)
-----------------
- [AWD] ssm_ypspur_handlerでparam.hがincludeされていなかったのを修正

1.7.0 (2010.9.26)
-----------------
- [AWD] B-Locoとの通信速度を変更可能に(--speed N)
- [AWD] PC側受信バッファオーバーフローのバグを修正(ADを使う場合)

1.6.1 (2010.9.20)
-----------------
- [AWD] MinGW用のエラー回避コードが不要になっていたので削除
- [AWD] 表示の詳細化(--verbose)の表示レベルの誤りを修正
- [AWD] stop_line, spinコマンドで制御周期を考慮して、振動を若干改善

1.6.0 (2010.9.15)
-----------------
- [AWD] パラメータに遠心加速度のリミットを追加
- [AWD] パラメータのバージョン管理を追加、MAX_CENTRI_ACCの項を追加し、VERSION 1.0とすること

  - 遠心加速度は例えば0.25[G]=2.45[m/ss]

- [AWD] 表示の抑制・詳細化に対応

1.5.0 (2010.9.11)
-----------------
- [AWD] set_pos_GLがロボットの動作に影響を与えないようにset_posの動作を変更
- [AWD] 走行制御は、SP座標系(Spur走行制御座標系)上で行うように変更
- [AWD] near_pos, near_ang, over_lineが正しく働かないバグを修正
- [AWD] ypspur-interpreterにSpurコマンドを追加

1.4.2 (2010.9.9)
----------------
- [AWD] Command analyzerの表示で、vel, wheel_velコマンドの引数が表示されないバグを修正
- [AWD] YPプロトコルのバージョン管理方法を変更 YPP:00:00 (CURRENT:AGE)、libtoolのバージョン管理に準拠
- [AWD] ypspur-interpreterにset_accel,set_angaccel,vel,wheel_velを追加
- [AWD] ypspur-interpreterをreadline無しでもコンパイルできるように変更
- [AWD] Windows環境で、sys/msq.h等のMessageQueue関連の関数・構造体の定義がない場合に対応
- [AWD] DLLファイルの生成に関するmakefileの修正

1.4.1 (2010.9.4)
----------------
- [AWD] --admaskオプションの認識のバグ修正

1.4.0 (2010.9.2)
----------------
- [STK] YPSpur_isfreeze、YPSpur_stop_lineのヘッダファイル定義が無かったので追加
- [STK] linuxでコンパイル時にdll.laファイルが作成されないようにした
- [STK] ssm関連のバグ修正
- [STK] ADをssmに書き出すようにした
- [STK] ssmにあったypspur用のssmtypeの定義をypspurに移した
- [AWD] PWS逆キネマティクス計算の符号を修正
- [AWD] 逆キネマの修正に伴う軌跡追従制御関連の修正
- [AWD] 加速度・角加速度・角速度設定値を正のみに制限
- [STK] YPプロトコルのバージョン表記を変更 YPP00:00:00 (メジャー:マイナー:リビジョン)
- [AWD] coordinator起動時にYPプロトコルのバージョンをチェックするように変更
- [STK] コンパイルオプションのCFLAGSのデフォルトをconfigure.acに書くようにした
- [AWD] ypspur-interpreterを追加

1.3.0 (2010.8.31)
-----------------
- [AWD] SH上でマイコンからPCのデータ送信を割り込みで行うように変更
- [AWD] odometry.cがSSM有りでコンパイルできないバグを修正
- [AWD] YP_get_ad_valueでADポートの値を取得可能に(coordinatorの引数で--admask 00001001のように取得するポート番号を指定, この例ではAD0,AD3を取得)
- [AWD] ADポートの値を取得可能にするため、sh_velをバージョンアップ
- [AWD] 他の環境のshクロスコンパイラの命名パターンを追加(sh-elf-*)
- [AWD] 通信が切れたとき、正しく終了しないバグを修正

1.2.2 (2010.8.24)
-----------------
- [AWD] Windows環境で、dll, lib, defファイルを出力するように

1.2.1 (2010.8.24)
-----------------
- [AWD] Spurコマンドを実行する座標系を誤ってBSにしていたのをGLに修正

1.2.0 (2010.8.23)
-----------------
- [AWD] Windows環境に対応―事実上のクロスプラットフォーム化 (MinGW + pthread for win32でコンパイル可能)
- [AWD] コマンドとオドメトリ間での座標系周りのバグを修正
- [AWD] YPSpur_isfreeze関数を追加

1.1.1 (2010.8.22)
-----------------
- [AWD] クロスプラットフォーム化に向けて、mingw用のエラー回避コード作成

1.1.0 (2010.8.18)
-----------------
- [AWD] 全面的なソースコードの構造変更(一部途中)
- [AWD] ypspur-coordinator起動時にメッセージキューの内容を破棄
- [AWD] 角度のみ指定して横方向の位置を指定しない走行コマンドYPSpur_orientを実装
- [AWD] get_pos_FSマクロの削除
- [AWD] 緊急停止コマンドYPSpur_freeze/unfreezeの実装
- [AWD] 加速度・角加速度を、パラメータファイルによるリミット値とユーザー設定値で分離, 加速度設定はYPSpur_set_accel, YPSpur_set_ang_accelで可能(!!加速度・角加速度の初期値は0なので、必ずユーザーが指定する必要あり!!)
- [AWD] 隠しオプション--enable-set-bsでBS座標系の書き換えが可能(シミュレーション等の実装用)
- [AWD] バージョン表示オプション

0.13.3 (2010.7.30)
------------------
- [STK] Ubuntu 9.04以前の環境でコンパイルできなくなるバグを修正

0.13.2 (2010.6.29)
------------------
- [AWD] stop_lineで追従直線まで到達せずに、止まる直線に到達した後に距離偏差による角振動が続く現象を修正
- [AWD] circleコマンドの追従中以外の安定性確保を適用し忘れていたのを再度適用
- [AWD] spin / stop_line の位置サーボ系命令の力学計算の誤りを再度修正

0.13.1 (2010.6.9)
-----------------
- [AWD] stop_lineコマンドで速度が負の時走りつづけるのを修正

0.13.0 (2010.6.9)
-----------------
- [AWD] stop_lineコマンドの実装、circleコマンドの追従中以外の安定性確保

  - これに伴い、libypspurを更新

    - libypspur_la_LDFLAGS = -version-info 1:0:1

- [AWD] 引数の隠しコマンドで、動作中にUSBデバイスが取り外された場合に自動再接続
- [AWD] エラー表示関連の統一
- [AWD] 引数の隠しコマンドで、msq keyを指定可能に。Spur_initexでmsq keyを指定した初期化が可能。

0.12.2 (2010.6.3)
-----------------
- [STK] libodmssmのバグフィックス
- [STK] libtooolのバージョン管理形式の変更

  - release @PACKAGE_VERSION@ から -version-info 0:0:0 に変更

     - libcarte2d_la_LDFLAGS = -version-info 0:0:0
     - libodomssm_la_LDFLAGS = -lssm -version-info 0:0:0
     - libypspur_la_LDFLAGS = -version-info 0:0:0

  - これに対応するためlibypspurを変更する毎にsrc/Makefile.am,auxlib/Makefile.amを修正する必要があるが、libypspurを変更しない限りバージョンが上がってもユーザープログラムをmakeし直す必要が無くなるはず。

0.12.1 (2010.5.28)
------------------
- [AWD] 動作中にUSBデバイスが取り外された場合にエラーを表示して終了するように変更。
- B-Locoの通信プロトコル確認時にタイムアウト処理を行い、3回試行してダメならエラーを表示して終了するように変更。
- 制御周期の割り込みをtimerfdからclock_nanosleepに変更。(kernel2.6系で利用可能。2.4系ではnanosleepを利用。)

0.11.2 (2010.4.20)
------------------
- [STK] ライブラリのヘッダファイルを名称の統一のため"yp-spur.h"から"ypspur.h"に変更。
- 互換性のため"yp-spur.h"に自動的にシンボリックリンクを貼るようにした。
- インストール時に自動的にldconfigを実行するようにした。 

0.11.1 (2010.4.15)
------------------
- [STK] パラメータファイルのパッケージを分離。パラメータの読み込みをpkg-configを使用するように変更。バージョン番号の管理形式を変更。

0.10 (2010.04.13)
-----------------
- [AWD] circleの軌跡追従制御式を修正(円が膨らまないように), Spur_set_angaccelの定義誤りを修正

0.9 ((2010.04.12))
------------------
- [AWD] spinの力学計算の間違いを修正

0.85 (2010.04.11)
-----------------
- [STK] sh-velのYPプロトコル通信部分のバッファオーバーフローのバグフィックス

0.8 (2010.04.11)
----------------
- [AWD] 受信と送信のスレッドを分けた
- [STK] set_accel, set_angaccelを追加。
- paramファイルが指定場所に無いときは/usr/local/share/の中を見てみるよう変更。

0.75 (2010.04.10)
-----------------
- [STK] 通信時にカウンタ値が送信されていないことのバグフィックス

0.65 (2010.04.09)
-----------------
- [AWD] coordinator起動時のプロトコルチェック時の通信関連バグフィックス

0.6 (2010.04.07)
----------------
- [AWD] coordinator起動時にVVコマンドで接続先のプロトコルをチェックするように変更

0.5 (2010.04.06)
----------------
- [AWD] サーボモードSTOP時(起動時・ウォッチドッグタイムアウト時)に、YP拡張コマンドを処理可能にした(VV:バージョン情報等の表示)

0.1 (2010.03.31)
----------------
- [STK] 加速度・角加速度設定のバグを修正、名前をSH-SpurからYP-Spurへ変更
