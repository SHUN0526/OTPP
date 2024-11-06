#include <WiFi.h>
#include <HTTPClient.h>
#include <Keypad.h>

const char* ssid = "이승훈의 iPhone";         // Wi-Fi SSID
const char* password = "0011001100"; // Wi-Fi 비밀번호
const String server_url = "http://172:5000";

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

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi 연결 중...");
  }
  Serial.println("WiFi 연결 성공");
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
        // 도어락 열림 신호
      } else {
        Serial.println("OTP 인증 실패 - 잘못된 OTP");
      }
    } else {
      Serial.println("서버 연결 오류");
    }
    http.end();
  }
}
