# ESP32 Deep Sleep Logger

## Engineering evidence

- `tools/battery_life.py` estimates average current and battery runtime from wake and sleep intervals.
- Run `python tools/battery_life.py --period-s 900` to evaluate a 15-minute logging schedule.
- The project demonstrates energy-aware firmware design rather than treating deep sleep as a single API call.

## Objective

Build a low-power temperature logger that wakes from deep sleep, takes one reading, publishes it (or prints it), then returns to sleep. The project demonstrates the ESP32 timer wakeup API and the power-management considerations needed for battery operation.

## Strategy

- Use the ESP32 deep-sleep timer to wake after a fixed interval (e.g. 60 s).
- On wake, initialize only the peripherals that are required for the sample.
- Print or publish the reading, then call esp_deep_sleep_start().
- Keep the awake window as short as possible to maximize battery life.

## What worked

- Timer wakeup with a 60-second period produced a predictable duty cycle.
- Reading the DHT only once per wake cycle kept the radio and sensor power time low.
- Serial output at the start of each wake made it easy to confirm the device was still alive during bench tests.

## What failed and how it was resolved

- Calling WiFi or MQTT before the radio was ready after deep sleep caused crashes. Resolution: for the first version, only print to serial; add networking only after a stable wake-and-sample path is proven.
- Forgetting to disable the brown-out detector in some deep-sleep configurations caused immediate reboots. Resolution: leave the default BOD settings and document the need to test on the actual power supply.
- Using a long String for the log line increased heap usage on every wake. Resolution: use a fixed buffer or simple Serial.print calls.

## Engineering principles and frameworks used

- Duty-cycled operation for power reduction.
- Timer-based deep sleep.
- Minimal work while awake.
- Explicit power-state management.

## Hardware

- ESP32 development board
- DHT22 sensor on GPIO 4
- USB power for development; battery for field use

## Software

PlatformIO or Arduino IDE. The sketch prints temperature and humidity then sleeps for 60 seconds.

## Possible extensions

- Store readings in RTC memory or NVS across sleep cycles.
- Wake on external GPIO as well as timer.
- Publish over MQTT only when a certain number of samples have been collected.
