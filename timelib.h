#ifndef TIMELIB_H_INCLUDED
#define TIMELIB_H_INCLUDED

#define ANULHRISTOS 1970
#define ANTIHRISTUL 1000

#define HOURSINDAY 24
#define SECSINHOUR 3600
#define DAYSINYEAR 365
#define SECSORMINS 60

#define FEBSHORT 28
#define FEBLONG 29
#define SHORTMONTH 30
#define LONGMONTH 31

#define BITNUM 32

#define HUNNID 100
#define FOHUNNID 400

#define DEC 12
#define NOV 11
#define OCT 10
#define SEPT 9
#define AUG 8
#define JUL 7
#define JUN 6
#define MAI 5
#define APR 4
#define MAR 3
#define FEB 2
#define IAN 1

#define TREISPE 13
#define PAISPE 14
#define CINSPE 15
#define SAISTREI 63

#define POWER_ZERO 1
#define POWER_I 2
#define POWER_II 4
#define POWER_III 8
#define POWER_IV 16
#define POWER_V 32


typedef struct {
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
} TTime;

typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned int year;
} TDate;

typedef struct {
    char name[MAI];
    signed char utc_hour_difference;
} TTimezone;

typedef struct {
    TDate date;
    TTime time;
    TTimezone *tz;
} TDateTimeTZ;

TTime convertUnixTimestampToTime(unsigned int timestamp);
TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp);
TDate convertUnixTimestampToDate(unsigned int timestamp);
TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index);
unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ);
void printDateTimeTZ(TDateTimeTZ datetimetz);

#endif  // TIMELIB_H_INCLUDED
