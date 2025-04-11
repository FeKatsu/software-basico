#include <stdio.h>

#define byte unsigned char

typedef struct instrucao{
    byte opcode;
    byte operand;
}Instrucao;

Instrucao prog [256];
byte memo [256] = {0};
int acc;
int stats;

int load$(int op){
    acc = memo[op];
}

int load(int op){
    acc = op;
}

int store(int op){
    memo[op] = acc;
}

int add(int op){
    acc += memo[op];
}

int sub(int op){
    acc -= memo[op];
}

int mul(int op){
    acc *= memo[op];
}

int div(int op){
    acc /= memo[op];
}

int inc(){
    acc += 1;
}

int dec(){
    acc -= 1;
}

int and(int op){
    acc = acc & memo[op];
}

int or(int op){
    acc = acc | memo[op];
}

int not(){
    acc = ~acc;
}

int jmp(int *pc, int op){
    *pc = op/2;
}

int jz(int *pc, int op){
    if(acc == 0) *pc = op/2;
    else *pc++;
}

int jnz(int *pc, int op){
    if(acc != 0) *pc = op/2;
    else *pc++;
}

int jg(int *pc, int op){
    if(acc > 0) *pc = op/2;
    else *pc++;
}

int jl(int *pc, int op){
    if(acc < 0) *pc = op/2;
    else *pc++;
}

int jge(int *pc, int op){
    if(acc >= 0) *pc = op/2;
    else *pc++;    
}

int jle(int *pc, int op){
    if(acc <= 0) *pc = op/2;
    else *pc++;
}

int main(){
    FILE *file = fopen("programa.exe", "rb");
    int inst = 0, pc = 0;
    short buff;

    while(fread(&prog[inst], sizeof(Instrucao), 1, file) != 0){
        inst++;
    }

    fclose(file);

    while (prog[pc].opcode != 19) {
        stats = 0;
        printf("pc inicio: %d\n", memo[1]);

        switch (prog[pc].opcode) {
            case 0: //LOAD$
                load$(prog[pc].operand);
                pc++;
                break;
            case 1: // LOAD
                load(prog[pc].operand);
                pc++;
                break;
            case 2: // STORE
                store(prog[pc].operand);
                pc++;
                break;
            case 3: // ADD
                add(prog[pc].operand);
                pc++;
                break;
            case 4: // SUB
                sub(prog[pc].operand);
                pc++;
                break;
            case 5: // MUL
                mul(prog[pc].operand);
                pc++;
                break;
            case 6: // DIV
                div(prog[pc].operand);
                pc++;
                break;
            case 7: // INC
                inc();
                pc++;
                break;
            case 8: // PC
                dec();
                pc++;
                break;
            case 9: // AND
                and(prog[pc].operand);
                pc++;
                break;
            case 10: // OR
                or(prog[pc].operand);
                pc++;
                break;
            case 11: // NOT
                not();
                pc++;
                break;
            case 12: //JMP
                jmp(&pc, prog[pc].operand);
                break;
            case 13: //JZ
                jz(&pc, prog[pc].operand);
                break;
            case 14: // JNZ
                jnz(&pc, prog[pc].operand);
                break;
            case 15: // JG
                jg(&pc, prog[pc].operand);
                break;
            case 16: // JL
                jl(&pc, prog[pc].operand);
                break;
            case 17: // JGE
                jge(&pc, prog[pc].operand);
                break;
            case 18: // JLE
                jle(&pc, prog[pc].operand);
                break;
        }

        acc = (byte)acc;
        if (acc == 0) {
            stats = stats | 1;
        }
        printf("pc fim: %d\n", pc);
        printf("acc: %d stat: %d\n", acc, stats);
    }


    return 0;
}