from flask import Flask,render_template,Response, request

app=Flask(__name__)
values = []
@app.route('/fsr_sensor', methods = ['POST', 'GET'])
def display():
    if request.method == 'POST':
        text = request.get_json()
        print(text)
    elif request.method == 'GET':
        print("GET request performed!")
    return "Success"
if __name__ == "__main__":
    app.run(host='0.0.0.0')