/***************************************************************************
  * file name cmim_audio_message_body.cpp
  * brief 语音消息内容
  * date 2019/02/12
  *************************************************************************/
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "cmhi_audio_message_body.h"

namespace cmhi_iov{

AudioMessageBody::AudioMessageBody(string local_path)
{
    int32_t len;
    string file_name;
    len = local_path.find_last_of("/");
    if (len != -1)        
    {           
        file_name = local_path.substr(len + 1, local_path.length() - len);
        this->set_file_name(file_name);
    }

	this->set_local_path(local_path) ;
	set_message_body_type(kMessageBodyTypeVoice);
}

AudioMessageBody::AudioMessageBody(string file_name, string local_path, int duration)
{
	this->set_file_name(file_name) ;
	this->set_local_path(local_path) ;
	set_duration(duration);
	set_message_body_type(kMessageBodyTypeVoice);
}

void AudioMessageBody::set_duration(int duration)
{
	this->duration = duration;
}

void AudioMessageBody::set_voice_to_word(string text) {
    this->voice_to_word = text;
}

int AudioMessageBody::get_duration()
{
	return duration;
}

string AudioMessageBody::get_voice_to_word() {
    return voice_to_word;
}

}

