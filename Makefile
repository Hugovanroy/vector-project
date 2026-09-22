# Build and test the flight computer

build:
	gcc firmware/flight_state.c -o flight

test: build
	python3 sim/flight_sim.py
	./flight

analyze:
	cppcheck --error-exitcode=1 firmware/flight_state.c

clean:
	rm -f flight flight_san