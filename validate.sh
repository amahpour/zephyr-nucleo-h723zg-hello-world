#!/bin/bash
set -e

echo "=== Step 1: Create Python Virtual Environment ==="
python3 -m venv ~/zephyrproject/.venv
source ~/zephyrproject/.venv/bin/activate
pip install west

echo "=== Step 2: Initialize Zephyr Workspace ==="
west init ~/zephyrproject
cd ~/zephyrproject

echo "=== Step 3: Fetch Zephyr and Required Libraries ==="
west update zephyr hal_stm32 cmsis cmsis_6

echo "=== Step 4: Export Zephyr CMake Package ==="
west zephyr-export

echo "=== Step 5: Install Python Dependencies ==="
west packages pip --install

echo "=== Step 6: Install ARM Toolchain ==="
cd ~/zephyrproject/zephyr
west sdk install -t arm-zephyr-eabi

echo "=== Step 7: Build Project ==="
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr
cd /workspace/project
west build -b nucleo_h723zg . --pristine

echo "=== Build completed successfully! ==="

