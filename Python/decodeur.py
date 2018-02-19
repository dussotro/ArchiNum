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

global tableauL
tableauL = []


class Label:
    def __init__(self, nomL = '', noInstr = 0):
        self.nom = nomL
        self.no_instr = noInstr

    def __getitem__(self, indice):
        if(indice == 0):
            return self.nom
        elif(indice == 1):
            return self.no_instr
        else:
            return "Error"

class Instruction:
    def __init__(self, motI = '', noReg1 = 0, noReg2 = 0, nb = 0):
        self.mot = motI
        self.no_reg1 = noReg1
        self.no_reg2 = noReg2
        self.nombre = nb

def instr2unsignedlongint(instr):
    global tableauL
    possibleCommande = "STOP-ADD-SUB-MULT-DIV-AND-OR-XOR-SHL-SHR-SLT-SLE-SEQ-LOAD-STORE-JMP-BRAZ-BRANZ-SCALL"
    possibleComande = possibleCommande.split("-")
    ret = 0
    if(instr.mot[-1]=="\n"):
        instr.mot = instr.mot[:-1]
    for i in range(len(possibleCommande)):
        if(instr.mot == possibleComande[i]):
            #on est tombe sur la bonne fonction
            break

    no_instr = i

    ret = no_instr << 27
    ret = ret & 0xFFFFFFFF

    imm = 1
    flag_label = 0

    if(no_instr == 0):
        ret = 0

    elif(no_instr == 15):
        #JMP
        noreg1 = instr.no_reg1
        print("valeur reg 1 : {}".format(instr.no_reg1))
        print("valeur reg 2 : {}".format(instr.no_reg2))
        print("valeur nombre : {}".format(instr.nombre))
        if(instr.no_reg1[0]==ref_registre):
            imm = 0
            instr.no_reg1 = instr.no_reg1[1:]
        else:
            flag_label = 1
            for q in range(len(tableauL)):
                if(tableauL[q][0] == instr.no_reg1):
                    noreg1 = int(tableauL[q][1])


        instr.nombre = instr.nombre[1:]

        reg1 = int(instr.nombre)
        ret += reg1
        ret = ret + (imm << 26)

        if(flag_label==0):
            o = int(instr.no_reg1)
            ret = ret + (o << 5)
        else:
            ret = ret + (int(noreg1) <<5)

    elif(no_instr == 16):
        #BRAZ
        instr.no_reg1 = instr.no_reg1[1:]
        reg1 = int(instr.no_reg1)

        if(0 <= instr.nombre[0] and instr.nombre[0] <= 9):
            o = int(instr.nombre)
            ret+=o
        else:
            for q in range(len(tableauL)):
                if(tableauL[q][0] == instr.nombre):
                    o = int(tableauL[q][1])
                    ret += o

        ret = ret + (reg1 << 22)

    elif(no_instr == 17):
        #BRANZ
        instr.no_reg1 = instr.no_reg1[1:]
        reg1 = int(instr.no_reg1)

        if(0 <= instr.nombre[0] and instr.nombre[0] <= 9):
            o = int(instr.nombre)
            ret+=o
        else:
            for q in range(len(tableauL)):
                if(tableauL[q][0] == instr.nombre):
                    o = int(tableauL[q][1])
                    ret += o

        ret = ret + (reg1 << 22)

    elif(no_instr == 18):
        o = int(instr.nombre[0])
        ret = ret + o

    else:
        reg1 = instr.no_reg1
        if(str(reg1)[0]==ref_registre):
            reg1 = reg1[1:]
        reg2 = instr.no_reg2
        if(str(reg2)[0]==ref_registre):
            reg2 = reg2[1:]

        if(str(instr.nombre)[0] == ref_registre):
            imm = 0
            instr.nombre = instr.nombre[1:]

        o = int(instr.nombre)
        ret = ret + int(reg2) + (o<<5) + (imm<<21) + (int(reg1)<<22)

    print("valeur hexa de la valeur de retour : {:32b}".format(ret))
    return ret


def chaine2instr(chaine):
    print("Decodage de " + chaine)
    instruction = Instruction()

    i = 0

    chaine = chaine[1:]
    chaine = chaine.split(' ')

    instruction.mot = chaine[0]

    esp = len(chaine)-1
    if(esp==0):
        #Do nothing
        poubelle=0
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

    for j in range(esp + 1):
        print("token {} : {}".format(j,chaine[j]))
    return instruction

def label2unsignedlongint(label):

    return ((int)(label.nom[0]) << 24) + ((int)(label.nom[1]) << 16) + ((int)(label.nom[2]) << 8) + ((int)(label.nom[3]))


if __name__== "__main__":
    global tableauL

    print("Ici Star Command")

    fichierL = open(sys.argv[1], "r")
    if(fichierL == None):
        print("Erreur Critique : Impossible de lire le code assembleur")
    else:
        print("Le fichier a été ouvert avec succès Woody")
        cptL = 0
        ligne = fichierL.readline()
        ligne = ligne[:-2]
        while ligne:
            if(ligne[0] == char_label_deb):
                print("Un label a été trouvé")
                ligne = ligne[1:-1]
                tableauL.append(Label(ligne,cptL+1))
            ligne = fichierL.readline()
            ligne = ligne[:-1]
            cptL+=1

        fichierL.close()

    fichier = open(sys.argv[1], "r")
    program = []
    cpt = 0
    if(fichier == None):
        print("Erreur critique : impossible de lire le code assembleur")
    else:
        print("Le fichier a été ouvert avec succès Woody")
        ligne = fichier.readline()
        while ligne:
            if(ligne[0]==char_instr):
                instruction = chaine2instr(ligne)
                int_instr = instr2unsignedlongint(instruction)
            program.append(int_instr)
            ligne = fichier.readline()
            cpt+=1
        fichier.close()

    fichierH = open(sys.argv[2], "w")
    if(fichierH == None):
        print("Erreur critique : impossible d'écrire dans le fichier de destination")
    else:
        m=0
        while(m<cpt):
            fichierH.write("{:08x}\n".format(program[m]))
            m+=1
        fichierH.close()
