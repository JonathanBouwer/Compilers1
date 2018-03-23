# Compilers Assignment

## YodaScript

YodaScript is a Star Wars based programming language with many keywords and statements based on the famous character Yoda, often using English phrases in a grammatically incorrect fashion. This language supports functions, conditional expressions, looping constructs, and more; with the entire program text lying between the keywords a_long_time_ago_in_a_galaxy_far_far_away and the_end. 

Sample code can be found in the ```sample``` folder.

## Language Design

The language can be described by the following Finite State Machines
![Library and Functions](images/Library.jpeg?raw=true "Library and Functions")
![Non-Binary Expressions](images/Expression.jpeg?raw=true "Non-Binary Expressions")
![Binary Expressions](images/ExpressionBin.jpeg?raw=true "Binary Expressions")
![Statements](images/Library.jpeg?raw=true "Statements")

## Building the Compiler

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
  
## Testing

Testing is done though ```TestSuite.py```, a python script which will attempt to compile all scripts ion the ```sample/TestSuite``` folder. To run the tests simply run ```python src/TestSuite.py``` on the terminal (Python 3 required).

To write your own test is simple! Create a file in the TestSuite directory with the final character before the file extension (i.e. the first ```.``` in the file name) being either ```F``` or ```S``` for Should Fail or Should Succeed respectively. Use the already written tests as a reference.
