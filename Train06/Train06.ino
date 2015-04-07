/* About...
	Train V0.0.0.2 Software!
	==========================
	Copyright (C) 2014 Arad Eizen (A.E Tec) LTD INC.
	This Program Makes The 64x16 RG LEDs Matrix Run The Train
	Main Board: Arduino Pro Mini.
	Language: cxx.
*/

#define FULL_START
#define FULL_CODE

//{ Includes

#include <EEPROM.h>				// For Saving Settings

#define USE_DEFAULT_PINS		// Use if the matrix is connected to the arduino pins like Connections.png... call "begin()"
#define USE_DOUBLE_BUFFER		// Use if you use a lot of graphics... if comment so comment AUTO_UPDATE_BUFFER too!
#define AUTO_UPDATE_BUFFER		// Without it, you will need to call "updateBuffer()" manually if you use USE_DOUBLE_BUFFER.
//#define AUTO_RESCAN_DISPLAY		// Stops the the current scan, so there are no flickering on the screen...
#define DISPLAY_64X16			// One matrix panel

#include <RedGreenMatrix.h>		// Main matrix library
#include "fonts/MyHeb5x5.h"		// Include the fonts we use
#include "fonts/MyHeb5x7.h"		// Include the fonts we use
#include "fonts/MyHeb15x16.h"	// Include the fonts we use

#ifdef FULL_START
	#include "Button14x14.h"		// Include the bitmap we use
#endif
#ifdef FULL_CODE
	#include "LongTrain84x16.h"

	#include "LeftRedTrainA38x16.h"
	#include "LeftRedTrainB23x16.h"
	#include "LeftRedTrainC46x16.h"
	#include "LeftRedTrainD50x16.h"

	#include "RightRedTrainA38x16.h"
	#include "RightRedTrainB23x16.h"
	#include "RightRedTrainC46x16.h"
	#include "RightRedTrainD50x16.h"

	#include "LeftGreenTrainA38x16.h"
	#include "LeftGreenTrainB23x16.h"
	#include "LeftGreenTrainC46x16.h"
	#include "LeftGreenTrainD50x16.h"

	#include "RightGreenTrainA38x16.h"
	#include "RightGreenTrainB23x16.h"
	#include "RightGreenTrainC46x16.h"
	#include "RightGreenTrainD50x16.h"

	#include "LeftTrainA38x16.h"
	#include "LeftTrainC46x16.h"
	#include "RightTrainA38x16.h"
	#include "RightTrainC46x16.h"

	#include "RedRailroadA32x16.h"
	#include "RedRailroadB32x16.h"
	#include "RedRailroadC32x16.h"
	#include "RedRailroadD32x16.h"
	#include "RedRailroadE32x16.h"
	#include "RedRailroadF32x16.h"
	#include "RedRailroadG34x16.h"

	#include "GreenRailroadA32x16.h"
	#include "GreenRailroadB32x16.h"
	#include "GreenRailroadC32x16.h"
	#include "GreenRailroadD32x16.h"
	#include "GreenRailroadE32x16.h"
	#include "GreenRailroadF32x16.h"
	#include "GreenRailroadG34x16.h"

	#include "RailroadA32x16.h"
	#include "RailroadB32x16.h"
#endif
//}

//{ Consts And Defines

//const uint8_t TRAIN_TIMEOUT			= 3;	// Train Timeout in Minutes
const uint8_t TRAIN_ON_TIME			= 5;	// Train Full Lap Time in Minutes

const uint8_t TRAIN_START_PIN		= 10;	// Digital Input Train Start Pin
const uint8_t TRAIN_RUN_PIN			= 11;	// Digital Output Train ON Pin
const uint8_t TRAIN_TIMEOUT_PIN		= A7;	// Analog Input Train Timeout (VCC to GND)

const uint8_t EEPROM_START_TIME		= 0;	// Start Time Address

#ifdef FULL_CODE
const uint8_t *trainsA[] = {LeftRedTrainA38x16, RightRedTrainA38x16, LeftGreenTrainA38x16, RightGreenTrainA38x16};

