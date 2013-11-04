/*
 * Message.h
 *
 *  Created on: Nov 3, 2013
 *      Author: egv
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <FBaseObject.h>
#include <FBase.h>
#include <FWebJson.h>

class Message: public Tizen::Base::Object
{
public:
	Tizen::Base::LongLong id;
	Tizen::Base::LongLong userId;
	Tizen::Base::LongLong date;
	Tizen::Base::LongLong readState;
	Tizen::Base::LongLong isOut;
	Tizen::Base::String title;
	Tizen::Base::String	body;
	Tizen::Base::Collection::ArrayList attachments;
	Tizen::Base::Collection::ArrayList fwdMessages;
	Tizen::Base::LongLong hasEmoji;
	Tizen::Base::LongLong isDeleted;

	// here go multichat messages
	Tizen::Base::LongLong chatId;
	Tizen::Base::Collection::ArrayList chatActive;
	Tizen::Base::LongLong usersCount;
	Tizen::Base::LongLong adminId;
	Tizen::Base::String photo50;
	Tizen::Base::String photo100;
	Tizen::Base::String photo200;

	Message();
	result FillWithJsonObject(const Tizen::Web::Json::JsonObject& object);
};

#endif /* MESSAGE_H_ */
