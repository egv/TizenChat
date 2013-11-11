
#ifndef _GETTOKENFORMCLASS_H_
#define _GETTOKENFORMCLASS_H_

#include <FBase.h>
#include <FWeb.h>
#include <FUi.h>



class GetTokenFormClass :
	public Tizen::Ui::Controls::Form,
 	public Tizen::Web::Controls::ILoadingListener,
 	public Tizen::Ui::Scenes::ISceneEventListener
{

// Construction
public:
	GetTokenFormClass(void);
	virtual ~GetTokenFormClass(void);
	bool Initialize();
	result OnInitializing(void);
	result OnTerminating(void);

// Implementation
protected:

// Generated call-back functions
public:


	virtual void OnEstimatedProgress(int progress);
	virtual void OnHttpAuthenticationCanceled(void);
	virtual bool OnHttpAuthenticationRequestedN(const Tizen::Base::String& host, const Tizen::Base::String& realm, const Tizen::Web::Controls::AuthenticationChallenge& authentication);
	virtual void OnLoadingCanceled(void);
	virtual void OnLoadingCompleted(void);
	virtual void OnLoadingErrorOccurred(Tizen::Web::Controls::LoadingErrorType error, const Tizen::Base::String& reason);
	virtual bool OnLoadingRequested(const Tizen::Base::String& url, Tizen::Web::Controls::WebNavigationType type);
	virtual void OnLoadingStarted(void);
	virtual void OnPageTitleReceived(const Tizen::Base::String& title);
	virtual Tizen::Web::Controls::DecisionPolicy OnWebDataReceived(const Tizen::Base::String& mime, const Tizen::Net::Http::HttpHeader& httpHeader);

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);
};

#endif
