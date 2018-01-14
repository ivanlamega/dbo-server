#include <stdio.h>
#include <stdlib.h>
#include "GameNetwork.h"

int main(int argc, char* argv[])
{
	Logger::Log("sGU_AVATAR_CHAR_INFO %i, sPC_PROFILE %i, sCHARSTATE %i", sizeof sGU_AVATAR_CHAR_INFO,
		sizeof sPC_PROFILE, sizeof sCHARSTATE);

	GameServer app;

	int rc = app.Create(argc, argv);
	if (0 != rc)
	{
		Logger::Log("Server Application Create Fail %d\n", rc);
		return rc;
	}

	app.Start();
	app.WaitForTerminate();
	return 0;
}