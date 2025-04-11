#include <stdio.h>
#include <stdlib.h>
#define byte unsigned char 

typedef struct instruction {
    byte opcode;
    byte operando;
} Instruction;

int main() {
    byte acc=0, pc=0, stat=0;
    byte opcode, operando;
    int memo[8]; //256 bits, 8x4x8 (8 inteiros, que são 4 bytes ou 32 bits)
    FILE *f = fopen("programa.exe", "rb"); // Abre o arquivo em modo binário
    if (!f) {
        printf("Erro ao abrir o arquivo");
        return 1;
    }

    
    Instruction program[16];
    Instruction instruction;

    printf("Instruções lidas:\n");
    
    int aux=0;
    while (fread(&program[aux], sizeof(Instruction), 1, f) != 0) {
        printf("linha %d: opcode: %02X, operando: %d\n", aux+1, program[aux].opcode, program[aux].operando);
        aux++;
    }

    while(instruction.opcode != 19){
        instruction = program[pc];
        
        switch (instruction.opcode)
        {
        case 0:
            //LOAD MEMORIA
            acc = memo[instruction.operando];
            break;
        case 1: 
            //LOAD CONST
            acc = instruction.operando;
            break;
        case 2:
            //STORE
            memo[instruction.operando] = acc;
            break;
        case 3:
            //ADD
            if ((acc + memo[instruction.operando]<acc)||(acc + memo[instruction.operando]<memo[instruction.operando])) stat = 2;
            acc = acc + memo[instruction.operando];
            if(acc>127 || acc<-128) stat = 4;
            break;
        case 4:
            //SUB
            if ((acc - memo[instruction.operando]>acc)||(acc - memo[instruction.operando]>memo[instruction.operando])) stat = 2;
            acc = acc - instruction.operando;
            if(acc>127 || acc<-128) stat = 4;
            break;
        case 5:
            //MUL
            acc = acc * instruction.operando;
            if(acc>127 || acc<-128) stat = 4;
            break;
        case 6:
            //DIV
            acc = acc / instruction.operando;
            if(acc>127 || acc<-128) stat = 4;
            break;
        case 7:
            //INC
            acc++;
            break;
        case 8:
            //DEC
            acc--;
            break;
        case 9:    
            //AND
            acc = acc & instruction.operando;
            break;
        case 10:
            //OR
            acc = acc | instruction.operando;
            break;
        case 11:
            //NOT
            acc = ~acc;
            break;
        case 12:
            //JMP
            pc = instruction.operando/2;
            break;
        case 13:
            //JZ
            if(acc==0) pc = instruction.operando/2;
            break;
        case 14:
            //JNZ
            if(acc!=0) pc = instruction.operando/2;
            break;
        case 15:
            //JG
            if(acc>0) pc = instruction.operando/2;
            break;
        case 16:
            //JL
            if(acc<0) pc = instruction.operando/2;
            break;
        case 17:
            //JGE
            if(acc>=0) pc = instruction.operando/2;
            break;
        case 18:
            //JLE
            if(acc<=0) pc = instruction.operando/2;
            break;
        case 19:
            //HLT
            printf("ACC: %d, STAT: %d", acc, 000);
            return 0;
            break;
        }
        pc=pc+1;
    }
    fclose(f);
    return 0;
}
