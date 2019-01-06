#if !defined(_BUZZER_H_)
#define _BUZZER_H_

#include "sys.h"

#define buzzer PCout(15)

void Buzzer_Init(void);
void Buzzer_Set(uint8_t sta);


#endif // _BUZZER_H_
