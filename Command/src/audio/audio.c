/*
 * audio.c
 *
 * Created: 06.11.2017 13:49:49
 *  Author: MLN
 */ 

#include <string.h>

#include "audio.h"


#ifndef BOARD_OSC0_HZ
	#define BOARD_OSC0_HZ 64000000
#endif

/************************************************************************/
/* DEFINITIONS                                                          */
/************************************************************************/

#define AUDIOOUTPUTMASK 0x000003FF
#define AUDIOMAXINPUT 0x3FF
#define WAVDATA_SIZE 512

#define NO_LOAD 0x00
#define LOAD_WAVDATA1 0x01
#define LOAD_WAVDATA2 0x02

#define NO_METADATA 0x00
#define ID3_METADATA 0x01
#define LISTINFO_METADATA 0x02



/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

uint8_t loadNextSector = LOAD_WAVDATA1;
uint8_t currentVolume = 0;

uint8_t wavData1[512] = {
	0x00,0x80,0x00,0x81,0x00,0x83,0x00,0x84,0x00,0x86,0x00,0x87,0x00,0x89,0x00,0x8a,
	0x00,0x8c,0x00,0x8e,0x00,0x8f,0x00,0x91,0x00,0x92,0x00,0x94,0x00,0x95,0x00,0x97,
	0x00,0x98,0x00,0x9a,0x00,0x9b,0x00,0x9d,0x00,0x9e,0x00,0xa0,0x00,0xa2,0x00,0xa3,
	0x00,0xa5,0x00,0xa6,0x00,0xa7,0x00,0xa9,0x00,0xaa,0x00,0xac,0x00,0xad,0x00,0xaf,
	0x00,0xb0,0x00,0xb2,0x00,0xb3,0x00,0xb5,0x00,0xb6,0x00,0xb7,0x00,0xb9,0x00,0xba,
	0x00,0xbc,0x00,0xbd,0x00,0xbe,0x00,0xc0,0x00,0xc1,0x00,0xc2,0x00,0xc4,0x00,0xc5,
	0x00,0xc6,0x00,0xc8,0x00,0xc9,0x00,0xca,0x00,0xcb,0x00,0xcd,0x00,0xce,0x00,0xcf,
	0x00,0xd0,0x00,0xd2,0x00,0xd3,0x00,0xd4,0x00,0xd5,0x00,0xd6,0x00,0xd7,0x00,0xd9,
	0x00,0xda,0x00,0xdb,0x00,0xdc,0x00,0xdd,0x00,0xde,0x00,0xdf,0x00,0xe0,0x00,0xe1,
	0x00,0xe2,0x00,0xe3,0x00,0xe4,0x00,0xe5,0x00,0xe6,0x00,0xe7,0x00,0xe8,0x00,0xe9,
	0x00,0xea,0x00,0xea,0x00,0xeb,0x00,0xec,0x00,0xed,0x00,0xee,0x00,0xee,0x00,0xef,
	0x00,0xf0,0x00,0xf1,0x00,0xf1,0x00,0xf2,0x00,0xf3,0x00,0xf3,0x00,0xf4,0x00,0xf5,
	0x00,0xf5,0x00,0xf6,0x00,0xf6,0x00,0xf7,0x00,0xf8,0x00,0xf8,0x00,0xf9,0x00,0xf9,
	0x00,0xfa,0x00,0xfa,0x00,0xfa,0x00,0xfb,0x00,0xfb,0x00,0xfc,0x00,0xfc,0x00,0xfc,
	0x00,0xfd,0x00,0xfd,0x00,0xfd,0x00,0xfd,0x00,0xfe,0x00,0xfe,0x00,0xfe,0x00,0xfe,
	0x00,0xfe,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
	0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
	0x00,0xfe,0x00,0xfe,0x00,0xfe,0x00,0xfe,0x00,0xfe,0x00,0xfd,0x00,0xfd,0x00,0xfd,
	0x00,0xfd,0x00,0xfc,0x00,0xfc,0x00,0xfc,0x00,0xfb,0x00,0xfb,0x00,0xfa,0x00,0xfa,
	0x00,0xfa,0x00,0xf9,0x00,0xf9,0x00,0xf8,0x00,0xf8,0x00,0xf7,0x00,0xf6,0x00,0xf6,
	0x00,0xf5,0x00,0xf5,0x00,0xf4,0x00,0xf3,0x00,0xf3,0x00,0xf2,0x00,0xf1,0x00,0xf1,
	0x00,0xf0,0x00,0xef,0x00,0xee,0x00,0xee,0x00,0xed,0x00,0xec,0x00,0xeb,0x00,0xea,
	0x00,0xea,0x00,0xe9,0x00,0xe8,0x00,0xe7,0x00,0xe6,0x00,0xe5,0x00,0xe4,0x00,0xe3,
	0x00,0xe2,0x00,0xe1,0x00,0xe0,0x00,0xdf,0x00,0xde,0x00,0xdd,0x00,0xdc,0x00,0xdb,
	0x00,0xda,0x00,0xd9,0x00,0xd7,0x00,0xd6,0x00,0xd5,0x00,0xd4,0x00,0xd3,0x00,0xd2,
	0x00,0xd0,0x00,0xcf,0x00,0xce,0x00,0xcd,0x00,0xcb,0x00,0xca,0x00,0xc9,0x00,0xc8,
	0x00,0xc6,0x00,0xc5,0x00,0xc4,0x00,0xc2,0x00,0xc1,0x00,0xc0,0x00,0xbe,0x00,0xbd,
	0x00,0xbc,0x00,0xba,0x00,0xb9,0x00,0xb7,0x00,0xb6,0x00,0xb5,0x00,0xb3,0x00,0xb2,
	0x00,0xb0,0x00,0xaf,0x00,0xad,0x00,0xac,0x00,0xaa,0x00,0xa9,0x00,0xa7,0x00,0xa6,
	0x00,0xa5,0x00,0xa3,0x00,0xa2,0x00,0xa0,0x00,0x9e,0x00,0x9d,0x00,0x9b,0x00,0x9a,
	0x00,0x98,0x00,0x97,0x00,0x95,0x00,0x94,0x00,0x92,0x00,0x91,0x00,0x8f,0x00,0x8e,
	0x00,0x8c,0x00,0x8a,0x00,0x89,0x00,0x87,0x00,0x86,0x00,0x84,0x00,0x83,0x00,0x81
	};
	
