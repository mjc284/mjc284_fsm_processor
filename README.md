# fsm_processor
An attempt to design a programmable finite state machine with basic conditional loop and input-output features.


## Concept
The aim of this project is to create and build a new processor architecture and language that is able to execute three types of instructions: outputting data, jumping to a specific instruction address, or skipping an instruction if the input data is equal to the immediate data in the instruction. The processor cannot compute or compare data by itself and needs externel SPI modules such as memory and ALU to operate as a standard processor. Therefore, the FSM processor has a much more primitive architecture compared to other microprocessors. 

The advantage of this primitive processor is that it only takes up merely 34 logic elements with 4-input LUTs, allowing the processor to be implemented on small CPLDs. This gives the FSM processor the potential to replace microprocessors for simple tasks that does not involve any computation and can be expressed as finite state machines or sequences of output data, such as low-spec laundry machines, LED light animators, and digital lock systems.

Refer to the following page for demonstration codes and video:

## Hardware

![Annotated_Circuit]("/circuit/Annotated Circuit.png" "Annotated_Circuit")

### Overview
The architecture of the FSM processor consist of three main parts: the 16-bit synchronous memory, a finite state machine based on the instruction op-code, a counter with reset, and a combinational circuit to control the counter reset based on the state. When the instruction is given by the memory, the finite state machine executes a sequence of states based on the three opcodes for the 'data output', 'jump', and 'conditional skip' instructions. The counter reset controller triggers the state machine to move on to the next state and resets the counter when the counter reaches a specific number of clock cycles for a given state. The focus of this design was to prioritize reducing the size of the processor while sacrificing the speed. Therefore, all of the data within the processor flow serially in order to reduce the the number of gates and resources. Furthermore, the small Intel CPLD used in this project only has a serial internal memory.

### Instruction Format
All instructions are 16-bits in length with the first two bits (towards the MSB) being the opcode:
![Instructions]("/pictures/Instructions.png" "Instructions")

### Finite State Machine
The processor executes the following state diagram based on the given opcode:
![State_Diagram]("/pictures/State_Diagram.jpeg" "State_Diagram")

The karnaugh-mapping and implementation of the finite state machine circuit was done using python scripts from my other project:

### Specs and Performance
* Maximum number of SPI peripherals: 64 modules
* Resource usage: 34 Logic Elements, 1 Synchronous Internal Memory
* Speed: 
  * 16 clock cycles per output instruction
  * 11 clock cycles per jump instruction
  * 10 clock cycles per conditional skip instruction.

## Software


## Project Directory Map
