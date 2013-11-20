/*
 * TizenChatDataManager.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: egv
 */

#include <FBase.h>
#include <FNet.h>
#include <FWebJson.h>

#include "ITizenChatDataManagerEventsListener.h"
#include "TizenChatDataManager.h"
#include "Utils.h"
#include "DatabaseManager.h"

#include "Message.h"
#include "User.h"

#define MAGIC_NUMBER 2000000000

const static wchar_t* HTTP_CLIENT_HOST_ADDRESS = L"https://api.vk.com";

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;
using namespace Tizen::Net::Http;
using namespace Tizen::Web::Json;

TizenChatDataManager::~TizenChatDataManager()
{
	DELETE_NON_NULL(__pHttpSession);
}

void
TizenChatDataManager::AddDataManagerEventsListener(const ITizenChatDataManagerEventsListener& listener)
{
	if (__pListeners == null)
	{
		__pListeners = new ArrayList();
		__pListeners->Construct();
	}

	if (!__pListeners->Contains(listener))
	{
		__pListeners->Add(listener);
	}
}

void
TizenChatDataManager::RemoveDataManagerEventsListener(const ITizenChatDataManagerEventsListener& listener)
{
	if (__pListeners == null)
	{
		__pListeners = new ArrayList();
		__pListeners->Construct();
	}

	if (__pListeners->Contains(listener))
	{
		__pListeners->Remove(listener);
	}
}

void
TizenChatDataManager::LoadLongPollHistory()
{
	if (getDialogsRequestRunning)
		return;

	if (__pLongPollServerData == null)
		return;

	getDialogsRequestRunning = true;
	String url(L"https://api.vk.com/method/messages.getLongPollHistory?max_msg_id=1&msgs_limit=200&ts=");
	url.Append(__pLongPollServerData->ts.ToString());

	result r = SendGetRequest(url, MakeTagForCode(GET_LONG_POLL_HISTORY_REQUEST_TAG));

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
		getDialogsRequestRunning = false;
	}
}

void
TizenChatDataManager::LoadLastMessages()
{
	if (getDialogsRequestRunning)
		return;

	if (__pLongPollServerData == null)
		return;

	getDialogsRequestRunning = true;
	String url(L"https://api.vk.com/method/messages.getDialogs?count=200");

	result r = SendGetRequest(url, MakeTagForCode(GET_DIALOGS_REQUEST_TAG));

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
		getDialogsRequestRunning = false;
	}
}

void
TizenChatDataManager::LoadChatHistory(int chatId, int offset, int count, int startMessageId, int rev)
{
	String url(L"https://api.vk.com/method/messages.getHistory?");

	if (offset > 0)
	{
		url.Append(L"offset=");
		url.Append(LongLong(offset).ToString());
		url.Append(L"&");
	}

	if (count > 0)
	{
		url.Append(L"count=");
		url.Append(LongLong(count).ToString());
		url.Append(L"&");
	}

	if (startMessageId > 0)
	{
		url.Append(L"start_message_id=");
		url.Append(LongLong(startMessageId).ToString());
		url.Append(L"&");
	}

	url.Append(L"rev=");
	url.Append(LongLong(rev).ToString());
	url.Append(L"&");

	url.Append(L"user_id=");
	url.Append(chatId < 0 ? -chatId : chatId + MAGIC_NUMBER);


	result r = SendGetRequest(url, MakeTagForCode(MESSAGES_GET_HISTORY_TAG));

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
		__getLongPollServerDataRequest = false;
	}
}

void
TizenChatDataManager::SendMessage(Message* pMessage)
{
	if (pMessage == null)
	{
		return;
	}

	String url(L"https://api.vk.com/method/messages.send?");
	url.Append(L"guid=");
	url.Append(pMessage->uuid.ToString());
	url.Append(L"&");

	if (pMessage->chatId.ToInt() < 0)
	{
		url.Append(L"type=0&user_id=");
		url.Append(pMessage->userId.ToString());
		url.Append(L"&");
	}
	else
	{
		url.Append(L"type=1&chat_id=");
		url.Append(pMessage->chatId.ToString());
		url.Append(L"&");
	}

	String encodedMessage;
	UrlEncoder::Encode(pMessage->body, L"UTF-8", encodedMessage);
	url.Append(L"message=");
	url.Append(encodedMessage);

	HashMap* tag = MakeTagForCode(MESSAGES_SEND_TAG);
	tag->Add(new String(L"message"), pMessage);
	result r = SendGetRequest(url, tag);

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
	}
}

