import numpy as np
import sys

class VM:

    def __init__(self):

        self.INSTR_LIM        = 100
        self.NUM_REGS         = 32
        self.TAILLE_MAX       = 100
        self.MAX_INSTRUCTIONS = 100

        self.regs    = np.zeros((self.NUM_REGS), dtype=int)
        self.data    = np.zeros((1,128))
        self.program = []
        self.pc      = 0

        self.running = 1

        self.instrNum = 0
        self.reg1     = 0
        self.reg2     = 0
        self.imm      = 0
        self.o        = 0
        self.a        = 0
        self.n        = 0

    def fetch(self):
        self.pc += 1
        return self.program[self.pc]

    def decode(self):

        print(" ###")

        self.instrNum = self.instr&(31<<27)>>27

        if (self.instrNum == 15):
            self.imm      = (self.instr & (1<<26)  )  >> 26
            self.o        = (self.instr & 0x3FFFE0 )  >>  5
            self.reg1     = (self.instr & 0x00001F)
        elif (self.instrNum == 16):
            self.reg1     = (self.instr & 0x7C00000)  >> 26
            self.a        = (self.instr & 0x3FFFFF )
        elif (self.instrNum == 17):
            self.reg1     = (self.instr & 0x7C00000)  >> 26
            self.a        = (self.instr & 0x3FFFFF )
        elif (self.instrNum == 18):
            self.n        = (self.instr & 0x7FFFFFF)
        else:
            self.reg1     = (self.instr & 0x7C00000)  >> 22
            self.imm      = (self.instr & 0x200000 )  >> 21
            self.o        = (self.instr & 0x1FFFE0 )  >>  5
            self.reg2     = (self.instr & 0X1F     )
        print(" imm {} \n reg 1 {} \n reg 2 {} \n a {} \n o {} \n n {} \n".format(self.imm, self.reg1, self.reg2, self.a, self.o, self.n))
        print(" ###")

    def evaluate(self):
        print("===> instruNum = {}".format(self.instrNum))
        if (self.instrNum == 0):
            # halt
            print("stop")
            self.running = 0
        elif self.instrNum == 1:
            # add
            if (self.imm == 1):
                print( "add r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] + self.o
            else:
                print( "add r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] + self.regs[self.o]
        elif self.instrNum ==  2:
            # add
            if (self.imm == 1):
                print( "sub r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] - self.o
            else:
                print( "sub r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] - self.regs[ self.o ]
        elif self.instrNum == 3:
            # mult
            if (self.imm == 1):
                print( "mult r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] * self.o
            else:
                print( "mult r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] * self.regs[ self.o ]
        elif self.instrNum == 4:
            # div
            if (self.imm == 1):
                print( "div r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] / self.o
            else :
                print( "div r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] / self.regs[ self.o ]
        elif self.instrNum == 5:
            # and
            if (self.imm == 1):
                print( "and r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] & self.o
            else :
                print( "and r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] & self.regs[ self.o ]

        elif self.instrNum == 6:
            #or
            if (self.imm == 1) :
                print( "or r{} #{} r{}".format( self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] | self.o
            else :
                print( "or r{} r{} r{}".format( self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] | self.regs[ self.o ]
        elif self.instrNum == 7:
            #xor
            if (self.imm == 1):
                print( "xor r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] ^ self.o

            else :
                print( "xor r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] ^ self.o
        elif self.instrNum == 8:
            # shl
            if (self.imm == 1):
                print( "shl r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] << self.o;
            else:
                print( "shl r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] << self.regs[ self.o ]

        elif self.instrNum == 9:
            # shr
            if (self.imm == 1):
                print( "shr r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] >> self.o

            else :
                print( "shr r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] >> self.regs[ self.o ]

        elif self.instrNum == 10:
            # slt
            if (self.imm == 1):
                print( "slt r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] < self.o

            else:
                print( "slt r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] < self.regs[ self.o ]

        elif self.instrNum == 11:
            # sle
            if (self.imm == 1):
                print( "sle r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] <= self.o

            else:
                print( "sle r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] <= self.regs[ self.o ]

        elif self.instrNum == 12:
            #seq
            if (self.imm == 1):
                print( "seq r{} #{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] == self.o

            else:
                print( "seq r{} r{} r{}".format(self.reg1, self.o, self.reg2) )
                self.regs[ self.reg2 ] = self.regs[ self.reg1 ] == self.regs[ self.o ]

        elif self.instrNum == 13:
            #load
            print( "load r{} #{} r{}".format(self.reg1, self.o, self.reg2))
            self.regs[ self.reg2 ] = self.regs[self.reg1 + self.o]


        elif self.instrNum == 14:
            #store
            print( "store r{} #{} r{}".format(self.reg1, self.o, self.reg2))
            self.regs[ self.reg1 + self.o ] = self.regs[self.reg2]


        elif self.instrNum == 15:
            # jmp
            print( "jmp r{} r{}", self.reg1, self.imm )
            if (self.pc + self.imm > self.INSTR_LIM or self.program[self.pc+self.imm] == 0):
                perror("Damn ! You jump too far !")

            self.instr = self.program[self.pc + self.imm]
            self.decode()
            self.evaluate()

        elif self.instrNum == 16:
            # braz
            print("braz r{} r{}".format(self.reg1, self.imm))
            if (self.regs[self.reg1] == 0):
                self.instr = self.program[self.imm]
                self.decode(instr)
                self.evaluate()

        elif self.instrNum == 17:
            #branz
            print("branz r{} r{}".format(self.reg1, self.imm))
            if (self.regs[self.reg1] != 0):
                self.instr = self.program[self.imm]
                self.decode(instr)
                self.evaluate()
     
        elif instrNum == 18:
            #scall

            print("scall n{}".format(self.n))
            n = 0

            if (self.imm == 0):
                self.data = input("Donnez un nombre : ")
                self.regs[self.reg1] = int(self.data)
            else:
                print("Dans le registre {} : {}".format(self.reg1, self.regs[self.reg1]))



    """ display all registers as 4-digit hexadecimal words """
    def showRegs(self):
      print( "self.regs = " )
      cpt=0
      for i in range(self.NUM_REGS // 4):
        print( "{:08x} {:08x} {:08x} {:08x} ".format(int(self.regs[ i ]), int(self.regs[i+1]), int(self.regs[i+2]), int(self.regs[i+3])))

    def run(self):
        while( self.running ):
            self.showRegs()
            self.instr = self.fetch()
            self.decode()
            self.evaluate()
        self.showRegs()


if __name__=='__main__':

    VirtualMachine = VM()

    fichier = open(sys.argv[1], 'r')
    if (fichier == None):
        print("Erreur de lecture de fichier")
    else:
        i = 0
        fichier_chaine = fichier.readlines()

    for ind,elem in enumerate(fichier_chaine):
        VirtualMachine.program.append(int("0x"+elem[:-1],0))
    print(VirtualMachine.program)

    VirtualMachine.run()
