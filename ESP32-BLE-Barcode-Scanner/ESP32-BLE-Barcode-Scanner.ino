#include <HardwareSerial.h>
#include <BleKeyboard.h> // Include the BLE keyboard library

// Create a BLE keyboard object
BleKeyboard bleKeyboard("Barcode_Reader", "ESP32 Manufacturer", 100);

// Create an instance of HardwareSerial for the GM67 barcode scanner (using Serial1)
HardwareSerial barcodeSerial(1); // Use Serial1 for the barcode scanner

// Battery voltage constants
const int batteryPin = 34;  // ADC pin where voltage divider is connected
const float fullBatteryVoltage = 1.98; // Voltage at 100% battery
const float emptyBatteryVoltage = 1.81; // Voltage at 0% battery
const float voltageDividerRatio = (10.0 + 44.0) / 44.0; // Voltage divider ratio

// FET control pin
const int fetPin = 12; // GPIO pin connected to the FET gate

// Function to read battery voltage and calculate percentage with predefined ranges
int getBatteryLevel() {
  const int numSamples = 10; // Number of samples to take for averaging
  int totalADC = 0;

  // Turn on the FET before reading the ADC
  digitalWrite(fetPin, HIGH);

  // Small delay to allow voltage to stabilize
  delay(10);

  for (int i = 0; i < numSamples; i++) {
    totalADC += analogRead(batteryPin);
    delay(10); // Small delay between readings to prevent noise
  }

  // Turn off the FET after reading the ADC
  digitalWrite(fetPin, LOW);

  // Average the ADC readings
  int avgADC = totalADC / numSamples;

  // Calculate battery voltage
  float batteryVoltage = (avgADC / 4095.0) * 3.3 * voltageDividerRatio; // Convert ADC to voltage

  // Map voltage to battery percentage (3.0V = 0%, 4.1V = 100%)
  int batteryPercentage = (int)((batteryVoltage - emptyBatteryVoltage) / (fullBatteryVoltage - emptyBatteryVoltage) * 100);

  // Ensure percentage is between 0 and 100
  batteryPercentage = constrain(batteryPercentage, 0, 100);

  Serial.print("Battery Voltage: "); // Debugging
  Serial.print(batteryVoltage);      // Print calculated voltage
  Serial.print("V, Battery Percentage: "); // Debugging
  Serial.println(batteryPercentage);  // Print calculated battery level

  // Apply the predefined ranges
  if (batteryPercentage > 90) {
    return 100; // Any value greater than 90% is reported as 100%
  } else if (batteryPercentage > 75) {
    return 75; // Between 98% and 75% is reported as 75%
  } else if (batteryPercentage > 50) {
    return 50; // Between 75% and 50% is reported as 50%
  } else if (batteryPercentage > 25) {
    return 25; // Between 50% and 25% is reported as 25%
  } else {
    return 0;  // Anything below 25% is reported as 0%
  }
}

void setup() {
  Serial.begin(115200); // Initialize Serial for debugging
  
  // Initialize HardwareSerial (Serial1) for GM67 communication (GPIO 13 for RX, TX not used)
  barcodeSerial.begin(9600, SERIAL_8N1, 13, -1);  // Baud rate 9600, GPIO 13 for RX, TX not used

  // Set the FET control pin as output
  pinMode(fetPin, OUTPUT);
  digitalWrite(fetPin, LOW); // Initially, turn off the FET

  // Start the BLE keyboard
  bleKeyboard.begin();

  // Set initial battery level
  int batteryLevel = getBatteryLevel();
  bleKeyboard.setBatteryLevel(batteryLevel);
}

void loop() {
  // Check if the BLE keyboard is connected to a device
  if (bleKeyboard.isConnected()) {
    // Set battery level in loop (in case the battery level changes over time)
    int batteryLevel = getBatteryLevel();
    bleKeyboard.setBatteryLevel(batteryLevel);

    // Check if data is available from the GM67 barcode scanner (Serial1)
    if (barcodeSerial.available()) {
      String barcodeData = "";

      // Read the data from the GM67 barcode scanner
      while (barcodeSerial.available()) {
        char receivedChar = barcodeSerial.read();
        barcodeData += receivedChar;
      }

      // Ensure the barcodeData is not empty and send it via BLE keyboard
      if (barcodeData.length() > 0) {
        bleKeyboard.print(barcodeData);
        bleKeyboard.write(KEY_RETURN); // Simulate pressing the Enter key
      }

      // Add a small delay for the BLE keyboard to process
      delay(50); 
    }
  }

  delay(50); // Small delay for readability
}
