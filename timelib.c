#include <stdio.h>
#include <stdlib.h>
#include "timelib.h"


TTime convertUnixTimestampToTime(unsigned int timestamp) {
    TTime result;
    unsigned int backup = timestamp % (SECSINHOUR * HOURSINDAY);
    result.hour = backup / SECSINHOUR;
    result.min = (backup % SECSINHOUR) / SECSORMINS;
    result.sec = backup % SECSORMINS;
    return result;
}


TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp) {
    TDate result;
    result.year = timestamp / (SECSINHOUR * HOURSINDAY * DAYSINYEAR) + ANULHRISTOS;
    unsigned int currentDay = (timestamp / (SECSINHOUR * HOURSINDAY)) % DAYSINYEAR;
    result.month = DEC;
    for (unsigned int cMth = 1; cMth < DEC; cMth ++) {
        if (cMth == IAN || cMth == MAR || cMth == MAI || cMth == JUL || cMth == AUG || cMth == OCT) {
            if (currentDay < LONGMONTH + 1) {
                result.month = cMth;
                break;
            }
            currentDay = currentDay - LONGMONTH;
        }
        if (cMth == APR || cMth == JUN || cMth == SEPT || cMth == NOV) {
            if (currentDay < LONGMONTH) {
                result.month = cMth;
                break;
            }
            currentDay = currentDay - SHORTMONTH;
        }
        if (cMth == FEB) {
            if (currentDay < FEBLONG) {
                result.month = cMth;
                break;
            }
            currentDay = currentDay - FEBSHORT;
        }
    }
    result.day = currentDay + 1;
    return result;
}

TDate convertUnixTimestampToDate(unsigned int timestamp) {
    TDate result;
    unsigned int backup = timestamp / (SECSINHOUR * HOURSINDAY);
    // printf("%u\n", backup);
    unsigned int cYear = ANULHRISTOS;
    unsigned int cMth = 1;
    unsigned int dayBackup = 0;
    //
    while (dayBackup == 0) {
        // short month detract
        if (cMth == APR || cMth == JUN || cMth == SEPT || cMth == NOV) {
            if (backup > SHORTMONTH) {
                backup = backup - SHORTMONTH;
                cMth = cMth + 1;
            } else {
                dayBackup = backup;
                break;
            }
        }
        // long month detract
        if (cMth == IAN || cMth == MAR || cMth == MAI || cMth == JUL || cMth == AUG || cMth == OCT) {
            if (backup > LONGMONTH) {
                backup = backup - LONGMONTH;
                cMth = cMth + 1;
            } else {
                dayBackup = backup;
                break;
            }
        }
            // last month detract
            if (cMth == DEC) {
                if (backup > LONGMONTH) {
                    backup = backup - LONGMONTH;
                    cMth = 1;
                    cYear = cYear + 1;
                } else {
                    dayBackup = backup;
                    break;
                }
            }
        if (cMth == FEB) {
            // feb detract
            if (cYear % APR == 0) {
                if (cYear % HUNNID == 0) {
                    if (cYear % FOHUNNID == 0) {
                        if (backup > FEBLONG) {
                            backup = backup - FEBLONG;
                            cMth = cMth + 1;
                        } else {
                            dayBackup = backup;
                            break;
                        }
                    } else {
                        if (backup > FEBSHORT) {
                            backup = backup - FEBSHORT;
                            cMth = cMth + 1;
                        } else {
                            dayBackup = backup;
                            break;
                        }
                    }
                } else {
                    if (backup > FEBLONG) {
                            backup = backup - FEBLONG;
                            cMth = cMth + 1;
                        } else {
                            dayBackup = backup;
                            break;
                        }
                }
            } else {
                if (backup > FEBSHORT) {
                    backup = backup - FEBSHORT;
                    cMth = cMth + 1;
                } else {
                    dayBackup = backup;
                    break;
                }
            }
        }
    }
    if (cMth == IAN || cMth == MAR || cMth == MAI || cMth == JUL || cMth == AUG || cMth == OCT) {
        if (dayBackup == LONGMONTH) {
            dayBackup = 1;
            cMth = cMth + 1;
        } else {
            dayBackup = dayBackup + 1;
        }
    } else {
        if (cMth == APR || cMth == JUN || cMth == SEPT || cMth == NOV) {
            if (dayBackup == SHORTMONTH) {
                dayBackup = 1;
                cMth = cMth + 1;
            } else {
                dayBackup = dayBackup + 1;
            }
        } else {
            if (cMth == DEC) {
                if (dayBackup == LONGMONTH) {
                    dayBackup = 1;
                    cMth = 1;
                    cYear = cYear + 1;
                } else {
                    dayBackup = dayBackup + 1;
                }
            } else {
                if (cMth == FEB) {
                    if (cYear % APR == 0) {
                        if (cYear % HUNNID == 0 && cYear % FOHUNNID == 0) {
                            if (dayBackup == FEBLONG) {
                                dayBackup = 1;
                                cMth = cMth + 1;
                            } else {
                                dayBackup = dayBackup + 1;
                            }
                        }
                        if (cYear % HUNNID == 0 && cYear % FOHUNNID != 0) {
                            if (dayBackup == FEBSHORT) {
                                dayBackup = 1;
                                cMth = cMth + 1;
                            } else {
                                dayBackup = dayBackup + 1;
                            }
                        }
                        if (cYear % HUNNID != 0) {
                            if (dayBackup == FEBLONG) {
                                dayBackup = 1;
                                cMth = cMth + 1;
                            } else {
                                dayBackup = dayBackup + 1;
                            }
                        }
                    } else {
                        if (dayBackup == FEBSHORT) {
                            dayBackup = 1;
                            cMth = cMth + 1;
                        } else {
                            dayBackup = dayBackup + 1;
                        }
                    }
                }
            }
        }
    }
    result.day = dayBackup;
    result.month = cMth;
    result.year = cYear;
    return result;
}

TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index) {
    TDateTimeTZ result;
    unsigned int timestampDif = SECSINHOUR * timezones[timezone_index].utc_hour_difference;
    result.tz = timezones + timezone_index;
    result.date = convertUnixTimestampToDate(timestamp + timestampDif);
    unsigned int timestampDay = (timestamp + timestampDif) % (SECSINHOUR * HOURSINDAY);
    result.time = convertUnixTimestampToTime(timestampDay);
    return result;
}

unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ datetime) {
    // fie datetime numele argumentului
    unsigned int timestamp = 0;
    for (int i = ANULHRISTOS; i < datetime.date.year; i ++) {
        if (i % APR == 0) {
            if (i % HUNNID == 0) {
                if (i % FOHUNNID == 0) {
                    timestamp = timestamp + ((DAYSINYEAR + 1) * SECSINHOUR * HOURSINDAY);
                } else {
                    timestamp = timestamp + (DAYSINYEAR * SECSINHOUR * HOURSINDAY);
                }
            } else {
                timestamp = timestamp + ((DAYSINYEAR + 1) * SECSINHOUR * HOURSINDAY);
            }
        } else {
            timestamp = timestamp + (DAYSINYEAR * SECSINHOUR * HOURSINDAY);
        }
    }
    for (int i = 1; i < datetime.date.month; i ++) {
        if (i == IAN || i == MAR || i == MAI || i == JUL || i == AUG || i == OCT) {
            timestamp = timestamp + (SECSINHOUR * HOURSINDAY * LONGMONTH);
        }
        if (i == NOV || i == APR || i == JUN || i == SEPT) {
            timestamp = timestamp + (SECSINHOUR * HOURSINDAY * SHORTMONTH);
        }
        if (i == FEB) {
            if (datetime.date.year % APR == 0) {
                if (datetime.date.year % HUNNID == 0) {
                    if (datetime.date.year % FOHUNNID == 0) {
                        timestamp = timestamp + (SECSINHOUR * HOURSINDAY * FEBLONG);
                    } else {
                        timestamp = timestamp + (SECSINHOUR * HOURSINDAY * FEBSHORT);
                    }
                } else {
                    timestamp = timestamp + (SECSINHOUR * HOURSINDAY * FEBLONG);
                }
            } else {
                timestamp = timestamp + (SECSINHOUR * HOURSINDAY * FEBSHORT);
            }
        }
    }
    timestamp = timestamp + (SECSINHOUR * HOURSINDAY * (datetime.date.day - 1));
    timestamp = timestamp + (SECSINHOUR * (datetime.time.hour - 1));
    timestamp = timestamp + (SECSORMINS * (datetime.time.min - 1));
    timestamp = timestamp + datetime.time.sec;
    int tzDif = datetime.tz->utc_hour_difference * SECSINHOUR;
    timestamp = timestamp - tzDif;
    timestamp = timestamp + SECSINHOUR + SECSORMINS;
    return timestamp;
}

void printDateTimeTZ(TDateTimeTZ datetimetz) {
    switch (datetimetz.date.month) {
    case IAN:
        printf("%02d ianuarie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case FEB:
        printf("%02d februarie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case MAR:
        printf("%02d martie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case APR:
        printf("%02d aprilie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case MAI:
        printf("%02d mai %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case JUN:
        printf("%02d iunie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case JUL:
        printf("%02d iulie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case AUG:
        printf("%02d august %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case SEPT:
        printf("%02d septembrie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case OCT:
        printf("%02d octombrie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case NOV:
        printf("%02d noiembrie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    case DEC:
        printf("%02d decembrie %d, ", datetimetz.date.day, datetimetz.date.year);
        break;
    default:
        break;
    }
    printf("%02d:%02d:%02d ", datetimetz.time.hour, datetimetz.time.min, datetimetz.time.sec);
    if (datetimetz.tz->utc_hour_difference >= 0) {
        printf("%s (UTC+%d)", datetimetz.tz->name, datetimetz.tz->utc_hour_difference);
    } else {
        printf("%s (UTC%d)", datetimetz.tz->name, datetimetz.tz->utc_hour_difference);
    }
}
