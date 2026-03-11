# Simple Port Scanner

This repository contains two basic TCP port scanners:

* **Python Port Scanner** – GUI-based and simple to use
* **C Port Scanner** – Multithreaded command-line scanner for faster scanning

These projects were created for learning purposes while exploring networking, systems programming, and basic cybersecurity tooling.

---

# Python Port Scanner

The Python version provides a **simple graphical interface** for scanning ports on a target host.

### Features

* GUI-based interface
* Easy to use
* Suitable for quick scans and beginners

### Usage

Run the script:

```bash
python scanner.py
```

Enter the target host and port range through the GUI.

---

# C Multithreaded Port Scanner

The C version is a **command-line multithreaded TCP scanner** designed to scan ports faster by using multiple threads.

### Features

* Multithreaded scanning
* Uses TCP connect scanning
* Designed for learning low-level networking and concurrency

### Compile

```bash
gcc portscanner.c -o portscanner -lpthread
```

### Usage

```bash
./portscanner <host> <start_port> <end_port> <number_of_threads>
```

### Example

```bash
./portscanner scanme.nmap.org 1 1000 50
```

This scans ports **1–1000** on `scanme.nmap.org` using **50 threads**.

---

# Disclaimer

This tool is intended **for educational purposes only**.
Only scan systems that you own or have explicit permission to test.

---
