//define all ppu flags
#define PPU_CTRL *((unsigned char*)0x2000)
#define PPU_MASK *((unsigned char*)0x2001)
#define PPU_STATUS *((unsigned char*)0x2002)
#define SCROLL *((unsigned char*)0x2005)
#define PPU_ADDRESS *((unsigned char*)0x2006)
#define PPU_DATA *((unsigned char*)0x2007)

//literally no idea
unsigned char index;

//initialize the story
const unsigned char STORY_BRIEFING[] = {"Gay idiot reveals his 200 IQ secret live on camera with 1200k people watching"};

//define the palette
const unsigned char PALETTE[] = {0x1f, 0x00, 0x10, 0x20};
//these are black, gray, light gray, and white

//now for actual code
void main(void) {
	//turn your screen off so you don't see it's gay
	PPU_CTRL = 0;
	PPU_MASK = 0;
	
	//load your ugly palette so you can see some gray
	PPU_ADDRESS = 0x3f; //now you have an address in your PP unit
	PPU_ADDRESS = 0x00; //tbh my iq isn't high enough to know why these do anything
	//it's a two byte address sent as high than low, little endian iirc
	//for loop now, I guess?
	for(index = 0; index < sizeof(PALETTE); ++index) {
		PPU_DATA = PALETTE[index];
	}
	//now time for text
	//still don't understand this
	//now I do actually
	PPU_ADDRESS = 0x21; //little endian again
	PPU_ADDRESS = 0xca; //we finna put it at the middle of the screen
	//now it's for loop time again
	for(index = 0; index < sizeof(STORY_BRIEFING); ++index) {
		PPU_DATA = STORY_BRIEFING[index];
	}
	
	//that messes up scroll because of autoincrement
	//so you fix it by sending two more two-byte addresses
	
	PPU_ADDRESS = 0;
	PPU_ADDRESS = 0;
	SCROLL = 0;
	SCROLL = 0;
	
	//turn that screen on right now young man
	PPU_CTRL = 0x90;
	PPU_MASK = 0x1e;
	
	//infinite loop to hold it on screen
	while(1);
}
	