const uint8_t *trainsB[] = {LeftRedTrainB23x16, RightRedTrainB23x16, LeftGreenTrainB23x16, RightGreenTrainB23x16};
const uint8_t *railroadsB[] = {RedRailroadA32x16, RedRailroadB32x16, RedRailroadF32x16, GreenRailroadA32x16, GreenRailroadB32x16, GreenRailroadF32x16};


const uint8_t *trainsCF[] = {LeftRedTrainC46x16, RightRedTrainC46x16, LeftGreenTrainC46x16, RightGreenTrainC46x16};
const uint8_t *railroadsCF[] = {RedRailroadC32x16, RedRailroadD32x16, RedRailroadE32x16, RedRailroadF32x16,
							GreenRailroadC32x16, GreenRailroadD32x16, GreenRailroadE32x16, GreenRailroadF32x16};

const uint8_t *trainsG[] = {LeftRedTrainD50x16, RightRedTrainD50x16, LeftGreenTrainD50x16, RightGreenTrainD50x16};
const uint8_t *railroadsG[] = {RedRailroadG34x16, GreenRailroadG34x16};
#endif

//{ Hebrew letters
#define ALEF		0x80
#define BET			0x81
#define GIMEL		0x82
#define DALED		0x83
#define HEY			0x84
#define VAV			0x85
#define ZAIN		0x86
#define HET			0x87
#define TET			0x88
#define YOD			0x89
#define HAF_SOFIT	0x8A
#define HAF			0x8B
#define LAMED		0x8C
#define MEM_SOFIT	0x8D
#define MEM			0x8E
#define NON_SOFIT	0x8F
#define NON			0x90
#define SAMECH		0x91
#define AIN			0x92
#define PEY_SOFIT 	0x93
#define PEY			0x94
#define ZADIK_SOFIT 0x95
#define ZADIK		0x96
#define KOF			0x97
#define REISH		0x98
#define SHEN		0x99
#define TAF			0x9A
//}

//}

//{ Veraibles

#ifdef FULL_START
char TRAIN_DONATED[] = {HEY, MEM, REISH, TAF, NON, ' ', TAF, BET, HAF, REISH, HEY, 0};
char BY_DIDI[] = {YOD, DALED, REISH, VAV, ' ', HEY, YOD, DALED, YOD, DALED, YOD, ' ', YOD, '"', AIN, 0};

char DISPLAY_BUILT[] = {HEY, TAF, NON, BET, NON, ' ', HEY, GIMEL, VAV, ZADIK, TAF, HEY, 0};
char BY_ARAD[] = {NON_SOFIT, ZAIN, YOD, YOD, ALEF, ' ', DALED, REISH, ALEF, ' ', YOD, '"', AIN, 0};

char TRAIN_BUILT[] = {':', HEY, REISH, ZAIN, AIN, VAV, ' ', HEY, KOF, VAV, ZAIN, HET, TAF, 0};
char BY_YOSI[] = {YOD, KOF, SAMECH, BET, SHEN, YOD, LAMED, TET, ' ', YOD, SAMECH, VAV, YOD, 0};

char PRESS[] = {':', VAV, ZADIK, HET, LAMED, 0};
#endif


#ifdef FULL_CODE
char TRAIN_LEAVE[] = {TAF, ALEF, ZADIK, VAV, YOD, ' ', TAF, BET, HAF, REISH, HEY, 0};
char IN_XX_MINUTES[] = {TAF, VAV, KOF, DALED, ' ', ' ', '0', ' ', DALED, VAV, AIN, BET, 0};
char GOOD_DRIVE[] = {'(' ,':' ,' ' ,HEY, BET, VAV, TET, ' ', HEY, AIN, YOD, SAMECH, NON, 0};

char WARNING[] = {'!', ' ', TAF, VAV, REISH, YOD, HEY, ZAIN, ' ', '!', 0};
char DOORS_OPEN[] = {TAF, VAV, HET, TAF, PEY, NON, ' ', TAF, VAV, TAF, LAMED, DALED, HEY, 0};
char DOORS_CLOSE[] = {TAF, VAV, REISH, GIMEL, SAMECH, NON, ' ', TAF, VAV, TAF, LAMED, DALED, HEY, 0};

