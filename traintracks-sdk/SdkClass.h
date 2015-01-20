//__________________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2015 Traintracks.io All Rights Reserved.
//  
//  SdkClass.h
//  Header file for Traintracks.io C++ SDK
//  
//__________________________________________________________________________________________________

#pragma once
#include "server_types.h"

#include <string>

#include <cpprest/http_client.h>
#include <cpprest/json.h>
//#include <cpprest/filestream.h>
#include <pplx/pplxtasks.h>
#include <vector>

//#include "DataField.h"

using namespace utility;
using namespace web;
using namespace pplx;

namespace Traintracks
{
	//
	// Trivial class o hold multiple pieces of data for a event submission
	//
class DataField
{
public:
	DataField(){}
	~DataField(){}
	DataField(const DataField& data) :
		EventName(data.EventName),
		ClientTimestamp(data.ClientTimestamp),
		EventData(data.EventData)
	{
	}

	DataField& operator=(const DataField& data)
	{
		EventName = data.EventName;
		ClientTimestamp = data.ClientTimestamp;
		EventData = data.EventData;
		return *this;
	}

	DataField(const string_t& cEventName, const string_t& cClientTimestamp, const string_t& cDataField) :
		EventName(cEventName),
		ClientTimestamp(cClientTimestamp),
		EventData(cDataField)
	{
	}

public:
	string_t EventName;
	string_t ClientTimestamp;
	string_t EventData;
};
typedef std::vector<DataField> DataFieldsList;


// The main class of the Traintracks.io C++ Event submission class
class SdkClass
{
public:
	SdkClass();
	~SdkClass() {}

	// Methods used to initialize the Traintracks.io class
	static void SetTraintracksURL(const string_t& cTraintracksURL) { SdkClass::TraintracksURL = cTraintracksURL; }

	// Fill in required data fields
	static void SetEventTimestamp(const string_t& cEventTimestamp) { SdkClass::EventTimestamp = cEventTimestamp; }
	static void SetEventName(const string_t& cEventName) { SdkClass::EventName = cEventName; }
	static void SetProductKey(const string_t& cProductKey) { SdkClass::ProductKey = cProductKey; }
	static void SetSecretProductKey(const string_t& cSecretProductKey) { SdkClass::SecretProductKey = cSecretProductKey; }
	static void SetSessionId(const string_t& cSessionId) { SdkClass::SessionId = cSessionId; }
	static void SetAppVersion(const string_t& cAppVersion) { SdkClass::AppVersion = cAppVersion; }
	static void SetDevice(const string_t& cDevice) { SdkClass::Device = cDevice; }
	static void SetUserId(const string_t& cUserId) { SdkClass::UserId = cUserId; }
	static void SetUserName(const string_t& cUserName) { SdkClass::UserName = cUserName; }
	static void SetBlockSize(int count) { SdkClass::BlockSize = count; }

	//Called to send n event to Traintracks
	int PostEvent(const string_t& cEventName, const string_t& cClientTimestamp, const string_t& cDataField);
	// Called to shutdown the EEvent submission process
	int EndPost();

protected:
	// Mthod that performs the actual sending of batched events to Traintracks
	int SendBlock();


protected:
	static string_t EventTimestamp;
	static string_t EventName;
	static string_t ProductKey;
	static string_t SessionId;	
	static string_t AppVersion;
	static string_t Device;
	static string_t UserId;
	static string_t UserName;
	static string_t SecretProductKey;

	static string_t TraintracksURL;

	static json::value baseJSON;
	static json::value bundleJSON;
	static int BlockSize;
	static int ItemErrors;

	static DataFieldsList TheDataFieldsList;

};
}


