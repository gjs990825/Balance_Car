#if !defined(_TASK_H_)
#define _TASK_H_

#include "sys.h"

void Detect_Distance(void);
void Send_ToRemote(void);
void Report_ToSerial(void);

extern uint16_t currentDistance;


#endif // _TASK_H_
