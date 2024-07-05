#include "chip8screen.h"
#include <assert.h>
#include <string.h>

static void chip8_screen_check_bound(int x, int y) {
    assert(x >= 0 && x < CHIP8_WIDTH && y >= 0 && y < CHIP8_HEIGHT);
}

void chip8_screen_init(struct chip8_screen* screen) {
    chip8_screen_clear(screen);
}

void chip8_screen_clear(struct chip8_screen* screen) {
    memset(screen->pixels, 0, sizeof(screen->pixels));
}

void chip8_screen_set(struct chip8_screen* screen, int x, int y) {
    chip8_screen_check_bound(x, y);
    screen->pixels[y][x] = true;
}

bool chip8_screen_is_set(struct chip8_screen* screen, int x, int y) {
    chip8_screen_check_bound(x, y);
    return screen->pixels[y][x];
}

bool chip8_screen_draw_sprite(struct chip8_screen* screen,int x,int y,const char* sprite,int num){
    bool pixel_collison=false;

    for(int ly=0;ly<num;ly++){
        char c=sprite[ly];
        for(int lx=0;lx<8;lx++){
            if((c & (0b10000000>>lx)) == 0){
                continue;
            }

            if(screen->pixels[(ly+y)%CHIP8_HEIGHT][(lx+x)%CHIP8_WIDTH]){
                pixel_collison=true;
            }

            
            //using % CHIP*_HEIGHT && WIDTH cause it make it wrap arond the screen if it ever flows out of the screen(ex-65%64=1)
            screen->pixels[(ly+y)%CHIP8_HEIGHT][(lx+x)%CHIP8_WIDTH] ^= true;
        }
    }
    return pixel_collison;
}

