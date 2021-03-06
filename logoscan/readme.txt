-----------------------------------------------------------------------
      ロゴ解析 プラグイン     ver 0.07      by MakKi
-----------------------------------------------------------------------

【機能】

  ロゴの含まれる画像からロゴを解析し、透過性ロゴフィルタ用のロゴデータを作成します。

【特徴】

  ロゴの含まれる画像から自動でロゴを解析します。
  解析結果のロゴデータは、ロゴデータファイル(*.lgd)として保存できます。
  また、透過性ロゴフィルタに直接データを渡すこともできます。

【インストール】

  logoscan.auf を AviUtl の認識可能なフォルダにコピーしてください。

【使用方法】

  ◇パラメータ
    ・位置X : ロゴ範囲の右上の座標
    ・位置Y : ロゴ範囲の左上の座標
    ・幅    : ロゴ範囲の幅
    ・高さ  : ロゴ範囲の高さ
    ・閾値  : 背景が単一色かを判定するための閾値。高いほうが判定が甘くなる

    ・有効フレームをマーク     : 有効フレームをマークする
    ・有効フレームリストを保存 : 有効フレームリストをテキストファイルとして保存する

  ◇解析手順
    �@ ロゴの含まれる画像をAviUtlに読み込みます。
       ロゴがフェードしている場面は「選択範囲を削除」等で除いてください
    �A フィルタを有効にすると設定ウィンドウのスライダーおよびマウスで画像の範囲を選択できるようになります。
    �B ロゴが完全に収まるように範囲を選択して、ロゴ解析 ボタンを押します。
       有効フレームリストを保存にチェックを入れていると、ここで出力ファイル名を聞いてきます。
    �C 解析が完了すると、ロゴ解析結果ダイアログが表示されます。
    �D ロゴの画像を確認し、ロゴの名称を入力します。
    �E 送信 ボタンを押すとロゴデータを直接 透過性ロゴ フィルタに渡します。
       保存 ボタンではロゴデータファイル(*.lgd)としてデータを保存します。
    �F 閉じるボタンでダイアログを閉じます。

【注意】

  AviUtl 0.98d 以降専用
  このプログラムはフリーソフトウェアです。
  このプログラムによって損害を負った場合でも、作者は責任を負いません。

【配布元】

  MakKi's SoftWare
  http://mksoft.hp.infoseek.co.jp/

【更新履歴】
  2008/01/07   ver 0.07  ・ロゴサイズの制限を撤廃
                         ・ロゴファイルの最大データ数拡張に対応
  2003/10/26   ver 0.06b ・フレームリスト保存ダイアログでキャンセルすると落ちるバグ修正。
  2003/10/21   ver 0.06a ・自前でRGB->YCbCrするようにした。(SSE2での不具合対策)
                         ・有効フレームリストを保存のチェックが入っていない時動かないバグ修正。
                         ・ログファイルのデフォルト名をソースファイル名からつくるようにした。
  2003/10/19   ver 0.06  ・有効フレームをマーク･ログファイル出力できるようにした。
  2003/10/01   ver 0.05  ・解析終了時にビープを鳴らすようにした。
                         ・その他細かな修正とちょっとだけ高速化。
  2003/07/03   ver 0.04  ・正式版公開
                         ・ロゴ解析を中断できるようにした
  2003/06/18   ver β03a ・最初のフレームを表示していると解析できないバグ修正
                         ・結果ダイアログのプレビュー背景色をRGBで指定するように変更
  2003/06/17   ver β03  ・エラーメッセージを一部修正
                         ・ロゴ範囲の形に色が変わってしまうバグ修正
                         ・背景が単一色かどうかを内部で判定するようにした
  2003/05/10   ver β02  ・背景色計算の精度を向上（してたらいいなぁ）
                         ・解析が255フレームを超えると落ちるバグを修正
                         ・ロゴ範囲の最大値を約４倍にした
  2003/04/29   ver β01  ・テスト版公開


mailto:makki_d210@yahoo.co.jp