# The EZ Compiler

## Requirements
- Any linux distro
- The NASM Compiler
- The C++ Compiler
### If you don't have a NASM Compiler or a C++ Compiler:
Do 
```sh
sudo apt install nasm  # To install the NASM Compiler
sudo apt install gcc   # To install the C++ Compiler
```
## Compiling the programm
Do 
```sh
./ezscr file.ez # Replace 'file' with the file name
```

## Running the Compiled programm
Do 
```sh
./folder/output # Replace 'folder' with the file name (removing the .ez)
```

# Syntax
## exit
```ez
&start: 
    exit 0
```
```sh
echo $?
  # >> 0
```
## stdout()
```ez
&start:
    stdout("Hello, World!")
    exit 0
```
## stdin()
```ez
&start:
    stdin()
    exit 0
```
## Functions
```ez
&start:
    exit_prg()
&exit_prg:
    exit 0
```

# Example:
```ez
&start:
  input: db stdin()
  stdout(input)
  ex()
&ex:
  exit 255
```
