# CENG322 - Operating Systems Assignments

This repository contains my programming assignments for the CENG322 Operating Systems course[cite: 1, 2]. The projects focus on scripting, directory management, process creation, and system calls in Linux environments.

## Directory Structure
* **`PA1-Bash-Backup-Utility/`**: Contains the Bash script for the directory backup and analysis tool.
* **`PA2-C-Shell-Interface/`**: Contains the C implementation of the custom shell interface.

---

## Programming Assignment 1: Bash Backup Utility

### Description
In this assignment, I implemented a simple backup utility using the Bash shell. The program analyzes a source directory, creates a backup of it, and compresses the result into a `.tar.gz` archive file. It is implemented entirely using Bash and basic Linux commands without relying on the `find` command[cite: 2]. 

### Features
* **Input Validation**: Verifies that exactly two arguments are provided and checks if the source directory exists[cite: 2].
* **Timestamp Generation**: Generates a timestamp formatted as `YYYY-MM-DD-HH-MM-SS` to name the backup directory, archive file, and analysis report[cite: 2].
* **Directory Analysis**: Traverses the directory structure using loops to calculate the total number of files, total number of subdirectories, and total size of all files in bytes[cite: 2].
* **Compression**: Copies the source into a temporary directory, compresses it using the `tar` command, and then cleans up the temporary folder[cite: 2].

### Usage
To run the backup utility, execute the script with the source and backup directory paths:
```bash
./backup <source_dir> <backup_dir>