char RESPECT_OLD[] = {'"', '.', '.', '.', MEM_SOFIT, VAV, KOF, TAF, ' ', HEY, BET, YOD, SHEN, ' ', YOD, NON, PEY, MEM, '"', 0};

char KEEP_CARD[] = {SAMECH, YOD, TET, REISH, HAF, ' ', REISH, VAV, MEM, SHEN, 0};
char UNTIL_END[] = {HEY, AIN, YOD, SAMECH, NON, HEY, ' ', PEY_SOFIT, VAV, SAMECH, ' ', DALED, AIN, 0};

char EXPRES_TRAIN[] = {SAMECH, REISH, PEY, SAMECH, KOF, ALEF, ' ', TAF, BET, HAF, REISH, 0};
char LIGHT_TRAIN[] = {HEY, LAMED, KOF, ' ', TAF, BET, HAF, REISH, 0};
char FAST_TRAIN[] = {HEY, REISH, YOD, HEY, MEM, ' ', TAF, BET, HAF, REISH, 0};

uint32_t oldMs, waitMs;
uint8_t curMode, oldMinutes, trainTimeout;
#endif


//}

//{ Main and Loop

void setup()
{
	delay(1);
	srand(analogRead(4));		// Init Randomizer
	begin();					// Init LED Matrix

	pinMode(TRAIN_RUN_PIN, OUTPUT);
	pinMode(TRAIN_START_PIN, INPUT_PULLUP);
	digitalWrite(TRAIN_RUN_PIN, true);

#ifdef FULL_CODE
	trainTimeout = map(analogRead(TRAIN_TIMEOUT_PIN), 0, 1023, TRAIN_ON_TIME, 15 + TRAIN_ON_TIME); // TRAIN_TIMEOUT;
	oldMinutes = EEPROM.read(EEPROM_START_TIME) + 1;	
	if (oldMinutes > trainTimeout) oldMinutes = 0;
	oldMs = millis();
	subOldMinutes();
#endif
#ifdef FULL_START

	ScrollTwoMsg(scrollMsg(MSG_A, TRAIN_DONATED, SCROLL_RIGHT_DOWN_IN, MIDDLE_ALIGNMENT, TOP_ALIGNMENT, MyHeb5x7, 1, RED_C,   BLACK_C),
				 scrollMsg(MSG_B, BY_DIDI, SCROLL_RIGHT_UP_IN, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT, MyHeb5x7, 1, GREEN_C, BLACK_C),
				 80);
	moveMsgs(6, SCROLL_RIGHT_UP_OUT, SCROLL_RIGHT_DOWN_OUT, HEIGHT_MAX - fontHeight, 70);
	
	
	ScrollTwoMsg(scrollMsg(MSG_A, DISPLAY_BUILT, SCROLL_LEFT_DOWN_IN, MIDDLE_ALIGNMENT, TOP_ALIGNMENT, MyHeb5x7, 1, GREEN_C,   BLACK_C),
				 scrollMsg(MSG_B, BY_ARAD, SCROLL_LEFT_UP_IN, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT + 1, MyHeb5x7, 1, RED_C, BLACK_C),
				 80);
	moveMsgs(6, SCROLL_LEFT_UP_OUT, SCROLL_LEFT_DOWN_OUT, HEIGHT_MAX - fontHeight, 70);
	
	ScrollTwoMsg(scrollMsg(MSG_A, TRAIN_BUILT, SCROLL_DOWN_IN, MIDDLE_ALIGNMENT, TOP_ALIGNMENT, MyHeb5x7, 1, GREEN_C,   BLACK_C),
				 scrollMsg(MSG_B, BY_YOSI, SCROLL_UP_IN, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT + 1, MyHeb5x7, 1, RED_C, BLACK_C),
				 80);
	moveMsgs(6, SCROLL_UP_OUT, SCROLL_DOWN_OUT, HEIGHT_MAX - fontHeight, 70);
	
	ScrollOneMsg(MSG_A, PRESS, SCROLL_LEFT_IN, RIGHT_ALIGNMENT + 6, 1, MyHeb15x16, 1, RED_C, BLACK_C, 60);
	
	drawBitmap(6, 1, Button14x14);
	for (int16_t i = 0; i < 23; i++)
	{
		swapColors();
		delay(180);
	}
	for (int16_t i = 0; i < WIDTH; i++)
	{
		moveDisplay(ROTATE_RIGHT, ORANGE_C);
		delay(30);
	}
	for (int16_t i = 0; i < 10; i++)
	{
		swapColors();
		delay(100);
	}
	for (int16_t i = 0; i < WIDTH - 4; i++)
	{
		if (i % 4 == 0) moveDisplay(SHIFT_DOWN, GREEN_C);
		moveDisplay(SHIFT_RIGHT, RED_C);
		delay(50);
	}
#endif
}

