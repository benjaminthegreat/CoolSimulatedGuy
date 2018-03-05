//define all ppu flags
#define PPU_CTRL *((unsigned char*)0x2000)
#define PPU_MASK *((unsigned char*)0x2001)
#define PPU_STATUS *((unsigned char*)0x2002)
#define SCROLL *((unsigned char*)0x2005)
#define PPU_ADDRESS *((unsigned char*)0x2006)
#define PPU_DATA *((unsigned char*)0x2007)

//literally no idea
unsigned char index;
//new stuff to handle nmi and text stuff
unsigned char NMI_flag;
unsigned char Frame_Count;
unsigned char textPosition;
unsigned char currentStory;


//initialize the story
const unsigned char STORY_BRIEFING_A[] = {"The year is 2018. Everybody has changed their Edmodo picture to Ben Smith. Except one man..."};
const unsigned char STORY_BRIEFING_B[] = {"Paul Mulvaney sticks stubbornly with his disgusting image of a man who looks somewhat, but not exactly like himself"};
const unsigned char STORY_BRIEFING_C[] = {"Unbeknownst to Mulvaney, a resistance force gathers in the shadows of his computer lab"};
const unsigned char textLimits[] = {sizeof(STORY_BRIEFING_A) - 1, sizeof(STORY_BRIEFING_B) - 1, sizeof(STORY_BRIEFING_C) - 1};
//define the palette
const unsigned char PALETTE[] = {0x1f, 0x00, 0x10, 0x20};
//these are black, gray, light gray, and white

//consolidate some code into methods, use prototypes here
void allOff(void);
void allOn(void);
void resetScroll(void);
void loadPalette(void);
void clearText(void);
void loadNextChar(void);

//now for actual code
void main(void) {
	allOff(); //screen off
	loadPalette(); //load the palette
	resetScroll(); //reset the scroll
	currentStory = 0;
	textPosition = 0;
	//our text starts here
	PPU_ADDRESS = 0x21; //now you have an address in your PP unit
	PPU_ADDRESS = 0x00; //tbh my iq isn't high enough to know why these do anything
	allOn();
	//now an infinite game loop because good code
	while(1) {
		//the best way to wait until the right time is just a while loop
		while(NMI_flag == 0);
		//reset it back to 0
		NMI_flag = 0;
		//now run this code until it once again increments, and also does the first text
		//while(NMI_flag == 0) {
			//if the text isn't done, fix it
			//if(textPosition < textLimits[currentStory]) {
				loadNextChar();
				resetScroll();
			//}
			//if the screen is over, load a new one and clear the old one
			if(Frame_Count == 200 && currentStory != 2) {
				++currentStory;
				clearText();
				PPU_ADDRESS = 0x21; //now you have an address in your PP unit
				PPU_ADDRESS = 0xc0; //tbh my iq isn't high enough to know why these do anything
				textPosition = 0;
				allOn();
			//}
		}
	}
}

void allOff(void) { //turns screen + NMI off
	PPU_CTRL = 0;
	PPU_MASK = 0;
}

void allOn(void) {  //turns screen + NMI on
	PPU_CTRL = 0x90;
	PPU_MASK = 0x1e;
}

//resets scroll to beginning
void resetScroll(void) {
	//these are all two byte values loaded little-endian style
	PPU_ADDRESS = 0;
	PPU_ADDRESS = 0;
	SCROLL = 0;
	SCROLL = 0;
}

//clears text
void clearText(void) {
	//turn the screen off for safety
	allOff();
	//first go to beginning of text
	PPU_ADDRESS = 0x21;
	PPU_ADDRESS = 0x00;
	//now go through the text and rek it
	for(index = 0; index < textLimits[currentStory - 1]; ++index) {
		PPU_DATA = 0;
	}
	//we should be good so put us live again
}

//now it's time to do some code
void loadNextChar(void) {
	PPU_ADDRESS = 0x21;
	PPU_ADDRESS = 0x00 + textPosition;
	if(textPosition > textLimits[currentStory]) PPU_DATA = 0;
	else {
		if(currentStory == 0) PPU_DATA = STORY_BRIEFING_A[textPosition];
		if(currentStory == 1) PPU_DATA = STORY_BRIEFING_B[textPosition];
		if(currentStory == 2) PPU_DATA = STORY_BRIEFING_C[textPosition];
	}
	++textPosition;
}	

//one last code
void loadPalette(void) {
	PPU_ADDRESS = 0x3f;
	PPU_ADDRESS = 0x00;
	for( index = 0; index < sizeof(PALETTE); ++index ){
		PPU_DATA = PALETTE[index];
	}
}