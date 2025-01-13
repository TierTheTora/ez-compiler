# The EZ Compiler
<img src="./ico.png" style="width: 100px;">

## Requirements
- Any unix based system
- A NASM Compiler
- A C++ Compiler
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
## stdout(), stdoutln()
```ez
&start:
    stdout("Hello, World!", 13)
    stdoutln("Hello, World!", 13)
    exit 0
```
You must include the length of the text to print it.
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
## Variables
```ez
&start:
    input: db stdin()
    buf: dw 100
    stdoutln(input, buf)
    exit 0
```
