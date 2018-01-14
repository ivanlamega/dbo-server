#include "AuthNetwork.h"
#include <NtlPacketCU.h>

int main(int argc, char* argv[])
{
	Logger::Log("Tam pcSumary: %i, chara_info %i, delinfo %i", sizeof sPC_SUMMARY, sizeof sCU_CHARACTER_INFO,
		sizeof sDELETE_WAIT_CHARACTER_INFO);

	AuthServer app;

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

