# Monte Carlo Project

This project implements a Monte Carlo method in C. The Monte Carlo method is a simulation technique used to model the probability of different outcomes in a process that cannot be predicted due to the intervention of random variables.

## Project Structure

The project consists of the following files:

- **principal.c**: The main file containing the `main` function and the core logic of the program.
- **mcarlo.c**: Contains the specific functions that implement the Monte Carlo method.
- **rutines.h**: Header file that declares the functions used in `mcarlo.c`.

## Files

### principal.c

This file contains the `main` function of the program. It is responsible for initializing the program, calling the necessary functions, and managing the overall execution of the Monte Carlo method.

### mcarlo.c

This file contains the implementation of the functions needed to perform the Monte Carlo method. It defines the logic for the simulation and probability calculations.

### rutines.h

The header file that declares the functions implemented in `mcarlo.c`. This allows `principal.c` and other files to use these functions.

## Compilation and Execution

To compile and run the program, follow these steps:

1. Open a terminal and navigate to the directory where the project files are located.

2. Compile the project using `gcc` or another C compiler. Here is an example using `gcc`:

    ```sh
    gcc -o montecarlo principal.c mcarlo.c
    ```

    This command will compile `principal.c` and `mcarlo.c` and generate an executable named `montecarlo`.

3. Run the program:

    ```sh
    ./montecarlo
    ```

## Contributions

If you would like to contribute to this project, please follow these steps:

1. Fork the repository.
2. Create a new branch with your changes: `git checkout -b my-new-branch`.
3. Make your changes and commit them: `git commit -m 'Add new features'`.
4. Push your changes to the remote repository: `git push origin my-new-branch`.
5. Create a Pull Request.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