void
TizenChatDataManager::ObtainLongPollServerData()
{
	if (__getLongPollServerDataRequest)
		return;

	__getLongPollServerDataRequest = true;
	String url(L"https://api.vk.com/method/messages.getLongPollServer");

	result r = SendGetRequest(url, MakeTagForCode(GET_LONG_POLL_SERVER_DATA_REQUEST_TAG));

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
		__getLongPollServerDataRequest = false;
	}
}

void
TizenChatDataManager::LoadUsers(Tizen::Base::Collection::ArrayList* userIds)
{
	String url(L"https://api.vk.com/method/users.get?user_ids=");

	String userIdsStr;
	Utils::getInstance().JoinNumbersArrayList(userIds, userIdsStr);

	url.Append(userIdsStr);
	url.Append(L"&name_case=Nom&fields=screen_name,sex,online,photo_medium,photo_medium_rec");

	result r = SendGetRequest(url, MakeTagForCode(USERS_GET_REQUEST_TAG));

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request to get users");
	}
}



LongPollServerData*
TizenChatDataManager::GetLongPollServerData()
{
	return __pLongPollServerData;
}

void
TizenChatDataManager::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	HashMap *userObject = static_cast<HashMap*>(httpTransaction.GetUserObject());
	Number* pOpCodeNumber = static_cast<Number*>(userObject->GetValue(String(L"tag")));
	int opCode = pOpCodeNumber->ToInt();

	switch (opCode)
	{
	case GET_DIALOGS_REQUEST_TAG:
	case MESSAGES_GET_HISTORY_TAG:
		AppLogDebug("will parse messages");
		ParseMessages(httpTransaction);
		break;

	case GET_LONG_POLL_HISTORY_REQUEST_TAG:
		AppLogDebug("will parse long poll messages");
		ParseLongPollHistory(httpTransaction);
		break;

	case GET_LONG_POLL_SERVER_DATA_REQUEST_TAG:
		ParseLongPollServerData(httpTransaction);
		break;

	case USERS_GET_REQUEST_TAG:
		ParseUsersGetData(httpTransaction);
		break;

	case MESSAGES_SEND_TAG:
		ParseSendMessageResult(httpTransaction);
		break;

	default:
		AppLogDebug("unknown operation: %d", opCode);
		break;
	}
}

void
TizenChatDataManager::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLogDebug("On transaction aborted");
}

void
TizenChatDataManager::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{

}

void
TizenChatDataManager::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool bAuthRequired)
{
}

void
TizenChatDataManager::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLogDebug("On transaction completed");
}

void
TizenChatDataManager::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert)
{

}

result
TizenChatDataManager::SendGetRequest(Tizen::Base::String& url, Tizen::Base::Object* tag)
{
	result r = E_SUCCESS;
	HttpTransaction* pHttpTransaction = null;
	HttpRequest* pHttpRequest = null;

	if (__pHttpSession == null)
	{
		__pHttpSession = new (std::nothrow) HttpSession();

		r = __pHttpSession->Construct(NET_HTTP_SESSION_MODE_NORMAL,
				null,
				HTTP_CLIENT_HOST_ADDRESS,
				null);
	}
	else
	{
		__pHttpSession->CloseAllTransactions();
		AppLogDebug("http session is not null, but it should be");
	}

	pHttpTransaction = __pHttpSession->OpenTransactionN();
	r = pHttpTransaction->AddHttpTransactionListener(*this);

	pHttpRequest = const_cast<HttpRequest*>(pHttpTransaction->GetRequest());

	pHttpTransaction->SetUserObject(tag);

	String correctUrl(url);
	if (correctUrl.Contains(L"?"))
	{
		correctUrl.Append(L"&");
	}
	else
	{
		correctUrl.Append(L"?");
	}
	correctUrl.Append(L"v=5.3&access_token=");
	correctUrl.Append(*(Utils::getInstance().accessToken()));

	AppLogDebug("request url: %S", correctUrl.GetPointer());


	r = pHttpRequest->SetUri(correctUrl);
	r = pHttpRequest->SetMethod(NET_HTTP_METHOD_GET);
	r = pHttpTransaction->Submit();

	return r;
}