void loop()
{
#ifdef FULL_CODE
	uint32_t curMs = millis();
	if ((curMs - oldMs) > 60000)
	{
		oldMs = curMs;
		if (oldMinutes) subOldMinutes();
	}
	
	if (!digitalRead(TRAIN_START_PIN)) // Start Button Pressed
	{
		if (oldMinutes)
		{
			ScrollTwoMsg(scrollMsg(MSG_A, TRAIN_LEAVE, SCROLL_RIGHT_IN, RIGHT_ALIGNMENT + 1, TOP_ALIGNMENT, MyHeb5x7, 1, RED_C,   BLACK_C),
						 scrollMsg(MSG_B, IN_XX_MINUTES, SCROLL_RIGHT_IN, RIGHT_ALIGNMENT + 1, BOTTOM_ALIGNMENT + 1, MyHeb5x7, 1, ORANGE_C, BLACK_C),
						 50);
			moveMsgs(4, SCROLL_UP_OUT, SCROLL_DOWN_OUT, HEIGHT_MAX - fontHeight, 70);
			showAnimation(random(5));
			while (!digitalRead(TRAIN_START_PIN)) delay(1); // Start Button Relleased			
		}
		else
		{
			oldMs = curMs;
			digitalWrite(TRAIN_RUN_PIN, false);
			
			oldMinutes = trainTimeout; // 1 more for animations
			subOldMinutes();
			
			ScrollTwoMsg(scrollMsg(MSG_A, TRAIN_LEAVE, SCROLL_RIGHT_IN, RIGHT_ALIGNMENT + 1, TOP_ALIGNMENT, MyHeb5x7, 1, RED_C, BLACK_C),
						 scrollMsg(MSG_B, GOOD_DRIVE, SCROLL_LEFT_IN, RIGHT_ALIGNMENT + 1, BOTTOM_ALIGNMENT + 1, MyHeb5x7, 1, GREEN_C, BLACK_C),
						 50);
			moveMsgs(4, SCROLL_LEFT_UP_OUT, SCROLL_RIGHT_DOWN_OUT, HEIGHT_MAX - fontHeight, 70);
			if (isTimeUp()) return;
			ScrollTwoMsg(scrollMsg(MSG_A, WARNING, SCROLL_LEFT_DOWN_IN, MIDDLE_ALIGNMENT, TOP_ALIGNMENT + 1, MyHeb5x7, 1, RED_C, BLACK_C),
						 scrollMsg(MSG_B, DOORS_CLOSE, SCROLL_RIGHT_UP_IN, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT, MyHeb5x7, 1, ORANGE_C, BLACK_C),
						 70);
			moveMsgs(4, SCROLL_DOWN_OUT, SCROLL_DOWN_OUT, HEIGHT_MAX, 70);
			if (isTimeUp()) return;
			showAnimation(0);
			if (isTimeUp()) return;
			ScrollOneMsg(MSG_A, RESPECT_OLD, SCROLL_RIGHT_IN_OUT, MIDDLE_ALIGNMENT, MIDDLE_ALIGNMENT, MyHeb15x16, 1, ORANGE_C, BLACK_C, 60);
			if (isTimeUp()) return;
			ScrollTwoMsg(scrollMsg(MSG_A, KEEP_CARD, SCROLL_DOWN_IN, MIDDLE_ALIGNMENT, TOP_ALIGNMENT, MyHeb5x7, 1, GREEN_C, BLACK_C),
						 scrollMsg(MSG_B, UNTIL_END, SCROLL_UP_IN, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT + 1, MyHeb5x7, 1, ORANGE_C, BLACK_C),
						 70);
			moveMsgs(4, SCROLL_LEFT_OUT, SCROLL_RIGHT_OUT, WIDTH_MAX, 40);
			
			curMs = 0;
			while (!isTimeUp()) showAnimation((++curMs) % 5);
		}
	}
#endif
}

