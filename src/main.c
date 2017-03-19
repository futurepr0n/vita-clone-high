/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <vita2d.h>
#include "audio/vita_audio.h"

// Defines
#define YES = 1
#define NO = 0
#define ON 1
#define OFF 0
#define MAX_NUM_ENEMIES 100
#define MAX_NUM_BULLETS 100
#define LEFT 0
#define RIGHT 1

// These are some variables for our use
int done = 0;
int bullets = 0;
int points = 0;
int hScore = 1000;


/*
 * These are for loading the Images
 */
extern unsigned char _binary_resources_image_png_start;
extern unsigned char _binary_resources_peter_png_start;
extern unsigned char _binary_resources_lockers_png_start;
extern unsigned char _binary_resources_abe_png_start;
extern unsigned char _binary_resources_fireball_png_start;

// Here we are going to define our structures for the objects
typedef struct{
	float x;
	float y;
	//int imgX;
	//int imgY;
	vita2d_texture *img;
	int isalive;
	int pctr;
}obj;

obj a_fireball;
obj a_player;
obj a_background1;
obj a_background2;

//Functions
void blitBackground(vita2d_texture *bg, float x, float y);
void blitBackgroundBW(vita2d_texture *bg, float x, float y);
void blitFireball(vita2d_texture *img, float x, float y);
void blitObj(obj object);
void control(SceCtrlData p1, vitaWav *sound);
void printScore(vita2d_pgf *pgf, vita2d_pvf *pvf);
void loadPlayer();
void loadCharacterData();

float a_player.x = 20.0f;
float a_player.y = 20.0f;

float bg_x = 0.0f;
float bg_y = 0.0f;

int main()
{
	SceCtrlData pad;
	vita2d_pgf *pgf;
	vita2d_pvf *pvf;
	vita2d_texture *image;
	vita2d_texture *peter;
	vita2d_texture *bg_i;
	vita2d_texture *bg_ii;
	vita2d_texture *bg_iii;
	//vita2d_texture *abe;
	vita2d_texture *fireball;
	vitaWav *fireball_sound;

	float rad = 0.0f;

	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	pgf = vita2d_load_default_pgf();
	pvf = vita2d_load_default_pvf();

	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	// Initialize Audio
	vitaWavInit();
	/*
	 * Load the statically compiled image.png file.
	 */
	image = vita2d_load_PNG_buffer(&_binary_resources_image_png_start);
	peter = vita2d_load_PNG_buffer(&_binary_resources_peter_png_start);
	bg_i = vita2d_load_PNG_buffer(&_binary_resources_lockers_png_start);
	bg_ii = vita2d_load_PNG_buffer(&_binary_resources_lockers_png_start);
	bg_iii = vita2d_load_PNG_buffer(&_binary_resources_lockers_png_start);
	//abe = vita2d_load_PNG_buffer(&_binary_resources_abe_png_start);
	fireball = vita2d_load_PNG_buffer(&_binary_resources_fireball_png_start);
	fireball_sound = vitaWavLoad("app0:resources/smb_fireball.wav");

	memset(&pad, 0, sizeof(pad));

  loadCharacterData();
	while (1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		vita2d_start_drawing();
		vita2d_clear_screen();

		if(bg_x < 0){
    	      		blitBackground(bg_ii, bg_x, bg_y);
 	   		if(bg_x<-512){
     	   			bg_x=0;
     	  }
    }

		if(bg_x > 0){
			blitBackgroundBW(bg_ii, bg_x, bg_y);
			if(bg_x>+512){
				bg_x=0;
			}
		}

		vita2d_draw_texture(bg_i, bg_x, bg_y);
		vita2d_draw_texture(bg_ii, bg_x + 512, bg_y);
		vita2d_draw_texture(bg_iii, bg_x + 1024, bg_y);

		control(pad, fireball_sound);

		//vita2d_draw_texture(abe, a_player.x, a_player.y);
		blitObj(a_player);
		printScore(pgf, pvf);

		vita2d_end_drawing();
		vita2d_swap_buffers();

		rad += 0.1f;
	}

	/*
	 * vita2d_fini() waits until the GPU has finished rendering,
	 * then we can free the assets freely.
	 */
	vita2d_fini();
	vita2d_free_texture(image);
	vita2d_free_texture(peter);
	vita2d_free_texture(bg_i);
	vita2d_free_texture(bg_ii);
	//vita2d_free_texture(a_player);
	vita2d_free_pgf(pgf);
	vita2d_free_pvf(pvf);
	vitaWavUnload(fireball_sound);

	sceKernelExitProcess(0);
	return 0;
}

