#ifndef _SESSIONFACTORY
#define _SESSIONFACTORY

#pragma once

class Session;

class SessionFactory
{
public:
	virtual Session* CreateSession(unsigned short sessionType) = 0;
};

#endif