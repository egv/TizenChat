/*
 * TizenChatDataManager.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: egv
 */

#include <FBase.h>
#include <FNet.h>
#include <FWebJson.h>

#include "TizenChatDataManager.h"
#include "Utils.h"
#include "Message.h"

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

void
TizenChatDataManager::LoadLastMessages()
{
	if (getDialogsRequestRunning)
		return;

	getDialogsRequestRunning = true;

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
		AppLogDebug("http session is not null, but it should be");
	}

	pHttpTransaction = __pHttpSession->OpenTransactionN();
	r = pHttpTransaction->AddHttpTransactionListener(*this);

	pHttpRequest = const_cast<HttpRequest*>(pHttpTransaction->GetRequest());

	String url(L"https://api.vk.com/method/messages.getDialogs?v=5.2&count=200&access_token=");
	url.Append(*(Utils::getInstance().accessToken()));

	AppLogDebug("request url: %S", url.GetPointer());

	pHttpTransaction->SetUserObject(new Integer(GET_DIALOGS_REQUEST_TAG));

	r = pHttpRequest->SetUri(url);
	r = pHttpRequest->SetMethod(NET_HTTP_METHOD_GET);
	r = pHttpTransaction->Submit();

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
	}
}

void
TizenChatDataManager::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	Number *pOpCodeNumber = static_cast<Number*>(httpTransaction.GetUserObject());
	int opCode = pOpCodeNumber->ToInt();

	switch (opCode)
	{
	case GET_DIALOGS_REQUEST_TAG:
		ParseMessages(httpTransaction);
		break;

	default:
		AppLogDebug("unknown operation: %d", opCode);
		break;
	}
}

void
TizenChatDataManager::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{

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

}

void
TizenChatDataManager::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert)
{

}

void
TizenChatDataManager::NotifyMessagesUpdated()
{

}

void
TizenChatDataManager::ParseMessages(HttpTransaction &httpTransaction)
{
/*
	getDialogsRequestRunning = false;

	if (__pLastMessages == null)
	{
		__pLastMessages = new ArrayListT<Message>();
		__pLastMessages->Construct(1000);
	}


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

				IJsonValue *pResponseJsonValue = null;
				pJsonObject->GetValue(new String(L"response"), pResponseJsonValue);

				JsonObject *pResponseJsonObject = static_cast<JsonObject*>(pResponseJsonValue);

				IJsonValue *pJsonArrayValue = null;
				pResponseJsonObject->GetValue(new String(L"items"), pJsonArrayValue);
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
							__pLastMessages->Add(*pMessage);
						}
					}
					delete pEnum;
				}
			}

			delete pBuffer;
			delete pJsonValue;

			AppLogDebug("total messages received: %d", __pLastMessages->GetCount());
		}
	}
*/
	NotifyMessagesUpdated();
}

void
TizenChatDataManager::ParseUser()
{

}
