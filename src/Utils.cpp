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
#include <FWebJson.h>

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::App;
using namespace Tizen::Io;
using namespace Tizen::Web::Json;

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

	registry.Flush();
}

result
Utils::LongLongFromJsonObject(const Tizen::Web::Json::JsonObject& object, const Tizen::Base::String& key, bool mandatory, Tizen::Base::LongLong &out)
{
	result r = E_SUCCESS;

	// check if object is a json object
	TryReturn(object.GetType() == JSON_TYPE_OBJECT, E_INVALID_ARG, "this is not a json object");

	// check if there is such a key
	bool hasKey;
	r = object.ContainsKey(&key, hasKey);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "%s turned to invalid arg", key.GetPointer());

	if (mandatory && !hasKey)
	{
		TryReturn(hasKey, E_INVALID_ARG, "no key %s in object", key.GetPointer());
	}

	if (hasKey)
	{
		IJsonValue *jsonValue = null;
		r = object.GetValue(&key, jsonValue);
		TryReturn(r == E_SUCCESS, E_INVALID_ARG, "%s turned to invalid arg", key.GetPointer());
		TryReturn(jsonValue != null, E_INVALID_ARG, "value for %s is null", key.GetPointer());

		bool typeIsCorrect = jsonValue->GetType() == JSON_TYPE_NUMBER || jsonValue->GetType() == JSON_TYPE_NULL;
		TryReturn(typeIsCorrect, E_INVALID_ARG, "%s is of incorrect type", key.GetPointer());

		if (jsonValue->GetType() != JSON_TYPE_NULL)
		{
			JsonNumber *jsonNumber = static_cast<JsonNumber*>(jsonValue);
			out = LongLong(jsonNumber->ToLongLong());
		}
	}

	return r;
}

result
Utils::StringFromJsonObject(const Tizen::Web::Json::JsonObject& object, const Tizen::Base::String& key, bool mandatory, Tizen::Base::String &out)
{
	result r = E_SUCCESS;

	// check if object is a json object
	TryReturn(object.GetType() == JSON_TYPE_OBJECT, E_INVALID_ARG, "this is not a json object");

	// check if there is such a key
	bool hasKey;
	r = object.ContainsKey(&key, hasKey);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "%s: turned to invalid arg", key.GetPointer());

	if (mandatory && !hasKey)
	{
		TryReturn(hasKey, E_INVALID_ARG, "no key %s in object", key.GetPointer());
	}

	if (hasKey)
	{
		IJsonValue *jsonValue = null;
		r = object.GetValue(&key, jsonValue);
		TryReturn(r == E_SUCCESS, E_INVALID_ARG, "%s: turned to invalid arg", key.GetPointer());
		TryReturn(jsonValue != null, E_INVALID_ARG, "value for %s is null", key.GetPointer());

		bool typeIsCorrect = jsonValue->GetType() == JSON_TYPE_STRING || jsonValue->GetType() == JSON_TYPE_NULL;
		TryReturn(typeIsCorrect, E_INVALID_ARG, "%s is of incorrect type", key.GetPointer());

		if (jsonValue->GetType() != JSON_TYPE_NULL)
		{
			JsonString *jsonString = static_cast<JsonString*>(jsonValue);
			out = String(*jsonString);
		}
	}

	return r;
}

result
Utils::LongLongArrayFromJsonObject(const Tizen::Web::Json::JsonObject& object, const Tizen::Base::String& key, bool mandatory, Tizen::Base::Collection::ArrayList &out)
{
	result r = E_SUCCESS;

	// check if object is a json object
	TryReturn(object.GetType() == JSON_TYPE_OBJECT, E_INVALID_ARG, "this is not a json object");

	// check if there is such a key
	bool hasKey;
	r = object.ContainsKey(&key, hasKey);
	TryReturn(r == E_SUCCESS, E_INVALID_ARG, "%s: turned to invalid arg", key.GetPointer());

	if (mandatory && !hasKey)
	{
		TryReturn(hasKey, E_INVALID_ARG, "no key %s in object", key.GetPointer());
	}

	if (hasKey)
	{
		IJsonValue *jsonValue = null;
		r = object.GetValue(&key, jsonValue);
		TryReturn(r == E_SUCCESS, E_INVALID_ARG, "%s: turned to invalid arg", key.GetPointer());
		TryReturn(jsonValue != null, E_INVALID_ARG, "value for %s is null", key.GetPointer());

		bool typeIsCorrect = jsonValue->GetType() == JSON_TYPE_ARRAY || jsonValue->GetType() == JSON_TYPE_NULL;
		TryReturn(typeIsCorrect, E_INVALID_ARG, "%s is of incorrect type", key.GetPointer());

		if (jsonValue->GetType() != JSON_TYPE_NULL)
		{
			JsonArray *pJsonArray = dynamic_cast<JsonArray*>(jsonValue);
			ArrayList arr(SingleObjectDeleter);

	        IEnumeratorT<IJsonValue*>* pEnum = pJsonArray->GetEnumeratorN();
	        if(pEnum)
	        {
	            while( pEnum->MoveNext() == E_SUCCESS )
	            {
	                IJsonValue* pJsonValue = null;
	                //Uses the pJsonValue
	                pEnum->GetCurrent(pJsonValue);
	                TryCatch(pJsonValue->GetType() == JSON_TYPE_NUMBER, delete pEnum, "wrong array content");

	                JsonNumber *jsonNumber = static_cast<JsonNumber*>(pJsonValue);
	                arr.Add(new LongLong(jsonNumber->ToLongLong()));
	            }
	            delete pEnum;
	        }
	        out.RemoveAll();
	        out.AddItems(arr);
		}
	}

	return r;

CATCH:
	return E_INVALID_ARG;
}

result
Utils::JsonValueAtPath(Tizen::Web::Json::JsonObject& object, const Tizen::Base::String& path, Tizen::Web::Json::IJsonValue*& jsonValue)
{
	result r = E_SUCCESS;

	String delim(L"/");
	StringTokenizer tok(path, delim);

	while(tok.HasMoreTokens())
	{
		String token;
		tok.GetNextToken(token);

		IJsonValue* __pJsonValue = null;
		JsonObject* __pJsonObject = jsonValue == null ? &object : static_cast<JsonObject*>(jsonValue);
		__pJsonObject->GetValue(new String(token), __pJsonValue);
		jsonValue = __pJsonValue;
	}

	return r;
}

result
Utils::JoinNumbersArrayList(Tizen::Base::Collection::ArrayList* arr, Tizen::Base::String& str)
{
	result r = E_SUCCESS;

	str = String(L"");

	if (arr->GetCount() > 0)
	{
		Number *number = static_cast<Number*>(arr->GetAt(0));
		str.Append(number->ToString());
		for (int i=1; i < arr->GetCount(); i++)
		{
			Number *number = static_cast<Number*>(arr->GetAt(i));
			str.Append(L",");
			str.Append(number->ToString());
		}
	}

	return r;
}

