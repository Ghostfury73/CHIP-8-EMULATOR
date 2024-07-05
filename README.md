##  Overview

The project CHIP-8-EMULATOR is a comprehensive emulator that simulates the functionality of the classic CHIP-8 system, allowing users to play retro games seamlessly. The codebase includes core components such as memory management, CPU operations, keypad input handling, and display screen rendering using SDL2. Users can interact with the emulator by loading ROM files for their favorite games, with support for displaying graphics and enhancing the overall gaming experience. By encapsulating the fundamental operations of the CHIP-8 system, this emulator provides a nostalgic yet accessible platform for retro gaming enthusiasts.

***Requirements***

Ensure you have the following dependencies installed on your system:

* **C complier** (e.g., GCC)

###  Installation

1. Clone the CHIP-8-EMULATOR repository:

```sh
git clone https://github.com/Ghostfury73/CHIP-8-EMULATOR.git
```

2. Change to the project directory:

```sh
cd CHIP-8-EMULATOR
```

###  Building CHIP-8-EMULATOR

Use the following command to run CHIP-8-EMULATOR:
(If using MinGW)

```sh
mingw32-make
```
###  Running CHIP-8-EMULATOR
Go inside "bin" folder
```sh
main.exe <location of game ROM>
```
Example
```sh
main.exe ./TETRIS
```

##  Project Roadmap

- [X] `► CHIP8 GAMES RUNNING`
- [ ] `► SUPER CHIP GAMES`
- [ ] `► ...`

---

##  Contributing

Contributions are welcome! Here are several ways you can contribute:

- **[Submit Pull Requests](https://github.com/Ghostfury73/CHIP-8-EMULATOR.git/blob/main/CONTRIBUTING.md)**: Review open PRs, and submit your own PRs.
- **[Join the Discussions](https://github.com/Ghostfury73/CHIP-8-EMULATOR.git/discussions)**: Share your insights, provide feedback, or ask questions.
- **[Report Issues](https://github.com/Ghostfury73/CHIP-8-EMULATOR.git/issues)**: Submit bugs found or log feature requests for Chip-8-emulator.

<details closed>
    <summary>Contributing Guidelines</summary>

1. **Fork the Repository**: Start by forking the project repository to your GitHub account.
2. **Clone Locally**: Clone the forked repository to your local machine using a Git client.
   ```sh
   git clone https://github.com/Ghostfury73/CHIP-8-EMULATOR.git
   ```
3. **Create a New Branch**: Always work on a new branch, giving it a descriptive name.
   ```sh
   git checkout -b new-feature-x
   ```
4. **Make Your Changes**: Develop and test your changes locally.
5. **Commit Your Changes**: Commit with a clear message describing your updates.
   ```sh
   git commit -m 'Implemented new feature x.'
   ```
6. **Push to GitHub**: Push the changes to your forked repository.
   ```sh
   git push origin new-feature-x
   ```
7. **Submit a Pull Request**: Create a PR against the original project repository. Clearly describe the changes and their motivations.

Once your PR is reviewed and approved, it will be merged into the main branch.


##  Acknowledgments

- CHIP-8 Technical Reference by Cowgod for providing detailed specifications of the CHIP-8 system
- SDL2 library and its contributors for the cross-platform development framework
- The CHIP-8 community for preserving and sharing ROMs and documentation
- Laurence Muller's "How to write an emulator (CHIP-8 interpreter)" article for valuable insights

[**Return**](#-quick-links)

---
