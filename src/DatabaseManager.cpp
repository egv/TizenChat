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
		String sql(L"create table if not exists Users (id integer primary key, first_name varchar(255), last_name varchar(255), sex integer, screen_name varchar(255), photo varchar(4096), photo_medium_rec varchar(4096), online integer)");
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
		String sql(L"create table if not exists Messages (id integer primary key, date integer, out integer, user_id integer, read_state integer, title varchar(255), body text, chat_id integer, admin_id integer)");
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
	sql.Append("m.id, max(m.date), m.out, m.read_state, m.title, m.body, m.chat_id, m.user_id, m.admin_id ");
	sql.Append("from Messages as m ");
	sql.Append("group by m.chat_id ");
	sql.Append("order by m.date asc");

	DbStatement* pStmt = __pDatabase->CreateStatementN(sql);
	AppAssert(pStmt);

	DbEnumerator* pEnum = __pDatabase->ExecuteStatementN(*pStmt);
	AppAssert(pEnum);

	while (pEnum->MoveNext() == E_SUCCESS)
	{
		pArrayList->Add(GetMessageFromEnumerator(pEnum));
	}

	delete pStmt;
	delete pEnum;

	return pArrayList;
}

Tizen::Base::Collection::ArrayList*
DatabaseManager::GetChatMessages(int chatId)
{
	ArrayList* pArrayList = new ArrayList();
	pArrayList->Construct();

	return pArrayList;
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

	String sql(L"insert or replace into Messages ");
	sql.Append(" (id, date, out, read_state, title, body, user_id, chat_id, admin_id) ");
	sql.Append(" values ");
	sql.Append(" (?, ?, ?, ?, ?, ?, ?, ?, ?)");

	__pDatabase->BeginTransaction();

	DbStatement *pStmt = __pDatabase->CreateStatementN(sql);

	pStmt->BindInt(0, pMessage->id.ToInt());					// message id
	pStmt->BindInt64(1, pMessage->date.ToInt());					// message date
	pStmt->BindInt(2, pMessage->isOut.ToInt());					// is it outgoing?
	pStmt->BindInt(3, pMessage->readState.ToInt());				// is it read?
	pStmt->BindString(4, pMessage->title);
	pStmt->BindString(5, pMessage->body);
	pStmt->BindInt(6, pMessage->userId.ToInt());
	pStmt->BindInt(7, pMessage->chatId.ToInt() == 0 ? -pMessage->userId.ToInt() : pMessage->chatId.ToInt());
	pStmt->BindInt(8, pMessage->adminId.ToInt());

	DbEnumerator* pEnum = __pDatabase->ExecuteStatementN(*pStmt);
    AppAssert(!pEnum);

    delete pStmt;
    delete pEnum;

    __pDatabase-> CommitTransaction();
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

	// 	sql.Append("m.id, max(m.date), m.out, m.read_state, m.title, m.body, m.chat_id, m.user_id, m.admin_id ");

	pEnum->GetIntAt(0, i);
	pMessage->id = LongLong(i);

	pEnum->GetInt64At(1, ll);
	pMessage->date = LongLong(ll);

	pEnum->GetIntAt(2, i);
	pMessage->isOut = LongLong(i);

	pEnum->GetIntAt(3, i);
	pMessage->readState = LongLong(i);

	pEnum->GetStringAt(4, pMessage->title);
	pEnum->GetStringAt(5, pMessage->body);

	pEnum->GetIntAt(6, i);
	pMessage->chatId = LongLong(i);

	pEnum->GetIntAt(7, i);
	pMessage->userId = LongLong(i);

	pEnum->GetIntAt(8, i);
	pMessage->adminId = LongLong(i);

	return pMessage;
}