uint8_t wavData2[512] = {
	0x00,0x80,0x00,0x7e,0x00,0x7c,0x00,0x7b,0x00,0x79,0x00,0x78,0x00,0x76,0x00,0x75,
	0x00,0x73,0x00,0x71,0x00,0x70,0x00,0x6e,0x00,0x6d,0x00,0x6b,0x00,0x6a,0x00,0x68,
	0x00,0x67,0x00,0x65,0x00,0x64,0x00,0x62,0x00,0x61,0x00,0x5f,0x00,0x5d,0x00,0x5c,
	0x00,0x5a,0x00,0x59,0x00,0x58,0x00,0x56,0x00,0x55,0x00,0x53,0x00,0x52,0x00,0x50,
	0x00,0x4f,0x00,0x4d,0x00,0x4c,0x00,0x4a,0x00,0x49,0x00,0x48,0x00,0x46,0x00,0x45,
	0x00,0x43,0x00,0x42,0x00,0x41,0x00,0x3f,0x00,0x3e,0x00,0x3d,0x00,0x3b,0x00,0x3a,
	0x00,0x39,0x00,0x37,0x00,0x36,0x00,0x35,0x00,0x34,0x00,0x32,0x00,0x31,0x00,0x30,
	0x00,0x2f,0x00,0x2d,0x00,0x2c,0x00,0x2b,0x00,0x2a,0x00,0x29,0x00,0x28,0x00,0x26,
	0x00,0x25,0x00,0x24,0x00,0x23,0x00,0x22,0x00,0x21,0x00,0x20,0x00,0x1f,0x00,0x1e,
	0x00,0x1d,0x00,0x1c,0x00,0x1b,0x00,0x1a,0x00,0x19,0x00,0x18,0x00,0x17,0x00,0x16,
	0x00,0x15,0x00,0x15,0x00,0x14,0x00,0x13,0x00,0x12,0x00,0x11,0x00,0x11,0x00,0x10,
	0x00,0x0f,0x00,0x0e,0x00,0x0e,0x00,0x0d,0x00,0x0c,0x00,0x0c,0x00,0x0b,0x00,0x0a,
	0x00,0x0a,0x00,0x09,0x00,0x09,0x00,0x08,0x00,0x07,0x00,0x07,0x00,0x06,0x00,0x06,
	0x00,0x05,0x00,0x05,0x00,0x05,0x00,0x04,0x00,0x04,0x00,0x03,0x00,0x03,0x00,0x03,
	0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x02,
	0x00,0x02,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x04,0x00,0x04,0x00,0x05,0x00,0x05,
	0x00,0x05,0x00,0x06,0x00,0x06,0x00,0x07,0x00,0x07,0x00,0x08,0x00,0x09,0x00,0x09,
	0x00,0x0a,0x00,0x0a,0x00,0x0b,0x00,0x0c,0x00,0x0c,0x00,0x0d,0x00,0x0e,0x00,0x0e,
	0x00,0x0f,0x00,0x10,0x00,0x11,0x00,0x11,0x00,0x12,0x00,0x13,0x00,0x14,0x00,0x15,
	0x00,0x15,0x00,0x16,0x00,0x17,0x00,0x18,0x00,0x19,0x00,0x1a,0x00,0x1b,0x00,0x1c,
	0x00,0x1d,0x00,0x1e,0x00,0x1f,0x00,0x20,0x00,0x21,0x00,0x22,0x00,0x23,0x00,0x24,
	0x00,0x25,0x00,0x26,0x00,0x28,0x00,0x29,0x00,0x2a,0x00,0x2b,0x00,0x2c,0x00,0x2d,
	0x00,0x2f,0x00,0x30,0x00,0x31,0x00,0x32,0x00,0x34,0x00,0x35,0x00,0x36,0x00,0x37,
	0x00,0x39,0x00,0x3a,0x00,0x3b,0x00,0x3d,0x00,0x3e,0x00,0x3f,0x00,0x41,0x00,0x42,
	0x00,0x43,0x00,0x45,0x00,0x46,0x00,0x48,0x00,0x49,0x00,0x4a,0x00,0x4c,0x00,0x4d,
	0x00,0x4f,0x00,0x50,0x00,0x52,0x00,0x53,0x00,0x55,0x00,0x56,0x00,0x58,0x00,0x59,
	0x00,0x5a,0x00,0x5c,0x00,0x5d,0x00,0x5f,0x00,0x61,0x00,0x62,0x00,0x64,0x00,0x65,
	0x00,0x67,0x00,0x68,0x00,0x6a,0x00,0x6b,0x00,0x6d,0x00,0x6e,0x00,0x70,0x00,0x71,
	0x00,0x73,0x00,0x75,0x00,0x76,0x00,0x78,0x00,0x79,0x00,0x7b,0x00,0x7c,0x00,0x7e
	};

