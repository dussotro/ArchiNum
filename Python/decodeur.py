#coding: utf-8

import sys

#variables globales
TAILLE_MAX = 100
MAX_INSTRUCTIONS = 100
MAX_LABEL = 50
LONGUEUR_MOT = 5
TAILLE_TAB_LABELS = 100

ref_registre = 'R'
char_label_deb = '<'
char_label_fin = '<'
char_instr = '>'


class Label:
    def __init__(self, nomL = '', noInstr = 0):
	self.nom = nomL
        self.no_instr = noInstr

class Instruction:
    def __init__(self, motI = '', noReg1 = 0, noReg2 = 0, nb = 0):
        self.mot = motI
        self.no_reg1 = noReg1
        self.no_reg2 = noReg2
        self.nombre = nb

def instr2unsignedlongint(Instruction instr):
    possibleCommande = "STOP-ADD-SUB-MULT-DIV-AND-OR-XOR-SHL-SHR-SLT-SLE-SEQ-LOAD-STORE-JMP-BRAZ-BRANZ-SCALL-"
    possibleComande = possibleCommande.split("-")
    ret = 0

    no_instr=0
    for i in possibleCommande):
        if(instr.mot = i):
            #on est tombe sur la bonne fonction
            break
        no_instr++

    ret = no_instr << 27
    ret = ret & 0xFFFFFFFF
    print(" ret : {}".format(ret))

    imm = 1
    flag_label = 0

    if(no_instr == 19):
        ret = 0

    elif(no_instr == 15):
        if(instr.no_reg1[0]==ref_registre):
            imm = 0
            instr.no_reg1 = instr.no_reg1[1:]
        else:
            flag_label = 1
            lettre1 = instr.no_reg1[0]
            lettre2 = instr.no_reg2[0]
        instr.nombre = instr.nombre[1:]

        reg1 = int(instr.nombre)
        ret += reg1
        ret = ret + (imm << 26)

        if(flag_label==0):
            o = int(instr.no_reg1)
            ret = ret + (o << 5)
        else:
            ret = ret + (lettre2 <<5)
            ret = ret + (lettre1 <<13)

    elif(no_instr == 16):
        instr.no_reg1 = instr.no_reg1[1:]
        reg1 = int(instr.no_reg1)

        if(0 <= instr.nombre[0] and instr.nombre[0] <= 9):
            o = int(instr.nombre)
            ret+=o
        else:
            for q in range(len(instr.nombre)):
                o = int(inst.nombre[q])
                ret = ret + (0 << (16 - (q+1)*8));

        ret = ret + (reg1 << 22)

    elif(no_instr == 17):
        instr.no_reg1 = instr.no_reg1[1:]
        reg1 = int(instr.no_reg1) 

        if(0 <= instr.nombre[0] and instr.nombre[0] <= 9):
            o = int(instr.nombre)
            ret+=o
        else:
            for q in range(len(instr.nombre)):
                o = int(inst.nombre[q])
                ret = ret + (0 << (16 - (q+1)*8));

        ret = ret + (reg1 << 22)

    elif(no_instr == 18):
        o = int(instr.nombre)
        ret = ret + o

    else:
        instr.no_reg1 = instr.no_reg1[1:]
        instr.no_reg2 = instr.no_reg2[1:]
        reg1 = instr.no_reg1
        reg2 = instr.no_reg2

        if(instr.nombre[0] == ref_registre):
            imm = 0
            instr.nombre = instr.nombre[1:]

        o = int(instr.nombre)
        ret = ret + reg2 + (o<<5) + (imm<<21) + (reg1<<22)

    return ret


def chaine2instr(chaine):
    print("Decodage de ", chaine)
    instruction = Instruction()

    i = 0
    
    chaine = chaine[1:]
    chaine = chaine.split(' ')

    instruction.mot = chaine[0]

    esp = len(chaine)-1
    if(esp==0):
        #Do nothing
    elif(esp==1):
        instruction.nombre = [1]
    elif(esp==2):
        instruction.no_reg1 = chaine[1]
        instruction.nombre  = chaine[2]
    elif(esp==3):
        instruction.no_reg1 = chaine[1]
        instruction.nombre  = chaine[2]
        instruction.no_reg2 = chaine[3]
    else:
        print("ERREUR CRITIQUE : Autodestruction dans 5 secondes")

    return instruction

def label2unsignedlongint(label):

    return ((int)label.nom[0] << 24) + ((int)label.nom[1] << 16) + ((int)label.nom[2] << 8) + ((int)label->nom[3])


if __name__== "__main__":

    fichierL = open(sys.argv[1], "r")
    if(fichierL == None):
        print("Erreur Critique : Impossible de lire le code assembleur")
    else:
        cptL = 0
        tableauL=[]
        ligne = fichierL.readline()
        while ligne:
            if(ligne[0] == char_label_deb):
                print("Un label a été trouvé")
                tableauL.append(Label(ligne,cptL+1))
        cptL++

        fichierL.close()

    fichier = open(sys.argv[1], "r")


    

