#include "FirmSoftReset.h"



uint32_t ulFSoftRest(void)
{
	uint32_t rResult;

	/*Clear RCU BCODE register HALT parameter to reissue reset*/
	rResult = adi_rcu_ClearBCodeReg(0,ADI_RCU_BCODE_REG_HALT);

	/* Unmask the reset control for system reset source */
	rResult = adi_rcu_ConfigureResetControl(0, true, true);

	/* Mask debugger from getting reset */
	rResult = adi_rcu_ControlDebugUnitReset(0, true);

	/*Issue System Reset*/
	adi_rcu_ControlSystemReset(0, true);

	return rResult;
}
