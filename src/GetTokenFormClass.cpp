#include <FBase.h>
#include <FIo.h>

#include "AppResourceId.h"
#include "GetTokenFormClass.h"

#include "Utils.h"

using namespace Tizen::Io;
using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Web::Controls;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Base::Utility;
using namespace Tizen::Base::Collection;


GetTokenFormClass::GetTokenFormClass(void)
{
}

GetTokenFormClass::~GetTokenFormClass(void)
{
}

bool
GetTokenFormClass::Initialize()
{
	Form::Construct(GetTokenForm);

	return true;
}

result
GetTokenFormClass::OnInitializing(void)
{
	result r = E_SUCCESS;

	Web* pWeb1 = static_cast<Web*>(GetControl(IDC_WEB1));  
	if(pWeb1)
	{
		pWeb1->SetLoadingListener(this);
		String url = String(L"https://oauth.vk.com/authorize?client_id=3968795&scope=notify,messages,friends,notifications&redirect_uri=");
		String redirectUri;
		Tizen::Base::Utility::UrlEncoder::Encode(String(L"http://oauth.vk.com/blank.html"), L"UTF-8", redirectUri);
		url.Append(redirectUri);
		url.Append(String(L"&display=mobile&v=5.2&response_type=token"));
		pWeb1->LoadUrl(url);
	}
	return r;
}

result
GetTokenFormClass::OnTerminating(void)
{
	result r = E_SUCCESS;

	return r;
}



void
GetTokenFormClass::OnEstimatedProgress(int progress)
{
}

void
GetTokenFormClass::OnHttpAuthenticationCanceled(void)
{
}

bool
GetTokenFormClass::OnHttpAuthenticationRequestedN(const Tizen::Base::String& host, const Tizen::Base::String& realm, const Tizen::Web::Controls::AuthenticationChallenge& authentication)
{
	return true;
}

void
GetTokenFormClass::OnLoadingCanceled(void)
{
}

void
GetTokenFormClass::OnLoadingCompleted(void)
{
	Web* pWeb1 = static_cast<Web*>(GetControl(IDC_WEB1));
	if(pWeb1)
	{
		String url = pWeb1->GetUrl();
		String prefix(L"https://oauth.vk.com/blank.html");

		if (url.StartsWith(prefix, 0))
		{
			url.Replace(prefix + String(L"#"), String(L""));
			String delim(L"&");
			StringTokenizer tok(url, delim);
			HashMap params(SingleObjectDeleter);
			params.Construct();

			AppLogDebug("url: %S", url.GetPointer());

			while(tok.HasMoreTokens())
			{
				String token;
				tok.GetNextToken(token);

				StringTokenizer tok1(token, String(L"="));
				if (tok1.GetTokenCount() == 2)
				{
					String key;
					String value;
					tok1.GetNextToken(key);
					tok1.GetNextToken(value);
					params.Add(new String(key), new String(value));
				}
			}

			if (params.GetCount() > 0)
			{
				if (!params.ContainsKey(String(L"error")))
				{
					// we have no error here
					String *token = static_cast<String*>(params.GetValue(String(L"access_token")));
					String *expire = static_cast<String*>(params.GetValue(String(L"expires_in")));

					Utils::getInstance().setAccessToken(*token);

					AppLogDebug("token is: %S, expire is: %S", token->GetPointer(), expire->GetPointer());

					SceneManager* pSceneManager = SceneManager::GetInstance();
					pSceneManager->GoForward(ForwardSceneTransition(L"Messages"));
				}
				else
				{
					String *errorDescription = static_cast<String*>(params.GetValue(String(L"error")));
					AppLogDebug("error logging in: %S", errorDescription->GetPointer());
				}
			}
			else
			{
				AppLogDebug("can not detect params");
			}
		}
		else
		{
			AppLogDebug("finished loading wrong url");
		}
	}
}

void
GetTokenFormClass::OnLoadingErrorOccurred(LoadingErrorType error, const Tizen::Base::String& reason)
{
}

bool
GetTokenFormClass::OnLoadingRequested(const Tizen::Base::String& url, WebNavigationType type)
{
	return false;
}

void
GetTokenFormClass::OnLoadingStarted(void)
{
}

void
GetTokenFormClass::OnPageTitleReceived(const Tizen::Base::String& title)
{
}

DecisionPolicy
GetTokenFormClass::OnWebDataReceived(const Tizen::Base::String& mime, const Tizen::Net::Http::HttpHeader& httpHeader) 
{
	return WEB_DECISION_CONTINUE;
}

void
GetTokenFormClass::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{
}

void
GetTokenFormClass::OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
								const Tizen::Ui::Scenes::SceneId& nextSceneId)
{
}
