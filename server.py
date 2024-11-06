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

# 기본 페이지에서 OTP 표시 및 자동 새로고침
@app.route("/", methods=["GET"])
def home():
    return f"""
    <html>
        <head>
            <meta http-equiv="refresh" content="10">
        </head>
        <body>
            <h1>현재 OTP: {otp}</h1>
            <p>페이지는 10초마다 자동으로 새로고침됩니다.</p>
        </body>
    </html>
    """

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
