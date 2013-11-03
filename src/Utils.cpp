/*
 * Utils.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: egv
 */

#include "Utils.h"

#include <FApp.h>
#include <FBase.h>
#include <FIo.h>

using namespace Tizen::Base;
using namespace Tizen::App;
using namespace Tizen::Io;

Utils::Utils() : registry()
{
	MAIN_SECTION_NAME = String(L"main_section_name");
	ACCESS_TOKEN_KEY = String(L"access_token");
	String pathName(L"tizen_chat.ini");

	result r = E_FAILURE;

	r = registry.Construct(App::GetInstance()->GetAppDataPath() + pathName, "a+");
	if (r == E_SUCCESS)
	{
		registry.AddSection(MAIN_SECTION_NAME);
	}
}

String* Utils::accessToken()
{
	String accessToken;
	result r;

	r = registry.GetValue(MAIN_SECTION_NAME, ACCESS_TOKEN_KEY, accessToken);

	return r == E_SUCCESS ? new String(accessToken) : null;
}

void Utils::setAccessToken(const String& accessToken)
{
	result r;
	r = registry.AddValue(MAIN_SECTION_NAME, ACCESS_TOKEN_KEY, accessToken);
	if (r == E_KEY_ALREADY_EXIST)
	{
		registry.SetValue(MAIN_SECTION_NAME, ACCESS_TOKEN_KEY, accessToken);
	}
}
