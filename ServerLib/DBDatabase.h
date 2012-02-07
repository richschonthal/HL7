//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _DBDATABASE_H_
#define _DBDATABASE_H_

//--------------------------------------------------------------------------------
#include <ReadWriteObject.h>

//--------------------------------------------------------------------------------
class CDBDatabaseElement
	{
	friend class CDBDatabase;

	protected:
		DWORD m_nElementId;

	public:
		CDBDatabaseElement()
			: m_nElementId(0)
			{
			};
		virtual ~CDBDatabaseElement() {}
	};

//--------------------------------------------------------------------------------
class CDBDatabase : public CReadWriteObject
	{
	public:
		class Map : public CMap<DWORD, DWORD, CDBDatabaseElement*, CDBDatabaseElement*&>
			{
			};

		class List : public CTypedPtrList<CPtrList, CDBDatabaseElement*>
			{
			};

	public:
		Map* m_pMap;
		List* m_pList;

	public:
		CDBDatabase();
		virtual ~CDBDatabase();

	public:
		// adds a the element to the list and the end of the queue
		bool Add(CDBDatabaseElement*);
		// remove element from list and queue
		int Remove(CDBDatabaseElement*, bool bDelete = false);
		// move element to the end of the queue and update
		// the position data memeber
		bool ReQueue(CDBDatabaseElement*);
		// move an element from this database to pDB
		bool Move(CDBDatabaseElement*, CDBDatabase* pDB);
		// gets the position of the head element
		POSITION GetHeadPosition();
		// gets the element at position pos, and advances pos to the next position - null at end
		CDBDatabaseElement* GetNext(POSITION& pos);
		// gets the element from the top of the queue, moves it to the end of the queue
		CDBDatabaseElement* GetNextAndReQueue();
		// locates an element by id
		CDBDatabaseElement* Find(DWORD);
		// returns the number of records in the database
		int GetCount();
	};

#endif //_DBDATABASE_H_
