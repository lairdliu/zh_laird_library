/***************************************************************************
  * file name cmhi_audio_message_body.h
  * brief 语音消息体
  * date 2019/02/12
  *************************************************************************/

#ifndef CMCC_IM_AUDIO_MESSAGE_BODY_H_
#define CMCC_IM_AUDIO_MESSAGE_BODY_H_

#include "cmhi_file_message_body.h"

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class AudioMessageBody: public FileMessageBody{
	
		public:
			
			AudioMessageBody(){}
			AudioMessageBody(string local_path);
			AudioMessageBody(string file_name, string local_path, int duration=0);
			~AudioMessageBody(){}

			void set_duration(int duration);
			void set_voice_to_word(string text);
			int get_duration();
			string get_voice_to_word();
			
			
			
		private:
			int duration;
			bool is_played;
			string voice_to_word;
		
	};

}
#endif //CMCC_IM_AUDIO_MESSAGE_BODY_H_


