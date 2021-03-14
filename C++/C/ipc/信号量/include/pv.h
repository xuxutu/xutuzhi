#ifndef __PV_H__
#define __PV_H__


//初始化semnums个信号灯的值(value)
extern int I(int semnums, int value);

//一般为减一操作
extern void P(int semid, int semnum, int value);

//一般为加一操作
extern void V(int semid, int semnum, int value);

//销毁信号量集
extern void D(int semid);


#endif
