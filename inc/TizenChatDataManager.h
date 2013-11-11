/*
 * TizenChatDataManager.h
 *
 *  Created on: Nov 4, 2013
 *      Author: egv
 */

#ifndef TIZENCHATDATAMANAGER_H_
#define TIZENCHATDATAMANAGER_H_

#import <FBase.h>
#import <FNet.h>

#import "Message.h"
#import "ITizenChatDataManagerEventsListener.h"
#import "LongPollServerData.h"

using namespace Tizen::Net::Http;

class TizenChatDataManager :
		public Tizen::Net::Http::IHttpTransactionEventListener
{
public:
    static TizenChatDataManager& GetInstance()
    {
        static TizenChatDataManager	instance; // Guaranteed to be destroyed.
        									  // Instantiated on first use.
        return instance;
    };

    // listeners management stuff
    void AddDataManagerEventsListener(const ITizenChatDataManagerEventsListener& listener);
    void RemoveDataManagerEventsListener(const ITizenChatDataManagerEventsListener& listener);

    // data-related methods
    Tizen::Base::Collection::ArrayList *GetLastMessages();
    void LoadLastMessages();
    void GetUser(int userId);

    // long poll server data
    void ObtainLongPollServerData();
    LongPollServerData* GetLongPollServerData();


    // HttpTransactionListener methods
	virtual void OnTransactionReadyToRead(Tizen::Net::Http::HttpSession& httpSession,
			                              Tizen::Net::Http::HttpTransaction& httpTransaction,
	                                      int availableBodyLen);
	virtual void OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r);
	virtual void OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize);
	virtual void OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool bAuthRequired);
	virtual void OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction);
	virtual void OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Tizen::Base::String* pCert);

private:
	static const int GET_LONG_POLL_SERVER_DATA_REQUEST_TAG = 100;
	static const int GET_DIALOGS_REQUEST_TAG = 101;

	LongPollServerData* __pLongPollServerData;
	Tizen::Net::Http::HttpSession* __pHttpSession;
    Tizen::Base::Collection::ArrayList* __pListeners;
    Tizen::Base::Collection::ArrayList* __pLastMessages;

    bool getDialogsRequestRunning;
    bool __getLongPollServerDataRequest;

    TizenChatDataManager() : __pHttpSession(null)
                           , __pLongPollServerData(null)
    {

    };                   // Constructor? (the {} brackets) are needed here.

    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    TizenChatDataManager(TizenChatDataManager const&);              // Don't Implement
    void operator=(TizenChatDataManager const&); 				    // Don't implement

    ~TizenChatDataManager();

    result SendGetRequest(Tizen::Base::String& url, Tizen::Base::Object* tag);

    //
    // Notifications
    //
    void NotifyLongPollServerDataUpdated();
    void NotifyMessagesUpdated();

    //
    // Parsers
    //
    void ParseLongPollServerData(HttpTransaction &httpTransaction);
    void ParseMessages(HttpTransaction& httpTransaction);
    void ParseUser();
};

#endif /* TIZENCHATDATAMANAGER_H_ */
