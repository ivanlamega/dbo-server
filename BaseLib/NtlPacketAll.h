#pragma once

#include "NtlPacketAU.h"
#include "NtlPacketCU.h"
#include "NtlPacketGU.h"
#include "NtlPacketTU.h"
#include "NtlPacketUA.h"
#include "NtlPacketUC.h"
#include "NtlPacketUG.h"
#include "NtlPacketUT.h"

/*
	NOTICE :

	1) Naming Op-code

		To distinguish type of a packet from other types, we use some alphabet letters in op-codes.
			U - User(Game Client)
			A - Auth Server
			C - Character Server
			G - Game Server
			T - Community Server

		For example, if we design a packet that is sent from a game server to a query server, its op-code must start with "GQ".

	2) Ranges that Op-code values belong to.

		SYSTEM series packets' Op code should be in [0, 99].

		UA series packets' Op code should be in [100, 999].
		AU series packets' Op code should be in [1000, 1999].

		UC series packets' Op code should be in [2000, 2999].
		CU series packets' Op code should be in [3000, 3999].

		UG series packets' Op code should be in [4000, 4999].
		GU series packets' Op code should be in [5000, 5999].

		UT series packets' Op code should be in [6000, 6999].
		TU series packets' Op code should be in [7000, 7999].
*/
