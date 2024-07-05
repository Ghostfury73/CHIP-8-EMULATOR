#include "chip8.h"
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

const char chip8_default_character_set[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(struct chip8* chip8){
    memset(chip8,0,sizeof(struct chip8));
    memcpy(&chip8->memory.memory,chip8_default_character_set,sizeof(chip8_default_character_set));
}

void chip8_load(struct chip8* chip8,const char* buf,size_t size){
    assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS],buf,size);
    chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

static void chip8_exec_extended_8(struct chip8* chip8,unsigned short opcode){
    unsigned char x=(opcode >>8)&0x000f;
    unsigned char y=(opcode >>4)&0x000f;
    unsigned char last_byte=opcode&0x000f;
    unsigned short temp=0;

    switch(last_byte){
        case 0x00:   //8xy0 -LD Vx,Vy Vx=Vy
            chip8->registers.V[x]=chip8->registers.V[y];
        break;
        
        case 0x01:   //8xy1 - OR Vx,Vy PERFORMS OR on Vx and Vy and stores in Vx
            chip8->registers.V[x]= chip8->registers.V[x] | chip8->registers.V[y];
        break;
        
        case 0x02:   //8xy2 - AND Vx,Vy PERFORMS AND on Vx and Vy and stores in Vx
            chip8->registers.V[x]= chip8->registers.V[x] & chip8->registers.V[y];
        break;
        
        case 0x03:   //8xy3 - XOR Vx,Vy PERFORMS XOR on Vx and Vy and stores in Vx
            chip8->registers.V[x]= chip8->registers.V[x] ^ chip8->registers.V[y];
        break;
        
        case 0x04:   //8xy4 - ADD Vx,Vy SET VF=carry 
            temp=chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0f]=false;
            if(temp>0xff){
                chip8->registers.V[0x0f]=true;
            }

            chip8->registers.V[x]=temp;
        break;
        
        case 0x05:   //8xy5 - SUB Vx,Vy Set vx=vx-vy set vf=not carry; 
            chip8->registers.V[0x0f]=false;
            if(chip8->registers.V[x]>chip8->registers.V[y]){
                chip8->registers.V[0x0f]=true;
            }
            chip8->registers.V[x]=chip8->registers.V[x] + chip8->registers.V[y];
        break;
        
        case 0x06:   //8xy6 - SHR Vx{,Vy}; 
            chip8->registers.V[0x0f]=chip8->registers.V[x] & 0x01;
            chip8->registers.V[x] /= 2;
        break;

        case 0x07:  //8xy7 - SUBN Vx,Vy;
            chip8->registers.V[0x0f] =chip8->registers.V[y]>chip8->registers.V[x];
            chip8->registers.V[x] = chip8->registers.V[y]-chip8->registers.V[x];
        break;

        case 0x0E:   //8xyE - SHL Vx{,Vy}; 
            chip8->registers.V[0x0f]=chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] *= 2;
        break;

    }
}

static char chip8_wait_for_key_press(struct chip8* chip8){
    SDL_Event event;

    while(SDL_WaitEvent(&event)){
        if(event.type!=SDL_KEYDOWN){
            continue;
        }

        char c =event.key.keysym.sym;
        char chip8_key =chip8_keyboard_map(&chip8->keyboard,c);

        if(chip8_key != -1){
            return chip8_key;
        }
    }

    return -1;
}

static void chip8_exec_extended_F(struct chip8* chip8,unsigned short opcode){
    unsigned char x=(opcode >>8)&0x000f;
    switch(opcode & 0x00ff){
        case 0x07:    //fx07 set Vx to delay timer value
            chip8->registers.V[x]=chip8->registers.delay_timer;
        break;

        case 0x0A:  //fx0A 
        {
            char pressed_key =chip8_wait_for_key_press(chip8);
            chip8->registers.V[x]=pressed_key;
        }
        break;

        case 0x15: //fx15 set delay timer=Vx
            chip8->registers.delay_timer=chip8->registers.V[x];
        break;

        case 0x18: //fx18 set sound timer=vx
            chip8->registers.sound_timer=chip8->registers.V[x];
        break;

        case 0x1e: //fx1e Add I,Vx
            chip8->registers.I +=chip8->registers.V[x];
        break;

        case 0x29: //fx29
            chip8->registers.I=chip8->registers.V[x]*CHIP8_DEFAULT_SPRITE_HEIGHT;
        break;

        case 0x33:  //fx33
        {
            unsigned char temp=chip8->registers.V[x];

            unsigned char ones =temp%10;
            temp =temp/10;
            unsigned char tens =temp%10;
            temp=temp/10;
            unsigned char hundreds=temp;

            chip8_memory_set(&chip8->memory,chip8->registers.I,hundreds);
            chip8_memory_set(&chip8->memory,chip8->registers.I+1,tens);
            chip8_memory_set(&chip8->memory,chip8->registers.I+2,ones);
        }
        break;

        case 0x55:  //fx55
        {
            for(int i=0;i<=x;i++){
                chip8_memory_set(&chip8->memory,chip8->registers.I+i,chip8->registers.V[i]);
            }
        }
        break;

        case 0x65:
        {
            for(int i=0;i<=x;i++){
                chip8->registers.V[i]=chip8_memory_get(&chip8->memory,chip8->registers.I+i);
            }
        }
        break;
    }
}


