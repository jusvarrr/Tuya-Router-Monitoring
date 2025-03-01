# Tuya Router Monitoring  
This project implements a system monitoring solution for OpenWRT-based routers integrated with the Tuya IoT Cloud Platform. The system monitors device parameters such as RAM usage and sends these metrics to the Tuya cloud.  

## Functional Requirements  
- The program accepts arguments on startup, including:  
  - **Device ID**  
  - **Product ID**  
  - **Secret key** (required for Tuya cloud communication)  
  - **Option to run the program as a daemon**  

- The program sends the following device parameters to the Tuya cloud:  
  - **Total RAM size**  
  - **Available RAM**  

- The program logs specific Tuya actions in the `tuya_action.log` file.  
- The system can receive and execute commands from Tuya with parameters to trigger specific actions on the device.  

## Non-Functional Requirements  
- Integrates the **Tuya IoT SDK** to create dynamic libraries that support communication with Tuya Cloud.  
- Utilizes **sysinfo** to retrieve system data such as RAM usage and CPU load.  
- Uses **ARGP** for handling Tuya authentication arguments passed to the program.  
- If specified, the program runs as a **daemon** in the background.  
- Execution messages are recorded in the system log using the **syslog** library.  
- A **Makefile** is provided to build the program.  

## Components  

### `argp_standalone`  
A package for building the argument parsing library installation file on the router. This enables parsing of configuration files for Tuya Cloud authentication on OpenWRT.  

### `libtuyasdk`  
A package for building the Tuya IoT Core SDK installation file. This SDK is required for establishing communication between the router and the Tuya Cloud platform.  

### `tuyadevicemonitor`  
The main package for building the installation file for the router monitoring system.  

- **`files/`** – Contains configuration files and initialization scripts for starting the monitoring process on the router.  
- **`src/`** – Contains header files and source code for the monitoring program.  
- **`Makefile`** – Defines the build process for generating the installation file on the host computer.  

---

## Building & Installation (OpenWRT SDK)  

### 1. Set Up the OpenWRT Build Environment  
Ensure your OpenWRT SDK is correctly set up on your host machine.  

### 2. Add the Repository to OpenWRT's Package Directory  
Clone this repository into OpenWRT’s `package/` directory:  
```bash
cd openwrt/package
git clone <repository_url>
```
### 3. Compile packages argp_standalone, libtuyasdk and tuyadevicemonitor in OpenWRT host machine environment with:
   
```bash
   make  package/argp_standalone/compile
   make  package/libtuyasdk/compile
   make  package/tuyadevicemonitor/compile
```

### 4. Deploy the installation file to the router using ssh:
   
```bash
   scp bin/packages/*/*.ipk device@<device_ip>:/tmp/
```
### 5. On the device run:
   
```bash
   opkg install /tmp/*.ipk
```
