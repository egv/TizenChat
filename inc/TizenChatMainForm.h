#ifndef _TIZENCHAT_MAIN_FORM_H_
#define _TIZENCHAT_MAIN_FORM_H_

#include <FApp.h>
#include <FBase.h>
#include <FSystem.h>
#include <FUi.h>
#include <FUiIme.h>
#include <FGraphics.h>
#include <FNet.h>
#include <gl.h>

using namespace Tizen::Net::Http;

class TizenChatMainForm
	: public Tizen::Ui::Controls::Form
	, public Tizen::Ui::IActionEventListener
	, public Tizen::Ui::Controls::IFormBackEventListener
	, public Tizen::Ui::Scenes::ISceneEventListener
	, public Tizen::Net::Http::IHttpTransactionEventListener
{

public:
	Tizen::Base::Collection::ArrayList* messages = null;

public:
	TizenChatMainForm(void);
	virtual ~TizenChatMainForm(void);
	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnFormBackRequested(Tizen::Ui::Controls::Form& source);
	virtual void OnActionPerformed(const Tizen::Ui::Control& source, int actionId);

	virtual void OnTransactionReadyToRead(Tizen::Net::Http::HttpSession& httpSession,
			                              Tizen::Net::Http::HttpTransaction& httpTransaction,
	                                      int availableBodyLen);
	virtual void OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r);
	virtual void OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize);
	virtual void OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool bAuthRequired);
	virtual void OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction);
	virtual void OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert);

protected:
	static const int ID_HEADER_ITEM1 = 101;
	static const int ID_HEADER_ITEM2 = 102;
	static const int ID_HEADER_ITEM3 = 103;
	static const int ID_HEADER_ITEM4 = 104;

	bool getDialogsRequestRunning;

	void GetDialogs(Tizen::Base::String& accessToken);

	virtual void OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
								   const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs);
	virtual void OnSceneDeactivated(const Tizen::Ui::Scenes::SceneId& currentSceneId,
									const Tizen::Ui::Scenes::SceneId& nextSceneId);

private:
	Tizen::Net::Http::HttpSession* __pHttpSession = null;

};

#endif	//_TIZENCHAT_MAIN_FORM_H_