void
TizenChatDataManager::NotifyError(Tizen::Base::LongLong errorCode, Tizen::Base::String errorMessage)
{
	if (__pListeners == null)
	{
		return;
	}

	IEnumerator* pEnum = __pListeners->GetEnumeratorN();
	ITizenChatDataManagerEventsListener* pObj = null;
	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pObj = (ITizenChatDataManagerEventsListener*)(pEnum->GetCurrent());
		pObj->OnDataManagerGotError(errorCode, errorMessage);
	}

	delete pEnum;
}


void
TizenChatDataManager::NotifyMessagesUpdated()
{
	if (__pListeners == null)
	{
		return;
	}

	IEnumerator* pEnum = __pListeners->GetEnumeratorN();
	ITizenChatDataManagerEventsListener* pObj = null;
	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pObj = (ITizenChatDataManagerEventsListener*)(pEnum->GetCurrent());
		pObj->OnDataManagerUpdatedMessages();
	}

	delete pEnum;
}

void
TizenChatDataManager::NotifyLongPollServerDataUpdated()
{
	if (__pListeners == null)
	{
		return;
	}

	IEnumerator* pEnum = __pListeners->GetEnumeratorN();
	ITizenChatDataManagerEventsListener* pObj = null;
	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pObj = (ITizenChatDataManagerEventsListener*)(pEnum->GetCurrent());
		pObj->OnDataManagerUpdatedLongPollServerData();
	}

	delete pEnum;
}



void
TizenChatDataManager::ParseLongPollServerData(HttpTransaction &httpTransaction)
{
	__getLongPollServerDataRequest = false;

	if (__pLongPollServerData == null)
	{
		__pLongPollServerData = new LongPollServerData();
	}

	bool hasError;
	LongLong errorCode;
	String errorMessage;

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	// The GetHttpStatusCode() method is used to get the HTTP status from the response header.
	// If the status code is HTTP_STATUS_OK, the transaction is a success.
	AppLogDebug("http response status code: %d", pHttpResponse->GetHttpStatusCode());
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		// The GetRawHeaderN() method gets the HTTP header information from the response,
		// while the ReadBodyN() method gets the content body information.
		if (pHttpHeader != null)
		{
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			IJsonValue *pJsonValue = JsonParser::ParseN(*pBuffer);
			if (pJsonValue->GetType() == JSON_TYPE_OBJECT)
			{
				JsonObject *pJsonObject = static_cast<JsonObject*>(pJsonValue);

				hasError = CheckForError(pJsonObject, errorCode, errorMessage);
				if (!hasError)
				{
					IJsonValue *pResponseJsonValue = null;
					pJsonObject->GetValue(new String(L"response"), pResponseJsonValue);

					JsonObject *pResponseJsonObject = static_cast<JsonObject*>(pResponseJsonValue);
					result r = __pLongPollServerData->FillWithJsonObject(*pResponseJsonObject);

					if (r != E_SUCCESS)
					{
						AppLogDebug("error parsing long poll object");
						delete __pLongPollServerData;
						__pLongPollServerData = null;
					}
				}
			}

			delete pBuffer;
			delete pJsonValue;
		}
	}

	if (hasError)
	{
		NotifyError(errorCode, errorMessage);
	}
	else
	{
		NotifyLongPollServerDataUpdated();
	}
}

void
TizenChatDataManager::ParseLongPollHistory(HttpTransaction &httpTransaction)
{
	AppLogDebug("hit parse long poll history");
	getDialogsRequestRunning = false;

	bool hasError;
	LongLong errorCode;
	String errorMessage;

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	// The GetHttpStatusCode() method is used to get the HTTP status from the response header.
	// If the status code is HTTP_STATUS_OK, the transaction is a success.
	AppLogDebug("http response status code: %d", pHttpResponse->GetHttpStatusCode());
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		// The GetRawHeaderN() method gets the HTTP header information from the response,
		// while the ReadBodyN() method gets the content body information.
		if (pHttpHeader != null)
		{
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			IJsonValue *pJsonValue = JsonParser::ParseN(*pBuffer);
			if (pJsonValue->GetType() == JSON_TYPE_OBJECT)
			{
				JsonObject *pJsonObject = static_cast<JsonObject*>(pJsonValue);
				hasError = CheckForError(pJsonObject, errorCode, errorMessage);
				if (!hasError)
				{
					ArrayList* pMessageList = ParseMessagesFromJsonPath(pJsonObject, String(L"response/messages/items"));
					DatabaseManager::GetInstance().SaveOrUpdateMessages(pMessageList);
				}
			}

			delete pBuffer;
			delete pJsonValue;
		}
	}

	if (hasError)
	{
		NotifyError(errorCode, errorMessage);
	}
	else
	{
		NotifyMessagesUpdated();
	}
}

