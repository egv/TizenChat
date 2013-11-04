/*
 * Message.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: egv
 */

#include "Utils.h"
#include "Message.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Web::Json;

Message::Message() : attachments(SingleObjectDeleter)
                     , fwdMessages(SingleObjectDeleter)
					 , chatActive(SingleObjectDeleter)
{

}

result
Message::FillWithJsonObject(const JsonObject& object)
{
	/*
	Tizen::Base::LongLong id;
	Tizen::Base::LongLong userId;
	Tizen::Base::LongLong date;
	Tizen::Base::Boolean readState;
	Tizen::Base::Boolean isOut;
	Tizen::Base::String title;
	Tizen::Base::String	body;
	Tizen::Base::ArrayList attachments;
	Tizen::Base::ArrayList fwdMessages;
	Tizen::Base::Boolean hasEmoji;
	Tizen::Base::Boolean isDeleted;

	// here go multichat messages
	Tizen::Base::LongLong chatId;
	Tizen::Base::Collection::ArrayList chatActive;
	Tizen::Base::Integer usersCount;
	Tizen::Base::LongLong adminId;
	Tizen::Base::String photo50;
	Tizen::Base::String photo100;
	Tizen::Base::String photo200;
	 */
	result r = E_SUCCESS;

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"id"), true, id);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get id");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"user_id"), true, userId);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get user_id");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"date"), true, date);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get date");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"read_state"), true, readState);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get readState");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"out"), true, isOut);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get out");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"title"), true, title);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get title");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"body"), true, body);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get body");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"emoji"), false, hasEmoji);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get emoji");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"deleted"), false, isDeleted);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get deleted");

	// non-mandatory fields
	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"chat_id"), false, chatId);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get chat_id");

	//chatActive = ArrayList(SingleObjectDeleter);
	r = Utils::getInstance().LongLongArrayFromJsonObject(object, String(L"chat_active"), false, chatActive);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get chat_active");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"users_count"), false, usersCount);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get users_count");

	r = Utils::getInstance().LongLongFromJsonObject(object, String(L"admin_id"), false, adminId);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get admin_id");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"photo_50"), false, photo50);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get photo50");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"photo_100"), false, photo100);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get photo100");

	r = Utils::getInstance().StringFromJsonObject(object, String(L"photo_200"), false, photo200);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "unable to get photo200");

	return r;
}