uint8_t *wavDataPointer = wavData1;
uint32_t wavDataIndex = 0;

AudioInfo fileData;

bool isFilePlaying;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/

void _setOutput (uint16_t, uint16_t);
uint8_t _fileVerification();




uint8_t audio_setFileToPlay(uint8_t fileNumber){
	uint16_t timerRC = 0;
	
	if (sdcard_setFileToRead(fileNumber) == false){
		return ERROR_NO_FILE;
	}
	
	sdcard_getNextSectorFast(wavData1);
	sdcard_getNextSectorFast(wavData2);
	
	uint8_t fileVerif = _fileVerification(wavData1);
	if (fileVerif != FILE_VERIFICATION_SUCCESS){
		return fileVerif;
	}
	isFilePlaying = true;
	
	wavDataIndex = fileData.firstDataByteIndex;
	timerRC = (BOARD_OSC0_HZ / 2) / (4 * fileData.sampleRate);
	
	// Volume setting, optimized to be as light as possible
	audio_setVolume(DEFAULTVOLUME);
	tc_write_rc(&AVR32_TC, TC1_CHANNEL, timerRC);
	tc_start(&AVR32_TC, TC1_CHANNEL);
}



/* audio_setVolume
 *
 * This function sends 2 bytes to N4: AD5300BRMZ to set its analog output.
 * The analog output sets in turn the volume of the N7: TPA6012A4 audio
 * amplifier.
 * 
 * Created 06.11.17 MLN
 * Last modified 13.11.17 MLN
 */
