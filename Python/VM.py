import numpy as np
import sys

NUM_REGS   = 32
TAILLE_MAX = 100

### variable ###
program        = []
running        = 1
regs           = np.zeros((NUM_REGS, ))
pc             = 0


### Champ d'instruction ###
instrNum = 0;
reg1     = 0;
reg2     = 0;
imm      = 0;
o        = 0;
a        = 0;
n        = 0;

def decode(instr):
    instrNum = (instr & 0xF8000000) >> 27
    if   instrNum == 15:
        imm      = (instr & 0x400000 )  >> 26
        o        = (instr & 0x3FFFE0 )  >>  5
        reg1     = (instr & 0x1F     )
    elif instrNum == 16 or instrNum == 17:
        reg1     = (instr & 0x7C00000)  >> 26
        a        = (instr & 0x3FFFFF )
    elif instrNum == 18:
        n        = (instr & 0x7FFFFFF)
    else:
      reg1     = (instr & 0x7C00000)  >> 22
      imm      = (instr & 0x200000 )  >> 21
      o        = (instr & 0x1FFFE0 )  >>  5
      reg2     = (instr & 0X1F     )


def showRegs():
    print("regs = ")
    cpt = 0;
    for i in range(NUM_REGS):
        printf("{:016X}".format(regs[i]))
        cpt += 1;
        if (cpt == 4):
            print("")
            cpt = 0
    print("")

def appli():
    if   instrNum == 0:
        print("stop")
        running = 0
    elif instrNum == 1:
        print("ADD r{} #{} r{}".format(reg1, o, reg2))
        regs[reg2] = regs[reg1] + o
    elif instrNum == 2:
        print("ADD r{} #{} r{}".format(reg1, o, reg2))
        regs[reg2] = regs[reg1] - o
    elif instrNum == 3:
        print("ADD r{} #{} r{}".format(reg1, o, reg2))
        regs[reg2] = regs[reg1] * o
    elif instrNum == 4:
        print("ADD r{} #{} r{}".format(reg1, o, reg2))
        regs[reg2] = regs[reg1] // o
    elif instrNum == 5:
        print("ADD r{} #{} r{}".format(reg1, o, reg2))
        regs[reg2] = regs[reg1] & o
    elif instrNum == 6:
        print("ADD r{} #{} r{}".format(reg1, o, reg2))
        regs[reg2] = regs[reg1] | o
    elif instrNum == 7:
        print("")
    elif instrNum == 8:
        print("")
    elif instrNum == 9:
        print("")
    elif instrNum == 10:
        print("")
    elif instrNum == 11:
        print("")
    elif instrNum == 12:
        print("")
    elif instrNum == 13:
        print("")
    elif instrNum == 14:
        print("")
    elif instrNum == 15:
        print("")
    elif instrNum == 16:
        print("")
    elif instrNum == 17:
        print("")
    elif instrNum == 18:
        print("")


if __name__=='__main__':
    file_hexa = open("program_hexa.txt", "r")

    for line in file_hexa:
        program.append( int(line) )

    file_hexa.close()

    if program == []:
        exit(1)

    while(running):
        showRegs()
        instr = fetch()
        decode(instr)
        appl()
    showRegs()
