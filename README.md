# FreeRTOS Installation Guide

This guide explains how to set up FreeRTOS on Linux, macOS, and Windows. Follow the instructions carefully based on your operating system.

---

## Linux / macOS Setup

### 1. Set Up VS Code
You can use [VS Code](https://code.visualstudio.com/) as the IDE for editing and working with the FreeRTOS code.

### 2. Check for GCC Installation
To check if GCC is installed on your system, open the terminal and run the following command:
```bash
gcc --version
```
If GCC is not installed, or the command is not found, follow the steps below to install it.
Install GCC
Update your package lists, upgrade existing packages, and install GCC with the following commands:
```bash
sudo apt update
sudo apt upgrade
sudo apt install build-essential
```

### 3. Navigate to the FreeRTOS Demo Directory
After installing GCC, navigate to the FreeRTOS demo directory for the Posix example:
```bash
cd FreeRTOS/Demo/Posix_GCC
```

### 4. Compile the Demo
Run the following command to compile the demo:
```bash
make
```
If there are no errors, a build directory will be created, and inside it, you will find the executable file posix_demo.


## Windows Setup
### 1. Install Visual Studio Community 2022
Download and install Visual Studio Community 2022(https://visualstudio.microsoft.com/)

### 2. Select Workloads During Installation
During installation, ensure the following option is selected:
Desktop Development with C++

### 3. Select Individual Components
In the "Individual Components" section, make sure to check:
- MSVS v142 - VS 2019 C++ x64/x86 build tools (v14.29-16.11)
- MSVS v142 - VS 2019 C++ x64/x86 Spectre-mitigated libraries (v14.29-16.11)

### 4. Open the FreeRTOS Project in Visual Studio
After installation, navigate to the following directory:
```bat
FreeRTOS/Demo/WIN32-MSVC
```
Open the solution file WIN32.sln by double-clicking it.

### 5. Compile the Demo
You can now compile the FreeRTOS demo in Visual Studio by building the solution.

# Examples
### Example 1
Files for this example can be found in the QueFiles branch.

### Additional Examples
More examples are available in the exercise materials.

# Additional Tips
For Linux/macOS, if you encounter errors while running make, ensure all required dependencies are installed.
For Windows, make sure that Visual Studio is fully updated and has all the necessary components selected for C++ development.
