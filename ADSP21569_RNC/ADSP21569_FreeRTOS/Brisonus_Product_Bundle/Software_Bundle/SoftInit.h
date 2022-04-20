#ifndef BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_SOFTINIT_H_
#define BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_SOFTINIT_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <assert.h>

#include "SoftDefines.h"

#include "../Firmware_Bundle/FirmSPU.h"
#include "../Firmware_Bundle/FirmSRU.h"
#include "../Firmware_Bundle/FirmTMR.h"
#include "../Firmware_Bundle/FirmUART.h"
#include "../Firmware_Bundle/FirmGPIO.h"
#include "../Firmware_Bundle/FirmSPORT.h"
#include "../Firmware_Bundle/FirmPWRConfig.h"
#include "../Firmware_Bundle/FirmFlashdriver.h"
#include "a2bapp.h"


void vsInit(void *pvParameters);

#endif /* BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_SOFTINIT_H_ */