void audio_setVolume (uint8_t volume){
	
	if (volume != currentVolume){
		currentVolume = volume;
		// Selecting DAC1
		spi_selectChip((volatile struct avr32_spi_t*)DAC1_SPI, DAC1_SPI_NPCS);
	
		/* Sending volume level to DAC1
		 * Volume has to be in the middle nibbles of a 2-byte integer
		 * as per AD5300BRMZ datasheet
		 */
		spi_write((volatile struct avr32_spi_t*)DAC1_SPI, (currentVolume<<4) & 0x0FF0);
	
		// Deselecting DAC1
		spi_unselectChip((volatile struct avr32_spi_t*)DAC1_SPI, DAC1_SPI_NPCS);
	
		if (currentVolume == 0) {
			// If volume superior to 0, do not shutdown amplifier
			gpio_clr_gpio_pin(PIN_SHUTDOWN);
		} else {
			gpio_set_gpio_pin(PIN_SHUTDOWN);
		}
	}
}



/* audio_playFile
 *
 * Plays a file from the SD card. Must be put in while(1) loop.
 * Loads file first sector, puts file info in a global variable
 * and loads next sector when finished sector flag is raised from
 * timer 1 interruption.
 *
 * Created 17.11.17 MLN
 * Last modified 21.12.17 MLN
 */
uint8_t audio_playFile(){
	
	if(!isFilePlaying){
		return 0;
	}
	
	// If SD card pulled out, audio playback is stopped and data is cleared
	if(!sdcard_checkPresence()){
		audio_stop();
		// Emptying wavData
		memset(wavData1, 0, sizeof(wavData1)/sizeof(wavData1[0]) );
		memset(wavData2, 0, sizeof(wavData2)/sizeof(wavData2[0]) );
		return 0;
	}
	
	
	// Audio loading
	switch(loadNextSector){
			
		case LOAD_WAVDATA1:
			while(!sdcard_getNextSectorFast(wavData1));
			loadNextSector = NO_LOAD;
			fileData.audioSampleTables--;
			break;
			
		case LOAD_WAVDATA2:
			while(!sdcard_getNextSectorFast(wavData2));
			loadNextSector = NO_LOAD;
			fileData.audioSampleTables--;
			break;
			
		case NO_LOAD:
			break;
	}
	
	
	/************************************************************************/
	/* END OF FILE ROUTINE													*/
	/* Stops audio playback by stopping the timer and returns a finish code	*/
	/************************************************************************/
	
	if (fileData.audioSampleTables <= 1){
		tc_stop(&AVR32_TC, TC1_CHANNEL);
		fileData.audioSampleTables = fileData.audioSampleBytes / WAVDATA_SIZE;
		//audio_firstCall = true;
		isFilePlaying = false;
		wavDataIndex = 0;
		
		return AUDIO_PLAY_FINISHED;
	}
	else {
		return AUDIO_PLAYING;
	}
}



/* audio_pauseFile
 *
 * Pauses current .wav file playback
 *
 * Created 20.11.17 MLN
 * Last modified 20.11.17 MLN
 */
void audio_pauseFile(void){
	tc_stop(&AVR32_TC, TC1_CHANNEL);
	loadNextSector = NO_LOAD;
}



void audio_togglePausePlay(void){
	if(isFilePlaying){
		tc_stop(&AVR32_TC, TC1_CHANNEL);
	}else{
		tc_start(&AVR32_TC, TC1_CHANNEL);
	}
	isFilePlaying = ! isFilePlaying;
}



void audio_stop(void){
	tc_stop(&AVR32_TC, TC1_CHANNEL);
	memset(&fileData,0,sizeof(AudioInfo));
	wavDataIndex = 0;
	wavDataPointer = NULL;
}



/* audio_freqStart
 *
 * Starts tc0 to generate a square wave at argument frequency.
 *
 * Created 08.11.17 MLN
 * Last modified 10.11.17 MLN
 */
void audio_freqStart (uint16_t freq){
	// Writes value to RC such that interrupt occurs at 2*freq Hz
	tc_write_rc(&AVR32_TC, TC0_CHANNEL, (BOARD_OSC0_HZ/8)/(2*freq) );
	// Starts timer0
	tc_start(&AVR32_TC, TC0_CHANNEL);
}



/* audio_freqStop
 *
 * Stops previously started tc0.
 *
 * Created 08.11.17 MLN
 * Last modified 10.11.17 MLN
 */
void audio_freqStop (void){
	tc_stop(&AVR32_TC, TC0_CHANNEL);
}



/* _setOutput
 *
 * Sets N5: AD5333BRUZ to output input argument on both channels.
 * Input must be 10-bit, right-aligned
 * Timing specifications can be found in AD5333BRUZ datasheet, page 3.
 * 	
 * Channel A corresponds to the right speaker
 * Channel B corresponds to the left speaker 
 *
 * Created 06.11.17 MLN
 * Last modified 23.11.17 MLN
 */
