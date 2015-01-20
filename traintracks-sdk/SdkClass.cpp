//___________________________________________________________________________________________
//  Traintracks.io CONFIDENTIAL: Copyright (C) 2015 Traintracks.io All Rights Reserved.
//  
//  SdkClass.cpp
//  
//  
//__________________________________________________________________________________________________
#include "stdafx.h"

#include "SdkClass.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

#include <string>
#include <sstream>
//#include <iostream>

#include "StringHelpers.h"

using namespace web::http;
using namespace web::http::client;
using namespace web;
using namespace concurrency::streams;
using namespace Traintracks;
//using namespace std;

const string_t textEventTimestamp = L"clientTimestamp";
const string_t textEventName = L"eventType";
const string_t textProductKey = L"productKey";
const string_t textSessionId = L"sessionId";	
const string_t textAppVersion = L"build";
const string_t textDevice = L"device";
const string_t textUserId = L"userId";
const string_t textUserName = L"userName";

string_t SdkClass::EventTimestamp;
string_t SdkClass::EventName;
string_t SdkClass::ProductKey;
string_t SdkClass::SessionId;	
string_t SdkClass::AppVersion;
string_t SdkClass::Device;
string_t SdkClass::UserId;
string_t SdkClass::UserName;
string_t SdkClass::SecretProductKey;

DataFieldsList SdkClass::TheDataFieldsList;

string_t SdkClass::TraintracksURL = L"http://localhost:8080";

json::value SdkClass::baseJSON;
json::value SdkClass::bundleJSON;
int SdkClass::BlockSize = 50;
int SdkClass::ItemErrors = 0;

// external functions
extern void md5(unsigned char *initial_msg, size_t initial_len);
extern utility::string_t HexDigest();


SdkClass::SdkClass()
{
}

int SdkClass::PostEvent(const string_t& cEventName, const string_t& cClientTimestamp, const string_t& cDataField)
{
	DataField mydata(cEventName, cClientTimestamp, cDataField);

	// test that the submitted json is value
	try {
		json::value testObj7 = json::value::parse(cDataField);
	}
	catch(json::json_exception& e)
	{
		// invalid json
		return -1;
	}

	// save the event
	SdkClass::TheDataFieldsList.push_back(mydata);

	// Batchs full?
	if(SdkClass::TheDataFieldsList.size() >= SdkClass::BlockSize)
	{
		// ==YES Send the batch
		SendBlock();
		try {
			// clear the buffer
			SdkClass::TheDataFieldsList.clear();
		}
		catch(...)
		{
			std::wcout << L"EXCEPTION on SdkClass::TheDataFieldsList.clear();";
			return -1;
		}
	}
	return 0;
}

// used to close down the system and insure everything iis sent
int SdkClass::EndPost()
{
	while(SdkClass::TheDataFieldsList.size() >= 0)
	{
		SendBlock();
		SdkClass::TheDataFieldsList.clear();
	}
	return 0;
}

// add sending code here
pplx::task<void> HTTPRequestCustomHeadersAsync(const string_t& jsonText, const string_t& hashOut, const string_t& cTraintracksURL, const string_t& cProductKey, const json::value block )
{
	http_client client(cTraintracksURL);

	// Manually build up an HTTP request with header and request URI.
	http_request request(methods::POST);
	string_t length;
	try {
		// get the length of the json
		length = StringHelpers::BuildTString(std::to_string( jsonText.length() ));
	}
	catch(...)
	{
		std::wcout << L"EXCEPTION on length conversion";
	}

	// make the headers
	request.headers().add(L"Content-Length", length);
	request.headers().add(L"Content-Type", L"application/json");
	request.headers().add(L"X-Product-Key", cProductKey);
	request.headers().add(L"X-Product-Auth", hashOut);

	try {
		// set json into the body
		request.set_body(block);
	}
	catch(...)
	{
		std::wcout << L"EXCEPTION on set_body";
	}

	// submit the request async
	return client.request(request).then([](task<http_response> responseTask)
	{
		try
		{
			// Get the response out of the response task.
			// This will throw an exception if an error occurred, for example if the connection was lost.
			http_response response = responseTask.get();
			std::wostringstream ss;
			ss << L"Server returned returned status code " << response.status_code() << L"." << std::endl;
			std::wcout << ss.str();
		} catch(const http_exception &e)
		{
			// inspect message and error code here and handle error
			std::cout << e.what();
		}
	});

	/* Sample output:
	Server returned returned status code 200.
	*/
}


int SdkClass::SendBlock()
{
	const string_t formatStr1 = L"{\"build\": \"";
	const string_t formatStr2 = L"\",\"clientTimestamp\": \"";
	const string_t formatStr3 = L"\", \"data\": ";
	const string_t formatStr4 = L" , \"device\": \"";
	const string_t formatStr5 = L"\", \"eventType\": \"";
	const string_t formatStr6 = L"\", \"playerId\": \"";
	const string_t formatStr7 = L"\", \"playerName\": \"";
	const string_t formatStr8 = L"\", \"sessionId\": \"";
	const string_t formatStr9 = L"\" }";

	int batch_count = 0;
	if(!SdkClass::TheDataFieldsList.empty())
	{
		json::value block;
		int idx = 0;
		DataFieldsList::iterator cit = SdkClass::TheDataFieldsList.begin();
		while(cit != SdkClass::TheDataFieldsList.end())
		{
			DataField& tmp = *cit;

			// Build the json string
			string_t cMessage = formatStr1;
			cMessage += AppVersion;
			cMessage += formatStr2;
			cMessage += tmp.ClientTimestamp;
			cMessage += formatStr3;
			cMessage += tmp.EventData;
			cMessage += formatStr4;
			cMessage += Device;
			cMessage += formatStr5;
			cMessage += tmp.EventName;
			cMessage += formatStr6;
			cMessage += UserId;
			cMessage += formatStr7;
			cMessage += UserName;
			cMessage += formatStr8;
			cMessage += SessionId;
			cMessage += formatStr9;

			try {
				json::value testObj7 = json::value::parse(cMessage);
				block[idx++] = testObj7;
				// only count successful items
				++batch_count;
			}
			catch(json::json_exception& e)
			{
				std::cout << "Exception " << e.what();
				// just skip this item
				++SdkClass::ItemErrors;
			}

			++cit;
		}
		string_t jsonU8Text;
		jsonU8Text = block.serialize();

		// convert text to ascii
		const std::string jsonText = StringHelpers::BuildString( jsonU8Text );
		const std::string secretKey = StringHelpers::BuildString( SecretProductKey );
		const std::string jsonSaltedText = jsonText + secretKey;

		// calculate the md5 fof the batch
		md5((uint8_t *)jsonSaltedText.c_str(), (size_t)jsonSaltedText.size());
		string_t hashOut = HexDigest();

		try {
			// send the batch async
			HTTPRequestCustomHeadersAsync(jsonU8Text, hashOut, TraintracksURL, ProductKey, block );
		}
		catch(...)
		{
			std::wcout << L"EXCEPTION on task call";
		}

	 }

	return batch_count;
}

