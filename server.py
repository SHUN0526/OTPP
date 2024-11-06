from flask import Flask, request, jsonify
import random
import time
from threading import Thread

app = Flask(__name__)
otp = None

# 1분마다 OTP를 업데이트하는 함수
def update_otp():
    global otp
    while True:
        otp = str(random.randint(100000, 999999))  # 6자리 랜덤 OTP 생성
        print(f"새로운 OTP: {otp}")  # 디버깅용 출력
        time.sleep(60)  # 1분마다 갱신

# Flask 서버 시작
@app.route("/get-otp", methods=["GET"])
def get_otp():
    return jsonify({"otp": otp})

@app.route("/verify-otp", methods=["POST"])
def verify_otp():
    data = request.get_json()
    input_otp = data.get("otp")
    if input_otp == otp:
        return jsonify({"status": "success"})
    else:
        return jsonify({"status": "failed"})

if __name__ == "__main__":
    Thread(target=update_otp).start()  # OTP 업데이트 스레드 시작
    app.run(host="0.0.0.0", port=5000)  # 서버 실행
