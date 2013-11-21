/*
 * LongPollServerData.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: egv
 */

#include <FBase.h>
#include <FWebJson.h>

#include "LongPollServerData.h"
#include "Utils.h"

using namespace Tizen::Base;
using namespace Tizen::Web::Json;

LongPollServerData::LongPollServerData()
{
}

result
LongPollServerData::FillWithJsonObject(const Tizen::Web::Json::JsonObject& jsonObject)
{
	result r = E_FAILURE;

	r = Utils::getInstance().StringFromJsonObject(jsonObject, String(L"server"), true, server);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "\"key\" field in long poll server info is invalid");

	r = Utils::getInstance().StringFromJsonObject(jsonObject, String(L"key"), true, key);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "\"server\" field in long poll server info is invalid");

	r = Utils::getInstance().LongLongFromJsonObject(jsonObject, String(L"ts"), true, ts);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "\"ts\" field in long poll server info is invalid");

	return r;
}

void
LongPollServerData::LogDebug(void)
{
	AppLogDebug("long poll server data {\n\tserver: %ls,\n\tkey: %ls,\n\ts: %d\n}", server.GetPointer(), key.GetPointer(), ts.ToInt());
}
