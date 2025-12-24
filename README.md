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

## Docker Validation

Validate the entire workflow (up to build) in a Docker container:

### Build Docker Image

```bash
docker build -t zephyr-test .
```

### Run Validation

The validation script will automatically:
1. Create Python virtual environment
2. Initialize Zephyr workspace
3. Fetch Zephyr and required libraries
4. Export Zephyr CMake package
5. Install Python dependencies
6. Install ARM toolchain
7. Build the project

```bash
docker run --rm zephyr-test
```

### Interactive Mode

To run the validation script interactively or debug:

```bash
docker run -it --rm zephyr-test /bin/bash
# Then run: /workspace/validate.sh
```

**Note:** The Docker validation only covers the build process. Flashing requires physical hardware access and is not included in the containerized workflow.

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

## Serial Terminal Testing

Connect to the device serial port:

```bash
screen /dev/ttyACM0 115200
```

**Expected behavior:**
- LED blinks every 500ms (visual check)
- Typing characters displays "You pressed: X" for each character

**Exit screen:** Press `Ctrl+A` then `K`, confirm with `Y`

**Note:** If `/dev/ttyACM0` doesn't exist, check available ports with `ls /dev/ttyACM*` or `ls /dev/ttyUSB*`