void
TizenChatDataManager::ParseUsersGetData(HttpTransaction &httpTransaction)
{
	bool hasError;
	LongLong errorCode;
	String errorMessage;

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	AppLogDebug("http response status code: %d", pHttpResponse->GetHttpStatusCode());
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		if (pHttpHeader != null)
		{
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			IJsonValue *pJsonValue = JsonParser::ParseN(*pBuffer);
			if (pJsonValue->GetType() == JSON_TYPE_OBJECT)
			{
				JsonObject *pJsonObject = static_cast<JsonObject*>(pJsonValue);

				hasError = CheckForError(pJsonObject, errorCode, errorMessage);
				if (!hasError)
				{
					IJsonValue *pJsonArrayValue = null;
					result r = Utils::getInstance().JsonValueAtPath(*pJsonObject, String(L"response"), pJsonArrayValue);
					if (r == E_SUCCESS)
					{
						JsonArray *pJsonArray = static_cast<JsonArray*>(pJsonArrayValue);

						IEnumeratorT<IJsonValue*>* pEnum = pJsonArray->GetEnumeratorN();
						if(pEnum)
						{
							while( pEnum->MoveNext() == E_SUCCESS )
							{
								IJsonValue* pJsonValue = null;
								//Uses the pJsonValue
								pEnum->GetCurrent(pJsonValue);
								JsonObject *pJsonMessageObject = static_cast<JsonObject*>(pJsonValue);

								User* pUser = new User();
								result r = pUser->FillWithJsonObject(*pJsonMessageObject);
								if (r == E_SUCCESS)
								{
									pUser->Log();
									DatabaseManager::GetInstance().SaveOrUpdateUser(pUser);
									delete pUser;
								}
							}

							delete pEnum;
						}
					}
					else
					{
						AppLogDebug("failed to get array at correct path");
					}
				}
			}
			delete pBuffer;
			delete pJsonValue;
		}
	}

	if (hasError)
	{
		NotifyError(errorCode, errorMessage);
	}
	else
	{
		NotifyMessagesUpdated();
	}
}

void
TizenChatDataManager::ParseMessages(HttpTransaction &httpTransaction)
{
	AppLogDebug("hit parse messages");
	getDialogsRequestRunning = false;

	bool hasError;
	LongLong errorCode;
	String errorMessage;

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	// The GetHttpStatusCode() method is used to get the HTTP status from the response header.
	// If the status code is HTTP_STATUS_OK, the transaction is a success.
	AppLogDebug("http response status code: %d", pHttpResponse->GetHttpStatusCode());
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		// The GetRawHeaderN() method gets the HTTP header information from the response,
		// while the ReadBodyN() method gets the content body information.
		if (pHttpHeader != null)
		{
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			IJsonValue *pJsonValue = JsonParser::ParseN(*pBuffer);
			if (pJsonValue->GetType() == JSON_TYPE_OBJECT)
			{
				JsonObject *pJsonObject = static_cast<JsonObject*>(pJsonValue);

				hasError = CheckForError(pJsonObject, errorCode, errorMessage);
				if (!hasError)
				{
					ArrayList* pMessageList = ParseMessagesFromJsonPath(pJsonObject, String(L"response/items"));
					DatabaseManager::GetInstance().SaveOrUpdateMessages(pMessageList);
				}
			}
			delete pBuffer;
			delete pJsonValue;
		}
	}

	if (hasError)
	{
		NotifyError(errorCode, errorMessage);
	}
	else
	{
		ArrayList* pMissigUserIds = DatabaseManager::GetInstance().GetUnknownUsers();
		if (pMissigUserIds->GetCount() > 0)
		{
			LoadUsers(pMissigUserIds);
		}

		delete pMissigUserIds;

		NotifyMessagesUpdated();
	}
}

void
TizenChatDataManager::ParseUser()
{

}

