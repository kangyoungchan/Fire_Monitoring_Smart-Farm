// Socket.IO 연결 (센서 데이터 실시간 수신)
const socket = io("http://192.168.1.120:5000");

// 센서 데이터 수신 이벤트
socket.on("sensor_update", (data) => {
    console.log("실시간 센서 데이터:", data);
    document.getElementById('T').textContent = `${parseInt(data.T)} °C`;
    document.getElementById('H').textContent = `${parseInt(data.H)} %`;
    document.getElementById('M').textContent = `${parseInt(data.M)} %`;
    document.getElementById('F').textContent = data.F;
    document.getElementById('S').textContent = data.S;

    document.getElementById('F').style.color = data.F === "위험" ? "red" : "green";
    document.getElementById('S').style.color = data.S === "위험" ? "red" : "green";

    const now = new Date().toLocaleTimeString(); // 그래프 받아오는 곳
    addData(tempChart, now, parseInt(data.T));
    addData(humChart, now, parseInt(data.H));
    addData(moistChart, now, parseInt(data.M));


    let logs =  JSON.parse(localStorage.getItem("sensorLogs")) || []; //센서 데이터 localStorage에 저장
    logs.push({
        time: new Date().toLocaleString(),
        T: data.T,
        H: data.H,
        M: data.M,
        F: data.F,
        S: data.S
    });

    if (logs.length > 20) logs.shift();
    localStorage.setItem("sensorLogs", JSON.stringify(logs));

    if(document.getElementById("datalogContent").style.display !== "none"){
    showLogs();
}

});

function addData(chart, label, value) {
    chart.data.labels.push(label);
    chart.data.datasets[0].data.push(value);
    if (chart.data.labels.length > 10) {
        chart.data.labels.shift();
        chart.data.datasets[0].data.shift();
    }
    chart.update();
}

// 온도 그래프
let tempCht = document.getElementById('temperatureChart').getContext('2d');
let tempChart = new Chart(tempCht, {
    type: 'line',
    data: {
        labels: [],
        datasets: [
            { label: '온도 (° C)', data: [], borderColor: 'red', fill: false},
        ]
    },
    options: {
        responsive: true,
        animation: false,
        scales: {
            x: { title: { display: true, text: "시간"}},
            y: {
                beginAtZero: true,
                min:0,
                max:50
            }
        }   
    }
})

// 습도 그래프
let humCht = document.getElementById('humidityChart').getContext('2d');
let humChart = new Chart(humCht, {
    type: 'line',
    data: {
        labels: [],
        datasets: [
            { label: '습도 (%)', data: [], borderColor: 'blue', fill: false},
        ]
    },
    options: {
        responsive: true,
        animation: false,
        scales: {
            x: { title: { display: true, text: "시간"}},
            y: {
                beginAtZero: true,
                min:0,
                max:100 
            }
        }
    }
})

// 수분 그래프
let moistCht = document.getElementById('moistureChart').getContext('2d');
let moistChart = new Chart(moistCht, {
    type: 'line',
    data: {
        labels: [],
        datasets: [
            { label: '수분 (%)', data: [], borderColor: 'green', fill: false},
        ]
    },
    options: {
        responsive: true,
        animation: false,
        scales: {
            x: { title: { display: true, text: "시간"}},
            y: {
                beginAtZero: true,
                min:0,
                max:100 
            }
        }
    }
})

function showLogs(limit = 15) {
    const logs = JSON.parse(localStorage.getItem("sensorLogs")) || [];
    const recentLogs = logs.slice(-limit);

    const tableRows = recentLogs.map(log =>
         `<tr>
            <td>${log.time}</td>
            <td>${log.T} °C</td>
            <td>${log.H} %</td>
            <td>${log.M} %</td>
            <td style="color:${log.F === '위험' ? 'red':'green'}">${log.F}</td>
            <td style="color:${log.S === '위험' ? 'red':'green'}">${log.S}</td>
        </tr>
    `).join('');

    document.getElementById("logContainer").innerHTML = `
        <table border="1">
            <tr>
                <th>Time</th><th>Temperature</th><th>Humidity</th>
                <th>Moisture</th><th>Fire</th><th>Smoke</th>
            </tr>
            ${tableRows}
        </table>`;
}

function downloadCSV() {
    const logs = JSON.parse(localStorage.getItem("sensorLogs")) || [];
    if (!logs.length) return alert("저장된 로그가 없습니다.");

    const csv = "\uFEFF시간,온도(°C),습도(%),수분(%),화재,연기\n" +
        logs.map(l => `"${l.time}","${l.T}","${l.H}","${l.M}","${l.F}","${l.S}"`).join("\n");

    const blob = new Blob([csv], { type: "text/csv;charset=utf-8;" });
    const link = document.createElement("a");
    link.href = URL.createObjectURL(blob);
    link.download = "sensor_logs.csv";
    link.click();
    URL.revokeObjectURL(link.href);
}


