import requests
r = requests.get("http://192.168.0.101:5000/fsr_sensor")
print(r.content)