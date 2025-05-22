# 📦 ESP32-BLE-Barcode-Scanner

An open-source ESP32-based **Bluetooth Low Energy (BLE)** barcode scanner that reads data from a **GM67 barcode module** and transmits it as HID keyboard input to any connected device — ideal for use in **POS systems** like **SmartPOS**.

---

## 🚀 Features

- 🔹 Reads barcode data via UART from GM67 scanner module  
- 🔹 Sends barcode as BLE HID keyboard input  
- 🔋 Monitors battery voltage and reports status to connected BLE host  
- 🔌 Controls FET for low-power battery sensing  
- ✅ Compatible with Android, Windows, macOS, and other BLE-supported devices  
- 🛒 Designed for seamless integration with SmartPOS systems  

---

## 📷 Hardware Used

- **ESP32 Dev Board**  
- **GM67 Barcode Scanner Module**  
- **Battery and FET circuitry**  
- Voltage divider for battery voltage sensing  

---

## 🧠 How It Works

1. **Barcode data** is received via `Serial1` (e.g., GPIO13)  
2. ESP32 acts as a **BLE HID keyboard** using the modified `BleKeyboard` library  
3. Battery voltage is sampled and converted to percentage using ADC + FET logic  
4. The scanned barcode is typed out on the connected device, followed by an `Enter` key press  

---

## 📁 Repository Structure


ESP32-BLE-Barcode-Scanner/
├── src/
│ └── main.ino # Main Arduino code
├── lib/
│ └── BleKeyboard/ # Modified BLE HID library
├── LICENSE # MIT License
└── README.md # Project documentation



---

## 🔧 Getting Started

1. **Install Arduino IDE**  
2. Add ESP32 board support via the Board Manager  
3. Clone this repo and open `main.ino`  
4. Place the `BleKeyboard` folder in your Arduino `libraries/` directory  
5. Connect your ESP32 and upload the code  

📌 **Note**: Use GPIO13 for RX from the GM67 scanner. TX is not used.

---

## 🪫 Battery Percentage Mapping

| Voltage Range | Reported Battery % |
|---------------|--------------------|
| > 1.98V       | 100%               |
| 1.91V–1.98V   | 75%                |
| 1.86V–1.91V   | 50%                |
| 1.81V–1.86V   | 25%                |
| < 1.81V       | 0%                 |

---

## 📜 License

This project is licensed under the **MIT License** – see the [LICENSE](LICENSE) file for details.

---

## 🤝 Contributions

Pull requests, feature suggestions, and bug reports are welcome!

---

## 🙌 Acknowledgments

Thanks to the developers behind the original [`BleKeyboard`](https://github.com/T-vK/ESP32-BLE-Keyboard) library.

---

## 🔗 Author

**Gayan De Silva**  
🔧 GitHub: [@gayandsilva](https://github.com/gayandsilva)  
📬 LinkedIn: [linkedin.com/in/gayandsilva](https://www.linkedin.com/in/ahtgayandesilva)
