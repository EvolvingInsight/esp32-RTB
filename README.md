
# ESP32 safe access controller (RTB) - Loïc Bouillon

This DIY project describes how to use an ESP32 microcontroller to manage the access to a safe box. For a given time and interval, one can access safe's content. It might prove useful in order to control an addiction. If you intend to install and build this project, consider reading all the instructions at least once.
<br>

## Main features are :

- multiple user can access the same safe, with different password.
- based on a real time clock.
- it logs into a microSD or internal memory the user who has opened the safe, also making it power outage proof.
- the use of a microSD gives the opportunity to store more data than in the EEPROM.
- minimum writing and reading to long-term storage.
- writes to a log to keep track of hardware failure or bugs.
<br><br>

### Things needed :

| Qty. | Component |
|---|---|
| 1 | Safe box |
| 1 | Espressif ESP32 Dev Module |
| 1 | Power supply for the ESP32 |
| 1 | Micro B USB cable |
| 1 | Relay 3v DC in |
| 1 | RTC DS3231 |
| 1 | MicroSD card (minimum 512MB) |
| 1 | MicroSD card adapter |
| 1 | Solder kit |
| 4 | Resistor 1/4w (about 1Kohm) |
| 4 | Buttons |
| 1 | 9v battery |

### How to install :

- **Verify the code**
	- Verify that the code suits you, change it to your needs. See *./src/DEFINITONS.hpp*.
- **Upload the code**
	- Upload the code to your ESP32 using the micro B USB cable. Software speaking, I'd rather use VScode combine with platform.io (extension).

### How to build :

- **Assemble the circuit**
	- With the help of *./doc/* and all the pictures in it, you should be able to connect everything together. See the [section below](#table-of-all-components-pin-connected-to-esp32s-pin) for a summary of all physical connection.
	- > **_NOTE:_** most buttons have a pull-down setup.
	- The other way around is to print the circuit itself, do so by using the RTB_gerber.zip located in *./doc/*.
- **Connect the circuit**
	- Before going any further, please verify that you don't need to pass a button already soldered to a wire, in a hole smaller than the button itself.
	- Then, connect the circuit to the relay and the said relay to a 9v battery (assuming the solenoid controlling the lock in your safe can take 9v, otherwise you'll need a 2 Watts resistor or more). Once again, see *./doc/* for an example.
	- Plug the power supply to your ESP32 and you should be ready to go.
- **Test**
	- Keep the door open and verify that everything works.
<br>

### Table of all components' pin connected to ESP32's pin :

| Component | Component's pin | ESP32's pin  |
|---|---|---|
| RTC | SCL | D22 (SCL, GPIO22) |
|   | SDA | D21 (SDA, GPIO21) |
|   | GND | GND |
|   | Vcc | 3.3v |
| - | - | - |
| SDcard | CS | IO5 |
|   | DI(MOSI) | IO23 |
|   | VSS1 | GND |
|   | VDD | 3.3v |
|   | SCLK | IO18 |
|   | VSS2 | GND |
|   | DO(MISO) | IO19 |
| - | - | - |
| Other | Button1 | GPIO12 |
|   | Button2 | GPIO13 |
|   | Button3 | GPIO14 |
|   | EnterPin | GPIO15 |
|   | LockPin | GPIO04 |

<br><br>

### Relevant upgrade that could be done :

- LCD screen to show remaining time.
- Visual indicator (LED, for instance) for any state of the ESP32.
- Buzzer to indicate wrong or good entry.
- Matrix keypad for a more complex and easy to remember password.
- Add an external Uninterrupted Power Supply (UPS) in case there is a need to use without electricity.
- Host a local web server and avoid physical buttons.
<br><br>

##### Notice the yellow writing on the picture below (they represent the buttons' order)...

<img src="./doc/esp32-installed.jpg" alt="Picture of ESP32 installed within the safe box" width="550" height="400">