import tkinter as tk
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

LED_LIVING = 17
LED_BATHROOM = 27
LED_CLOSET = 22

// Set pins as OUTPUT
GPIO.setup(LED_LIVING, GPIO.OUT)
GPIO.setup(LED_BATHROOM, GPIO.OUT)
GPIO.setup(LED_CLOSET, GPIO.OUT)

// Function to turn ON only one LED
def turn_on(pin):
    GPIO.output(LED_LIVING, GPIO.LOW)
    GPIO.output(LED_BATHROOM, GPIO.LOW)
    GPIO.output(LED_CLOSET, GPIO.LOW)
    GPIO.output(pin, GPIO.HIGH)

// Function to turn ON all LEDs
def turn_all_on():
    GPIO.output(LED_LIVING, GPIO.HIGH)
    GPIO.output(LED_BATHROOM, GPIO.HIGH)
    GPIO.output(LED_CLOSET, GPIO.HIGH)

# GUI setup
root = tk.Tk()
root.title("Smart Home Lights")

# Individual buttons
btn1 = tk.Button(root, text="Living Room", command=lambda: turn_on(LED_LIVING))
btn1.pack(pady=5)

btn2 = tk.Button(root, text="Bathroom", command=lambda: turn_on(LED_BATHROOM))
btn2.pack(pady=5)

btn3 = tk.Button(root, text="Closet", command=lambda: turn_on(LED_CLOSET))
btn3.pack(pady=5)

# Button to turn all LEDs ON
btn_all = tk.Button(root, text="Turn All Lights ON", command=turn_all_on)
btn_all.pack(pady=5)

# Exit button
def exit_app():
    GPIO.cleanup()
    root.destroy()

exit_btn = tk.Button(root, text="Exit", command=exit_app)
exit_btn.pack(pady=10)

root.mainloop()
