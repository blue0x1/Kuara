<h1 align="center">Kuara Webshell</h1>




<br>


**Kuara** is a C++ webshell designed specifically for Windows systems, allowing for web-accessible command execution.

> :warning: **Disclaimer**: Kuara is intended for legal and authorized use only. Deploying it without proper authorization or for malicious activities is strictly discouraged and potentially illegal. Ensure proper permissions before operating on any systems.

## Features
<br> 

- **Web Interface**: Command execution through a browser.
- **URL Decoding**: Ensures precise command interpretation.
- **FUD (Fully Undetectable)**: As of its last iteration, Kuara is undetectable by common threat detection systems. However, always verify against up-to-date threat detection platforms.

## Prerequisites
<br> 
- A Windows OS 
- g++ compiler (supporting C++11 or newer).

## Compilation
<br> 

```bash
g++ -o kuara.exe KuaraWebShell.cpp -lws2_32
```

## Usage
<br> 

1. **Launch the webshell**:
```bash
./kuara.exe
```
  The web server will start on port 8080.

2. Access the shell in your browser at:

```bash
http://localhost:8080
```
Engage with the Kuara interface to input and execute system commands.
<br> 

## Screenshots

![Kuara Screenshot 1](https://github.com/blue0x1/Kuara/raw/main/1.png)

![Kuara Screenshot 2](https://github.com/blue0x1/Kuara/raw/main/2.png)

