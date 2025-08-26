


> **Note:** All address iterations start from zero.  
> **Communication:** The PC acts as the Modbus **Master** while the controller functions as the **Slave**.  
> **Limitations:** The device connected to the controller does *not* support reading/writing coils or discrete input bits, nor writing multiple holding registers (function code `0x10`).
> **baud-rate:** 115200
All register values are stored as `uint16_t` (16-bit unsigned integers).

Reading input registers (0–16) provides measurement data from both the Multimeter and the Load Bank.

# Modbus Input Registers (0–16)

## Multimeter Measurements (Registers 0–3)

| Register Address | Variable    | Description                |
|-----------------:|-------------|----------------------------|
| 0                | AC_Voltage  | AC voltage (mV or V)       |
| 1                | AC_Current  | AC current (mA or A)       |
| 2                | AC_Power    | AC power (W)               |
| 3                | AC_Freq     | AC frequency (Hz)          |

## Load Bank Measurements (Registers 4–16)

| Register Address | Variable            | Description                               |
|-----------------:|---------------------|-------------------------------------------|
| 4                | slave_address       | Device Modbus address                     |
| 5                | Operating Mode      | 0=CV, 1=CC, 2=CW, 3=CR, 4=CV+CC           |
| 6                | Test Enable Switch  | 1=ON, 0=OFF        |
| 7                | temp_q1             | Temp sensor 1 (°C )                       |
| 8                | temp_q2             | Temp sensor 2 (°C )                       |
| 9                | in_volt             | Measured voltage                          |
| 10               | DC_Voltage          | DC voltage (Vx100)                        |
| 11               | DC_Current          | DC current (Ax1000)                       |
| 12               | DC_Power            | DC power (Wx100)                          |
| 13               | q_block             | Measurement block value                   |
| 14               | q_cap               | Capacity value                            |
| 15               | Watt_Hour           | Energy (Wh)                               |
| 16               | ok_time             | Status time/count                         |



---

# Modbus Holding Registers (0–8)

> **Note:** Holding registers start at zero and are used for configuration and control.

## Multimeter Setpoints (Registers 0–1)

| Register Address | Name             | Description                         |
|-----------------:|------------------|-------------------------------------|
| 0                | AC_Current_SP    | Set AC current (A×1000)             |
| 1                | AC_Voltage_SP    | Set AC voltage (V×100)              |

## Load Bank Configuration (Registers 2–8)

| Register Address | Name                   | Description                         |
|-----------------:|------------------------|-------------------------------------|
| 2                | Operating Mode         | 0=CV,1=CC,2=CW,3=CR,4=CV+CC         |
| 3                | Test Enable Switch     | 0=OFF,1=ON                          |
| 4                | Set Voltage (CV)       | Voltage setpoint (V×100)            |
| 5                | Set Current (CC)       | Current setpoint (A×1000)           |
| 6                | Set Power (CW)         | Power setpoint (W×100)              |
| 7                | Set Resistance (CR)    | Resistance setpoint (Ω×10)          |
| 8                | Auxiliary Functions    | e.g. 1=fan, 2=buzzer, 10=reset Wh ,70= Toggle between temp 1,2  sensors ,  120 =save settings



# Operation Instructions

## Multimeter Operation
1. Set the desired **AC_Current_SP** or **AC_Voltage_SP** using holding registers 0 or 1.
2. These values represent the stimulus to be applied to the **DUT (Device Under Test)**.
3. Monitor the response using input registers 0–3 (voltage, current, power, frequency).


## Load Bank Operation
**Note:** You can activate either the buzzer or the fan via the `Auxiliary Functions` register (8). Avoid sending `120` unless you're sure about saving settings to non-volatile memory.

## Load Bank Operation
1. Choose the **Operating Mode** by writing to holding register 2:
   - `0` → Constant Voltage (CV) default mode 
   - `1` → Constant Current (CC)
   - `3` → Constant Resistance (CR)
   - `4` → Hybrid Mode (CV + CC)
2. Set the corresponding parameters — `Set Voltage (CV)`, `Set Current (CC)`, `Set Power (CW)`, and `Set Resistance (CR)` in registers 4–7 Respectivly .
3. Enable the load by writing `1` to **Test Enable Switch** (holding register 3).
4. Use input registers 4–16 to monitor temperature, voltage, current, power, and energy.
5. Disable the load by writing `0` to **Test Enable Switch** (holding register 3). after finishing the test.