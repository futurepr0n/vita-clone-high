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

/*
 * Symbol of the image.png file
 */
extern unsigned char _binary_image_png_start;
extern unsigned char _binary_peter_png_start;
extern unsigned char _binary_lockers_png_start;
extern unsigned char _binary_abe_png_start;



// attempt audio file

//char path[MAX_PATH_LENGTH];
//extern const char binary_smb_fireball_wav = sprintf(path, MAX_PATH_LENGTH, "smb_fireball.wav");

void blitBackground(vita2d_texture *bg, float x, float y);
void blitBackgroundBW(vita2d_texture *bg, float x, float y);

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
	vita2d_texture *abe;
	vitaWav *fireball_sound = vitaWavLoad("ux0:/data/audio/smb_fireball.wav");

	float rad = 0.0f;
    float peter_x = 20.0f;
    float peter_y = 20.0f;

	float p1_pos_x = 64.0f;
	float p1_pos_y = 128.0f;

	float bg_x = 0.0f;
	float bg_y = 0.0f;


	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	pgf = vita2d_load_default_pgf();
	pvf = vita2d_load_default_pvf();

	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

	// Initialize Audio
	vitaAudioInit(0x40);
	vitaWavInit();
	/*
	 * Load the statically compiled image.png file.
	 */
	image = vita2d_load_PNG_buffer(&_binary_image_png_start);
	peter = vita2d_load_PNG_buffer(&_binary_peter_png_start);
	bg_i = vita2d_load_PNG_buffer(&_binary_lockers_png_start);
	bg_ii = vita2d_load_PNG_buffer(&_binary_lockers_png_start);
	bg_iii = vita2d_load_PNG_buffer(&_binary_lockers_png_start);
	abe = vita2d_load_PNG_buffer(&_binary_abe_png_start);

//	fireball_sound = vitaWavLoad(binary_smb_fireball_wav);

	memset(&pad, 0, sizeof(pad));

	while (1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);

		if (pad.buttons & SCE_CTRL_START)
			break;
		
		if(pad.buttons & SCE_CTRL_SQUARE)
			vitaWavPlay(&fireball_sound);
		
		if (pad.buttons & SCE_CTRL_RIGHT && p1_pos_x <= 800){
		//	peter_x += 10.0f;
			if(p1_pos_x <= 150){
				p1_pos_x += 10.0f;
			} else {
				bg_x -= 10.0f;
			}
		}

		if (pad.buttons & SCE_CTRL_LEFT && p1_pos_x >= 20){
		//	peter_x -= 10.0f;
			if(p1_pos_x >= 60){
				p1_pos_x -= 10.0f;
			}else{
				bg_x += 10.0f;
			}
		
		}

		if (pad.buttons & SCE_CTRL_UP && p1_pos_y >= 20){
			p1_pos_y -= 10.0f;
		}
		
		if (pad.buttons & SCE_CTRL_DOWN && p1_pos_y <= 400){
			p1_pos_y += 10.0f;

		}


		if (pad.lx >= 130 && p1_pos_x <= 800) {
		//	peter_x += 10.0f;
			if(p1_pos_x <= 150){
				p1_pos_x += 10.0f;
			} else {
				bg_x -= 10.0f;
			}
		}

		if (pad.lx <= 120 && p1_pos_x >= 20 ){
		//	peter_x -= 10.0f;
			if(p1_pos_x >= 60){
				p1_pos_x -= 10.0f;
			}else{
				bg_x += 10.0f;
			}
		}
		if (pad.ly >= 140 && p1_pos_y <= 400){
			p1_pos_y += 10.0f;

		}
		
		if (pad.ly <= 120 && p1_pos_y >= 20){
			p1_pos_y -= 10.0f;
	
		}


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


		// vita2d_draw_rectangle(p1_pos_x, p1_pos_y, 75, 150, RGBA8(255, 0, 0, 255));
//		vita2d_draw_rectangle(680, 350, 100, 150, RGBA8(0, 0, 255, 255));
//		vita2d_draw_fill_circle(200, 420, 100, RGBA8(0, 255,0 ,255));
		vita2d_draw_texture(bg_i, bg_x, bg_y);
		vita2d_draw_texture(bg_ii, bg_x + 512, bg_y);
		vita2d_draw_texture(bg_iii, bg_x + 1024, bg_y);

//		vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);
//		vita2d_draw_texture(peter, peter_x, peter_y);
		vita2d_draw_texture(abe, p1_pos_x, p1_pos_y);
		//vita2d_draw_texture(bg_i, bg_x, bg_y);

//		vita2d_draw_line(500, 30, 800, 300, RGBA8(255, 0, 255, 255));

		vita2d_pgf_draw_text(pgf, 700, 430, RGBA8(0,255,0,255), 1.0f, "Clone High");

		vita2d_pvf_draw_text(pvf, 700, 480, RGBA8(0,255,0,255), 1.0f, "by futurepr0n!");

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
	vita2d_free_texture(abe);
	vita2d_free_pgf(pgf);
	vita2d_free_pvf(pvf);

	sceKernelExitProcess(0);
	return 0;
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
