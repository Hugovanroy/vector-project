# Rocket flight simulator -> writes a recorded flight to a CSV file

dt = 0.1
gravity = -9.8
altitude = 0.0
velocity = 0.0
t = 0.0
thrust = 30.0

output = open("sim/flight_data.csv", "w")   # open a file for writing
output.write("time,altitude,accel_g\n")      # column names (header row)

# --- Pad: sit still for 1 second (accelerometer reads 1 g) ---
while t < 1.0:
    accel_g = 1.0
    output.write(f"{t:.1f},{altitude:.1f},{accel_g:.1f}\n")
    t = t + dt

# --- Boost: motor burns for 2 seconds ---
while t < 3.0:
    accel = thrust + gravity
    velocity = velocity + accel * dt
    altitude = altitude + velocity * dt
    accel_g = thrust / 9.8            # big shove while thrusting (~3 g)
    output.write(f"{t:.1f},{altitude:.1f},{accel_g:.1f}\n")
    t = t + dt

# --- Coast: freefall until it lands (accelerometer reads ~0 g) ---
while altitude > 0:
    accel = gravity
    velocity = velocity + accel * dt
    altitude = altitude + velocity * dt
    accel_g = 0.0
    output.write(f"{t:.1f},{altitude:.1f},{accel_g:.1f}\n")
    t = t + dt

output.close()
print("Flight data written to sim/flight_data.csv")