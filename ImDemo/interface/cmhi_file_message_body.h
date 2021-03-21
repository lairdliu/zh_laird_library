/***************************************************************************
  * file name cmhi_file_message_body.h
  * brief 文件类消息
  * date 2019/02/02
  *************************************************************************/

#ifndef CMCC_IM_FILE_MESSAGE_BODY_H_
#define CMCC_IM_FILE_MESSAGE_BODY_H_


#include "base_typedefs.h"
#include "cmhi_message_body.h"

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

class FileMessageBody: public MessageBody{
	
		public:
			
			FileMessageBody(){}
			FileMessageBody(string local_path);
			~FileMessageBody(){}
			
			void set_file_name(string file_name);
			string get_file_name(){ return file_name; }
			void set_origin_link(string origin_link);
			string get_origin_link(){return origin_link; }
			void set_file_length(uint32_t file_length);
			uint32_t get_file_length(){return file_length; }
			void set_local_path(string local_path);
			string get_local_path(){return local_path; }
		private:
			string file_name;		//文件名称
			string origin_link;		//原始链接
			uint32_t file_length;	//文件大小
			string local_path;
		
	};

}
#endif //CMCC_IM_FILE_MESSAGE_BODY_H_


