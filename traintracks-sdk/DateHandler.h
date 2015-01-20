//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2015 Traintracks.io All Rights Reserved.
//  
//  DateHandler.h
//  
//  Description: 
//  
//  
//__________________________________________________________________________________________________
#ifndef DATE_HANDLER_H 
#define DATE_HANDLER_H

#include <string>

//__________________________________________________________________________________________________
//  
//  Class: DateHandler
//  
//  Description: 
//  
//__________________________________________________________________________________________________
class DateHandler 
{
public:
  // constructors
  DateHandler();
  DateHandler(int d,int m,int y);

  void setYear(int y);
  void setMonth(int m);
  void setDay(int d);

  int getYear();
  int getMonth();
  int getDay();    
  void getShortMonth(std::string& val);
  void getFullMonth(std::string& val);		

  void addDays(int d);
  void addMonths(int m);
  void addYears(int y);		
  void lessDays(int d);
  void lessMonths(int m);
  void lessYears(int y);
  int differenceInDays(DateHandler d);
  int differenceInMonths(DateHandler d);
  int differenceInYears(DateHandler d);

  bool operator > (DateHandler d);
  bool operator < (DateHandler d);
  bool operator == (DateHandler d);
  bool operator != (DateHandler d);
  void operator = (DateHandler d);

  bool isLeapYear();

protected:
  void validate();
  int getDaysInMonth();
  void correctDays();
  void restore();	

protected:
  int m_nYear;
  int m_nMonth;
  int m_nDay; 

  int m_nOldYear;
  int m_nOldMonth;
  int m_nOldDay;

};

#endif  // DATE_HANDLER_H