void _setOutput (uint16_t inputA, uint16_t inputB){
	
	// Selecting chip
	AVR32_GPIO.port[1].ovrc = 1 << (DAC_CS_PIN & 0x1F);
	// Writing parallel input (10-bit)
	AVR32_GPIO.port[1].ovrs = (0x000003FF & inputB);
	AVR32_GPIO.port[1].ovrc = (0x000003FF & ~(inputB));
	// Writing input registers B
	AVR32_GPIO.port[1].ovrc = 1 << (DAC_WR_PIN & 0x1F);
	AVR32_GPIO.port[1].ovrs = 1 << (DAC_WR_PIN & 0x1F);
	// Writing parallel input (10-bit)
	AVR32_GPIO.port[1].ovrs = (0x000003FF & inputA);
	AVR32_GPIO.port[1].ovrc = (0x000003FF & ~(inputA));
	// Selecting channel A
	AVR32_GPIO.port[1].ovrs = 1 << (DAC_A0_PIN & 0x1F);
	// Writing input register A
	AVR32_GPIO.port[1].ovrc = 1 << (DAC_WR_PIN & 0x1F);
	AVR32_GPIO.port[1].ovrs = 1 << (DAC_WR_PIN & 0x1F);
	// Selecting channel B
	AVR32_GPIO.port[1].ovrc = 1 << (DAC_A0_PIN & 0x1F);
	// Loading data from input to output registers
	AVR32_GPIO.port[1].ovrc = 1 << (DAC_LDAC_PIN & 0x1F);
	AVR32_GPIO.port[1].ovrs = 1 << (DAC_LDAC_PIN & 0x1F);
	// De-selecting chip
	AVR32_GPIO.port[1].ovrs = 1 << (DAC_CS_PIN & 0x1F);
	
}



/* _fileVerification
 *
 * http://soundfile.sapp.org/doc/WaveFormat/
 * http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html more detailed
 * Checks validity of audio parameters and loads relevant ones in fileData.
 * Returns program-wide error codes as defined in audio.h
 *
 * Created 17.11.17 MLN
 * Last modified 20.12.17 MLN
 */
