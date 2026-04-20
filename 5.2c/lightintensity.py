import tkinter as tk
import RPi.GPIO as GPIO

# Set GPIO mode to BCM (we use pin numbers like 18, 27, etc.)
GPIO.setmode(GPIO.BCM)

# Assign pins to each room LED
LED_LIVING = 18   # Living room (supports PWM for brightness)
LED_BATHROOM = 27
LED_CLOSET = 22

# Set all pins as output
GPIO.setup(LED_LIVING, GPIO.OUT)
GPIO.setup(LED_BATHROOM, GPIO.OUT)
GPIO.setup(LED_CLOSET, GPIO.OUT)

# Create PWM object for living room LED (for brightness control)
pwm = GPIO.PWM(LED_LIVING, 1000)  # 1000 Hz frequency
pwm.start(0)  # Start with LED turned off

# Function to turn ON only one room at a time
def turn_on(pin):
    # First, turn OFF all LEDs
    pwm.ChangeDutyCycle(0)  # stop living room light
    GPIO.output(LED_BATHROOM, GPIO.LOW)
    GPIO.output(LED_CLOSET, GPIO.LOW)

    # Now turn ON the selected room
    if pin == LED_LIVING:
        # Living room uses slider, so no direct ON here
        pass
    else:
        GPIO.output(pin, GPIO.HIGH)

# This function changes brightness based on slider value
def change_brightness(val):
    duty = int(val)  # get slider value (0–100)
    pwm.ChangeDutyCycle(duty)  # adjust brightness

# Create main window
root = tk.Tk()
root.title("Smart Home Lights")

# Button for living room
btn1 = tk.Button(root, text="Living Room",
                 command=lambda: turn_on(LED_LIVING))
btn1.pack(pady=5)

# Button for bathroom
btn2 = tk.Button(root, text="Bathroom",
                 command=lambda: turn_on(LED_BATHROOM))
btn2.pack(pady=5)

# Button for closet
btn3 = tk.Button(root, text="Closet",
                 command=lambda: turn_on(LED_CLOSET))
btn3.pack(pady=5)

# Slider to control brightness of living room light
slider = tk.Scale(root, from_=0, to=100,
                  orient=tk.HORIZONTAL,
                  label="Living Room Brightness",
                  command=change_brightness)
slider.pack(pady=10)

# Function to safely exit the program
def exit_app():
    pwm.stop()          # stop PWM
    GPIO.cleanup()      # reset all GPIO pins
    root.destroy()      # close GUI window

# Exit button
exit_btn = tk.Button(root, text="Exit", command=exit_app)
exit_btn.pack(pady=10)

# Start the GUI loop
root.mainloop()