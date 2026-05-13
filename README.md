# Orange Pi Zero 3 Touch Shutdown Monitor

A lightweight, library-free C service that allows you to safely shut down an Orange Pi Zero 3 using a cheap TTP223 touch sensor. It provides visual feedback by hijacking the onboard Red LED to stay lit during the halt process.

## Features

* **Zero Dependencies:** Uses the Linux Sysfs interface directly.
* **Low Overhead:** Efficient C polling loop with minimal CPU usage.
* **Visual Feedback:** Automatically turns the onboard Red LED solid during shutdown to signal the process is active.
* **Modern Kernel Support:** Compatible with Debian/Armbian on Kernel 6.1+.

## Hardware Requirements

* Orange Pi Zero 3 (v1.2)
* TTP223 Touch Sensor Module
* 3x Jumper Wires (Female-to-Female)

## Wiring Diagram

The TTP223 module is powered directly from the Orange Pi's GPIO header.

| TTP223 Pin | Orange Pi Zero 3 Pin | Function          |
| ---------- | -------------------- | ----------------- |
| VCC        | Pin 1                | 3.3V Power        |
| GND        | Pin 9                | Ground            |
| SIG        | Pin 7                | GPIO PC9 (Signal) |

## Installation

### 1. Compile the Monitor

Clone this repository or copy the `power_monitor.c` file to your Orange Pi, then compile it:

```bash
gcc -o power_monitor power_monitor.c
sudo mv power_monitor /usr/local/bin/
sudo chmod +x /usr/local/bin/power_monitor
```

### 2. Setup Systemd Service

To ensure the monitor starts automatically at boot, create a service file:

```bash
sudo nano /etc/systemd/system/power_monitor.service
```

Paste the following configuration:

```ini
[Unit]
Description=Touch Shutdown Monitor
After=multi-user.target

[Service]
Type=simple
ExecStart=/usr/local/bin/power_monitor
Restart=always
User=root

[Install]
WantedBy=multi-user.target
```

### 3. Enable and Start

```bash
sudo systemctl daemon-reload
sudo systemctl enable power_monitor
sudo systemctl start power_monitor
```

## How It Works

1. **GPIO Monitoring:** The program exports GPIO 73 (PC9) via `/sys/class/gpio`.
2. **The Trigger:** When the TTP223 SIG pin goes HIGH (3.3V) upon touch, the program detects the value change.
3. **LED Hijack:** It sets the `red_led` trigger to `none` (disabling system control) and sets brightness to `1`.
4. **Safe Halt:** It executes the `poweroff` command, ensuring the filesystem is synced and unmounted safely before power is cut.

## Notes

* **Kernel 6.1+:** On many Debian-based firmwares for the H618, the Red LED is turned off by default after boot. This program ensures it turns back on to provide status.
* **Floating Pins:** If you use a mechanical switch instead of a touch module, remember to add a 10k resistor between Pin 7 and GND to prevent "ghost" triggers.

## License

MIT
