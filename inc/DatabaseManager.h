/*
 * DatabaseManager.h
 *
 *  Created on: Nov 12, 2013
 *      Author: egv
 */

#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <FBase.h>
#include <FIo.h>

#include "User.h"
#include "Message.h"

class DatabaseManager
{
public:
    static DatabaseManager& GetInstance()
    {
        static DatabaseManager	instance; // Guaranteed to be destroyed.
        								  // Instantiated on first use.
        return instance;
    }

    Tizen::Base::Collection::ArrayList* GetLastMessages(void);
    Tizen::Base::Collection::ArrayList* GetChatMessages(int chatId);
    void SaveOrUpdateMessage(Message* pMessage);
    void SaveOrUpdateMessages(Tizen::Base::Collection::ArrayList* pMessageList);
    int PKForMessage(Message* pMessage);

    void SaveOrUpdateUser(User *pUser);
    User* GetUserById(Tizen::Base::LongLong userId);
    Tizen::Base::Collection::ArrayList* GetUnknownUsers();

private:
	Tizen::Io::Database* __pDatabase;

    DatabaseManager();                   // Constructor? (the {} brackets) are needed here.

    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    DatabaseManager(DatabaseManager const&);              // Don't Implement
    void operator=(DatabaseManager const&);               // Don't implement

    User* GetUserFromEnumerator(Tizen::Io::DbEnumerator* pEnum);
    Message* GetMessageFromEnumerator(Tizen::Io::DbEnumerator* pEnum);

    void SaveMessage(Message* pMessage);
    void UpdateMessage(Message* pMessage);
};

#endif /* DATABASEMANAGER_H_ */
