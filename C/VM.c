#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INSTR_LIM  100
#define NUM_REGS   32
#define TAILLE_MAX 100
#define MAX_INSTRUCTIONS 100

unsigned regs[ NUM_REGS ];
char data[128];
unsigned program[];
/* program counter */
int pc = 0;

/* fetch the next word from the program */
int fetch()
{
  return program[ pc++ ];
}

/* instruction fields */
int instrNum = 0;
int reg1     = 0;
int reg2     = 0;
int imm      = 0;
int o        = 0;
int a        = 0;
int n        = 0;

/* decode a word */
void decode( int instr )
{
  instrNum = (instr & 0xF8000000) >> 27;
  switch (instrNum) {
    case 15:
      imm      = (instr & 0x400000 )  >> 26;
      o        = (instr & 0x3FFFE0 )  >>  5;
      reg1     = (instr & 0x1F     )       ;
      break;
    case 16:
      reg1     = (instr & 0x7C00000)  >> 26;
      a        = (instr & 0x3FFFFF )       ;
      break;
    case 17:
      reg1     = (instr & 0x7C00000)  >> 26;
      a        = (instr & 0x3FFFFF )       ;
      break;
    case 18:
      n        = (instr & 0x7FFFFFF)       ;
      break;
    default:
      reg1     = (instr & 0x7C00000)  >> 22;
      imm      = (instr & 0x200000 )  >> 21;
      o        = (instr & 0x1FFFE0 )  >>  5;
      reg2     = (instr & 0X1F     )       ;
  }

}

/* the VM runs until this flag becomes 0 */
int running = 1;

/* evaluate the last decoded instruction */
void eval()
{
  switch( instrNum )
  {
    case 0:
      /* halt */
      printf( "stop\n" );
      running = 0;
      break;
    case 1:
      /* add   */
      printf( "add r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] + o;
      break;
    case 2:
      /* sub   */
      printf( "sub r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] - o;
      break;
    case 3:
      /* mult  */
      printf( "mult r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] * o;
      break;
    case 4:
      /* div   */
      printf( "div r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] / o;
      break;
    case 5:
      /* and   */
      printf( "and r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] & o;
      break;
    case 6:
      /* or    */
      printf( "or r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] | o;
      break;
    case 7:
      /* xor   */
      printf( "xor r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] ^ o;
      break;
    case 8:
      /* shl   */
      printf( "shl r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] << o;
      break;
    case 9:
      /* shr   */
      printf( "shr r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] >> o;
      break;
    case 10:
      /* slt   */
      printf( "slt r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] < o;
      break;
    case 11:
      /* sle   */
      printf( "sle r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] <= o;
      break;
    case 12:
      /* seq   */
      printf( "seq r%d #%d r%d\n", reg1, o, reg2 );
      regs[ reg2 ] = regs[ reg1 ] == o;
      break;
    case 13:
      /* load  */
      printf( "load r%d #%d r%d\n", reg1, o, reg2);
      regs[ reg2 ] = regs[reg1 + o];
      break;
    case 14:
      /* store */
      printf( "store r%d #%d r%d\n", reg1, o, reg2);
      regs[ reg1 + o ] = regs[reg2];
      break;
    case 15:
      /* jmp   */
      printf( "jmp r%d r%d", reg1, imm );
      if (pc + imm > INSTR_LIM || program[pc+imm] == 0) {
        perror("Damn ! You jump too far !");
        break;
      }
      int instr = program[pc + imm];
      decode(instr);
      eval();
      break;
    case 16:
      /* braz  */
      printf("braz r%d r%d", reg1, imm);
      if (regs[reg1] == 0) {
        int instr = program[imm];
        decode(instr);
        eval();
      }
      break;
    case 17:
      /* branz */
      printf("branz r%d r%d", reg1, imm);
      if (regs[reg1] != 0) {
        int instr = program[imm];
        decode(instr);
        eval();
      }
      break;
    case 18:
      /* scall */
      printf("scall n%d", n);
      reg1 = 0;
      switch(imm)
      {
        case 0:
          read(0,data,128);
          regs[reg1] = atoi(data);
          break;
        case 1:
          printf("data : %d", reg1);
          break;
      }
      break;
  }
}

/* display all registers as 4-digit hexadecimal words */
void showRegs()
{
  printf( "regs = \n" );
  int cpt=0;
  for(int i=0; i<NUM_REGS; i++ ) {
    printf( "%016X ", regs[ i ] );
    cpt++;
    if(cpt == 4)
    {
      printf(" \n");
      cpt = 0;
    }
  }
  printf( "\n" );
}

void run()
{
  FILE* fichier = fopen("program_hexa.txt", "r");
  //printf("avant boucle de lecture\n");
  if(fichier == NULL){
    printf("Erreur lors de la lecture du fichier de programme");
  }
  else {
      //L'ouverture du fichier a reussi, on va pouvoir proceder a la traduction
      int i=0;
      char *chaine= (char *)malloc(TAILLE_MAX * sizeof(char));
      while (fgets(chaine, TAILLE_MAX, fichier) != NULL) {
        printf("%s", chaine); //afficher les lignes de la fiche d'instructio
        program[i] = atoi(chaine);
        i++;
    }
    fclose(fichier);

    while( running )
    {
      showRegs();
      int instr = fetch();
      decode( instr );
      eval();
    }
    showRegs();
  }
}

int main( int argc, const char * argv[] )
{
  run();
  return 0;
}
