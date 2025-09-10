import cv2
import requests
import json
import serial
import threading
import time
import csv
import os
import numpy as np
from collections import deque
from flask import Flask, Response, jsonify, request, render_template
from flask_socketio import SocketIO, emit
from flask_cors import CORS

ACCESS_TOKEN = ""
REST_API_KEY = "7ad4f404a0c487729049fe89e6d66c62"
REFRESH_TOKEN = "NYmWYmchZ7OLyqoTnhec4Uq9A9cLLCXJAAAAAgoNG5oAAAGY8-kbRqbXH4eeWQ3B"
TOKEN_URL = "https://kauth.kakao.com/oauth/token"

uart_buffer = ""
uartReciveData = ""
lastUartReciveData = ""
last_date = time.strftime("%Y-%m-%d")
SensorData_dict = {}
sendSensorData_dict = {}


ser = serial.Serial(port='/dev/serial0', baudrate= 115200, timeout= 0.5)

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

def save_frame(frame, prefix="event"):
    timestamp = time.strftime("%Y%m%d_%H%M%S")
    filename = f"{prefix}_{timestamp}.jpg"
    path = os.path.join('/root/winshare', filename)
    cv2.imwrite(path, frame)
    

def gen_frames():
    fireAlarm = False
    lastFireAlarm = False
    smokeAlarm = False
    lastSmokeAlarm = False
    
    while True:
        success, frame = cap.read()
        if not success:
            break
        fireDetect_sensor = int(SensorData_dict.get('F',0))
        smokeValue = int(SensorData_dict.get('S',0))
    
        if fireDetect_sensor == 1:
            fireAlarm = True
            if lastFireAlarm != fireAlarm:
               save_frame(frame, prefix="fire")
               lastFireAlarm = True
        else:
            fireAlarm = False
            lastFireAlarm = fireAlarm
                
        if smokeValue == 1:
            smokeAlarm = True
            if lastSmokeAlarm != smokeAlarm:
                save_frame(frame, prefix="smoke")
                lastSmokeAlarm = True
        else:
            smokeAlarm = False
            lastSmokeAlarm = smokeAlarm
                
        ret, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 95])
        frame = buffer.tobytes()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')


def sendWarningAlarm():
    fireAlarm = False
    lastFireAlarm = False
    smokeAlarm = False
    lastSmokeAlarm = False
    while True:
        global SensorData_dict
        fireDetect_sensor = int(SensorData_dict.get('F',0))
        smokeValue = int(SensorData_dict.get('S',0))
        
        if fireDetect_sensor == 1:
            fireAlarm = True
            if lastFireAlarm != fireAlarm:
                sendAlarm(ACCESS_TOKEN, f"⚠️화재가 감지되었습니다! \n일시:{time.strftime('%Y-%m-%d %H:%M:%S')}")
                lastFireAlarm = True
        else:
            fireAlarm = False
            lastFireAlarm = fireAlarm
                
        if smokeValue == 1:
            smokeAlarm = True
            if lastSmokeAlarm != smokeAlarm:
                sendAlarm(ACCESS_TOKEN, f"⚠️연기가 감지되었습니다! \n일시:{time.strftime('%Y-%m-%d %H:%M:%S')}")
                lastSmokeAlarm = True
        else:
            smokeAlarm = False
            lastSmokeAlarm = smokeAlarm
        
                      

def get_access_token():
    tokenInformation = {
        "grant_type": "refresh_token",
        "client_id": REST_API_KEY,
        "refresh_token": REFRESH_TOKEN
    }
    response = requests.post(TOKEN_URL, data=tokenInformation)
    
    if response.status_code == 200:
        token_info = response.json()
        access_token = token_info.get("access_token")
        print(f"[{time.strftime('%Y-%m-%d %H:%M:%S')}] 새 Access Token 발급 완료: {access_token}")
        # 응답에 refresh_token이 새로 오면 업데이트 필요
        
        if "refresh_token" in token_info:
            print("리프레시 토큰이 갱신되었습니다. 새 값으로 저장하세요.")
            print("새 refresh_token:", token_info["refresh_token"])
        return access_token
    else:
        raise Exception(f"토큰 발급 실패: {response.status_code} - {response.text}")
    
def sendAlarm(access_token, message):
   
    url = "https://kapi.kakao.com/v2/api/talk/memo/default/send"
    headers = {
        "Authorization": f"Bearer {access_token}"
    }
    data = {
        "template_object": json.dumps({
            "object_type": "text",
            "text": message,
            "link": {
                "web_url":"http://192.168.1.120:5000",
                "mobile_web_url":"http://192.168.1.120:5000"
            }
        })
    }
    
    
    response = requests.post(url, headers=headers, data=data)
    if response.status_code == 200:
        print("메시지 전송 성공")
    else:
        print(f"전송 실패: {response.status_code} - {response.text}")


def hour_renwal_access_token():
    global ACCESS_TOKEN
    while True:
        ACCESS_TOKEN = get_access_token()
        time.sleep(3600)

