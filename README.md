# fsm_processor
An attempt to design a programmable finite state machine with basic conditional loop and input-output features.

## Table of contents
- [Concept](#Concept)
- [Hardware](#Hardware)
  - [Overview](#Overview1)
  - [Instruction Format](#Instruction_Format)
  - [Finite State Machine](#Finite_State_Machine)
  - [Specs and Performance](#Specs_and_Performance)
  - [Essential SPI Modules](#Essential)
- [Software](#Software)
  - [Overview](#Overview2)
  - [Assembly](#Assembly)
  - [MIF Generation](#MIF_Generation)
  - [Compilation](#Compilation)
- [Project Directory Map](#Project_Directory_Map)


## Concept <a name="Concept"></a>
The aim of this project is to create and build a new processor architecture and language that is able to execute three types of instructions: outputting data, jumping to a specific instruction address, or skipping an instruction if the input data is equal to the immediate data in the instruction. The processor cannot compute or compare data by itself and needs externel SPI modules such as memory and ALU to operate as a standard processor. Therefore, the FSM processor has a much more primitive architecture compared to other microprocessors. 

The advantage of this primitive processor is that it only takes up merely 34 logic elements with 4-input LUTs, allowing the processor to be implemented on small CPLDs. This gives the FSM processor the potential to replace microprocessors for simple tasks that does not involve any computation and can be expressed as finite state machines or sequences of output data, such as low-spec laundry machines, LED light animators, and digital lock systems.

Refer to the following page for demonstration codes and video:

## Hardware <a name="Hardware"></a>

![Annotated_Circuit](/circuit/Annotated_Circuit.png "Annotated_Circuit")

### Overview <a name="Overview1"></a>
The architecture of the FSM processor consist of three main parts: the 16-bit synchronous memory, a finite state machine based on the instruction op-code, a counter with reset, and a combinational circuit to control the counter reset based on the state. When the instruction is given by the memory, the finite state machine executes a sequence of states based on the three opcodes for the 'data output', 'jump', and 'conditional skip' instructions. The counter reset controller triggers the state machine to move on to the next state and resets the counter when the counter reaches a specific number of clock cycles for a given state. The focus of this design was to prioritize reducing the size of the processor while sacrificing the speed. Therefore, all of the data within the processor flow serially in order to reduce the the number of gates and resources. Furthermore, the small Intel CPLD used in this project only has a serial internal memory.

### Instruction Format <a name="Instruction_Format"></a>
All instructions are 16-bits in length with the first two bits (towards the MSB) being the opcode:
![Instructions](/pictures/Instructions.png "Instructions")

### Finite State Machine <a name="Finite_State_Machine"></a>
The processor executes the following state diagram based on the given opcode:
![State_Diagram](/pictures/State_Diagram.jpg "State_Diagram")

The karnaugh-mapping and implementation of the finite state machine circuit was done using python scripts from my other project:

### Specs and Performance <a name="Specs_and_Performance"></a>
* Maximum number of SPI peripherals: 64 modules
* Resource usage: 34 Logic Elements, 1 Synchronous Internal Memory
* Speed: 
  * 16 clock cycles per output instruction
  * 11 clock cycles per jump instruction
  * 10 clock cycles per conditional skip instruction.

### Essential SPI Modules <a name="Essential"></a>
The project includes 3 types of SPI modules that may be essential for minimal operation of the FSM processor. These modules in the /src/hardware/spi_modules/ folder include an SPI register, output, and counter. Two SPI counters are attached to address 0 and 1, two SPI registers are attached to address 2 and 3, and one SPI output is attached to address 4 with led connections.

The SPI counter operates with 3 types of instruction data from the MOSI:
* "0b00000000" = no operation/send back count data through MISO
* "0b10000000" = increment by one
* "0b11110000" = reset to zero

The SPI register operates with 2 types of instruction data from the MOSI:
* "0b00000000" = no operation/send back stored register data through MISO
* else         = store new MOSI data into the register

The SPI output operates with 3 types of instruction data from the MOSI:
* "0b00000000" = no operation/send back output data through MISO
* "0b11111111" = clear output data to zero.
* else         = update output data with data recieved from MOSI.

The demo codes in the /src/software/demo use these modules for blinking or animating LEDs.

## Software <a name="Software"></a>

### Overview <a name="Overview2"></a>
The software part of the project involves creating a custom assembly language, a custom program language, and assemblers and compilers to read the code and generate binary data for the processor memory. The program language is much different from other software languages in that it purely serves to describe the states or sequences of instructions to be executed, without any computation, comparison, or read/write. Therefore, the custom language for this processor, which I named as the state sequence description language (SSD), closely resembles c code without arithmetic, comparison, and variable assignment operators. 

### Assembly <a name="Assembly"></a>
There are three types of assembly instructions which are 'data output', 'jump', and 'conditional skip'.

The 'data output' instruction outputs serial spi data to a SPI module at a specific address. 
```
out [data]
```
The [data] is a 14 bit number in which the upper 6 bits specify the SPI slave address and lower 8 bits specify the data to send. 

The 'jump' instruction jumps to a specified instruction address. 
```
jmp [address]
```
The [address] is a 9 bit number for the next instruction address to jump to. 

The 'conditional skip' instruction skips the next instruction if the comparison data is equal to MISO data sent from the SPI slave. The selected SPI device is the SPI device written to during the last 'data output' instruction. This means in order to perform the 'conditional skip' to a new SPI device, a 'data output' with no-operation data should be sent to the desired SPI device first. During the retrival of the MISO data the FSM processor outputs 8 bits of '0' to the SPI device. Therefore, it is advised to require all peripheral SPI devices to have a no-operation code for the 8 bit data '00000000'.
```
beq [comparison_data]
```
The [comparison_data] is an 8 bit number to be compared with the 8 bit MISO data. 
All numerical arguments can be entered as decimal (123... or 0d123...), binary (0b1101...), or hex (0xf1e...).

Example: 
In example.asm,
```
# Comments can be added after hashes.
out 0b11010010101010            # Output '10101010' to SPI device at address '110100'.
beq 0b10101010                  # Skip next instruction if MISO output from SPI device at '110100' is '10101010'.
jmp 0                           # Goto first instruction.
out 0b11111100001111            # Output '00001111' to SPI device at address '111111'.
jmp 0                           # Goto first instruction.
```
Compilation: 
In shell,
```
/src/software/build/assembler example.asm example.bin
```
Output:
In example.bin (through hexdump),
```
34aa aa80 c000 3f0f c000
```

### MIF Generation <a name="MIF_Generation"></a>
The project has yet to implement a method to upload the program into the internal ram memory without flashing the CPLD itself. For now, the program data is inserted during synthesis by the Intel memory initialization file (MIF). The project includes a tool convert the compiled or assembled binary into a MIF.

MIF Generation:
In shell,
```
/src/software/build/mif_generator example.bin program.mif
```

Output:
In program.mif,
```
-- Generated Memory Initialization File for FSM Processor (.mif)

WIDTH=16;
DEPTH=512;

ADDRESS_RADIX=UNS;
DATA_RADIX=UNS;

CONTENT BEGIN
	0	:	13482;
	1	:	43648;
	2	:	49152;
	3	:	16143;
	4	:	49152;
	5	:	49152;
	[6..511]	:	0;
END;
```

### Compilation <a name="Compilation"></a>
The language made for this processor is named the "state sequence description language". The syntax is very similar to C with only 'if', 'while', 'define', plus a new 'state' statement. The compiler for this language follows the basic model of tokenizing the code first and processing the type of statement, scope, and variables afterwards. The following example code exaplains all the possible syntax in this language:

```
//Comments can be added -
/* In the same way as in C/C++ */

//The define keyword allows the creation of a variable to be substituted by its value during compilation.
define a = 5;

//States act in a similar way as functions in C, but without any arguments or returning values.
//States can only be accessed by the keyword 'goto' and cannot be directly called like functions.
state sample
{
    out(6, 0); //Output nop ("0b00000000") to SPI device at address '6' (selecting that device for next 'if' statement).
    if(5) //If 8 bit data in SPI device at address '6' equals 5:
    {
        out(4, 1);
        out(a, 2);
    }
    else
    {
        out(6, 0);
        while(6) //While 8 bit data in SPI device at address '6' equals 6:
        {
            out(5, a);
        }
    }
    goto return; //Goto instructions can jump to any labels in any state.
}

//There has to be a 'main' state in which the codes starts at.
state main 
{
    loop1: //Labels act the same way as in C/C++.
    goto sample; //Jump to the beginning of state 'sample'.
    return:
    goto main; //Loop back to state 'main'.
}
```

Compilation, Disassembly, and MIF Generation:

In shell,
```
/src/software/build/compiler ./example.ssdf example.bin
/src/software/build/disassembler ./example.bin example.asm
/src/software/build/mif_generator ./example.bin program.mif
```

In example.bin,
```
c040 c000 0600 8140 c100 0401 0502 c1a0 0600 8180 c1a0 0505 c120
```

In example.asm,
```
#Assembly file generated by Disassembler

jmp 2
jmp 0
out 1536
beq 5
jmp 8
out 1025
out 1282
jmp 13
out 1536
beq 6
jmp 13
out 1285
jmp 9
jmp 9
```

In program.mif,
```
-- Generated Memory Initialization File for FSM Processor (.mif)

WIDTH=16;
DEPTH=512;

ADDRESS_RADIX=UNS;
DATA_RADIX=UNS;

CONTENT BEGIN
	0	:	49216;
	1	:	49152;
	2	:	1536;
	3	:	33088;
	4	:	49408;
	5	:	1025;
	6	:	1282;
	7	:	49568;
	8	:	1536;
	9	:	33152;
	10	:	49568;
	11	:	1285;
	12	:	49440;
	13	:	49440;
	[14..511]	:	0;
END;
```

Refer to the following directory for example assembly and program codes:
/src/software/build/compiler

### How to Build from Source Code <a name="How_To"></a>
After installing cmake in shell,
```
cd /src/software
make clean
make
```

## Project Directory Map <a name="Project_Directory_Map"></a>
src  
├── hardware                # Folder containing all hardware description in verilog code.  
├── software                  
│   ├── src                 # Source code.  
│   ├── demo                # Demo example codes.  
│   └── build               # Compiled executables.  
