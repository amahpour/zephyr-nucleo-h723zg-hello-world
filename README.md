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

### 7. Install OpenOCD (for flashing)

OpenOCD is required for flashing the device. Alternatively, you can use STM32CubeProgrammer from STM32CubeIDE.

```bash
sudo apt install openocd
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

### Hardware Target (Nucleo H723ZG)

```bash
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr

cd ~/code/zephyr-nucleo-h723zg-hello-world
west build -b nucleo_h723zg . --pristine
west flash --runner openocd
```

**Note:** The `--runner openocd` flag uses OpenOCD for flashing (recommended). If you have STM32CubeProgrammer installed, you can use `west flash` without the runner flag, but OpenOCD is the preferred method for Nucleo boards.

### Simulator Target (Native Sim)

Build and run on native simulator (runs natively on your host):

```bash
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr

cd ~/code/zephyr-nucleo-h723zg-hello-world
west build -b native_sim . --pristine
west build -t run
```

**Expected output:**
```
uart connected to pseudotty: /dev/pts/X
*** Booting Zephyr OS build vX.X.X ***
```

**Testing UART Echo via PTY:**

The native simulator creates a pseudotty (PTY) for UART communication. To test:

1. **Terminal 1** - Run the simulator:
   ```bash
   west build -t run
   # Note the PTY device shown (e.g., /dev/pts/5)
   ```

2. **Terminal 2** - Connect to the PTY:
   ```bash
   # Option A: Use screen (interactive)
   screen /dev/pts/X 115200
   # Type characters - you'll see "You pressed: X" for each
   # Exit: Ctrl+A then K, confirm Y

   # Option B: Send characters directly
   echo "hello" > /dev/pts/X
   # Check Terminal 1 for "You pressed: h", "You pressed: e", etc.
   ```

**Note:** Replace `/dev/pts/X` with the actual PTY device shown when the simulator starts.

**Note:** native_sim has built-in GPIO emulator and LED support defined in `zephyr/boards/native/native_sim/native_sim.dts` - no additional board configuration files needed.

**GPIO Debug Output:**

The native_sim build includes GPIO tracing that shows LED state changes:

```
[DEBUG] (GPIO) pin 0 configured (ret=0)
[DEBUG] (GPIO) LED state: OFF
[DEBUG] (GPIO) LED state: ON
[DEBUG] (GPIO) LED state: OFF
...
```

This is implemented via Zephyr's tracing subsystem in `src/trace_hooks.c`, which hooks into GPIO operations without modifying `main.c`. The `gpio_emul_output_get()` API reads the actual GPIO state from the emulator. Enabled by `boards/native_sim.conf` with `CONFIG_TRACING_GPIO=y`.

**GDB Debugging (main.c):**

Since native_sim compiles to a native Linux executable, you can debug `main.c` directly with GDB:

1. **Build with no optimizations** (so variables aren't optimized out):
   ```bash
   west build -b native_sim . --pristine -- -DCONFIG_NO_OPTIMIZATIONS=y
   ```

2. **Run GDB interactively:**
   ```bash
   gdb ./build/zephyr/zephyr.exe
   ```

3. **Example GDB session** (inspecting `main.c` variables):
   ```gdb
   (gdb) break /full/path/to/src/main.c:21    # Break at gpio_pin_configure_dt
   (gdb) run
   (gdb) print led                             # Inspect LED gpio_dt_spec
   (gdb) print led.port->name                  # Shows "gpio_emul"
   (gdb) print led.pin                         # Shows pin number (0)
   (gdb) backtrace                             # Show call stack
   (gdb) continue
   (gdb) quit
   ```

   **Important:** Use the full absolute path to `main.c` when setting breakpoints, otherwise GDB may hit the wrong `main.c` (from native_simulator).

4. **Batch mode with timeout** (for scripted testing):
   ```bash
   timeout 10 gdb -q -batch \
     -ex "break /full/path/to/src/main.c:21" \
     -ex "run" \
     -ex "print led" \
     -ex "print led.port->name" \
     -ex "quit" \
     ./build/zephyr/zephyr.exe
   ```

5. **VS Code Debugging:**
   
   The `.vscode/launch.json` and `.vscode/tasks.json` files are pre-configured for debugging:
   - Press `F5` to build (with debug symbols) and launch the debugger
   - Set breakpoints in `src/main.c` or `src/trace_hooks.c` by clicking the gutter
   - Use the Debug panel to inspect variables like `led` and `state`
   
   **Requires:** [C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) for VS Code.

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