//}

//{ Animations
#ifdef FULL_CODE
void showAnimation(uint8_t id)
{
	uint8_t n, t, d = 100;
	switch(id)
	{
		case 0:
			for (int16_t i =  WIDTH; i >= -84; i--)
			{
				fillScreen2(BLACK_C);
				drawBitmap(i, 0, LongTrain84x16);
				delay(d);
			}
			break;
		case 1:
			n = random(1, 5);
			t = random(4);
			d = random(20, 100);
			
			uint8_t bA[n], dA[n];
			int16_t s;
			
			s = WIDTH;
			for (uint8_t j = 0; j < n; j++)
			{
				bA[j] = random(2) << 1;
				dA[j] = random(4, WIDTH) + 38;
				s += dA[j];
			}
			s -= dA[n - 1] - 38;
			
			for (int16_t i = 0, x; i <= s; i++)
			{
				if (t & 1) x = i - 38;
				else x = WIDTH - i;
				
				fillScreen2(BLACK_C);
				if (t & 2) drawRail2((-x / 2 % 5), ORANGE_C);
				else drawRail1((-x / 2 % 9), ORANGE_C);
				
				for (uint8_t j = 0; j < n; j++)
				{
					drawBackBitmap(x, 0, trainsA[(t & 1) | bA[j]], (t & 1 ? RightTrainA38x16 : LeftTrainA38x16));
					if (t & 1) x -= dA[j];
					else x += dA[j];
				}

				updateBuffer();
				delay(d);
			}
			fillDisplay(BLACK_C);
/*
			t = random(4);
			d  = random(20, 100);

			for (int16_t i = 0, x; i <= WIDTH + 38; i++)
			{
				if (t & 1) x = i - 38;
				else x = WIDTH - i;
				
				fillScreen2(BLACK_C);
				drawRail1((-x / 2 % 9), ORANGE_C);
				drawBackBitmap(x, 0, trainsA[t], (t & 1 ? RightTrainA38x16 : LeftTrainA38x16));

				
				updateBuffer();
				delay(d);
			}
		*/			
			break;
		case 2:
			n = random(1, 4);
			t = random(5);
			d = random(20, 100);
			
			uint8_t bG[n];

			for (uint8_t j = 0; j < n; j++)
				bG[j] = random(2);
				
			for (int16_t i = 0, x; i <= WIDTH + (50 * 2 + n * (3 + 34)) + 3; i++)
			{
				if (t & 1) x = i - 50;
				else x = WIDTH - i;
				
				fillScreen2(BLACK_C);
				//drawRail2((i / 2 % 5), ORANGE_C);	

				drawTopBitmap(x, 0, trainsG[t & 3]);
				
				if (t & 1) x -= 3;
				else x += 50;
				
				for (uint8_t j = 0, k; j < n; j++)
				{
					k = bG[j];
					drawJoint1(x, k + 1);
					if (t & 1) x -= 34;
					else x += 3;
					drawTopBitmap(x, 0, railroadsG[k]);
					if (t & 1) x -= 3;
					else x += 34;
				}
				
				drawJoint1(x, (t & 2 ? GREEN_C : RED_C));
				if (t & 1) x -= 50;
				else x += 3;
				drawTopBitmap(x, 0, trainsG[(((t & 4) >> 1) | ~(t & 1)) & 3]);
				
				updateBuffer();
				delay(d);
			}
			break;
		case 3:
			n = random(2, 6);
			t = random(4);
			d = random(20, 100);
			uint8_t bCF[n];

			for (uint8_t j = 0; j < n; j++)
				bCF[j] = random(8);

			for (int16_t i = 0, x; i <= WIDTH + (46 + n * (3 + 32)); i++)
			{
				if (t & 1) x = i - 46;
				else x = WIDTH - i;
				
				fillScreen2(BLACK_C);
				drawBackBitmap(x, 0, trainsCF[t], (t & 1 ? RightTrainC46x16 : LeftTrainC46x16));
				
				if (t & 1) x -= 3;
				else x += 46;
				
				for (uint8_t j = 0, k; j < n; j++)
				{
					k = bCF[j];
					drawJoint1(x, (k > 3 ? RED_C : GREEN_C));
					if (t & 1) x -= 32;
					else x += 3;
					drawTopBitmap(x, 0, railroadsCF[k]);
					if (t & 1) x -= 3;
					else x += 32;
				}

				updateBuffer();
				delay(d);
			}
			break;
		case 4:
			n = random(2, 6);
			t = random(4);
			d = random(20, 100);
			uint8_t bB[n];

			for (uint8_t j = 0; j < n; j++)
				bB[j] = random(6);

			for (int16_t i = 0, x; i <= WIDTH + (23 + n * (3 + 32)); i++)
			{
				if (t & 1) x = i - 23;
				else x = WIDTH - i;
				
				fillScreen2(BLACK_C);
				drawTopBitmap(x, 0, trainsB[t]);
				
				if (t & 1) x -= 3;
				else x += 23;
				
				for (uint8_t j = 0, k; j < n; j++)
				{
					k = bB[j];
					drawJoint1(x, (k > 2 ? RED_C : GREEN_C));
					if (t & 1) x -= 32;
					else x += 3;
					drawTopBitmap(x, 0, railroadsB[k]);
					if (t & 1) x -= 3;
					else x += 32;
				}

				updateBuffer();
				delay(d);
			}
			break;
	}
}
#endif
//}

