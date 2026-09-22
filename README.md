# Vector

A model rocket flight computer I'm building to learn embedded and flight software.

It reads altitude and acceleration and figures out what phase the rocket is in such as on the pad, boosting, coasting, or falling and fires the parachute at the top of the flight.

Right now it all runs in software, a Python simulator generates a fake flight and feeds it through the flight computer, so I can test everything without launching anything. STM32 is next

## What's in here
- `firmware/` — the flight computer logic in C. the state machine, apogee detection, and unit tests
- `sim/` — a physics-based flight simulator in Python that generates test flights
- A CI pipeline that builds, tests, and runs static + dynamic analysis on every push

## Running it
make test # build and run the whole thing
make analyze # static analysis
make clean # remove build files
