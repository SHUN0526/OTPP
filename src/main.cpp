#include <WiFi.h>
#include <HTTPClient.h>
#include <Keypad.h>

const char* ssid = "이승훈의 iPhone";         // Wi-Fi SSID
const char* password = "0011001100";                   // Wi-Fi 비밀번호
const String server_url = "http://172.20.10.2:5000"; // 서버 URL

// 키패드 설정
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = { 19, 18, 5, 17 };
byte colPins[COLS] = { 16, 4, 0, 2 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int buzzerPin = 13; // 부저가 연결된 핀

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(buzzerPin, OUTPUT);  // 부저 핀을 출력 모드로 설정

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi 연결 중...");
  }
  Serial.println("WiFi 연결 성공");
}

// 맞았을 때 소리
void playSuccessSound() {
  tone(buzzerPin, 2000); // 높은 소리 (2000Hz)
  delay(200);            // 짧게 0.2초 울림
  noTone(buzzerPin);
  delay(100);            // 잠깐 멈춤
  tone(buzzerPin, 2000); // 다시 높은 소리
  delay(200);
  noTone(buzzerPin);
}

// 틀렸을 때 소리
void playWarningSound() {
  tone(buzzerPin, 1000); // 낮은 소리 (1000Hz)
  delay(500);            // 길게 0.5초 울림
  noTone(buzzerPin);
  delay(100);            // 잠깐 멈춤
  tone(buzzerPin, 1000); // 다시 낮은 소리
  delay(500);
  noTone(buzzerPin);
}

void loop() {
  // OTP 입력 처리
  String input_otp = "";
  Serial.println("OTP를 입력하세요:");
  while (input_otp.length() < 6) {
    char key = keypad.getKey();
    if (key) {
      input_otp += key;
      Serial.print(key);  // 입력된 키 출력
    }
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server_url + "/verify-otp");
    http.addHeader("Content-Type", "application/json");

    String requestData = "{\"otp\":\"" + input_otp + "\"}";
    int httpResponseCode = http.POST(requestData);

    if (httpResponseCode == 200) {
      String response = http.getString();
      if (response.indexOf("success") >= 0) {
        Serial.println("OTP 인증 성공 - 도어락 열림");
        playSuccessSound(); // 인증 성공 시 소리
      } else {
        Serial.println("OTP 인증 실패 - 잘못된 OTP");
        playWarningSound(); // 인증 실패 시 소리
      }
    } else {
      Serial.println("서버 연결 오류");
    }
    http.end();
  }
}