bool
TizenChatDataManager::CheckForError(Tizen::Web::Json::JsonObject* pJsonObject, Tizen::Base::LongLong& errorCode, Tizen::Base::String& errorMessage)
{
	bool hasKey;
	String* key = new String(L"error");

	pJsonObject->ContainsKey(key, hasKey);
	if (hasKey)
	{
		IJsonValue* __pJsonValue = null;
		pJsonObject->GetValue(key, __pJsonValue);

		JsonObject *__pJsonObject = static_cast<JsonObject*>(__pJsonValue);
		Utils::getInstance().LongLongFromJsonObject(*__pJsonObject, String(L"error_code"), true, errorCode);
		Utils::getInstance().StringFromJsonObject(*__pJsonObject, String(L"error_msg"), true, errorMessage);
	}

	return hasKey;
}

Tizen::Base::Collection::ArrayList*
TizenChatDataManager::ParseMessagesFromJsonPath(Tizen::Web::Json::JsonObject* pJsonObject, Tizen::Base::String path)
{
	ArrayList* pArrayList = new ArrayList();
	pArrayList->Construct(200);

	IJsonValue *pJsonArrayValue = null;
	result r = Utils::getInstance().JsonValueAtPath(*pJsonObject, String(L"response/items"), pJsonArrayValue);
	if (r == E_SUCCESS)
	{
		JsonArray *pJsonArray = static_cast<JsonArray*>(pJsonArrayValue);

		IEnumeratorT<IJsonValue*>* pEnum = pJsonArray->GetEnumeratorN();
		if(pEnum)
		{
			while( pEnum->MoveNext() == E_SUCCESS )
			{
				IJsonValue* pJsonValue = null;
				//Uses the pJsonValue
				pEnum->GetCurrent(pJsonValue);
				JsonObject *pJsonMessageObject = static_cast<JsonObject*>(pJsonValue);

				Message* pMessage = new Message();
				result r = pMessage->FillWithJsonObject(*pJsonMessageObject);
				if (r == E_SUCCESS)
				{
					pArrayList->Add(pMessage);
				}
			}

			delete pEnum;
		}
	}
	else
	{
		AppLogDebug("failed to get array at correct path");
	}

	return pArrayList;
}

void
TizenChatDataManager::ParseSendMessageResult(HttpTransaction &httpTransaction)
{
	bool hasError;
	LongLong errorCode;
	String errorMessage;

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	// The GetHttpStatusCode() method is used to get the HTTP status from the response header.
	// If the status code is HTTP_STATUS_OK, the transaction is a success.
	AppLogDebug("http response status code: %d", pHttpResponse->GetHttpStatusCode());
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		// The GetRawHeaderN() method gets the HTTP header information from the response,
		// while the ReadBodyN() method gets the content body information.
		if (pHttpHeader != null)
		{
			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			IJsonValue *pJsonValue = JsonParser::ParseN(*pBuffer);
			if (pJsonValue->GetType() == JSON_TYPE_OBJECT)
			{
				JsonObject *pJsonObject = static_cast<JsonObject*>(pJsonValue);

				hasError = CheckForError(pJsonObject, errorCode, errorMessage);
				if (!hasError)
				{
					HashMap* tag = (HashMap*)httpTransaction.GetUserObject();
					Message* pMessage = (Message*)tag->GetValue(String(L"message"));
					Utils::getInstance().LongLongFromJsonObject(*pJsonObject, String(L"response"), true, pMessage->id);
					AppLogDebug("got new message id: %d", pMessage->id.ToInt());
					DatabaseManager::GetInstance().SaveOrUpdateMessage(pMessage);
				}
			}
			delete pBuffer;
			delete pJsonValue;
		}
	}

	if (hasError)
	{
		NotifyError(errorCode, errorMessage);
	}
	else
	{
		ArrayList* pMissigUserIds = DatabaseManager::GetInstance().GetUnknownUsers();
		if (pMissigUserIds->GetCount() > 0)
		{
			LoadUsers(pMissigUserIds);
		}

		delete pMissigUserIds;

		NotifyMessagesUpdated();
	}
}

Tizen::Base::Collection::HashMap*
TizenChatDataManager::MakeTagForCode(int code)
{
	HashMap* result = new HashMap;
	result->Construct();

	result->Add(new String(L"tag"), new Integer(code));

	return result;
}
