/*
 * User.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: egv
 */

#include "User.h"
#include "Utils.h"

#include <FBase.h>
#include <FWebJson.h>

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

result
User::FillWithJsonObject(const Tizen::Web::Json::JsonObject& object)
{
	result r = E_SUCCESS;

	/*

	Tizen::Base::LongLong id;
	Tizen::Base::String firstName;
	Tizen::Base::String lastName;
	Tizen::Base::String screenName;
	Tizen::Base::LongLong sex;
	Tizen::Base::String photo;
	Tizen::Base::String photoMediumRec;
	Tizen::Base::LongLong online;


		 */

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"id"), true, id);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get id");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"first_name"), true, firstName);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get first name");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"last_name"), true, lastName);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get last name");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"screen_name"), true, screenName);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get screen name");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"photo_medium"), true, photo);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get photo_medium");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"photo_medium_rec"), true, photoMediumRec);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get photo_medium_rec");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"sex"), true, sex);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get sex");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"online"), true, online);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get online");

	return r;
}
