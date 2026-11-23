// Line Follower - Garis HITAM, lantai PUTIH
// Versi dengan Serial Monitor (untuk kalibrasi)

// Sensor IR
#define IR_LEFT   12
#define IR_RIGHT  11

// Motor kanan
#define ENA  6
#define IN1  7
#define IN2  8

// Motor kiri
#define IN3  9
#define IN4  10
#define ENB  5

// ---- PENGATURAN ----
#define GARIS_HITAM  HIGH   // ubah ke LOW jika logika sensor terbalik

#define SPEED_MAJU    120
#define SPEED_LUAR    140
#define SPEED_DALAM    70
#define SAMPEL          5

int arahTerakhir = 0;       // -1 kiri, 1 kanan, 0 lurus

void setup() {
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);       // <-- Serial aktif
  berhenti();
  delay(2000);
}

void loop() {
  bool kiri  = bacaGaris(IR_LEFT);
  bool kanan = bacaGaris(IR_RIGHT);

  // Tampilkan di Serial Monitor
  Serial.print("Kiri: ");
  Serial.print(kiri);
  Serial.print(" | Kanan: ");
  Serial.println(kanan);

  if (!kiri && !kanan) {
    motor(SPEED_MAJU, SPEED_MAJU);
    arahTerakhir = 0;
  }
  else if (kiri && !kanan) {
    motor(-SPEED_DALAM, SPEED_LUAR);
    arahTerakhir = -1;
  }
  else if (!kiri && kanan) {
    motor(SPEED_LUAR, -SPEED_DALAM);
    arahTerakhir = 1;
  }
  else {
    if (arahTerakhir == -1)      motor(0, SPEED_MAJU);
    else if (arahTerakhir == 1)  motor(SPEED_MAJU, 0);
    else                         motor(SPEED_MAJU, SPEED_MAJU);
  }

  delay(50);                // supaya tulisan tidak terlalu cepat
}

bool bacaGaris(int pin) {
  int hitung = 0;
  for (int i = 0; i < SAMPEL; i++) {
    if (digitalRead(pin) == GARIS_HITAM) hitung++;
  }
  return hitung > (SAMPEL / 2);
}

void motor(int kiri, int kanan) {
  // Motor kiri (tetap seperti kode pertama)
  digitalWrite(IN3, kiri >= 0 ? HIGH : LOW);
  digitalWrite(IN4, kiri >= 0 ? LOW : HIGH);
  analogWrite(ENB, constrain(abs(kiri), 0, 255));

  // Motor kanan (arah DIBALIK karena polaritas kabel berlawanan)
  digitalWrite(IN1, kanan >= 0 ? LOW : HIGH);
  digitalWrite(IN2, kanan >= 0 ? HIGH : LOW);
  analogWrite(ENA, constrain(abs(kanan), 0, 255));
}

void berhenti() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
