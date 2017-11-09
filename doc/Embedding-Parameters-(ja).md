YP-Spur対応のモータドライバには、パラメータファイルの情報や、パラメータを埋め込める物があります。(T-frogプロジェクト TF-2MD3-R4/R6など)

# パラメータ名の指定

モータドライバのデバイスに、ターミナルソフト(Unix系の場合 `cat /dev/ttyACM? & cat - > /dev/ttyACM?` コマンド、Windowsの場合 TeraTermなど)で下記のコマンドを送ります。
```
$SETNAME[name_of_the_parameter_file]
$EEPROMSAVE
```

ypspur-coordinatorは、パラメータファイルが指定されていない場合、`[name_of_the_parameter_file].param` を読み込もうとします。
`[name_of_the_parameter_file]` が `embedded` の場合、ドライバに埋め込まれたパラメータファイルを読み込みます。

# パラメータファイルの埋め込み

まず、パラメータ名を `embedded` に指定します。
```
$SETNAMEembedded
$EEPROMSAVE
```

パラメータファイルの内容を書き込みます。
```
SETEMBEDDEDPARAM
parameter: value
parameter: value
parameter: value
parameter: value
parameter: value

```

パラメータの最後に、連続する改行を入力して下さい。
途中に改行が連続しないように注意します。
TF-2MD3-R4/R6の場合、最大のサイズは 512 bytes です。

# パラメータファイルの読み込み

```
GETEMBEDDEDPARAM
```
