import tkinter as tk
import RPi.GPIO as GPIO


GPIO.setmode(GPIO.BCM)

LED_LIVING = 17
LED_BATHROOM = 27
LED_CLOSET = 22

GPIO.setup(LED_LIVING, GPIO.OUT)
GPIO.setup(LED_BATHROOM, GPIO.OUT)
GPIO.setup(LED_CLOSET, GPIO.OUT)


def turn_on(pin):
    GPIO.output(LED_LIVING, GPIO.LOW)
    GPIO.output(LED_BATHROOM, GPIO.LOW)
    GPIO.output(LED_CLOSET, GPIO.LOW)
    
    GPIO.output(pin, GPIO.HIGH)

 // to GUI setup
root = tk.Tk()
root.title("Smart Home Lights")

Buttons
btn1 = tk.Button(root, text="Living Room", command=lambda: turn_on(LED_LIVING))
btn1.pack(pady=5)

btn2 = tk.Button(root, text="Bathroom", command=lambda: turn_on(LED_BATHROOM))
btn2.pack(pady=5)

btn3 = tk.Button(root, text="Closet", command=lambda: turn_on(LED_CLOSET))
btn3.pack(pady=5)

 // to Exit button
def exit_app():
    GPIO.cleanup()
    root.destroy()

exit_btn = tk.Button(root, text="Exit", command=exit_app)
exit_btn.pack(pady=10)

root.mainloop()