//GetAudioRawData
#include "rawdata/rawdata_audio_helper_interface.h"
#include "ZoomSDKAudioRawData.h"
#include "zoom_sdk_def.h" 
#include <iostream>
#include <fstream>

void ZoomSDKAudioRawData::onOneWayAudioRawDataReceived(AudioRawData* audioRawData, uint32_t node_id)
{
	//std::cout << "Received onOneWayAudioRawDataReceived" << std::endl;
	//add your code here
}

void ZoomSDKAudioRawData::onMixedAudioRawDataReceived(AudioRawData* audioRawData)
{
	std::cout << "Received onMixedAudioRawDataReceived" << std::endl;
	//add your code here


	static std::ofstream pcmFile;
	pcmFile.open("audio.pcm", std::ios::out | std::ios::binary | std::ios::app);

	if (!pcmFile.is_open()) {
		std::cout << "Failed to open wave file" << std::endl;
		return;
	}
	
		// Write the audio data to the file
		pcmFile.write((char*)audioRawData->GetBuffer(), audioRawData->GetBufferLen());
		//std::cout << "buffer length: " << audioRawData->GetBufferLen() << std::endl;
		std::cout << "buffer : " << audioRawData->GetBuffer() << std::endl;

		// Close the wave file
		pcmFile.close();
		pcmFile.flush();
}
void ZoomSDKAudioRawData::onShareAudioRawDataReceived(AudioRawData* data_)
{
}

void ZoomSDKAudioRawData::onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) {};
