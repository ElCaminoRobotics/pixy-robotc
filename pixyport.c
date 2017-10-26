#define PIXY_START_WORD        0xaa55
#define PIXY_START_WORD_CC     0xaa56
#define PIXY_BACKWARD_WORD     0x55aa


typedef enum {
	NORMAL_BLOCK,
	CC_BLOCK
} BlockType;

static BlockType g_blockType; // use this to remember the next object block type

long getWord(ubyte input, ubyte input2) {
	long result;
	result = input2 << 8;
	writeDebugStreamLine("input2 code is: %x", input2);
	result = result | input;
	writeDebugStreamLine("input code is: %x", input);
	writeDebugStreamLine("input2 code is: %x", input2);
	return result;
}

int getStart(const TUARTs uart) {
	long w, lastw;
	ubyte word1, word2;
	lastw = 0x0fff;
	setBaudRate(uart, baudRate19200);

	while(true) {
		word1 = getChar(uart);

		while(word1 == 0xff) {
			word1 = getChar(uart);
		}
		word2 = getChar(uart);
		while(word2 == 0xff) {
				word2 = getChar(uart);
		}


		w = getWord(word1,word2);
		writeDebugStreamLine("word code is: %x", w);

		//writeDebugStreamLine("the size of short is : %d", sizeof(w))
		if(w == PIXY_START_WORD && lastw == w) {
			g_blockType = NORMAL_BLOCK;
			return 1;
		} else if (w == PIXY_START_WORD_CC && lastw == PIXY_START_WORD) {
			g_blockType = CC_BLOCK;
			return 1;
		} else if (w == PIXY_BACKWARD_WORD) {
			int backward = true;
		}
		lastw = w;
	}
}

task main()
{
	int i=0, curr, prev = 0;
	while(true) {
	    curr = getStart(uartTwo);
	    if(prev && curr) {    // two start codes means start of new frame
	       i++;
	       writeDebugStreamLine("Frame: %d", i);
	  }
	  prev = curr;
	}

}
