#include "MiniPID.h"
#include "MPA.h"
#include <chrono>
#include <iostream>
#include <thread>

#define USE_PID true // turn on/off the PID control implemention with MiniPID.
#define STEP_TEST false // send step signals to valve

/*
   Test program for actuating a single valve.
   Can test out PID controller.
   Outputs graph of pressure profile as output.

         example_forceController.cpp has similar function but uses the
   forceController class, and can actuate multiple valves.
 */

int step_func(int i) {
    // step function to send to valve
    if (i % 1500 < 750) {
        return 300;
    } else {
        return 450;
    }
}

int main() {
    // Create MPA controller.
    MPA mpa("192.168.1.101", "502");
    unsigned int valveNum = 0;          // test valves 0 to 15
    unsigned int commandPressure = 450; // tested 0 to 1000 mbar
    unsigned int endPressure = 0;

    // Ziegler-Nichols method
    double Ku = 2.6;
    double Tu = 0.14;
    double KP = 0.6 * Ku;
    double KI = KP / (Tu / 2.0) * 0.002;
    double KD = KP * Tu / 2.0 / 0.002;

    MiniPID pid(KP, KI, KD); // create PID controller
    pid.setOutputLimits(
            50); // setting a good output limit is important so as not oscillate

    // Connect.
    if (!mpa.connect()) {
        std::cout << "Failed to connect to MPA." << std::endl;
        return -1;
    }

    // Set valve 0 to 1 bar.
    mpa.set_single_pressure(valveNum, commandPressure);
    int sensorvalue;
    double output;

    int cycles = 1000;
    std::vector<double> x(cycles), pressures(cycles),
            commandpressures(cycles); // for logging pressure profile
    for (int i = 0; i < cycles; i++) {
        // Wait 1 ms.
        // std::this_thread::sleep_for(std::chrono::millisecofalsends(1));

        if (STEP_TEST) {
            commandPressure = step_func(i);
        }
        commandpressures.at(i) = commandPressure;
        // Read pressure of valve 0.
        sensorvalue = mpa.get_single_pressure(valveNum);
        x.at(i) = i;
        pressures.at(i) = sensorvalue;
        output = commandPressure + pid.getOutput(sensorvalue, commandPressure);

        if (USE_PID) {
            mpa.set_single_pressure(valveNum, static_cast<int>(output));
        } else {
            mpa.set_single_pressure(valveNum, commandPressure);
        }

        if (i % 50 == 0) {
            std::cout << "Valve " << valveNum << " sensor: " << sensorvalue
                      << " mbar\toutput: " << output << " mbar" << std::endl;
        }
    }

    // Set valve 0 to 0 bar (off).
    mpa.set_single_pressure(valveNum, endPressure);

    // Wait 100 ms.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Read pressure of valve 0.
    std::cout << "Valve " << valveNum << " :" << mpa.get_single_pressure(valveNum)
              << " mbar" << std::endl;

    // Disconnect.
    mpa.disconnect();

    return 0;
}
