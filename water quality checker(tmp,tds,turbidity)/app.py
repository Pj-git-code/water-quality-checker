
from flask import Flask, render_template, jsonify
import json
import subprocess
import threading

app = Flask(__name__)

# Start sensor_writer.py in the background when app.py is launched
def start_sensor_writer():
    subprocess.Popen (["python", "sensor_writer.py"])

@app.route('/')
def dashboard():
    return render_template('dashboard.html')

@app.route('/data')
def data():
    try:
        with open("sensor_data.json", "r") as f:
            sensor_data = json.load(f)
        return jsonify(sensor_data)
    except Exception as e:
        return jsonify({"error": str(e)})

if __name__ == '__main__':
    # Run sensor_writer.py in the background when the Flask app starts
    start_sensor_writer()

    app.run(debug=True)