//{ Drawings

void drawJoint1(int16_t x, uint8_t c)
{
	drawFastVLine2(x + 0, 9, 3, c);
	drawPixel2(x + 1, 10, c);
	drawFastVLine2(x + 2, 9, 3, c);
}

void drawRail1(int16_t x, uint8_t c)
{
	drawFastHLine2(0, 9, WIDTH, c);
	drawFastHLine2(0, 15, WIDTH, c);
	for (uint8_t i = 0, j = 0; i < 9; i++, j += 9)
		drawLine2(x + j - 8, 14, x + j - 4, 10, c);
}

void drawRail2(int16_t x, uint8_t c)
{
	drawFastHLine2(0, 9, WIDTH, c);
	drawFastHLine2(0, 15, WIDTH, c);
	for (uint8_t i = 0, j = 0; i < 15; i++, j += 5)
	{
		drawFastVLine2(x + j - 4, 10, 5, c);
		drawFastVLine2(x + j - 3, 10, 5, c);
	}
}

//}

//{ Oters

#ifdef FULL_CODE
bool isTimeUp()
{
	bool retVal = ((millis() - oldMs) > (TRAIN_ON_TIME * 60000));
	if (retVal) digitalWrite(TRAIN_RUN_PIN, true);
	return retVal;
}

void subOldMinutes()
{
	if (oldMinutes == 0) return;
	uint8_t minutes = --oldMinutes;
	
	EEPROM.write(EEPROM_START_TIME, minutes);
	IN_XX_MINUTES[6] = '0' + minutes % 10;
	minutes /= 10;
	IN_XX_MINUTES[5] = (minutes ? '0' + minutes : ' ');
}
#endif

void moveMsgs(uint8_t sec, uint8_t dirA, uint8_t dirB, uint8_t steps, uint8_t speed)
{
	delay(sec * 1000);
	
	dirMsg(MSG_A, dirA);
	dirMsg(MSG_B, dirB);

	while (steps)
	{
		moveMsg(MSG_A);
		moveMsg(MSG_B);
		delay(speed);
		steps--;
	}
}

void drawBackBitmap(int16_t x,  int16_t y, const uint8_t *b1, const uint8_t *b2)
{
	drawTopBitmap(x, y, b1);
	drawTopBitmap(x, y, b2);
}

void drawTopBitmap(int16_t x,  int16_t y, const uint8_t *b)
{
	drawBitmap2(x, y, b, BLACK_C, ORANGE_C);
}

//}