// 시간 업데이트 이벤트
socket.on("time_update", (data) => {
    console.log("실시간 시간:", data);
    document.getElementById('timeValue').textContent = data.time;
});



// 모드 설정
function sendMode(mode) {
    fetch('http://192.168.1.120:5000/set_mode', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ mode: mode })
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => alert(`모드 전환 성공 -> ${data.sent}`))
    .catch(error => console.error("Error setting mode:", error));
}

// 장치 제어


// 자동 모드 시간 설정
function sendTime(on, off) {
    fetch('http://192.168.1.120:5000/set_time', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ on: on, off: off })
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => alert(`시간 설정 성공 -> ON:${data.on}, OFF:${data.off}`))
    .catch(error => console.error("Error setting time:", error));
}

// 자동 모드 값 설정
function sendValue(temp, hum, moist) {
    fetch('http://192.168.1.120:5000/set_value', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ T: temp, H: hum, M: moist })
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => alert(`값 설정 성공 -> ${data.T}/${data.H}/${data.M}`))
    .catch(error => console.error("Error setting value", error));
}


// 로그인 성공 시
document.getElementById('loginForm').addEventListener('submit', e => {
    e.preventDefault();
    const user = document.getElementById('username').value;
    const pass = document.getElementById('password').value;
    if (user === "admin" && pass === "admin123") {
        alert("로그인 성공!");
        document.getElementById('loginFormContainer').style.display = 'none';
        document.getElementById('mainContent').style.display = 'block';
        document.getElementById('userInfo').textContent = `${user}`;
    } else alert("아이디 또는 비밀번호가 틀렸습니다.");
});
document.getElementById('logoutBtn').addEventListener('click', () => {
    document.getElementById('mainContent').style.display = 'none';
    document.getElementById('loginFormContainer').style.display = 'block';
});



// 출력/입력/데이터출력 페이지 전환
document.getElementById('showOutputButton').addEventListener('click', function() {
    document.getElementById('outputContentContainer').style.display = 'flex';
    document.getElementById('inputContent').style.display = 'none';
    document.getElementById('datalogContent').style.display ='none';
});
document.getElementById('inputButton').addEventListener('click', function() {
    document.getElementById('outputContentContainer').style.display = 'none';
    document.getElementById('inputContent').style.display = 'block';
    document.getElementById('datalogContent').style.display ='none';
});
document.getElementById('showDataButton').addEventListener('click', function() {
    document.getElementById('outputContentContainer').style.display = 'none';
    document.getElementById('inputContent').style.display = 'none';
    document.getElementById('datalogContent').style.display = 'block';
    showLogs();
});


// 모드 전환
let currentMode = 'auto';
document.getElementById('autoButton').addEventListener('click', function() {
    currentMode = 'auto';
    alert("자동 모드로 전환되었습니다.");
    document.getElementById('autoSettings').style.display = 'none';
    document.getElementById('manualControls').style.display = 'none';
    sendMode('AUTO');
});
document.getElementById('controlButton').addEventListener('click', function() {
    currentMode = 'manual';
    alert("수동 모드로 전환되었습니다.");
    document.getElementById('autoSettings').style.display = 'block';
    document.getElementById('manualControls').style.display = 'block';
    sendMode('CONTROL');
});

// 수동 장치 제어
function controlDevice(device, action) {
    fetch('http://192.168.1.120:5000/set_active', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ device: device, action: action }) 
    })
    .then(response => {
        if (response.ok) alert(`${device} ${action} 명령 전송 성공`);
        else alert(`${device} ${action} 명령 전송 실패`);
    })
    .catch(error => {
        console.error('Error sending control command', error);
        alert('서버 통신 중 오류 발생');
    });
}

// 자동 모드 시간 적용
document.getElementById('setTimeButton').addEventListener('click', function() {
    const on = parseInt(document.getElementById('timeOn').value);
    const off = parseInt(document.getElementById('timeOff').value);
    sendTime(on, off);
});

// 자동 모드 센서 값 적용
document.getElementById('setValueButton').addEventListener('click', function() {
    const temp = parseInt(document.getElementById('tempVal').value);
    const hum = parseInt(document.getElementById('humVal').value);
    const moist = parseInt(document.getElementById('moistVal').value);
    sendValue(temp, hum, moist);
});


// 버튼 이벤트 등록 (수동 제어)
document.getElementById('ledOnButton').addEventListener('click', () => controlDevice('LED', 'ON'));
document.getElementById('ledOffButton').addEventListener('click', () => controlDevice('LED', 'OFF'));
document.getElementById('fanOnButton').addEventListener('click', () => controlDevice('FAN', 'ON'));
document.getElementById('fanOffButton').addEventListener('click', () => controlDevice('FAN', 'OFF'));
document.getElementById('waterOnButton').addEventListener('click', () => controlDevice('WATER', 'ON'));
document.getElementById('waterOffButton').addEventListener('click', () => controlDevice('WATER', 'OFF'));