uint8_t _fileVerification(){
	uint16_t headerIndex = 8;
	uint16_t i = 0;
	uint32_t subchunkSize = 0;
	uint8_t metadataFormat = NO_METADATA;
	// Verifying file format ID in RIFF header
	if (wavData1[headerIndex] != 'W' || wavData1[headerIndex + 1] != 'A' || wavData1[headerIndex + 2] != 'V' || wavData1[headerIndex + 3] != 'E'){
		return ERROR_FORMAT;
	}
	
	
	// Beginning fetch AudioInfo routine
	
	// Finding "fmt " subchunk in header, then jumping to next relevant block
	while (wavData1[headerIndex] != 'f' && wavData1[headerIndex + 1] != 'm' && wavData1[headerIndex + 2] != 't' && wavData1[headerIndex + 3] != ' ') {
		headerIndex++;
		if (headerIndex > SECTOR_SIZE){
			return ERROR_NO_FMT_SUBCHUNK;
		}
	}
	// Jumps to Subchunk1Size block
	headerIndex += 4;
	// Jumps to AudioFormat block
	headerIndex += 4;
	
	if (wavData1[headerIndex + 0] != 1) {
		return ERROR_FILE_COMPRESSED;
	}
	
	// Jumps to NumChannels block
	headerIndex += 2;
	fileData.channelNumber = wavData1[headerIndex + 0];
	
	// Jumps to SampleRate block
	headerIndex += 2;
	// Puts 4 SampleRate bytes on a single 32-bit integer before assigning it to fileData.sampleRate
	fileData.sampleRate = 
		(wavData1[headerIndex + 3] << 24) |
		(wavData1[headerIndex + 2] << 16) |
		(wavData1[headerIndex + 1] << 8) |
		(wavData1[headerIndex + 0])
	;
	
	// Jumps to ByteRate block
	headerIndex += 4;
	
	// Jumps to BlockAlign block
	headerIndex += 4;
	// Assigns fileData.blockAlign value
	fileData.blockAlign = 
		(wavData1[headerIndex + 1] << 8) |
		(wavData1[headerIndex + 0])
	;
	
	// Jumps to BitsPerSample block
	headerIndex += 2;
	fileData.bitsPerSample =
		(wavData1[headerIndex + 1] << 8) |
		(wavData1[headerIndex + 0])
	;
	
	
	// Finding LISTINFO metadata (if it exists)
	headerIndex = 0;
	metadataFormat = LISTINFO_METADATA;
	while (wavData1[headerIndex] != 'I' && wavData1[headerIndex + 1] != 'N' && wavData1[headerIndex + 2] != 'F' && wavData1[headerIndex + 3] != 'O') {
		headerIndex++;
		// If INFO tag not found, then no LISTINFO metadata
		if (headerIndex > SECTOR_SIZE){
			headerIndex = 0;
			metadataFormat = NO_METADATA;
			break;
		}
	}
	
	// Finding ID3 metadata (if it exists)
	headerIndex = 0;
	if (metadataFormat == LISTINFO_METADATA){
		metadataFormat = ID3_METADATA;
	}
	
	while (wavData1[headerIndex] != 'I' && wavData1[headerIndex + 1] != 'D' && wavData1[headerIndex + 2] != '3') {
		headerIndex++;
		// If ID3 tag not found, then no metadata
		if (headerIndex > SECTOR_SIZE){
			headerIndex = 0;
			metadataFormat = NO_METADATA;
			break;
		}
	}
	
	// Fetch metadata given different formats
	switch (metadataFormat){
		case NO_METADATA:
			// Assigning default values
			fileData.title[0] = headerIndex + 30;
			fileData.title[1] = 'T';
			fileData.artist[0] = 'U';
			fileData.artist[1] = 'A';
			fileData.album[0] = 'U';
			fileData.album[1] = 'a';
			fileData.creationYear[0] = '1';
			fileData.creationYear[1] = '9';
			fileData.creationYear[2] = '1';
			fileData.creationYear[3] = '7';
		break;
		
		
		
		case ID3_METADATA:
			// Finding Artist tag
			while (wavData1[headerIndex + i] != 'T' && wavData1[headerIndex + i + 1] != 'P' && wavData1[headerIndex + i + 2] != 'E' && wavData1[headerIndex + i + 3] != '1'){
				i++;
				if (i > SECTOR_SIZE - 2){
					i = 0;
					break;
				}
			}
			//i += 4;
			/*subchunkSize =
				(wavData1[headerIndex + i + 3] << 24) |
				(wavData1[headerIndex + i + 2] << 16) |
				(wavData1[headerIndex + i + 1] << 8) |
				(wavData1[headerIndex + i + 0]);*/
			//i += 4;
			// Applying found tag to fileData
			for (uint8_t j = 0; j < MAXMETADATACHARS; j++){
				fileData.artist[j] = wavData1[headerIndex + i + j];
			}
		
		
			i = 0;
			// Finding Title tag
			while (wavData1[headerIndex + i] != 'T' && wavData1[headerIndex + i + 1] != 'I' && wavData1[headerIndex + i + 2] != 'T' && wavData1[headerIndex + i + 3] != '2'){
				i++;
				if (i > SECTOR_SIZE - 2){
					i = 0;
					break;
				}
			}
			//i += 4;
			/*subchunkSize =
				(wavData1[headerIndex + i + 3] << 24) |
				(wavData1[headerIndex + i + 2] << 16) |
				(wavData1[headerIndex + i + 1] << 8) |
				(wavData1[headerIndex + i + 0]);*/
			//i += 4;
			// Applying found tag to fileData
			for (uint8_t j = 0; j < MAXMETADATACHARS; j++){
				fileData.title[j] = wavData1[headerIndex + i + j];
			}
		
		
			i = 0;
			// Finding Album tag
			while (wavData1[headerIndex + i] != 'T' && wavData1[headerIndex + i + 1] != 'A' && wavData1[headerIndex + i + 2] != 'L' && wavData1[headerIndex + i + 3] != 'B'){
				i++;
				if (i > SECTOR_SIZE - 2){
					i = 0;
					break;
				}
			}
			//i += 4;
			/*subchunkSize =
				(wavData1[headerIndex + i + 3] << 24) |
				(wavData1[headerIndex + i + 2] << 16) |
				(wavData1[headerIndex + i + 1] << 8) |
				(wavData1[headerIndex + i + 0]);*/
			//i += 4;
			// Applying found tag to fileData
			for (uint8_t j = 0; j < MAXMETADATACHARS; j++){
				fileData.album[j] = wavData1[headerIndex + i + j];
			}
		
		
			i = 0;
			// Finding year tag
			while (wavData1[headerIndex + i] != 'T' && wavData1[headerIndex + i + 1] != 'D' && wavData1[headerIndex + i + 2] != 'R' && wavData1[headerIndex + i + 3] != 'L'){
				i++;
				if (i > SECTOR_SIZE - 2){
					i = 0;
					break;
				}
			}
			//i += 4;
			/*subchunkSize =
				(wavData1[headerIndex + i + 3] << 24) |
				(wavData1[headerIndex + i + 2] << 16) |
				(wavData1[headerIndex + i + 1] << 8) |
				(wavData1[headerIndex + i + 0]);*/
			//i += 4;
			// Applying found tag to fileData
			for (uint8_t j = 0; j < 4; j++){
				fileData.creationYear[j] = wavData1[headerIndex + i + j];
			}
		break;
		
		
		
		case LISTINFO_METADATA:
			// Finding Artist tag
			while (wavData1[headerIndex + i] != 'I' && wavData1[headerIndex + i + 1] != 'A' && wavData1[headerIndex + i + 2] != 'R' && wavData1[headerIndex + i + 3] != 'T'){
				i++;
				if (headerIndex > SECTOR_SIZE - 4){
					headerIndex = 0;
					break;
				}
			}
			i += 4;
			subchunkSize =
				(wavData1[headerIndex + i + 3] << 24) |
				(wavData1[headerIndex + i + 2] << 16) |
				(wavData1[headerIndex + i + 1] << 8) |
				(wavData1[headerIndex + i + 0]);
			i += 4;
			// Applying found tag to fileData
			for (uint8_t j = 0; j < MAXMETADATACHARS; j++){
				fileData.artist[j] = wavData1[headerIndex + i + j];
			}
		
		
			i = 0;
			// Finding Title tag
			while (wavData1[headerIndex + i] != 'I' && wavData1[headerIndex + i + 1] != 'N' && wavData1[headerIndex + i + 2] != 'A' && wavData1[headerIndex + i + 3] != 'M'){
				i++;
				if (headerIndex > SECTOR_SIZE - 4){
					headerIndex = 0;
					break;
				}
			}
			i += 4;
			subchunkSize =
				(wavData1[headerIndex + i + 3] << 24) |
				(wavData1[headerIndex + i + 2] << 16) |
				(wavData1[headerIndex + i + 1] << 8) |
				(wavData1[headerIndex + i + 0]);
			i += 4;
			// Applying found tag to fileData
			for (uint8_t j = 0; j < MAXMETADATACHARS; j++){
				fileData.title[j] = wavData1[headerIndex + i + j];
			}
		
		
			i = 0;
			// Finding Album tag
			while (wavData1[headerIndex + i] != 'I' && wavData1[headerIndex + i + 1] != 'P' && wavData1[headerIndex + i + 2] != 'R' && wavData1[headerIndex + i + 3] != 'D'){
				i++;
				if (headerIndex > SECTOR_SIZE - 4){
					headerIndex = 0;
					break;
				}
			}
			i += 4;
			subchunkSize =
				(wavData1[headerIndex + i + 3] << 24) |
				(wavData1[headerIndex + i + 2] << 16) |
				(wavData1[headerIndex + i + 1] << 8) |
				(wavData1[headerIndex + i + 0]);
			i += 4;
			// Applying found tag to fileData
			for (uint8_t j = 0; j < MAXMETADATACHARS; j++){
				fileData.album[j] = wavData1[headerIndex + i + j];
			}
		
		
			i = 0;
			// Finding year tag
			while (wavData1[headerIndex + i] != 'I' && wavData1[headerIndex + i + 1] != 'C' && wavData1[headerIndex + i + 2] != 'R' && wavData1[headerIndex + i + 3] != 'D'){
				i++;
				if (headerIndex > SECTOR_SIZE - 4){
					headerIndex = 0;
					break;
				}
			}
			i += 4;
			/*subchunkSize =
				(wavData1[headerIndex + i + 3] << 24) |
				(wavData1[headerIndex + i + 2] << 16) |
				(wavData1[headerIndex + i + 1] << 8) |
				(wavData1[headerIndex + i + 0]);*/
			i += 4;
			// Applying found tag to fileData
			for (uint8_t j = 0; j < 4; j++){
				fileData.creationYear[j] = wavData1[headerIndex + i + j];
			}
		break;
			
	}
	
	
	// Finding "data" chunk
	while (wavData1[headerIndex] != 'd' && wavData1[headerIndex + 1] != 'a' && wavData1[headerIndex + 2] != 't' && wavData1[headerIndex + 3] != 'a') {
		headerIndex++;
		if (headerIndex > SECTOR_SIZE){
			return ERROR_NO_DATA_SUBCHUNK;
		}
	}
	
	
	// Jumps to Subchunk2Size
	headerIndex += 4;
	fileData.audioSampleBytes =
		(wavData1[headerIndex + 3] << 24) |
		(wavData1[headerIndex + 2] << 16) |
		(wavData1[headerIndex + 1] << 8) |
		(wavData1[headerIndex + 0])
	;
	
	fileData.audioSampleTables = fileData.audioSampleBytes / WAVDATA_SIZE;
	
	
	// Jumps to Data
	headerIndex += 4;
	fileData.firstDataByteIndex = headerIndex;
	
	// Ending fetch AudioInfo routine
	
	
	// Computing track duration
	//fileData.sDuration = fileData.audioSampleBytes / (fileData.channelNumber * fileData.blockAlign) / fileData.sampleRate;
	
	// Verifying fileData parameters
	if (
		(fileData.channelNumber != AUDIO_CHANNELS) ||
		/*(fileData.sampleRate != AUDIO_SAMPLERATE) ||*/
		(fileData.blockAlign != AUDIO_BLOCKALIGN) ||
		(fileData.bitsPerSample != AUDIO_BPS)
	) {
		return ERROR_INCOMPATIBLE_FILE;
	}
	return true;
}



