/***********************************************************************************
  Filename:     user_interface.c

  Description:  Serial Termnial User Interface 

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

#include "utilMisc.h"
#include <hal_board.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "user_interface.h"

char drum_piano_guitar=0;
char real_virtual = 0;
char drum_piano_guitar_rv=0;
char piano_type=0;
char guitar_type=0;
char audio_termination=0;
char record_notes = FALSE;
char play_recorded_notes=FALSE;
char track=FALSE;
char playback=FALSE;

char rv_loop = FALSE;
char wrv_loop = FALSE;
char piano_loop = FALSE;
char guitar_loop = FALSE;
char audio_loop = FALSE;
char record_loop = FALSE;
char track_loop = FALSE;
char playback_loop = FALSE;

//void print_welcome_message(void)
//{
//	printf_pc_code((__code char*)"\n\r***********************************************************************\n\r");
//	printf_pc_code((__code char*)"************  EMBEDDED SYSTEMS DESIGN : SPRING 2011 :      ************\n\r");
//	printf_pc_code((__code char*)"************  Project Embedded Garageband                  ************\n\r");
//	printf_pc_code((__code char*)"************  Maulik Kapuria, Niket Shah & Vishal Verma    ************\n\r");
//	printf_pc_code((__code char*)"***********************************************************************\n\r\n\n\r");
//}

void print_rv_screen(void)
{
	printf_pc_code((__code char*)"\n\rSelect from the options below:\n\n\n\r");
	printf_pc_code((__code char*)"1. Play Real Instrument\n\r");
	printf_pc_code((__code char*)"2. Play Virtual Instrument\n\n\r");
}

void print_sel_real_screen(void)
{
	
	printf_pc_code((__code char*)"\n\rYou have Selected Real Instruments.\n\n\r");
	printf_pc_code((__code char*)"1. Play Drums\n\r");
	printf_pc_code((__code char*)"2. Play Piano\n\n\r");
}

void print_sel_virtual_screen(void)
{
	
	printf_pc_code((__code char*)"\n\rYou have Selected Virtual Instruments.\n\n\r");
	printf_pc_code((__code char*)"1. Play Drums\n\r");
	printf_pc_code((__code char*)"2. Play Piano\n\r");
	printf_pc_code((__code char*)"3. Play Guitar\n\n\r");
}


void print_sel_piano_types(void)
{
	

	printf_pc_code((__code char*)"\n\rThe following Piano Types are supported\n\n\r");

	printf_pc_code((__code char*)"1. Acoustic Grand Piano\n\r");
	printf_pc_code((__code char*)"2. Bright Acoustic Piano\n\r");
	printf_pc_code((__code char*)"3. Electric Grand Piano\n\r");
	printf_pc_code((__code char*)"4. Honky Tonky Piano\n\r");
	printf_pc_code((__code char*)"5. Electric Piano 1\n\r");
	printf_pc_code((__code char*)"6. Electric Piano 2\n\n\r");
}


void print_sel_guitar_types(void)
{
	printf_pc_code((__code char*)"The following Guitar Types are supported\n\n\r");

	printf_pc_code((__code char*)"1. Acoustic Guitar (Nylon)\n\r");
	printf_pc_code((__code char*)"2. Acoustic Guitar (Steel)\n\r");
	printf_pc_code((__code char*)"3. Electric Guitar (Jazz)\n\r");
	printf_pc_code((__code char*)"4. Electric Guitar (Clean)\n\r");
	printf_pc_code((__code char*)"5. Electric Guitar (Muted)\n\r");
	printf_pc_code((__code char*)"6. Overdriven Guitar\n\r");
	printf_pc_code((__code char*)"7. Distortion Guitar\n\r");
	printf_pc_code((__code char*)"8. Guitar Harmonics\n\n\r");
}

void print_audio_termination_screen(void)
{
	
	printf_pc_code((__code char*)"\n\rWhere do you Want to play your MIDI notes?\n\n\r");
	printf_pc_code((__code char*)"1. Play on computer (MIDI synthesizer software)\n\r");
	printf_pc_code((__code char*)"2. Play on attached speaker (VS1053 MIDI Decoder)\n\n\r");
}

void print_record_screen(void)
{
	
	printf_pc_code((__code char*)"\n\rWould you like to record your music?\n\n\r");
	printf_pc_code((__code char*)"1. Play & Record on SD Card\n\r");
	printf_pc_code((__code char*)"2. Play without recording on SD card\n\r");
	printf_pc_code((__code char*)"3. PlayBack my previously stored Music\n\n\r");
}

void print_track_screen(void)
{
	printf_pc_code((__code char*)"\n\rSelect Track to record your music on:\n\n\r");
	printf_pc_code((__code char*)"1. Track-0\n\r");
	printf_pc_code((__code char*)"2. Track-1\n\r");
	printf_pc_code((__code char*)"3. Track-2\n\r");
	printf_pc_code((__code char*)"4. Track-3\n\n\r");
}

void print_playback_screen(void)
{
	printf_pc_code((__code char*)"\n\rSelect Track to playback:\n\n\r");
	printf_pc_code((__code char*)"1. Track-0\n\r");
	printf_pc_code((__code char*)"2. Track-1\n\r");
	printf_pc_code((__code char*)"3. Track-2\n\r");
	printf_pc_code((__code char*)"4. Track-3\n\n\r");
}


char expect_value(char max_number)
{
	char bufsize_ascii[5];
	char bufsize={0};
	char mybuf[2]={0};

	bzero(mybuf,2);

	printf_pc_code((__code char*)"Please Enter one of the options, VALID [1-");
	sprintf(mybuf,"%d",max_number); //TODO

	printf_pc(mybuf);

	printf_pc_code((__code char*)"]:");

	bzero(bufsize_ascii,5);																		//zero out buffer

	getstr(bufsize_ascii);																		//wait for string from user
	bufsize = atoi(bufsize_ascii);																//convert to int

	clear_screen();

	if(bufsize > 0 && bufsize <=max_number)
		return bufsize;
	else
		return 0;
}


void get_track_option(void)
{
	char mybuf[4];
	char user_data=0;

	while(track_loop == TRUE)
	{
		print_track_screen();
		user_data = expect_value(4);

		if( user_data != 0)
		{	//Goes here if data is valid;
			bzero(mybuf,4);
			//sprintf(mybuf,"%d",user_data); //TODO

							switch(user_data)
						{
						case 1:
						{
							track=0;
							break;
						}

						case 2:
						{
							track=1;
							break;
						}

						case 3:
						{
							track=2;
							break;
						}

						case 4:
						{
							track=3;
							break;
						}
						default:
							break;
						}

			break;	//break out of while loop
		}
		else
		{
			printf_pc_code((__code char*)"INVALID DATA\n\r");
			halMcuWaitMs(1000);
			clear_screen();
		}
	}
}


void get_playback_option(void)
{
	char mybuf[4];
	char user_data=0;

	while(playback_loop == TRUE)
	{
		print_playback_screen();
		user_data = expect_value(4);

		if( user_data != 0)
		{	//Goes here if data is valid;
			bzero(mybuf,4);
			//sprintf(mybuf,"%d",user_data); //TODO

							switch(user_data)
						{
						case 1:
						{
							playback=0;
							break;
						}

						case 2:
						{
							playback=1;
							break;
						}

						case 3:
						{
							playback=2;
							break;
						}

						case 4:
						{
							playback=3;
							break;
						}
						default:
							break;
						}

			break;	//break out of while loop
		}
		else
		{
			printf_pc_code((__code char*)"INVALID DATA\n\r");
			halMcuWaitMs(1000);
			clear_screen();
		}
	}
}



void get_audio_termination(void)
{
	char mybuf[4];
	char user_data=0;

	while(audio_loop == TRUE)
	{
		print_audio_termination_screen();
		user_data = expect_value(2);

		if( user_data != 0)
		{	//Goes here if data is valid;
			bzero(mybuf,4);
			//sprintf(mybuf,"%d",user_data); //TODO
			if(user_data == 1)
			{
				printf_pc_code((__code char*)"\n\n\rPC will be used as Audio Termination.\n\r");
				audio_termination = PC;
			}
			else
			{
				printf_pc_code((__code char*)"VS1053 will be used as Audio Termination.\n\r");
				audio_termination = VS1053;
			}
			break;	//break out of while loop
		}
		else
		{
			printf_pc_code((__code char*)"INVALID DATA\n\r");
			halMcuWaitMs(1000);
			clear_screen();
		}
	}
}


void get_record_option(void)
{
	char mybuf[4];
	char user_data=0;

	while(record_loop == TRUE)
	{
		print_record_screen();
		user_data = expect_value(3);

		if( user_data != 0)
		{																	//Goes here if data is valid;
			bzero(mybuf,4);
			if(user_data == 1)
			{
				record_notes = TRUE;
			}
			else if(user_data == 2)
			{
				record_notes = FALSE;
			}
			else
			{
				play_recorded_notes = TRUE;

				rv_loop = FALSE;
				wrv_loop= FALSE;
				guitar_loop=FALSE;
				piano_loop = FALSE;
				audio_loop=FALSE;
				record_loop=FALSE;
				track_loop=FALSE;
				playback_loop=TRUE;

			}

			break;	//break out of while loop
		}
		else
		{
			printf_pc_code((__code char*)"INVALID DATA\n\r");
			halMcuWaitMs(1000);
			clear_screen();
		}
	}
}


void get_real_virtual(void)
{
	char mybuf[4];
	char user_data=0;

	while(rv_loop == TRUE)
	{
		print_rv_screen();
		user_data = expect_value(2);

		if( user_data != 0)
		{																//Goes here if data is valid;
			bzero(mybuf,4);

			switch(user_data)
			{
				case 1:
				{
					//printf_pc_code((__code char*)"\n\rReal Instruments Selected***\n\r");
					real_virtual = REAL_INSTRUMENT;
					break;
				}

				case 2:
				{
					//printf_pc_code((__code char*)"\n\r***Virtual Instruments Selected***\n\r");
					real_virtual = VIRTUAL_INSTRUMENT;
					break;
				}

				default:
					break;
			}

			break;	//break out of while loop
		}
		else
		{
			printf_pc_code((__code char*)"INVALID DATA\n\r");
			halMcuWaitMs(1000);
			clear_screen();
		}
	}
}


void select_within_real_virtual(void)
{

	char mybuf[4];
	char user_data=0;


	if(real_virtual == REAL_INSTRUMENT)
	{
		while(wrv_loop == TRUE)
		{
			print_sel_real_screen();
			user_data = expect_value(2);

				if( user_data != 0)
				{
					bzero(mybuf,4);
					//sprintf(mybuf,"%d",user_data);  //TODO
					////printf_pc_code((__code char*)"User pressed");
					////printf_pc_code((__code char*)"%s",mybuf);

					switch(user_data)
					{
						case 1:
						{
							printf_pc_code((__code char*)"\n\r***Drum is the selected Instrument***\n\r");
							drum_piano_guitar_rv = DRUM_REAL_INSTRUMENT;
							drum_piano_guitar = DRUM;
							break;
						}

						case 2:
						{
							printf_pc_code((__code char*)"***\n\rPiano is the selected Instrument***\n\r");
							drum_piano_guitar_rv = PIANO_REAL_INSTRUMENT;
							drum_piano_guitar = PIANO;
							break;
						}

						default:
							break;
					}

					break;
				}
				else
				{
					printf_pc_code((__code char*)"INVALID DATA\n\r");
					halMcuWaitMs(1000);
					clear_screen();
				}
		}
	}
	else if(real_virtual == VIRTUAL_INSTRUMENT) 	//Virtual Instrument
	{
		while(wrv_loop == TRUE)
		{
			print_sel_virtual_screen();
			user_data = expect_value(3);

				if( user_data != 0)
				{
					bzero(mybuf,4);
					//sprintf(mybuf,"%d",user_data);  //TODO
					////printf_pc_code((__code char*)"User pressed");
					////printf_pc_code((__code char*)"%s",mybuf);

					switch(user_data)
					{
						case 1:
						{
							printf_pc_code((__code char*)"\n\r***Drum is the selected Virtual Instrument***\n\r");
							drum_piano_guitar_rv = DRUM_VIRTUAL_INSTRUMENT;
							drum_piano_guitar = DRUM;
							break;
						}

						case 2:
						{
							printf_pc_code((__code char*)"\n\r***Piano is the selected Virtual Instrument***\n\r");
							drum_piano_guitar_rv = PIANO_VIRTUAL_INSTRUMENT;
							drum_piano_guitar = PIANO;
							break;
						}

						case 3:
						{
							printf_pc_code((__code char*)"\n\r***Guitar is the selected Virtual Instrument***\n\r");
							drum_piano_guitar_rv = GUITAR_VIRTUAL_INSTRUMENT;
							drum_piano_guitar = GUITAR;
							break;
						}

						default:
							break;
					}


					break;
				}
				else
				{
					printf_pc_code((__code char*)"INVALID DATA\n\r");
					halMcuWaitMs(1000);
					clear_screen();
				}

		}
	}

}


void select_sub_instruments(void)
{
	if(drum_piano_guitar == DRUM)
	{
		printf_pc_code((__code char*)"No sub-options for Drums available\n\r");
	}
	else
	{
		////printf_pc_code((__code char*)"Suboptions for PIANO/GUITAR available\n\r");
		switch(drum_piano_guitar)
		{
			case PIANO:
			{
				select_piano_type();
				break;
			}

			case GUITAR:
			{
				select_guitar_type();
				break;
			}

			default:
				break;
		}
	}
}


void select_piano_type(void)
{

	char mybuf[4];
	char user_data=0;

	while(piano_loop == TRUE)
			{
				print_sel_piano_types();
				user_data = expect_value(6);

					if( user_data != 0)
					{
						bzero(mybuf,4);
						//sprintf(mybuf,"%d",user_data);
						////printf_pc_code((__code char*)"User pressed");
						////printf_pc_code((__code char*)"%s",mybuf);

						switch(user_data)
						{
							case 1:
							{
								//printf_pc_code((__code char*)"\n\r***Acoustic Grand Piano is the selected Instrument***\n\n\r");
								piano_type = ACOUSTIC_GRAND_PIANO;
								break;
							}
							case 2:
							{
								//printf_pc_code((__code char*)"\n\r***Bright Acoustic Piano is the selected Instrument***\n\r");
								piano_type = BRIGHT_ACOUSTIC_PIANO;
								break;
							}
							case 3:
							{
								//printf_pc_code((__code char*)"\n\r***Electric Grand Piano is the selected Instrument***\n\n\r");
								piano_type = ELECTRIC_GRAND_PIANO;
								break;
							}
							case 4:
							{
								//printf_pc_code((__code char*)"\n\r***Honky Tonky Piano is the selected Instrument***\n\n\r");
								piano_type = HONKY_TONKY_PIANO;
								break;
							}
							case 5:
							{
								//printf_pc_code((__code char*)"\n\r***Electric Piano 1 is the selected Instrument***\n\n\r");
								piano_type = ELECTRIC_PIANO_1;
								break;
							}
							case 6:
							{
								//printf_pc_code((__code char*)"\n\r***Electric Piano 2 is the selected Instrument***\n\n\r");
								piano_type = ELECTRIC_PIANO_2;
								break;
							}

							default:
								break;
						}

						break;
					}
					else
					{
						printf_pc_code((__code char*)"INVALID DATA\n\r");
						halMcuWaitMs(1000);
						clear_screen();
					}
			}
}

void select_guitar_type(void)
{

	char mybuf[4];
	char user_data=0;

	while(guitar_loop == TRUE)
			{
				print_sel_guitar_types();
				user_data = expect_value(8);

					if( user_data != 0)
					{
						bzero(mybuf,4);
						//sprintf(mybuf,"%d",user_data);
						////printf_pc_code((__code char*)"User pressed");
						////printf_pc_code((__code char*)"%s",mybuf);

						switch(user_data)
						{
							case 1:
							{
								//printf_pc_code((__code char*)"\n\r***Acoustic Guitar (Nylon) is the selected Instrument***\n\n\r");
								piano_type = ACOUSTIC_GUITAR_NYLON;
								break;
							}

							case 2:
							{
								//printf_pc_code((__code char*)"\n\r***Acoustic Guitar (Steel) is the selected Instrument***\n\n\r");
								piano_type = ACOUSTIC_GUITAR_STEEL;
								break;
							}


							case 3:
							{
								//printf_pc_code((__code char*)"\n\r***Electric Guitar (Jazz) is the selected Instrument***\n\n\r");
								piano_type = ELECTRIC_GUITAR_JAZZ;
								break;
							}



							case 4:
							{
								//printf_pc_code((__code char*)"\n\r***Electric Guitar (Clean) is the selected Instrument***\n\n\r");
								piano_type = ELECTRIC_GUITAR_CLEAN;
								break;
							}


							case 5:
							{
								//printf_pc_code((__code char*)"\n\r***Electric Guitar (Muted) is the selected Instrument***\n\n\r");
								piano_type = ELECTRIC_GUITAR_MUTED;
								break;
							}


							case 6:
							{
								//printf_pc_code((__code char*)"\n\r***Overdriven Guitar is the selected Instrument***\n\n\r");
								piano_type = OVERDRIVEN_GUITAR;
								break;
							}


							case 7:
							{
								//printf_pc_code((__code char*)"\n\r***Distortion Guitar is the selected Instrument***\n\n\r");
								piano_type = DISTORTION_GUITAR;
								break;
							}


							case 8:
							{
								//printf_pc_code((__code char*)"\n\r***Guitar Harmonics is the selected Instrument***\n\n\r");
								piano_type = GUITAR_HARMONICS;
								break;
							}

							default:
								break;
						}

						break;
					}
					else
					{
						printf_pc_code((__code char*)"INVALID DATA\n\r");
						halMcuWaitMs(1000);
						clear_screen();
					}
			}
}


void start_my_menu(void)
{

	drum_piano_guitar=0;
	real_virtual=0;
	piano_type=0;
	guitar_type=0;
	drum_piano_guitar_rv=0;
	audio_termination=VS1053;
	record_notes=FALSE;
	play_recorded_notes=FALSE;
	track=FALSE;
	playback=FALSE;

	rv_loop = TRUE;
	wrv_loop= TRUE;
	guitar_loop=TRUE;
	piano_loop = TRUE;
	audio_loop=TRUE;
	record_loop=TRUE;
	track_loop=TRUE;
	playback_loop=TRUE;
	
	clear_screen();
	//print_welcome_message();

	get_audio_termination();
	get_record_option();

	if(record_notes == TRUE)
		get_track_option();
	
	if(play_recorded_notes == TRUE)
		get_playback_option();

	get_real_virtual();

	select_within_real_virtual();


	select_sub_instruments();

	//clear_screen();
	printf_pc_code((__code char*)"\n\r**************************************");
	printf_pc_code((__code char*)"\n\rAll the initialization is now complete");
	printf_pc_code((__code char*)"\n\r**************************************");
}



void clear_screen(void)
{
	printf_pc_code((__code char*)"\033c");			//Reset Terminal
	halMcuWaitMs(200);

	printf_pc_code((__code char*)"\033[2J");			//Clear Screen
	halMcuWaitMs(200);
}

