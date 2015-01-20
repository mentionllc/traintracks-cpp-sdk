//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2015 Traintracks.io All Rights Reserved.
//  
//  DateHandler.cpp
//  
//  Description: 
//  
//  
//__________________________________________________________________________________________________

#include "stdafx.h"
#include <string>
#include <ctime>
#include <vector>
#include "DateHandler.h"

//________________________________________________
//  
//________________________________________________
using namespace std;

//__________________________________________________________________________________________________
//  Function: DateHandler::DateHandler
//  
//  Description: 
//__________________________________________________________________________________________________
DateHandler::DateHandler() 
{
  time_t now;
  time(&now); 
  struct tm *l_time;			 
  l_time = localtime(&now);	
  m_nYear = l_time->tm_year + 1900;
  m_nMonth = l_time->tm_mon + 1;
  m_nDay = l_time->tm_mday;
  m_nOldYear = m_nYear;
  m_nOldMonth = m_nMonth;
  m_nOldDay = m_nDay;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::DateHandler
//  
//  Description: 
//__________________________________________________________________________________________________
DateHandler::DateHandler(int day,int month,int year) 
{
  DateHandler dt;
  m_nOldYear = dt.m_nYear;
  m_nOldMonth = dt.m_nMonth;
  m_nOldDay = dt.m_nDay;
  m_nYear = year;
  m_nMonth = month;
  m_nDay = day;
  validate();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::setYear
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::setYear(int year) 
{
  m_nOldYear = m_nYear;
  m_nYear = year;
  validate();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::setDay
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::setDay(int day) 
{
  m_nOldDay = m_nDay;
  m_nDay = day;
  validate();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::setMonth
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::setMonth(int month) 
{
  m_nOldMonth = m_nMonth;
  m_nMonth = month;
  validate();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::getYear
//  
//  Description: 
//__________________________________________________________________________________________________
int DateHandler::getYear() 
{
  return m_nYear;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::getMonth
//  
//  Description: 
//__________________________________________________________________________________________________
int DateHandler::getMonth() 
{
  return m_nMonth;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::getShortMonth
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::getShortMonth(std::string& val) 
{
  if(m_nMonth == 1) 
  {
    val = "JAN";
  }
  else if(m_nMonth == 2) 
  {
    val = "FEB";
  }
  else if(m_nMonth == 3) 
  {
    val = "MAR";
  }
  else if(m_nMonth == 4) 
  {
    val = "APR";
  }
  else if(m_nMonth == 5) 
  {
    val = "MAY";
  }
  else if(m_nMonth == 6) 
  {
    val = "JUN";
  }
  else if(m_nMonth == 7) 
  {
    val = "JUL";
  }
  else if(m_nMonth == 8) 
  {
    val = "AUG";
  }
  else if(m_nMonth == 9) 
  {
    val = "SEP";
  }
  else if(m_nMonth == 10) 
  {
    val = "OCT";
  }
  else if(m_nMonth == 11) 
  {
    val = "NOV";
  }
  else if(m_nMonth == 12) 
  {
    val = "DEC";
  }
  else
  {
    val = "ERROR";
  }
}

//__________________________________________________________________________________________________
//  Function: DateHandler::getFullMonth
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::getFullMonth(std::string& val) {
  if(m_nMonth == 1) 
  {
    val = "January";
  }
  else if(m_nMonth == 2) 
  {
    val = "February";
  }
  else if(m_nMonth == 3) 
  {
    val = "March";
  }
  else if(m_nMonth == 4) 
  {
    val = "April";
  }
  else if(m_nMonth == 5) 
  {
    val = "May";
  }
  else if(m_nMonth == 6) 
  {
    val = "June";
  }
  else if(m_nMonth == 7) 
  {
    val = "July";
  }
  else if(m_nMonth == 8) 
  {
    val = "August";
  }
  else if(m_nMonth == 9) 
  {
    val = "September";
  }
  else if(m_nMonth == 10) 
  {
    val = "October";
  }
  else if(m_nMonth == 11) 
  {
    val = "November";
  }
  else if(m_nMonth == 12) 
  {
    val = "December";
  }	
}

//__________________________________________________________________________________________________
//  Function: DateHandler::getDay
//  
//  Description: 
//__________________________________________________________________________________________________
int DateHandler::getDay() 
{
  return m_nDay;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::addDays
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::addDays(int days) 
{
  m_nOldDay = m_nDay;    
  for(int i=0;i<days;i++) 
  {	
    m_nDay++;
    int d = getDaysInMonth();
    if(m_nDay > d) 
    {
      m_nDay = 1;
      m_nMonth++;
      if(m_nMonth > 12) 
      {
        m_nMonth = 1;
        m_nYear++;			
      }
    }	
  }
  correctDays();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::addMonths
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::addMonths(int mons) 
{
  m_nOldMonth = m_nMonth;
  for(int i=0;i<mons;i++) 
  {
    if(m_nMonth > 12) 
    {
      m_nMonth = 1;
      m_nYear++;				
    }
    m_nMonth++;
  }
  correctDays();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::addYears
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::addYears(int yrs) 
{
  m_nOldYear = m_nYear;
  m_nYear += yrs;	
  correctDays();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::lessDays
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::lessDays(int days) 
{
  m_nOldDay = m_nDay;
  for(int i=0;i<days;i++) 
  {
    m_nDay--;
    if(m_nDay <= 0) 
    {
      m_nMonth--;
      if(m_nMonth <= 0) 
      {
        m_nMonth = 12;
        m_nYear--;
      }
      int d = getDaysInMonth();
      m_nDay = d;
    }
  }
  correctDays();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::lessMonths
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::lessMonths(int mons) 
{
  m_nOldMonth = m_nMonth;
  for(int i=0;i<mons;i++) 
  {
    if(m_nMonth <= 0) 
    {
      m_nMonth = 12;
      m_nYear--;				
    }
    m_nMonth--;
  }
  correctDays();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::lessYears
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::lessYears(int yrs) 
{
  m_nOldYear = m_nYear;	
  m_nYear -= yrs;
  if(m_nYear < 0) m_nYear *= -1;
  correctDays();
  validate();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::differenceInDays
//  
//  Description: 
//__________________________________________________________________________________________________
int DateHandler::differenceInDays(DateHandler date) 
{
  int diff = 0;  
  DateHandler tmp_this = *this;
  DateHandler tmp_date = date;
  if(*this > date) 
  {	
    while(true) 
    {
      if(*this == tmp_date) 
        break;
      tmp_date.addDays(1);
      diff++;
    }
  }
  else if(*this < date)
  {
    while(true) 
    {
      if(tmp_this == tmp_date) 
        break;
      tmp_this.addDays(1);
      diff++;
    }
  }
  else 
  {
    diff = 0;
  }
  return diff;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::differenceInMonths
//  
//  Description: 
//__________________________________________________________________________________________________
int DateHandler::differenceInMonths(DateHandler date) 
{
  int diff = 0;    
  DateHandler tmp_this = *this;
  DateHandler tmp_date = date;
  if(*this > date) 
  {	
    while(true) 
    {
      if(tmp_this.m_nYear == tmp_date.m_nYear) 
      {
        if(tmp_this.m_nMonth == tmp_date.m_nMonth) break;
      }
      tmp_date.addMonths(1);
      diff++;
    }    
  }
  else if(*this < date) 
  {	
    while(true) 
    {
      if(tmp_this.m_nYear == tmp_date.m_nYear) 
      {
        if(tmp_this.m_nMonth == tmp_date.m_nMonth) break;
      }
      tmp_this.addMonths(1);
      diff++;
    }   
  }
  else 
  {
    diff = 0;
  }
  return diff;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::differenceInYears
//  
//  Description: 
//__________________________________________________________________________________________________
int DateHandler::differenceInYears(DateHandler date) 
{
  int ret = m_nYear - date.getYear();
  if(ret < 0) ret *= -1;
  return ret;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::operator>
//  
//  Description: 
//__________________________________________________________________________________________________
bool DateHandler::operator>(DateHandler date) 
{
  if(date.getYear() < m_nYear) 
  {
    return true;
  }
  else if(date.getYear() == m_nYear) 
  {
    if(date.getMonth() < m_nMonth) 
    {
      return true;
    }
    else if(date.getMonth() == getMonth()) 
    {
      if(date.getDay() < m_nDay) 
      {
        return true;
      }
      else 
      {
        return false;
      }
    }
    else 
    {
      return false;
    }
  
  }
  else 
  {
    return false;
  }
  return false;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::operator<
//  
//  Description: 
//__________________________________________________________________________________________________
bool DateHandler::operator<(DateHandler date) 
{
  if(date.getYear() > m_nYear) 
  {
    return true;
  }else if(date.getYear() == m_nYear) 
  {
    if(date.getMonth() > m_nMonth) 
    {
      return true;
    }
    else if(date.getMonth() == getMonth()) 
    {
      if(date.getDay() > m_nDay) 
      {
        return true;
      }
      else 
      {
        return false;
      }
    }
    else 
    {
      return false;
    }
  }
  else 
  {
    return false;
  }
  return false;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::operator==
//  
//  Description: 
//__________________________________________________________________________________________________
bool DateHandler::operator==(DateHandler date) 
{
  if(date.getYear() == getYear() && date.getMonth() == getMonth() &&
    date.getDay() == getDay()) 
  {
      return true;
  }
  return false;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::operator!=
//  
//  Description: 
//__________________________________________________________________________________________________
bool DateHandler::operator!=(DateHandler date) 
{
  if(date.getYear() != getYear() || date.getMonth() != getMonth() ||
    date.getDay() != getDay()) 
  {
      return true;
  }
  return false;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::operator=
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::operator=(DateHandler date) 
{
  m_nOldYear = m_nYear;
  m_nOldMonth = m_nMonth;
  m_nOldDay = m_nDay;
  m_nYear = date.getYear();
  m_nMonth = date.getMonth();
  m_nDay = date.getDay();
  validate();
}

//__________________________________________________________________________________________________
//  Function: DateHandler::validate
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::validate() {    
  if(m_nYear <= 0) 
  {		
    //sprintf(buff,"Not a valid year: %d",m_nYear);
    restore();
  }
  if(m_nMonth < 1 || m_nMonth > 12) 
  {		
    //sprintf(buff,"Not a valid month: %d",m_nMonth);
    restore();
  }
  if(m_nDay < 1 || m_nDay > 31) 
  {			
    //sprintf(buff,"Not a valid day: %d",m_nDay);
    restore();
  }
  if(m_nDay > getDaysInMonth()) 
  {			
    //sprintf(buff,"Not a valid day: %d",m_nDay);
    restore();
  }    
}

//__________________________________________________________________________________________________
//  Function: DateHandler::getDaysInMonth
//  
//  Description: 
//__________________________________________________________________________________________________
int DateHandler::getDaysInMonth() 
{
  if(m_nMonth == 1) 
  {
    return 31;
  }
  else if(m_nMonth == 2) 
  {
    if(isLeapYear()) 
      return 29;
    else 
      return 28;		
  }
  else if(m_nMonth == 3) 
  {
    return 31;
  }
  else if(m_nMonth == 4) 
  {
    return 30;
  }
  else if(m_nMonth == 5) 
  {
    return 31;
  }
  else if(m_nMonth == 6) 
  {
    return 30;
  }
  else if(m_nMonth == 7) 
  {
    return 31;
  }
  else if(m_nMonth == 8) 
  {
    return 31;
  }
  else if(m_nMonth == 9) 
  {
    return 30;
  }
  else if(m_nMonth == 10) 
  {
    return 31;
  }
  else if(m_nMonth == 11) 
  {
    return 30;
  }
  else if(m_nMonth == 12) 
  {
    return 31;
  }
  return -1;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::isLeapYear
//  
//  Description: 
//__________________________________________________________________________________________________
bool DateHandler::isLeapYear() 
{
  bool ret = true;
  if(m_nYear <= 1752) 
  {
    if(m_nYear % 4 == 0) 
      return true;
  }
  else 
  {
    if((m_nYear % 4 == 0 && m_nYear % 100 != 0)
      || m_nYear % 400 == 0) 
    {
        return true;
    }
  }
  return false;
}

//__________________________________________________________________________________________________
//  Function: DateHandler::correctDays
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::correctDays() 
{
  int days = getDaysInMonth();
  if(m_nDay > days) 
  {
    m_nDay = days;
  }
}

//__________________________________________________________________________________________________
//  Function: DateHandler::restore
//  
//  Description: 
//__________________________________________________________________________________________________
void DateHandler::restore() 
{
  m_nYear = m_nOldYear;
  m_nMonth = m_nOldMonth;
  m_nDay = m_nOldDay;
}


