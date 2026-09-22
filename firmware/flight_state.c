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

int main(void) {
        run_tests();
    float altitudes[] = {0, 0, 10, 40, 80, 110, 120, 115, 80, 40, 10, 0};
    float accels[]    = {1, 1,  1,  1,  1,   1,   1,   1,  1,  1,  1, 1};
    int num_readings = 12;

    FlightState current = PAD;
    float max_altitude = 0.0;

    for (int i = 0; i < num_readings; i++) {
        float alt = altitudes[i];
        float acc = accels[i];

        if (alt > max_altitude) max_altitude = alt;

        FlightState next = update_state(current, alt, acc, max_altitude);

        if (next != current) {
            printf("Reading %d: altitude=%.0f accel=%.1f  ->  %s\n",
                   i, alt, acc, flightStateToString(next));
            current = next;
        }
    }

    printf("Final state: %s\n", flightStateToString(current));
    return 0;
}