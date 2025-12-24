# Hello World - Nucleo H723ZG

Blinky LED + UART character echo demo for Zephyr RTOS.

## Prerequisites

### 1. Create Python Virtual Environment

```bash
python3 -m venv ~/zephyrproject/.venv
source ~/zephyrproject/.venv/bin/activate
pip install west
```

### 2. Initialize Zephyr Workspace

```bash
west init ~/zephyrproject
cd ~/zephyrproject
```

### 3. Fetch Zephyr and Required Libraries

```bash
west update zephyr hal_stm32 cmsis cmsis_6
```

- `zephyr`: Core RTOS
- `hal_stm32`: STM32 Hardware Abstraction Layer  
- `cmsis` / `cmsis_6`: ARM CMSIS headers

### 4. Export Zephyr CMake Package

```bash
west zephyr-export
```

### 5. Install Python Dependencies

```bash
west packages pip --install
```

### 6. Install ARM Toolchain

```bash
cd ~/zephyrproject/zephyr
west sdk install -t arm-zephyr-eabi
```

## Build and Flash

```bash
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr

cd ~/code/zephyr-nucleo-h723zg-hello-world
west build -b nucleo_h723zg . --pristine
west flash
```

## Usage

Connect via serial terminal (115200 baud) to see:
- LED blinks every 500ms
- Type characters to see "You pressed: X"

