/*
 * TizenChatDataManager.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: egv
 */

#include <FBase.h>
#include <FNet.h>
#include <FWebJson.h>

#include "ITizenChatDatamanagerEventsListener.h"
#include "TizenChatDataManager.h"
#include "Utils.h"
#include "Message.h"
#include "DatabaseManager.h"

const static wchar_t* HTTP_CLIENT_HOST_ADDRESS = L"https://api.vk.com";

using namespace Tizen::Base;
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


ArrayList*
TizenChatDataManager::GetLastMessages()
{
	return __pLastMessages;
}

void
TizenChatDataManager::LoadLongPollHistory()
{
	if (getDialogsRequestRunning)
		return;

	if (__pLongPollServerData == null)
		return;

	getDialogsRequestRunning = true;
	String url(L"https://api.vk.com/method/messages.getLongPollHistory?v=5.2&max_msg_id=1&msgs_limit=200&ts=");
	url.Append(__pLongPollServerData->ts.ToString());
	url.Append("&access_token=");
	url.Append(*(Utils::getInstance().accessToken()));

	result r = SendGetRequest(url, new Integer(GET_LONG_POLL_HISTORY_REQUEST_TAG));

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
	String url(L"https://api.vk.com/method/messages.getDialogs?v=5.2&count=200&access_token=");
	url.Append(*(Utils::getInstance().accessToken()));

	result r = SendGetRequest(url, new Integer(GET_DIALOGS_REQUEST_TAG));

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
		getDialogsRequestRunning = false;
	}
}


void
TizenChatDataManager::ObtainLongPollServerData()
{
	if (__getLongPollServerDataRequest)
		return;

	__getLongPollServerDataRequest = true;
	String url(L"https://api.vk.com/method/messages.getLongPollServer?v=5.2&access_token=");
	url.Append(*(Utils::getInstance().accessToken()));

	result r = SendGetRequest(url, new Integer(GET_LONG_POLL_SERVER_DATA_REQUEST_TAG));

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
		__getLongPollServerDataRequest = false;
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
	Number *pOpCodeNumber = static_cast<Number*>(httpTransaction.GetUserObject());
	int opCode = pOpCodeNumber->ToInt();

	switch (opCode)
	{
	case GET_DIALOGS_REQUEST_TAG:
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
	AppLogDebug("request url: %S", url.GetPointer());

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

	r = pHttpRequest->SetUri(url);
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

	if (__pLastMessages == null)
	{
		__pLastMessages = new ArrayList();
		__pLastMessages->Construct();
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
					IJsonValue *pJsonArrayValue = null;
					result r = Utils::getInstance().JsonValueAtPath(*pJsonObject, String(L"response/messages/items"), pJsonArrayValue);
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
								// FIXME: add them as a bunch in one transaction
								if (r == E_SUCCESS)
								{
									DatabaseManager::GetInstance().SaveOrUpdateMessage(pMessage);
									__pLastMessages->Add(*pMessage);
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

			AppLogDebug("total messages received: %d", __pLastMessages->GetCount());
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

	if (__pLastMessages == null)
	{
		__pLastMessages = new ArrayList();
		__pLastMessages->Construct();
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
									DatabaseManager::GetInstance().SaveOrUpdateMessage(pMessage);
									__pLastMessages->Add(*pMessage);
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

			AppLogDebug("total messages received: %d", __pLastMessages->GetCount());
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

