
#include "tokenWatcher.h"

long monitorSlotEvent()
{
	CK_RV rv = CKR_OK;

	while (1) {

		long sn = -1;
		CK_SLOT_ID slot;

		rv = functionList->C_WaitForSlotEvent(0, &slot, NULL_PTR);

		if (CKR_CRYPTOKI_NOT_INITIALIZED == rv)
			break;
		if (rv != CKR_OK)
			break;

		CK_SLOT_INFO slotInfo;
		rv = functionList->C_GetSlotInfo(slot, &slotInfo);

		if (CKR_CRYPTOKI_NOT_INITIALIZED == rv)
			break;
		if (rv != CKR_OK)
			break;

		if (CKF_TOKEN_PRESENT & slotInfo.flags) {
			CK_TOKEN_INFO tokenInfo;
			CK_RV rv = CKR_OK;

			rv = functionList->C_GetTokenInfo(slot, &tokenInfo);

			if (rv != CKR_OK) {
				break;
			}

			sn = strtol(tokenInfo.serialNumber, NULL, (int)sizeof(tokenInfo.serialNumber));
			return sn;
		}
	}
	return -1;
}