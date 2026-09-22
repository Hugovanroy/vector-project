#include <stdio.h>
typedef enum {
    PAD,
    BOOST,
    COAST,
    DESCENT,
    LANDED 
}  FlightState;

const char* flightStateToString(FlightState state) {
    switch (state) {
        case PAD: return "PAD";
        case BOOST: return "BOOST";
        case COAST: return "COAST";
        case DESCENT: return "DESCENT";
        case LANDED: return "LANDED";
        default: return "UNKNOWN";
    }
}

FlightState update_state(FlightState current, float altitude,
                         float accel_g, float max_altitude) {
    switch (current) {
        case PAD:
            if (accel_g > 2.0) return BOOST; 
            break;
        case BOOST:
            if (accel_g < 1.0) return COAST;
            break;
        case COAST:
            if (altitude < max_altitude - 1.0) return DESCENT;  
            break;
        case DESCENT:
            if (altitude < 1.0) return LANDED;    
            break;
        case LANDED:
            break;
    }
    return current;  
}   

void check(const char* name, FlightState got, FlightState expected) {
    if (got == expected) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s (expected %s, got %s)\n",
               name, flightStateToString(expected), flightStateToString(got));
    }
}

void run_tests(void) {
    check("liftoff on big shove",  update_state(PAD, 0, 5.0, 0),    BOOST);
    check("no false launch",       update_state(PAD, 0, 1.0, 0),    PAD);
    check("coast after burnout",    update_state(BOOST, 80, 0.5, 80), COAST);
    check("apogee reached",         update_state(COAST, 100, 0.0, 120), DESCENT);
    check("landing",                update_state(DESCENT, 0, 0.0, 120), LANDED);
}

void replay_flight(const char* filename) {
    FILE* file = fopen(filename, "r");     // open the file for reading
    if (file == NULL) {                    // always check it actually opened
        printf("Could not open %s\n", filename);
        return;
    }

    char header[100];
    fgets(header, sizeof(header), file);   // read and discard the header row

    FlightState current = PAD;
    float max_altitude = 0.0;
    float t, altitude, accel_g;

    // read one row at a time: three numbers separated by commas
    while (fscanf(file, "%f,%f,%f", &t, &altitude, &accel_g) == 3) {
        if (altitude > max_altitude) max_altitude = altitude;

        FlightState next = update_state(current, altitude, accel_g, max_altitude);
        if (next != current) {
            printf("t=%.1f  alt=%.1f  ->  %s\n", t, altitude, flightStateToString(next));
            current = next;
        }
    }

    fclose(file);
    printf("Replay finished. Final state: %s\n", flightStateToString(current));
}

int main(void) {
    run_tests();
    printf("\n--- Replaying simulated flight ---\n");
    replay_flight("sim/flight_data.csv");
    return 0;
}