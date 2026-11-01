# gar-board-m5sticks3

M5Stack StickS3 (K150) の独立した基板定義とArduino USB/LCDアダプター。
基板は端子・LCD・電源・物理ボタンを所有し、アプリの質問やセッションを解釈しません。

- 135×240 ST7789P3、非タッチ、A(GPIO11)・B(GPIO12)の2ボタン。
- 側面リセットは通常入力として扱いません。Plus2のGPIO4電源保持は使用しません。
- 電源管理はM5PM1。無線の実装先は今後の独立したBLE通信部品です。
- 現段階のアダプターはUSBのみ。BLE・SPP・Wi-Fiの実装は含みません。
- 公式資料から定義した初期構成です。実機ビルド・書き込み・電源制御は未検証です。

出典: https://docs.m5stack.com/en/core/StickS3
