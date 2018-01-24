#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INSTR_LIM  100
#define NUM_REGS   32
#define TAILLE_MAX 100
#define MAX_INSTRUCTIONS 100

unsigned regs[ NUM_REGS ];
char data[128];
unsigned long int program[MAX_INSTRUCTIONS];
/* program counter */
int pc = 0;

/* fetch the next word from the program */
unsigned long int fetch()
{

  return program[ pc++ ];
}

/* instruction fields */
unsigned int instrNum = 0;
int reg1     = 0;
int reg2     = 0;
int imm      = 0;
int o        = 0;
int a        = 0;
int n        = 0;

/* decode a word */
void decode( unsigned long int instr )
{

  instrNum = 0;
  reg1     = 0;
  reg2     = 0;
  imm      = 0;
  o        = 0;
  a        = 0;
  n        = 0;

  printf("instr        : %lu\n", instr);
  printf("instr & mask : %lu\n", instr & (31<<27));

  instrNum = (instr & (31<<27)) >> 27;
  //printf("numero d'instruction : %x\n instr & mask : %x \n instr&mask>> : %x \n", instrNum, instr & 0xFFFFFFFF, (instr & 0xFFFFFFFF) >> 27);
  switch (instrNum) {
    case 15:
      imm      = (instr & (1<<26)  )  >> 26;
      o        = (instr & 0x3FFFE0 )  >>  5;
      reg1     = (instr & 0x00001F)       ;
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
      if (imm == 1) {
        printf( "add r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] + o;
        break;
      }
      else {
        printf( "add r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] + regs[o];
        break;
      }
    case 2:
      /* sub   */
      if (imm == 1) {
        printf( "sub r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] - o;
        break;
      }
      else {
        printf( "sub r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] - regs[ o ];
        break;
      }
    case 3:
      /* mult  */
      if (imm == 1) {
        printf( "mult r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] * o;
        break;
      }
      else {
        printf( "mult r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] * regs[ o ];
        break;
      }
    case 4:
      /* div   */
      if (imm == 1) {
        printf( "div r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] / o;
        break;
      }
      else {
        printf( "div r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] / regs[ o ];
        break;
      }
    case 5:
      /* and   */
      if (imm == 1) {
        printf( "and r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] & o;
        break;
      }
      else {
        printf( "and r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] & regs[ o ];
        break;
      }
    case 6:
      /* or    */
      if (imm == 1) {
        printf( "or r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] | o;
        break;
      }
      else {
        printf( "or r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] | regs[ o ];
        break;
      }
    case 7:
      /* xor   */
      if (imm == 1) {
        printf( "xor r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] ^ o;
        break;
      }
      else {

      }
    case 8:
      /* shl   */
      if (imm == 1) {
        printf( "shl r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] << o;
        break;
      }
      else {
        printf( "shl r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] << regs[ o ];
        break;
      }
    case 9:
      /* shr   */
      if (imm == 1) {
        printf( "shr r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] >> o;
        break;
      }
      else {
        printf( "shr r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] >> regs[ o ];
        break;
      }
    case 10:
      /* slt   */
      if (imm == 1) {
        printf( "slt r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] < o;
        break;
      }
      else {
        printf( "slt r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] < regs[ o ];
        break;
      }
    case 11:
      /* sle   */
      if (imm == 1) {
        printf( "sle r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] <= o;
        break;
      }
      else {
        printf( "sle r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] <= regs[ o ];
        break;
      }
    case 12:
      /* seq   */
      if (imm == 1) {
        printf( "seq r%d #%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] == o;
        break;
      }
      else {
        printf( "seq r%d r%d r%d\n", reg1, o, reg2 );
        regs[ reg2 ] = regs[ reg1 ] == regs[ o ];
        break;
      }
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
      n = 0;
      switch(imm)
      {
        case 0:
          printf("Donnez un nombre : ");
          read(0,data,128);
          printf("\n");
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
    printf( "%08X ", regs[ i ] );
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
    while( running )
    {
      showRegs();
      unsigned int instr = fetch();
      decode( instr );
      eval();
    }
    showRegs();
}

int main( int argc, const char * argv[] )  {

  FILE* fichier = fopen(argv[1], "r");
  //printf("avant boucle de lecture\n");
  if(fichier == NULL){
    printf("Erreur lors de la lecture du fichier de programme \n");
  }

  else {
      //L'ouverture du fichier a reussi, on va pouvoir proceder a la traduction
      int i=0;
      char *chaine= (char *)malloc(TAILLE_MAX * sizeof(char));
      while (fgets(chaine, TAILLE_MAX, fichier) != NULL) {
        printf("%s", chaine); //afficher les lignes de la fiche d'instructio
        //program[i] = strtoul(program[i], chaine);
        program[i] = strtoul(chaine, NULL, 16);
        i++;
    }

  fclose(fichier);

  run();
  return 0;
  }
}
