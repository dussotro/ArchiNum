import numpy as np
import sys
import time
from math import log


class CacheLine:
    #simule une ligne de Cache

    def __init__(self, taille):
        self.use = 0
        self.valid = 0
        self.tag = 0
        self.data = [0] * taille

class Cache:
    #simulation d'un Cache à correspondance directe

    def __init__(self, S, B, memorySize):

        #initialisation quantités S,E,B,m avec E=1 et m= log2(taille.memoire)
        self.nbSets = S
        self.setSize = int(log(S,2)) #taille du champ set
        self.nbLines = 1
        self.blockSize = B
        self.nbBits = int(log(memorySize,2))

        self.mask_set = 2**(self.setSize+1)-1

        self.lines = [CacheLine(self.blockSize) for i in range(memorySize // self.blockSize)] #création des lignes de notre Cache

    def getTag(self, adresse):
        #retourne le tag d'une adresse
        return adresse >> (self.blockSize+self.setSize)

    def getSet(self, adresse):
        numSet =  (adresse >> self.blockSize) & self.mask_set
        return self.lines[numSet]

    def read(self, addresse):
        #Lit un bloc de mémoire
        tagA = getTag(addresse)  # Tag of cache line
        setA = getSet(addresse)  # Set of cache lines

        if (self.lines[setA].tag == tagA and self.lines[setA].valid):
            return self.lines[setA].data

    def write(self, adresse, donnee):
        #ecrit une donnee dans un bloc memoire
        tagA = getTag(addresse)  # Tag of cache line
        setA = getSet(addresse)  # Set of cache lines

        if (self.lines[setA].tag == tagA and self.lines[setA].valid):
            self.lines[setA].data == donnee





class VM:

    def __init__(self):

        self.INSTR_LIM        = 100
        self.NUM_REGS         = 32
        self.TAILLE_MAX       = 100
        self.MAX_INSTRUCTIONS = 100

        self.regs    = np.zeros((self.NUM_REGS), dtype=int)
        self.data    = np.zeros((1,128))
        self.program = []
        self.pc      = -1

        self.running = 1

        self.instrNum = 0
        self.reg1     = 0
        self.reg2     = 0
        self.imm      = 0
        self.o        = 0
        self.a        = 0
        self.n        = 0

        self.cache = Cache(16,8,1024)

    def fetch(self):
        self.pc += 1
        return self.program[self.pc]

    def decode(self):
        #decodage d'une ligne ecrite au format hexadecimal
        self.instrNum = (self.instr & (31 << 27)) >> 27

        if (self.instrNum == 15):
            self.imm      = (self.instr & (1<<26)  )  >> 26
            self.o        = (self.instr & 0x3FFFE0 )  >>  5
            self.reg1     = (self.instr & 0x00001F)
        elif (self.instrNum == 16):
            self.reg1     = (self.instr & 0x7C00000)  >> 22
            self.a        = (self.instr & 0x3FFFFF )
        elif (self.instrNum == 17):
            self.reg1     = (self.instr & 0x7C00000)  >> 22
            self.a        = (self.instr & 0x3FFFFF )
        elif (self.instrNum == 18):
            self.reg1     = (self.instr & 0x7C00000)  >> 22
            self.imm        = (self.instr & 0x3FFFFFF)
        else:
            self.reg1     = (self.instr & 0x7C00000)  >> 22
            self.imm      = (self.instr & 0x200000 )  >> 21
            self.o        = (self.instr & 0x1FFFE0 )  >>  5
            self.reg2     = (self.instr & 0X1F     )

    def evaluate(self, renouvelable = 1):
        #petite precision : le parametre "renouvelable" empeche l'appel recursif infini de la fonction
        print("evaluation ===> instruNum = {}".format(self.instrNum))
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

            self.cache.read(self.reg1 + self.o)


        elif self.instrNum == 14:
            #store
            print( "store r{} #{} r{}".format(self.reg1, self.o, self.reg2))
            self.regs[ self.reg1 + self.o ] = self.regs[self.reg2]

            self.cache.write(self.reg1 + self.o , self.regs[self.reg2])


        elif self.instrNum == 15:
            # jmp
            print( "jmp r{} L{}".format(self.reg1, self.o ))
            self.regs[self.reg1] = self.pc + 1
            self.pc = self.o
            self.instr = self.program[self.o]
            self.decode()
            if(renouvelable==1):
                self.evaluate(0)

        elif self.instrNum == 16:
            # braz
            print("braz r{} L{}".format(self.reg1, self.a))
            if (self.regs[self.reg1] == 0):
                self.instr = self.program[self.a]
                self.pc = self.a
                self.decode()
                if(renouvelable==1):
                    self.evaluate(0)

        elif self.instrNum == 17:
            #branz
            print("branz r{} L{}".format(self.reg1, self.a))
            if (self.regs[self.reg1] != 0):
                self.instr = self.program[self.a]
                self.pc = self.a
                self.decode()
                if(renouvelable==1):
                    self.evaluate(0)

        elif self.instrNum == 18:
            #scall

            print("scall n {}".format(self.imm))

            if (self.imm == 0):
                self.data = input("Donnez un nombre : ")
                self.regs[self.reg1] = int(self.data)
            else:
                print("Dans le registre {} : {}".format(self.reg1, self.regs[self.reg1]))



    """ display all registers as 4-digit hexadecimal words """
    def showRegs(self):
      print( "self.regs = " )

      for i in range(self.NUM_REGS // 4):
        print( "{:08x} {:08x} {:08x} {:08x} ".format(int(self.regs[ 4*i ]), int(self.regs[4*i+1]), int(self.regs[4*i+2]), int(self.regs[4*i+3])))

    def run(self):
        while( self.running ):
            self.showRegs()
            self.instr = self.fetch()
            self.decode()
            self.evaluate()
        self.showRegs()


if __name__=='__main__':

    tps1 = time.clock()

    VirtualMachine = VM()

    fichier = open(sys.argv[1], 'r')
    if (fichier == None):
        print("Erreur de lecture de fichier")
    else:
        fichier_chaine = fichier.readlines()

    for ind,elem in enumerate(fichier_chaine):
        VirtualMachine.program.append(int("0x"+elem[:-1],0))
    print(VirtualMachine.program)

    VirtualMachine.run()

    tps2 = time.clock()

    print("temps d'execution du program : {} sec".format(tps2-tps1))