static void chip8_exec_extended(struct chip8* chip8,unsigned short opcode){

    unsigned short nnn=opcode & 0x0fff;
    unsigned char x=(opcode >>8)&0x000f;
    unsigned char y=(opcode >>4)&0x000f;
    unsigned char kk=opcode & 0x00ff;
    unsigned char n=opcode & 0x000f;

    switch(opcode & 0xf000){
        case 0x1000:            //jump to location nnn
            chip8->registers.PC=nnn;
        break;

        case 0x2000:           //call subroutine at nnn
            chip8_stack_push(chip8,chip8->registers.PC);
            chip8->registers.PC=nnn;
        break;

        case 0x3000:         //SE Vx, byte Skip next instruction if Vx = kk.
            if(chip8->registers.V[x]==kk){
                chip8->registers.PC+=2;
            }
        break;
        
        case 0x4000:         //SE Vx, byte Skip next instruction if Vx != kk.
            if(chip8->registers.V[x]!=kk){
                chip8->registers.PC+=2;
            }
        break;
        
        case 0x5000:         //SE Vx,Vy Skip next instruction if Vx == Vy.
            if(chip8->registers.V[x]==chip8->registers.V[y]){
                chip8->registers.PC+=2;
            }
        break;
        
        case 0x6000:         //LD Vx,byte set Vx=kk.
            chip8->registers.V[x]=kk;
        break;
        
        case 0x7000:         //Add Vx,byte.Set Vx=Vx+kk.
            chip8->registers.V[x] +=kk;
        break;

        case 0x8000:
            chip8_exec_extended_8(chip8,opcode);
        break;

        case 0x9000:   //skip next instruction if Vx!=Vy
            if(chip8->registers.V[x]!=chip8->registers.V[y]){
                chip8->registers.PC+=2;
            }
        break;
        
        case 0xA000:   //sets I register to nnn
            chip8->registers.I=nnn;
        break;

        case 0xB000:    //jump to loaction nnn+V0
            chip8->registers.PC = nnn + chip8->registers.V[0x00];
        break;
        
        case 0xC000:    //generates a random number between 0-255 and Vx = randomnum & kk
            srand(clock());
            chip8->registers.V[x]=(rand()%255) & kk;
        break;

        case 0xD000:   // Draw sprite on screen
        {
            const char* sprite=(const char*) &chip8 ->memory.memory[chip8->registers.I];
            chip8->registers.V[0x0f]=chip8_screen_draw_sprite(&chip8->screen,chip8->registers.V[x],chip8->registers.V[y],sprite,n);
        }
        break;

        case 0xE000:   
        {
            switch(opcode & 0x00ff){
                case 0x9e:    //skip the next instruction if the key with value of vx is pressed
                    if(chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x])){
                        chip8->registers.PC +=2;
                    }
                break;

                case 0xa1:      //skip the next instruction if the key with value of vx is not pressed
                    if(!chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x])){
                        chip8->registers.PC +=2;
                    }
                break;
            }
        }
        break;
            
        case 0xF000:
            chip8_exec_extended_F(chip8,opcode);
        break;

    }

    
}
void chip8_exec(struct chip8* chip8,unsigned short opcode){
    switch(opcode){

        
        case 0x00E0:                                        //CLS: clear the display
            chip8_screen_clear(&chip8->screen);
        break;

        case 0x00EE:                                        //RET:Return from subroutine
            chip8->registers.PC=chip8_stack_pop(chip8);
        break;

        default:
            chip8_exec_extended(chip8,opcode);

    }
}