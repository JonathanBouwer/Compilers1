# Compilers Assignment
### Compilation Windows
This requires Microsoft Visual C++ is installed, this can be chosen during a Visual Studio Installation.

  - Open a developer command prompt with and navigate to this directory
  - Running `make` will copmpile the application
  - Running `make run` will run the application passing the next 3 comnmand line arguments
  - Runnning `make clean` will cleanup build artifacts
  - Running `make debug` will generate debug information for visual studio

### Compilation Linux
This requires `g++` and `make` are installed 
  - Open a terminal in the current directory
  - Running `make` will run the application
  - Running `make clean` will cleanup build artifacts
  - Running `src/main` will run the application passing all command line arguments (Make doesn't support passing command line arguments)