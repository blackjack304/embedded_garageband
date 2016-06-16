/***********************************************************************************
  Filename:     user_interface.h

  Description:  Serial Termnial User Interface 

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_

#include "hal_defs.h"
#include "hal_types.h"

#define REAL_INSTRUMENT		73
#define VIRTUAL_INSTRUMENT	74

#define PIANO	75
#define DRUM	76
#define GUITAR 	77

#define DRUM_REAL_INSTRUMENT	78
#define PIANO_REAL_INSTRUMENT	79
#define DRUM_VIRTUAL_INSTRUMENT		80
#define PIANO_VIRTUAL_INSTRUMENT	81
#define GUITAR_VIRTUAL_INSTRUMENT	82

#define PC 83
#define VS1053 84

#define ACOUSTIC_GRAND_PIANO	1
#define BRIGHT_ACOUSTIC_PIANO	2
#define ELECTRIC_GRAND_PIANO	3
#define HONKY_TONKY_PIANO	4
#define ELECTRIC_PIANO_1	5
#define ELECTRIC_PIANO_2	6

#define ACOUSTIC_GUITAR_NYLON	25
#define ACOUSTIC_GUITAR_STEEL	26
#define ELECTRIC_GUITAR_JAZZ	27
#define ELECTRIC_GUITAR_CLEAN	28
#define ELECTRIC_GUITAR_MUTED	29
#define OVERDRIVEN_GUITAR	30
#define DISTORTION_GUITAR	31
#define GUITAR_HARMONICS	32


void clear_screen();

void start_my_menu(void);

//void print_welcome_message(void);
char expect_value(char max_number);

void print_rv_screen(void);
void print_sel_real_screen(void);
void print_sel_virtual_screen(void);
void print_sel_piano_types(void);
void print_sel_guitar_types(void);
void print_audio_termination_screen(void);
void print_record_screen(void);
void print_track_screen(void);
void print_playback_screen(void);

void get_real_virtual(void);
void select_within_real_virtual(void);
void select_sub_instruments(void);
void select_piano_type(void);
void select_guitar_type(void);
void get_record_option(void);
void get_audio_termination(void);
void get_track_option(void);
void get_playback_option(void);

#endif /* USER_INTERFACE_H_ */
