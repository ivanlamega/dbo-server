#ifndef _NETWORKPROCESSOR
#define _NETWORKPROCESSOR

#pragma once

#include "Base.h"
#include "Thread.h"

class Network;
class Session;

class NetworkProcessor : public RunObject
{
public:
	NetworkProcessor(Network* pNetwork);
	~NetworkProcessor();

	int Create();
	void Destroy();
	int PostNetEvent(WPARAM wParam, LPARAM lParam);
	void Run();
	void Close();

private:
	HANDLE _hEventIOCP;
};

#endif