/* Timer 1 interruption
 *
 * This interruption occurs at 44.1 kHz for .wav playback
 * Uses nextSample flag to tell audio_playFile to load
 * the next sample, and so that tc1 doesn't refresh
 * audio output if it has not changed since last interruption
 *
 * Created 09.11.17 MLN
 * Last modified 23.11.17 MLN
 */
__attribute__((__interrupt__)) void tc1_irq( void ){

	// Updating output first to have a fixed interval update, independent
	// from the data fetching time
	static uint16_t audioR;
	static uint16_t audioL;
	
	_setOutput(audioR, audioL);
	
	// Audio loading
	audioL = *(wavDataPointer + wavDataIndex);
	audioL += (*(wavDataPointer + wavDataIndex + 1)) << 8;
	
	wavDataIndex += 2;
	if (wavDataIndex >= WAVDATA_SIZE){
		//audioR = AUDIOMAXINPUT;
		wavDataIndex = 0;
		if (wavDataPointer == wavData1) {
			wavDataPointer = wavData2;
			loadNextSector = LOAD_WAVDATA1;
		}
		else {
			wavDataPointer = wavData1;
			loadNextSector = LOAD_WAVDATA2;
		}
	}
	
	audioR = *(wavDataPointer + wavDataIndex);
	audioR += (*(wavDataPointer + wavDataIndex + 1)) << 8;
	
	wavDataIndex += 2;
	if (wavDataIndex >= WAVDATA_SIZE){
		//audioR = AUDIOMAXINPUT;
		wavDataIndex = 0;
		if (wavDataPointer == wavData1) {
			wavDataPointer = wavData2;
			loadNextSector = LOAD_WAVDATA1;
		}
		else {
			wavDataPointer = wavData1;
			loadNextSector = LOAD_WAVDATA2;
		}
	}
	
	// Converting signed audio samples into unsigned ones with a DC component of 0x8000
	audioL += 0x8000;
	audioL >>= 6;
	audioR += 0x8000;
	audioR >>= 6;
	
	// Clearing interrupt flag
	AVR32_TC.channel[TC1_CHANNEL].SR;
}



/* Timer 0 interruption
 *
 * This interruption generates a square wave on audio output.
 *
 * Created 09.11.17 MLN
 * Last modified 13.11.17 MLN
 */
__attribute__((__interrupt__)) void tc0_irq( void ){
	if (audioR == 0 || audioL == 0)
	{
		audioR = AUDIOMAXINPUT;
		audioL = AUDIOMAXINPUT;
	}
	else
	{
		audioR = 0;
		audioL = 0;
	}
	_setOutput(audioR, audioL);
	
	// Clearing interrupt flag
	AVR32_TC.channel[TC0_CHANNEL].SR;
}
