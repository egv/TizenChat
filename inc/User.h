/*
 * User.h
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#ifndef USER_H_
#define USER_H_

#include <FBase.h>
#include <FWebJson.h>

class User: public Tizen::Base::Object
{
public:

	Tizen::Base::LongLong id;
	Tizen::Base::String firstName;
	Tizen::Base::String lastName;
	Tizen::Base::String screenName;
	Tizen::Base::LongLong sex;
	Tizen::Base::String photo;
	Tizen::Base::String photoMediumRec;
	Tizen::Base::LongLong online;

	User() {};
	result FillWithJsonObject(const Tizen::Web::Json::JsonObject& object);
	void Log(void);
};



#endif /* USER_H_ */
