/*
 * pid.h
 *
 * Created: 24-Nov-18 18:19:01
 *  Author: ScorpionIPX
 */ 


#ifndef PID_H_
#define PID_H_

#define TICKS_PER_REVOLUTION 660
#define REFRESH_RATE_MS 163.84

void init_optocoupler(void);
void init_pid(void);
void fuzzy(void);
unsigned int defuzzy(unsigned int error);

int fuzzy_table[50];

#endif /* PID_H_ */