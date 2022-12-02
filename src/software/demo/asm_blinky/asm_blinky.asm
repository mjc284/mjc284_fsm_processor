#Assembly code instruction set:

#out data => output data: ({slave_select[5:0], mosi[7:0]})
#beq data => skip following instruction if data[7:0] = miso[7:0]
#jmp address => jump to address[8:0]

#Demo program to blink an led with delay.
#Recommended FSM processor clock frequency: 50 MHz

jmp 5
out 1025
jmp 18
out 1279
jmp 0
out 128
beq 255
jmp 16
out 240
out 384
beq 255
jmp 14
out 496
jmp 15
jmp 5
jmp 17
jmp 5
jmp 1
out 128
beq 255
jmp 29
out 240
out 384
beq 255
jmp 27
out 496
jmp 28
jmp 18
jmp 30
jmp 18
jmp 3