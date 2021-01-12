#ifndef __TIMERX_H
#define __TIMERX_H
#include "system.h"
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_Change(u16 arr);
void TIM3_Set(u8 sta);
void TIM4_Int_Init(u16 arr,u16 psc);

#endif
