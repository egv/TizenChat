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
    void LoadLastMessages();
    void LoadLongPollHistory();

    void LoadUsers(Tizen::Base::Collection::ArrayList* userIds);
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
	static const int GET_LONG_POLL_HISTORY_REQUEST_TAG = 102;
	static const int USERS_GET_REQUEST_TAG = 102;


	LongPollServerData* __pLongPollServerData;
	Tizen::Net::Http::HttpSession* __pHttpSession;
    Tizen::Base::Collection::ArrayList* __pListeners;

    bool getDialogsRequestRunning;
    bool __getLongPollServerDataRequest;

    TizenChatDataManager() : __pLongPollServerData(null)
                           , __pHttpSession(null)
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
    // util methods
    //
    bool CheckForError(Tizen::Web::Json::JsonObject* pJsonObject, Tizen::Base::LongLong& errorCode, Tizen::Base::String& errorMessage);

    //
    // Notifications
    //
    void NotifyLongPollServerDataUpdated();
    void NotifyMessagesUpdated();
    void NotifyError(Tizen::Base::LongLong errorCode, Tizen::Base::String errorMessage);

    //
    // Parsers
    //
    void ParseUsersGetData(HttpTransaction &httpTransaction);
    void ParseLongPollServerData(HttpTransaction &httpTransaction);
    void ParseLongPollHistory(HttpTransaction &httpTransaction);
    void ParseMessages(HttpTransaction& httpTransaction);
    void ParseUser();

    Tizen::Base::Collection::ArrayList* ParseMessagesFromJsonPath(Tizen::Web::Json::JsonObject* pJsonObject, Tizen::Base::String path);
};

#endif /* TIZENCHATDATAMANAGER_H_ */
