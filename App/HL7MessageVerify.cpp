//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// HL7MessageVerify.cpp: implementation of the CHL7MessageVerify class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "ntserviceeventlogmsg.h"
#include "HL7MessageVerify.h"

#include "globals.h"

#include "PACSDatabase.h"
#include "HL7DBMessagedef.h"
#include "HL7DBFieldDef.h"
#include "HL7DBMsgSegDef.h"
#include "HL7DBRuleDef.h"
#include "HL7DBSegmentDef.h"
#include "HL7DBVersionDef.h"
#include "HL7DataTypes.h"

#include "HL7Segment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_RESULTMAP(CMsgVerifyResult, m_map)
	errNoMsh, IDS_ERR_NO_MSH,
	errNoSeg, IDS_ERR_NO_SEGMENTS,
	errNoSegId, IDS_ERR_NO_SEG_ID,
	errInvalidString, IDS_ERR_INVALID_STRING,
	errInvalidTime, IDS_ERR_INVALID_TIME,
	errMandFieldMissing, IDS_ERR_MAND_FIELD_MISSING,
	errFieldTooLarge, IDS_ERR_FIELD_TOO_LARGE,
	errFieldItemMissing, 
END_RESULTMAP(CMsgVerifyResult, m_map)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CHL7MessageVerify::CHL7MessageVerify(CHL7Message& msg)
	{
	// first make sure there are segments in this msg
	CHL7Segment* pSeg = msg.GetSegment(0);

	if(pSeg == NULL)
		{
		SetResultCode(errNoSeg);
		return;
		}

	// make sure the first one is an MSH
	CString sTemp(pSeg->GetField(0));
	if(sTemp != "MSH")
		{
		SetResultCode(errNoMsh);
		return;
		}

	SetResultCode(0);
	return;
/*
	for(int i = 1;pSeg != NULL; i++)
		{
		if(! VerifySegment(pSeg))
			return;
		pSeg = msg.GetSegment(i);
		}

	SetResultCode(0);
*/
	}

//--------------------------------------------------------------------------------
bool CHL7MessageVerify::VerifySegment(const CHL7Segment* pSeg)
	{
	// make sure there is a segment name
	CString sSegName(pSeg->GetField(0));

	if(sSegName.IsEmpty())
		{
		SetResultCode(errNoSegId);
		return false;
		}

	// look up the seg id
	CHL7DBSegmentDef defSeg;

	const CHL7DBSegmentDefItem* pSegItem = defSeg.Find(sSegName);

	if(pSegItem == NULL)
		{
		SetResultCode(errNoSegId);
		return false;
		}

	// get a list of fields to check
	CHL7DBFieldDef defField(pSeg->GetMsgPtr()->GetVersion(), pSegItem->m_nSegId);

	int nCount = defField.GetRecordCount();

	SetResultCode(0);

	// first find the entry for this field
	for(int nFieldNo = 0; nFieldNo < nCount; nFieldNo++)
		{
		const CHL7DBFieldDefItem* pItem = defField.GetIndexedItem(nFieldNo);
		if(pItem == NULL)
			{
			SetResultCode(errFieldItemMissing);
			break;
			}

		// dont bother if we dont have to
		if(! pItem->m_bMandatory)
			continue;

		CString sTemp(pSeg->GetField(pItem->m_nFieldId));

		sTemp.TrimLeft();
		sTemp.TrimRight();

		if(sTemp.IsEmpty())
			{
			SetResultCode(errMandFieldMissing);
			break;
			}

		// check to make sure the field isnt larger
		// than it's supposed to be
		if(sTemp.GetLength() > pItem->m_nFieldLen)
			{
			SetResultCode(errFieldTooLarge);
			break;
			}

		bool bRv = true;
		int nErr = 0;

		switch(CHL7DataTypes::Find(pItem->m_pDatatype))
			{
			case CHL7DataTypes::DT_ST:
				bRv = IsStringOk(sTemp);
				nErr = errInvalidString;
				break;

			case CHL7DataTypes::DT_TS:
				bRv = VerifyDateTime(sTemp);
				nErr = errInvalidTime;
				break;
			}
// ok it works, lets ignore errors for now
// and get to work on putting msgs in the db

		if(! bRv)
			{
			SetResultCode(nErr);
			break;
			}
		}

	return ! HasErrors();
	}

