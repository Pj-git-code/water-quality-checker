
import serial
import time
import json

arduino_port = 'COM6'  # Update to your actual port
baud_rate = 9600
ser = serial.Serial(arduino_port, baud_rate, timeout=1)
time.sleep(2)

print("Reading sensor data from Arduino...\n")

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            print("Received:", line)

            try:
                parts = line.split('|')

                temp_value = float(parts[0].split(':')[1].strip().replace("°C", ""))
                tds_value = float(parts[1].split(':')[1].strip().replace("ppm", ""))
                turb_value = float(parts[2].split(':')[1].strip().replace("NTU", ""))  # ✅ fix here

                # Store clean values in JSON
                data = {
                    "temperature": f"{temp_value:.2f} ",
                    "turbidity": f"{turb_value:.2f} ",
                    "tds": f"{tds_value:.2f} "
                }

                with open("sensor_data.json", "w") as f:
                    json.dump(data, f, indent=4)

                print(f"✅ Saved JSON: {data}")

            except (IndexError, ValueError) as parse_error:
                print("❌ Parsing error:", parse_error)

except KeyboardInterrupt:
    print("Stopped by user.")

finally:
    ser.close()