app = Flask(__name__)
app.config['SECRET_KEY'] = 'raspi_secret'
CORS(app)
socketio = SocketIO(app, cors_allowed_origins="*")

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/video_feed')
def video_feed():
    return Response(gen_frames(),mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/set_mode', methods=['POST'])
def set_mode():
    mode = request.json.get("mode", "AUTO")
    if ser:
        try:
            ser.write(f"MODE:{mode}\r\n".encode())
            return jsonify({"status": "ok", "sent": mode})
        except Exception as e:
            return jsonify({"error": str(e)}), 500
    return jsonify({"error": "Serial port not available"}), 503

# LED, FAN, WATER작동( ON / OFF)
@app.route('/set_active', methods=['POST'])
def set_active():
    device = request.json.get("device")
    action = request.json.get("action")
    if ser:
        try:
            ser.write(f"ACTIVE:{device}_{action}\r\n".encode())
            return jsonify({"status": "ok", "sent": f"{device}_{action}"})
        except Exception as e:
            return jsonify({"error": str(e)}), 500
    return jsonify({"error": "Serial port not available"}), 503


# LED ON / OFF 시간(Auto 모드용 스케줄 관리 구간)
@app.route('/set_time', methods=['POST'])
def set_time():
    on_time = request.json.get("on")
    off_time = request.json.get("off")
    if ser:
        ser.write(f"TIME:ON_{on_time}/OFF_{off_time}\r\n".encode())
        return jsonify({"status": "ok", "on": on_time, "off": off_time})
    return jsonify({"error": "Serial port not available"}), 503


# 온/습도/수분 기준값(Auto 모드용 설정)
@app.route('/set_value', methods=['POST'])
def set_value():
    temp = request.json.get("T")
    humid = request.json.get("H")
    moist = request.json.get("M")
    if ser:
        ser.write(f"VALUE:T_{temp}/H_{humid}/M_{moist}\r\n".encode())
        return jsonify({"status": "ok", "T": temp, "H": humid, "M": moist})
    return jsonify({"error": "Serial port not available"}), 503

def run_flask():
    app.run(host='0.0.0.0', port=5000, debug=False, use_reloader=False, threaded=True)

            
try: 
    if __name__ == '__main__':
        try:
            ACCESS_TOKEN = get_access_token()
        except Exception as e:
            print("토큰 발급 중 오류:", e, flush=True)
        
        run_flask_thread = threading.Thread(target=run_flask)
        run_flask_thread.daemon = True   
        run_flask_thread.start()
        
        sendWarningAlarm_thread = threading.Thread(target=sendWarningAlarm)
        sendWarningAlarm_thread.daemon = True   
        sendWarningAlarm_thread.start()
    
        hour_renwal_access_token_thread = threading.Thread(target=hour_renwal_access_token)
        hour_renwal_access_token_thread.daemon = True   
        hour_renwal_access_token_thread.start()
        
        
    while True:  
        date = time.strftime("%Y-%m-%d")
        date_time = time.strftime("%Y-%m-%d %H:%M:%S")
        fileName = f"/root/winshare/{date}_sensor_data.csv"
        header = ["Timestamp","Temperature","Humidity","Moisture","Fire","Smoke"]
        
        if  date != last_date:
            last_date = date
            fileName = f"{last_date}_sensor_data.csv"
            
        
        if ser.in_waiting > 0:
            uart_buffer += ser.read(ser.in_waiting).decode('utf-8', errors='ignore')

            # 줄바꿈이 있으면 완성된 줄만 처리
            while '\n' in uart_buffer:
                line, uart_buffer = uart_buffer.split('\n', 1)  # 첫 줄과 나머지 분리
                uartReciveData = line.strip()
                if uartReciveData:
                    lastUartReciveData = uartReciveData
                    print(f"수신: {uartReciveData}")


            
        SensorData = lastUartReciveData.split()
        
        if len(SensorData) < 7:  # 필요한 항목 개수보다 적으면
            continue  # 다음 루프로 넘어감
  
        for p in SensorData:
            if ':' in p:
                key, value = p.split(':',1)
                if not value.strip().isdigit():
                    value = '0'
                SensorData_dict[key] = value
                sendSensorData_dict[key] = value
        
        for key in ['T', 'H', 'M', 'F', 'S']:
            if key not in SensorData_dict:
                SensorData_dict[key] = '0'
                sendSensorData_dict[key] = '0'
    
        sendSensorData_dict.pop('TimeStamp',None)
        sendSensorData_dict.pop('Mode',None)
        
        if int(SensorData_dict.get('F',0)) == 1:
            fireDetect = "위험"
            sendSensorData_dict['F'] = "위험"
        else:
            fireDetect = "정상"
            sendSensorData_dict['F'] = "정상"
            
        if int(SensorData_dict.get('S',0)) == 1:
            smokeDetect = "위험"
            sendSensorData_dict['S'] = "위험"
        else:
            smokeDetect = "정상"
            sendSensorData_dict['S'] = "정상"
            
        socketio.emit("time_update", {'time': time.strftime('%Y-%m-%d %H:%M:%S')})      
        socketio.emit("sensor_update", sendSensorData_dict)
        
        
        csv_data = [
            date_time, 
            int(SensorData_dict.get('T',0)), 
            int(SensorData_dict.get('H',0)), 
            int(SensorData_dict.get('M',0)), 
            fireDetect, 
            smokeDetect
            ]
        

        write_header = not os.path.exists(fileName)
        
        try:   
            with open(fileName, mode='a', newline='', encoding='utf-8-sig') as file:
                writer = csv.writer(file)
                if write_header:
                    writer.writerow(header)
                writer.writerow(csv_data)
        except PermissionError:
            print(f"{fileName} 파일이 다른 프로그램에서 열려 있어 저장을 건너뜁니다.")
            


        time.sleep(1)   
    
except KeyboardInterrupt:
    print("종료합니다.")
        
finally:
    ser.close()