// the following code is from the original HL7 app
// only the (function) names have been changed to protect the innocent
//--------------------------------------------------------------------------------
bool CHL7MessageVerify::IsTimeOk(LPCTSTR var)
{
  
  char temp_time[17];;
  int len, hours_present, mins_present, secs_present, counter, ignoring_spaces;
  char * pos, ch;
  
  if (!(*var))
    return (FALSE );

  /* copy var into temp_time, removing any trailing spaces, and
  returning error if an unacceptable char is encountered */
  counter=0;
  ignoring_spaces=0;
  pos=temp_time;
  ch=var[0];

  while(counter<13 && ch) /* since the input is not necessarily null terminated */
  {
    if (ch==' ')
      ignoring_spaces=1;
    else if (ignoring_spaces) /* non space found after a space char */
      return (FALSE ); 

    /* only allow a non digit if either we are ignoring spaces or
    this is the 6th position and the character is a '.' */
    if (!isdigit(ch))
    {
      if (counter==6 && ch=='.')
        *pos++=ch; /* copy the char in */
      else if (ch!=' ')
        return (FALSE );
    }
    else
      *pos++=ch; /*copy digit in */

    counter++;
    if (counter<13) /* to ensure we are not reading past the end of the string */
      ch=var[counter];
  }
  *pos='\0'; /* finally, null terminate pos */
      

  len=strlen(temp_time);

  /* determine which fields are present */
  hours_present=mins_present=secs_present=0;
  if (len==2)
    hours_present=1;
  else if (len==4)
    hours_present=mins_present=1;
  else if (len==6)
    hours_present=mins_present=secs_present=1;
  else if (len>6 && len<=13 && temp_time[6]=='.') 
  /* allow length to be more than 6 only if the fraction of a second extension is
    preceeded with a '.' */
    hours_present=mins_present=secs_present=1;
  else
    return (FALSE);

  /* Extract and range check each field, note that the
  fractions of a second field does not need checking as the
  check earlier to ensure it is all digits is sufficient.
  We can use strncmp since the characters in the fields are guaranteed to be
  digits if we have got this far.*/
  if (hours_present){
    if (strncmp(temp_time, "23", 2)>0)
      return (TRUE);
  }
  if (mins_present){
    if (strncmp(temp_time+2, "59", 2)>0)
      return (TRUE);
  }
  if (secs_present){
    if (strncmp(temp_time+4, "59", 2)>0)
      return (FALSE);
  }

  /* all tests passed */
  return (TRUE);
}


//--------------------------------------------------------------------------------
bool CHL7MessageVerify::IsDateOk(LPCTSTR var)
{
   
  int counter, day,mon,year;
  char day_s[3], mon_s[3], year_s[5];
  int month_length[]={31,29,31,30,31,30,31,31,30,31,30,31,31};

  if (!(*var))
    return (FALSE);

  /* check string contains 8 digits */
  for(counter=0; counter<8; counter++){
    if (!isdigit(var[counter]))
			return (FALSE ); 
  }

  /* range check the fields */
  strncpy(day_s, var+6, 2);
  day_s[2]='\0';
  strncpy(mon_s, var+4, 2);
  mon_s[2]='\0';
  strncpy(year_s, var, 4);
  year_s[4]='\0';

  day=atoi(day_s);
  mon=atoi(mon_s);
  year=atoi(year_s);

  if (mon<1 || mon>12)
    return (FALSE );

  if (day<1 || day>month_length[mon-1])
    return (FALSE);

  /* special case for 29th of feb */
  if (mon==2 && day==29 &&
    /* the year not a leap year */
    !(((year%4==0) && (year%100!=0)) || (year%400==0)) )
    return (FALSE);

  /* check year is something sensible... */
  if (year<1000 || year>3000)
    return (FALSE );

  /* all tests successfully passed */
  return (TRUE);
}