void blitObj(obj object){
	vita2d_draw_texture(object.img, object.x, object.y);
}

void blitBackground(vita2d_texture *bg, float x, float y){
	float newX = x+512;
	vita2d_draw_texture(bg, newX, y);
	vita2d_draw_texture(bg, newX + 512, y);

}

void blitBackgroundBW(vita2d_texture *bg, float x, float y){
	float newX = x-512;
	vita2d_draw_texture(bg, newX, y);
	vita2d_draw_texture(bg, newX - 512, y);

}

void blitFireball(vita2d_texture *img, float x, float y){
	for(int i = x; i < 810; i++){

		vita2d_draw_texture(img,i,y);
	}
}

void loadCharacterData(){
	loadPlayer();
	//loadEnemies();
	//loadChain();
}

void control(SceCtrlData p1, vitaWav *sound){

	if (p1.buttons & SCE_CTRL_START){
		//break;
	}

	if(p1.buttons & SCE_CTRL_SQUARE){
		// fireball_x = a_player.x + 10;
		// fireball_y = a_player.y;
		vitaWavPlay(sound);
		// blitFireball(fireball, fireball_x, fireball_y);
	}

	if (p1.buttons & SCE_CTRL_RIGHT && a_player.x <= 800){
	//	peter_x += 10.0f;
		if(a_player.x <= 150){
			a_player.x += 10.0f;
		} else {
			bg_x -= 10.0f;
		}
	}

	if (p1.buttons & SCE_CTRL_LEFT && a_player.x >= 20){
	//	peter_x -= 10.0f;
		if(a_player.x >= 60){
			a_player.x -= 10.0f;
		}else{
			bg_x += 10.0f;
		}

	}

	if (p1.buttons & SCE_CTRL_UP && a_player.y >= 20){
		a_player.y -= 10.0f;
	}

	if (p1.buttons & SCE_CTRL_DOWN && a_player.y <= 400){
		a_player.y += 10.0f;

	}


	if (p1.lx >= 130 && a_player.x <= 800) {
	//	peter_x += 10.0f;
		if(a_player.x <= 150){
			a_player.x += 10.0f;
		} else {
			bg_x -= 10.0f;
		}
	}

	if (p1.lx <= 120 && a_player.x >= 20 ){
	//	peter_x -= 10.0f;
		if(a_player.x >= 60){
			a_player.x -= 10.0f;
		}else{
			bg_x += 10.0f;
		}
	}
	if (p1.ly >= 140 && a_player.y <= 400){
		a_player.y += 10.0f;

	}

	if (p1.ly <= 120 && a_player.y >= 20){
		a_player.y -= 10.0f;

	}

}

void loadPlayer(){
	a_player.x = 64.0f;
	a_player.y = 128.0f;
	//player.imgX = 46;
	//player.imgY = 24;
	a_player.img = vita2d_load_PNG_buffer(&_binary_resources_abe_png_start);
	if(!a_player.img){
		printf("Player image failed to load...");
	}
	a_player.isalive = 1;
	//blitObj(player);
	//return player;
}

void printScore(vita2d_pgf *pgf, vita2d_pvf *pvf){
	vita2d_pgf_draw_text(pgf, 700, 430, RGBA8(0,255,0,255), 1.0f, "Clone High");
	vita2d_pgf_draw_text(pgf, 500, 430, RGBA8(0,255,0,255), 1.0f, "vitaWav fireball");
	vita2d_pvf_draw_text(pvf, 700, 480, RGBA8(0,255,0,255), 1.0f, "by futurepr0n!");
}
