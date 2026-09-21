#include <DHT.h>

// Deklarasi Pin Fisik NodeMCU
const byte ldrPin   = A0; // Pin A0 untuk LDR
const byte dhtPin   = D5; // Pin D5 untuk DHT
const byte relayPin = D6; // Pin D6 untuk Relay
const byte ledPin   = D1; // Pin D1 untuk LED Indikator

// Tipe sensor DHT22 (sensor putih)
#define DHTTYPE DHT22 
DHT dht(dhtPin, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Konfigurasi pin aktuator
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // KONDISI AWAL (Default MATI)
  digitalWrite(relayPin, HIGH); // Relay Active-Low (HIGH = MATI)
  digitalWrite(ledPin, LOW);    // LED Indikator (LOW = MATI)

  Serial.println("\n==========================================");
  Serial.println("   Sistem Smart Warehouse Siap Dijalankan ");
  Serial.println("==========================================");
}

void loop() {
  // Jeda 2 detik wajib untuk sensor DHT
  delay(2000); 

  // Pembacaan sensor
  int ldrValue = analogRead(ldrPin);
  float temp = dht.readTemperature();

  // Print data LDR ke Serial Monitor
  Serial.print("Cahaya (ADC): ");
  Serial.print(ldrValue);

  // Cek apakah pembacaan DHT error (NaN)
  if (isnan(temp)) {
    Serial.println(" | Suhu: [DHT ERROR/KABEL LOOSE]");
    temp = 0.0; 
  } else {
    Serial.print(" | Suhu: ");
    Serial.print(temp);
    Serial.println(" °C");
  }

  // =========================================================================
  // TIPS TESTING: Batas ambang diturunkan/dinaikkan agar gampang dites
  // - Suhu > 20.0 (supaya suhu ruangan biasa langsung terdeteksi panas)
  // - ATAU LDR < 1023 (supaya dalam kondisi terang pun LDR langsung memicu nyala)
  // Ganti kembali nilainya sesuai modul praktikum setelah selesai uji coba!
  // =========================================================================
  if (temp > 20.0 || ldrValue < 1023) {
    digitalWrite(relayPin, LOW); // Relay Active-Low (LOW = ON / Berbunyi Klik)
    digitalWrite(ledPin, HIGH);  // LED Indikator ON

    Serial.println(">>> STATUS: PERINGATAN! Kondisi Buruk -> Relay & LED AKTIF!");
  } else {
    digitalWrite(relayPin, HIGH); // Relay Active-Low (HIGH = OFF)
    digitalWrite(ledPin, LOW);   // LED Indikator OFF

    Serial.println(">>> STATUS: Kondisi Ruangan Aman.");
  }
  
  Serial.println("------------------------------------------");
}