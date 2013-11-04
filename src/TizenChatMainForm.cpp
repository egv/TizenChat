
#include <FWebJson.h>

#include "TizenChatMainForm.h"
#include "AppResourceId.h"

#include "Utils.h"
#include "Message.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::App;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Web::Json;
using namespace Tizen::Net::Http;

const static wchar_t* HTTP_CLIENT_HOST_ADDRESS = L"https://api.vk.com";

TizenChatMainForm::TizenChatMainForm(void)
{
}

TizenChatMainForm::~TizenChatMainForm(void)
{
	if (__pHttpSession)
		delete __pHttpSession;

	if (messages)
		delete messages;
}

bool
TizenChatMainForm::Initialize(void)
{
	Construct(IDF_FORM);

	return true;
}

result
TizenChatMainForm::OnInitializing(void)
{
	AppLogDebug("access token: %S", Utils::getInstance().accessToken()->GetPointer());

	result r = E_SUCCESS;

	// TODO:
	// Add your initialization code here
	Header* pHeader = GetHeader();
	if (pHeader)
	{
		pHeader->AddActionEventListener(*this);
	}

	// Setup back event listener
	SetFormBackEventListener(this);

	GetDialogs(*Utils::getInstance().accessToken());

	return r;
}

result
TizenChatMainForm::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO:
	// Add your termination code here
	return r;
}

void
TizenChatMainForm::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
	SceneManager* pSceneManager = SceneManager::GetInstance();
	//	pSceneManager->GoForward(SceneTransitionId(ID_SCNT_3));
	//	pSceneManager->GoForward(SceneTransitionId(ID_SCNT_6));
	AppAssert(pSceneManager);

	switch(actionId)
	{
	case ID_HEADER_ITEM1:
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_1));
		break;
	case ID_HEADER_ITEM2:
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_2));
		break;
	case ID_HEADER_ITEM3:
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_6));
		break;
	case ID_HEADER_ITEM4:
		pSceneManager->GoForward(SceneTransitionId(ID_SCNT_3));
		break;

	default:
		break;
	}
}

void
TizenChatMainForm::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
	UiApp* pApp = UiApp::GetInstance();
	AppAssert(pApp);
	pApp->Terminate();
}

void
TizenChatMainForm::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
		const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
	// TODO: Add your implementation codes here

}

void
TizenChatMainForm::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
		const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
	// TODO: Add your implementation codes here

}


void
TizenChatMainForm::GetDialogs(String& accessToken)
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

	r = pHttpRequest->SetUri(url);
	r = pHttpRequest->SetMethod(NET_HTTP_METHOD_GET);
	r = pHttpTransaction->Submit();

	if (r != E_SUCCESS)
	{
		AppLogDebug("error sending request");
	}
}

void
TizenChatMainForm::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	getDialogsRequestRunning = false;

	if (messages == null)
	{
		messages = new ArrayList(SingleObjectDeleter);
	}

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	// The GetHttpStatusCode() method is used to get the HTTP status from the response header. If the status code is HTTP_STATUS_OK, the transaction is a success.
	AppLogDebug("http response status code: %d", pHttpResponse->GetHttpStatusCode());
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		// The GetRawHeaderN() method gets the HTTP header information from the response, while the ReadBodyN() method gets the content body information.
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
							messages->Add(pMessage);
						}
					}
					delete pEnum;
				}
			}

			delete pBuffer;
			delete pJsonValue;

			AppLogDebug("total messages received: %d", messages->GetCount());
		}
	}
}

void
TizenChatMainForm::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{

}

void
TizenChatMainForm::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{

}

void
TizenChatMainForm::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool bAuthRequired)
{

}

void
TizenChatMainForm::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{

}

void
TizenChatMainForm::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert)
{

}
