/*
 * DatabaseManager.cpp
 *
 *  Created on: Nov 12, 2013
 *      Author: egv
 */

#include "DatabaseManager.h"

#include <FApp.h>
#include <FIo.h>
#include <FBase.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Io;

DatabaseManager::DatabaseManager(void)
{
	String dbName = App::GetInstance()->GetAppDataPath() + L"messages.db";

	__pDatabase = new Database();
	__pDatabase->Construct(dbName, "a+");

	//
	// Create database structure
	//
	{
		String sql(L"create table if not exists VKUsers (id integer primary key, first_name varchar(255), last_name varchar(255), sex integer, screen_name varchar(255), photo varchar(4096), photo_medium_rec varchar(4096), online integer)");
		result r = __pDatabase->ExecuteSql(sql, true);
		if (IsFailed(r))
		{
			AppLogDebug("failed sql: %S", sql.GetPointer());
		}
		else
		{
			AppLogDebug("sql executed ok: %S", sql.GetPointer());
		}
	}

	{
		String sql(L"create table if not exists Messages (pk INTEGER PRIMARY KEY, id integer, date integer, out integer, user_id integer, read_state integer, title varchar(255), body text, chat_id integer, admin_id integer, uuid integer)");
		result r = __pDatabase->ExecuteSql(sql, true);
		if (IsFailed(r))
		{
			AppLogDebug("failed sql: %S", sql.GetPointer());
		}
		else
		{
			AppLogDebug("sql executed ok: %S", sql.GetPointer());
		}
	}

	{
		String sql(L"create table if not exists UsersInMessages (id integer primary key, message_id integer, user_id integer)");
		result r = __pDatabase->ExecuteSql(sql, true);
		if (IsFailed(r))
		{
			AppLogDebug("failed sql: %S", sql.GetPointer());
		}
		else
		{
			AppLogDebug("sql executed ok: %S", sql.GetPointer());
		}
	}
}

//
// Getting messages
//
Tizen::Base::Collection::ArrayList*
DatabaseManager::GetLastMessages(void)
{
	ArrayList* pArrayList = new ArrayList();
	pArrayList->Construct();

	String sql("select ");
	sql.Append("m.pk, m.id, max(m.date), m.out, m.read_state, m.title, m.body, m.chat_id, m.user_id, m.admin_id, m.uuid ");
	sql.Append("from Messages as m ");
	sql.Append("group by m.chat_id ");
	sql.Append("order by m.date asc");

	DbStatement* pStmt = __pDatabase->CreateStatementN(sql);
	AppAssert(pStmt);

	DbEnumerator* pEnum = __pDatabase->ExecuteStatementN(*pStmt);
	if (pEnum)
	{

		while (pEnum->MoveNext() == E_SUCCESS)
		{
			pArrayList->Add(GetMessageFromEnumerator(pEnum));
		}
		delete pEnum;
	}

	delete pStmt;

	return pArrayList;
}

Tizen::Base::Collection::ArrayList*
DatabaseManager::GetChatMessages(int chatId)
{
	ArrayList* pArrayList = new ArrayList();
	pArrayList->Construct();

	String sql("select ");
	sql.Append("m.pk, m.id, m.date, m.out, m.read_state, m.title, m.body, m.chat_id, m.user_id, m.admin_id, m.uuid ");
	sql.Append("from Messages as m ");
	sql.Append("where chat_id = ? ");
	sql.Append("order by m.date asc");

	DbStatement* pStmt = __pDatabase->CreateStatementN(sql);
	AppAssert(pStmt);

	pStmt->BindInt(0, chatId);

	DbEnumerator* pEnum = __pDatabase->ExecuteStatementN(*pStmt);
	if (pEnum)
	{
		while (pEnum->MoveNext() == E_SUCCESS)
		{
			pArrayList->Add(GetMessageFromEnumerator(pEnum));
		}

		delete pEnum;
	}

	delete pStmt;

	return pArrayList;
}

//
// Saving user
//
void
DatabaseManager::SaveOrUpdateUser(User* pUser)
{
	if (__pDatabase == null || pUser == null)
	{
		return;
	}

	String sql(L"insert or replace into VKUsers ");
	sql.Append(" (id, sex, online, first_name, last_name, screen_name, photo, photo_medium_rec) ");
	sql.Append(" values ");
	sql.Append(" (?, ?, ?, ?, ?, ?, ?, ?) ");

	__pDatabase->BeginTransaction();

	DbStatement *pStmt = __pDatabase->CreateStatementN(sql);
	AppAssert(pStmt);

	pStmt->BindInt(0, pUser->id.ToInt());					// message id
	pStmt->BindInt(1, pUser->sex.ToInt());					// message date
	pStmt->BindInt(2, pUser->online.ToInt());					// is it outgoing?
	pStmt->BindString(3, pUser->firstName);				// is it read?
	pStmt->BindString(4, pUser->lastName);
	pStmt->BindString(5, pUser->screenName);
	pStmt->BindString(6, pUser->photo);
	pStmt->BindString(7, pUser->photoMediumRec);

	DbEnumerator* pEnum = __pDatabase->ExecuteStatementN(*pStmt);

    delete pStmt;
    delete pEnum;

    __pDatabase-> CommitTransaction();
}