//--------------------------------------------------------------------------------
bool CHL7MessageVerify::SplitDateTime(char *output_date, char * output_time, LPCTSTR var)
{
   
  int len;
  char temp_datetime[25];
  char * pos, ch;

  /* copy the string to a temp. variable,
  null terminating at the first space, or null encountered*/
  len=0;
  ch=var[0];
  pos=temp_datetime;
  while (len<26 && ch && !isspace(ch))
  {
    *pos++=ch;
    /* if the end of the string has not been reached, copy the character in */
    len++;
    if (len<26)
      ch=var[len];
  }
  /* null terminate temp_datetime */
  *pos='\0';

  /* remove any time offset suffix by replacing the start of it by NULL */
  strtok(temp_datetime, "+-");

  /* copy the date and time parts into the supplied variables*/
  strncpy(output_date, temp_datetime, 8); /* YYYYMMDD */
  output_date[8]='\0';

  /* if the input date has any fields omitted, fill them with defaults
  since DA data type does not allow fields to be missed, but DT does */
  if (len==2) /* just century - first of january*/
    strcat(output_date, "000101");
  else if (len==4) /* just year, first of jan */
    strcat(output_date, "0101");
  else if (len==6) /* year and month, first of month */
    strcat(output_date, "01");
  else if (len==0)
    return  (FALSE);

  /* if the length is more than 8, copy in the date up to the
  time offset part (if present) - this is ignored at present */
  if (len>8){
    strcpy(output_time, temp_datetime+8); /* the rest....*/
  } else {
    strcpy(output_time, "00"); /* the minimum information that time can contain */
	}

  return (TRUE);
}


//--------------------------------------------------------------------------------
bool CHL7MessageVerify::VerifyDateTime(LPCTSTR var)
{
  char temp_time[21];
  char temp_date[8];
	 

  if (SplitDateTime(temp_date, temp_time, var)!=TRUE)
    return (FALSE);

  if (IsDateOk(temp_date)!=TRUE)
    return (FALSE);

  if (IsTimeOk(temp_time)!=TRUE)
    return (FALSE);

  /* TO DO: check ZZZZ here since this is ignored by the split date time fn */

  return (TRUE);
}

//--------------------------------------------------------------------------------
bool CHL7MessageVerify::IsSequenceOk(LPCTSTR var)
{
	int len;
	int counter;

	if (!(*var))
		return (FALSE);
	len =  strlen(var);
	/* check if it is a string */
	for(counter=0; counter < len; counter++){
		if (!isdigit(var[counter]))
			return (FALSE ); 
	}
	return (TRUE);
}	

//--------------------------------------------------------------------------------
bool CHL7MessageVerify::IsIdOk(LPCTSTR var)
{
	int len; 
	int counter;

	if (!(*var))
		return (FALSE);
	len = strlen(var);
	/* check if it is a string */
	for(counter=0; counter<len; counter++){
		if (!isalnum(var[counter]))
			return (FALSE ); 
	}
	return (TRUE);
}

//--------------------------------------------------------------------------------
bool CHL7MessageVerify::IsNumericOk(LPCTSTR var)
{
	int len ;
	int counter;

	if (!(*var))
    return (FALSE);
	len = strlen(var);
	 /* check if it is numeric */
  for(counter=0; counter<len; counter++){
	  if ((!isdigit(var[counter])) &&( var[counter] != '.') && ( var[counter] != '+') && ( var[counter] != '-') )
			return (FALSE ); 
  }
  return (TRUE);
}

//--------------------------------------------------------------------------------
bool CHL7MessageVerify::IsStringOk(LPCTSTR var)
{
	int len;
	int counter;

	if (!(*var))
    return (FALSE);
	len = strlen(var);

	 /* check if it is a string */
  for(counter=0; counter<len; counter++){
    if (var[counter] < 0x20 || var[counter] > 0x7e)
			return (FALSE ); 
  }
  return (TRUE);
}
