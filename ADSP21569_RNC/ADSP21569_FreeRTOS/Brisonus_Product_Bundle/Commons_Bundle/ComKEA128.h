/*
 * ComKEA128.h
 *
 *  Created on: Jun 18, 2020
 *      Author: vince
 */

#ifndef _COMKEA128_H_
#define _COMKEA128_H_

#define KEA128_BYTE_START1			(0x25)
#define KEA128_BYTE_START2			(0x09)

#define KEA128_SPI_LEN_OUT			(10u)
#define KEA128_INDEX_START			(0u)
#define KEA128_INDEX_RPM			(1u)
#define KEA128_INDEX_SPD			(3u)
#define KEA128_INDEX_TBD1			(5u)
#define KEA128_INDEX_TBD2			(7u)
#define KEA128_INDEX_CKSUM			(9u)


typedef enum{

	NO_ERROR,TRIED_BUT_ERROR

}KEA128_COM_ERROR;

typedef struct{
	uint8_t data[KEA128_SPI_LEN_OUT];
	uint8_t index;
}KEA128_SPI_DATA;


#endif /* BRISONUS_PRODUCT_BUNDLE_COMMONS_BUNDLE_COMKEA128_H_ */
