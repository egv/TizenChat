#ifndef TIZENCHATDATAMANAGEREVENTSLISTENER_H_
#define TIZENCHATDATAMANAGEREVENTSLISTENER_H_

#import <FBase.h>

class ITizenChatDataManagerEventsListener :
	public Tizen::Base::Object
{
public:
	virtual ~ITizenChatDataManagerEventsListener();

	virtual void OnDataManagerUpdatedMessages() = 0;
	virtual void OnDataManagerUpdatedUser(int userId) = 0;

	virtual void OnDataManagerGotError(Tizen::Base::String errorText) = 0;
};

#endif
