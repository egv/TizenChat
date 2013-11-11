/*
 * LongPollServerData.h
 *
 *  Created on: Nov 11, 2013
 *      Author: egv
 */

#ifndef LONGPOLLSERVERDATA_H_
#define LONGPOLLSERVERDATA_H_

#include <FBaseObject.h>
#include <FBase.h>
#include <FWebJson.h>

class LongPollServerData : public Tizen::Base::Object
{
public:

	Tizen::Base::String key;
	Tizen::Base::String server;
	Tizen::Base::LongLong ts;

	LongPollServerData();

	result FillWithJsonObject(const Tizen::Web::Json::JsonObject& object);
};

#endif /* LONGPOLLSERVERDATA_H_ */
