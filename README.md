# City Emergency Dispatch Simulation

## Overview
This project is a simulation of a city emergency dispatch system that manages emergency calls such as fires, accidents, and medical incidents. The system includes a central dispatcher that processes and forwards calls to the relevant departments: police, fire department, and ambulance. Each department operates with a limited number of resources and prioritizes calls efficiently.

## Features
- **Random Event Generation:** Simulated emergency calls triggered at random intervals.
- **Dispatcher Queue:** Central dispatcher processes and forwards calls based on event type and priority.
- **Emergency Departments:** Police, fire, ambulance, and other departments handle calls with limited resources.
- **Resource Management:** Ensures optimal use of available resources and queues excess calls.
- **Logging System:** Records actions and system performance for tracking and analysis.
- **Synchronization & Locking:** Prevents race conditions using mutexes for concurrent operations.
- **Fault Management:** Handles resource shortages and system failures efficiently.

## Technologies Used
- Programming Language: [C]
- Multithreading for concurrency
- Priority queues for efficient task management
- Logging mechanisms (e.g., printf, file logging

## Setup Instructions
1. Clone the repository:
   ```sh
   git clone https://github.com/SalmanAmer/City-Emergency-Dispatch-Simulation-FreeRTOS
   ```
2. Navigate to the project folder:
   ```sh
   cd FreeRTOS-main/FreeRTOS/Demo/Posix_GCC/RT_Project
   ```
3. Compile and run the project:
   ```sh
   make
   build/posix_demo
   ```

## Usage
- The simulation runs automatically, generating random events and handling them based on available resources.
- Logs display real-time dispatching and resource allocation.
- You can adjust resource limits and event frequency in the code settings.

## Future Improvements
- Implement a graphical interface for real-time monitoring.
- Enhance the priority system for better efficiency.
- Add additional emergency scenarios.


---

**Author:** [Salman Amer]

Feel free to contribute or suggest improvements!

