# 🏥 HMO Management System – C Project

## 📌 Overview

The **HMO Management System** is a comprehensive C-based application designed to simulate and manage the operations of a modern Health Maintenance Organization (HMO). Developed as a university project, it provides an integrated solution for handling appointments, patients, doctors, pharmacists, prescriptions, and pharmacies—mimicking real-world healthcare operations.

The system was developed with a focus on modular programming, memory management, data handling, and functional design, providing a complete command-line interface for interaction with the system.

## 🎯 Key Features

- Add, edit, delete, and search:
  - Patients, Doctors, Pharmacists, and Drugs
- Manage and schedule appointments
- Create and manage medical prescriptions
- Sort and search drug inventory by name, expiration date, or serial ID
- Simulate purchasing medications based on prescriptions
- Generate appointment approval documents
- Export and import full system data via text and binary files
- Compress and decompress admin data using RLE (Run-Length Encoding)

## 🧩 System Modules

The project is composed of modular structures, each responsible for a key aspect of the HMO:

### 📁 Core Structs
- **HMO**: Main system controller
- **HMO_Admin**: Health center administrator data
- **Patient**: Medical record, birthdate, gender
- **Doctor**: Specialization, license ID, gender
- **Pharmacist**: License details, permissions
- **Pharmacy**: Drugs, address, responsible pharmacist
- **Drug**: Name, serial ID, expiration
- **Appointment**: Doctor-patient meeting with time/date
- **Prescription**: Drug, dose, expiration, related IDs
- **Date / Time**: Utilities for validation, formatting, and comparison

### ⚙️ Key Functionalities
- Dynamic memory management and linked list usage
- Binary/text file I/O for saving/loading system state
- Sorting algorithms and search filters
- Formatted printing and record displays
- Error handling and input validation

## 🧪 Compression Module

An RLE (Run-Length Encoding) compression algorithm was implemented to compress the **HMO_Admin** struct before writing it to a file. This involved:

1. Serializing the struct into a byte stream
2. Applying RLE to reduce size
3. Writing compressed data with header metadata
4. Supporting decompression and restoration

## 🛠 Technologies

- **Language**: C
- **Concepts**: Structs, Pointers, File I/O, Dynamic Memory, Linked Lists
- **Tools**: GCC, Visual Studio Code

## 📂 Project Structure

```
├── main.c
├── hmo.h / hmo.c
├── doctor.h / doctor.c
├── patient.h / patient.c
├── pharmacist.h / pharmacist.c
├── drug.h / drug.c
├── pharmacy.h / pharmacy.c
├── appointment.h / appointment.c
├── prescription.h / prescription.c
├── utils.h / utils.c
├── rle_compression.c
├── input.txt / output.txt
└── README.md
```

## 📌 Final Notes

This project demonstrates how real-world healthcare processes can be modeled using core principles of C programming. It balances data structure complexity with usability, aiming to reflect real hospital/HMO logic in a simplified, academic context.


## License

Copyright (c) 2024 Netanel Boris Cohen Niazov

## Authors

- Netanel Boris Cohen Niazov
