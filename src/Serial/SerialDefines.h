#pragma once

//. 사용 안할듯... 그냥 코드에서 보는게 낫다
enum ASCII_CODE {

	eNONE = 0x00
	, eSTX = 0x02
	, eETX = 0x03
	, eCR = 0x0D // byte containing the carriage return character
	, eLF = 0x0A

	, eDEVICE_CODE_X = 0x58 // 'X'
	, eDEVICE_CODE_x = 0x78 // 'x'


	, eESC = 0x1B  //Byte containing the escape character
	, eACK = 0x06 // start symbol for success
	, eNAK = 0x15  // start symbol for failure
	
	, eL = 0x4C  
	, eP = 0x50

};
