from flask import Flask,render_template,Response, request
import json
app=Flask(__name__)


text_arr = []
@app.route('/fsr_sensor', methods = ['POST', 'GET'])
def display():
    if request.method == 'POST':
        
        text = request.get_json()
        print(text)
        text_arr.append(text)
        return "Success"
    elif request.method == 'GET':
        return text_arr[-1]
    
if __name__ == "__main__":
    app.run(host='0.0.0.0')