User*
DatabaseManager::GetUserById(Tizen::Base::LongLong userId)
{
	User* pUser = null;

	String sql("select u.id, u.first_name, u.last_name, u.screen_name, u.photo, u.photo_medium_rec, u.sex, u.online ");
	sql.Append("from VKUsers AS u ");
	sql.Append("where u.id = ? ");

	DbStatement *pStmt = __pDatabase->CreateStatementN(sql);
	AppAssert(pStmt);

	pStmt->BindInt(0, userId.ToInt());

	DbEnumerator *pEnum = __pDatabase->ExecuteStatementN(*pStmt);
	if (pEnum)
	{
		if (pEnum->MoveNext() == E_SUCCESS)
		{
			pUser = GetUserFromEnumerator(pEnum);
		}

		delete pEnum;
	}
	delete pStmt;

	return pUser;
}

void
DatabaseManager::SaveMessage(Message* pMessage)
{
	result r;

	String insertSql(L"insert into Messages ");
	insertSql.Append(" (id, date, out, read_state, title, body, user_id, chat_id, admin_id, uuid) ");
	insertSql.Append(" values ");
	insertSql.Append(" (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	DbStatement *pStmt = __pDatabase->CreateStatementN(insertSql);
	pStmt->BindInt(0, pMessage->id.ToInt());					// message id
	pStmt->BindInt64(1, pMessage->date.ToInt());					// message date
	pStmt->BindInt(2, pMessage->isOut.ToInt());					// is it outgoing?
	pStmt->BindInt(3, pMessage->readState.ToInt());				// is it read?
	pStmt->BindString(4, pMessage->title);
	pStmt->BindString(5, pMessage->body);
	pStmt->BindInt(6, pMessage->userId.ToInt());
	pStmt->BindInt(7, pMessage->chatId.ToInt() == 0 ? -pMessage->userId.ToInt() : pMessage->chatId.ToInt());
	pStmt->BindInt(8, pMessage->adminId.ToInt());
	pStmt->BindInt(9, pMessage->uuid.ToInt());

	DbEnumerator* pEnum = __pDatabase->ExecuteStatementN(*pStmt);
	AppAssert(!pEnum);

	delete pEnum;
	delete pStmt;
}

void
DatabaseManager::UpdateMessage(Message* pMessage)
{
	if (pMessage->pk.ToInt() == 0)
	{
		AppLogDebug("trying to udpate message with pk=0");
		return;
	}

	String updateSql(L"update Messages ");
	updateSql.Append(L"set ");
	updateSql.Append(L"id = ?, date = ?, out = ?, read_state = ?, title = ?, body = ?, user_id = ?, chat_id = ?, admin_id = ?, uuid = ? ");
	updateSql.Append(L"where pk = ?");

	DbStatement *pStmt = __pDatabase->CreateStatementN(updateSql);
	pStmt->BindInt(0, pMessage->id.ToInt());					// message id
	pStmt->BindInt64(1, pMessage->date.ToInt());					// message date
	pStmt->BindInt(2, pMessage->isOut.ToInt());					// is it outgoing?
	pStmt->BindInt(3, pMessage->readState.ToInt());				// is it read?
	pStmt->BindString(4, pMessage->title);
	pStmt->BindString(5, pMessage->body);
	pStmt->BindInt(6, pMessage->userId.ToInt());
	pStmt->BindInt(7, pMessage->chatId.ToInt() == 0 ? -pMessage->userId.ToInt() : pMessage->chatId.ToInt());
	pStmt->BindInt(8, pMessage->adminId.ToInt());
	pStmt->BindInt(9, pMessage->uuid.ToInt());
	pStmt->BindInt(10, pMessage->pk.ToInt());

	DbEnumerator* pEnum = __pDatabase->ExecuteStatementN(*pStmt);
	AppAssert(!pEnum);
	delete pEnum;
	delete pStmt;
}

int
DatabaseManager::PKForMessage(Message* pMessage)
{
	int pk = pMessage->pk.ToInt();
	if (pk > 0)
	{
		return pk;
	}

	String searchSql;
	int param;
	if (pMessage->id.ToInt() > 0)
	{
		searchSql.Append("select pk from Messages where id = ?");
		param = pMessage->id.ToInt();
	}
	else
	{
		searchSql.Append("select pk from Messages where uuid = ?");
		param = pMessage->uuid.ToInt();
	}

	DbStatement* pSearchStmt = __pDatabase->CreateStatementN(searchSql);
	result r = pSearchStmt->BindInt(0, param);
	if (IsFailed(r)) {
		AppLogDebug("failed to bind message id:%d uuid: %d", pMessage->id.ToInt(), pMessage->uuid.ToInt());
	}
	else
	{
		DbEnumerator* pEnum = __pDatabase->ExecuteStatementN(*pSearchStmt);
		if (pEnum != null)
		{
			pEnum->MoveNext();
			pEnum->GetIntAt(0, pk);
		}
		delete pEnum;
	}
	delete pSearchStmt;

	return pk;
}

void
DatabaseManager::SaveOrUpdateMessages(Tizen::Base::Collection::ArrayList* pMessageList)
{
	if (__pDatabase == null || pMessageList == null || pMessageList->GetCount() == 0)
	{
		return;
	}

	__pDatabase-> BeginTransaction();

	IEnumerator* pListEnum = pMessageList->GetEnumeratorN();
	Message* pMessage = null;
	while (pListEnum->MoveNext() == E_SUCCESS)
	{
		pMessage = static_cast<Message*>(pListEnum->GetCurrent());

		int pk = pMessage->pk.ToInt();
		if (pk == 0)
		{
			pk = PKForMessage(pMessage);
		}

		if (pk > 0)
		{
			pMessage->pk = LongLong(pk);
			UpdateMessage(pMessage);
		}
		else
		{
			SaveMessage(pMessage);
		}
	}

	__pDatabase-> CommitTransaction();
}

//
// Saving messages
//
void
DatabaseManager::SaveOrUpdateMessage(Message* pMessage)
{
	if (__pDatabase == null || pMessage == null)
	{
		return;
	}

	__pDatabase->BeginTransaction();

	int pk = pMessage->pk.ToInt();
	if (pk == 0)
	{
		pk = PKForMessage(pMessage);
	}

	if (pk > 0)
	{
		pMessage->pk = LongLong(pk);
		UpdateMessage(pMessage);
	}
	else
	{
		SaveMessage(pMessage);
	}

    __pDatabase-> CommitTransaction();
}

Tizen::Base::Collection::ArrayList*
DatabaseManager::GetUnknownUsers()
{
	ArrayList* pArrayList = new ArrayList();
	pArrayList->Construct(1000);

	String sqls[] = {
			String("select distinct user_id from Messages where user_id not in (select id from VKUsers)"),
			String("select distinct admin_id from Messages where user_id not in (select id from VKUsers)")
	};

	for (int i=0; i < 2; i++)
	{
		DbStatement *pStmt = __pDatabase->CreateStatementN(sqls[i]);
		AppAssert(pStmt);

		DbEnumerator *pEnum = __pDatabase->ExecuteStatementN(*pStmt);
		if (pEnum)
		{
			while (pEnum->MoveNext() == E_SUCCESS)
			{
				int i;
				pEnum->GetIntAt(0, i);

				LongLong* userId = new LongLong(i);
				if (!pArrayList->Contains(*userId))
				{
					pArrayList->Add(userId);
				}
			}

			delete pEnum;
		}
		delete pStmt;
	}

	return pArrayList;
}


//
// Parsing messages from result
//
Message*
DatabaseManager::GetMessageFromEnumerator(Tizen::Io::DbEnumerator* pEnum)
{
	Message* pMessage = new Message();

	int i;
	long long ll;
	String s;

	// 	sql.Append("m.pk, m.id, max(m.date), m.out, m.read_state, m.title, m.body, m.chat_id, m.user_id, m.admin_id ");

	pEnum->GetIntAt(0, i);
	pMessage->pk = LongLong(i);

	pEnum->GetIntAt(1, i);
	pMessage->id = LongLong(i);

	pEnum->GetInt64At(2, ll);
	pMessage->date = LongLong(ll);

	pEnum->GetIntAt(3, i);
	pMessage->isOut = LongLong(i);

	pEnum->GetIntAt(4, i);
	pMessage->readState = LongLong(i);

	pEnum->GetStringAt(5, pMessage->title);
	pEnum->GetStringAt(6, pMessage->body);

	pEnum->GetIntAt(7, i);
	pMessage->chatId = LongLong(i);

	pEnum->GetIntAt(8, i);
	pMessage->userId = LongLong(i);

	pEnum->GetIntAt(9, i);
	pMessage->adminId = LongLong(i);

	pEnum->GetIntAt(10, i);
	pMessage->uuid = LongLong(i);

	return pMessage;
}

//
// Parsing user from result
//
User*
DatabaseManager::GetUserFromEnumerator(Tizen::Io::DbEnumerator* pEnum)
{
	User* pUser = new User();

	int i;
	long long ll;
	String s;

//	String sql("select u.id, u.first_name, u.last_name, u.screen_name, u.photo, u.photo_medium_rec, u.sex, u.online ");

	pEnum->GetIntAt(0, i);
	pUser->id = LongLong(i);

	pEnum->GetStringAt(1, pUser->firstName);
	pEnum->GetStringAt(2, pUser->lastName);
	pEnum->GetStringAt(3, pUser->screenName);
	pEnum->GetStringAt(4, pUser->photo);
	pEnum->GetStringAt(5, pUser->photoMediumRec);

	pEnum->GetIntAt(6, i);
	pUser->sex = LongLong(i);

	pEnum->GetIntAt(7, i);
	pUser->online = LongLong(i);

	return pUser;
}

