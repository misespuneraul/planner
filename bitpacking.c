#include <stdio.h>
#include <stdlib.h>
#include "timelib.h"

typedef struct {
    unsigned int dataInt;
    unsigned int ctrlBit;
    unsigned int corrupted;
} IntVerified;

typedef struct {
    unsigned int bitBegin;
    unsigned int bitEnd;
    unsigned int intBegin;
    unsigned int intEnd;
    unsigned int corrupted;
} PackedDate;

    // returneaza 1 daca trebuie inversate
unsigned int dateCompare(TDate first, TDate second) {
    if (second.year < first.year) {
        return 1;
    } else {
        if (second.year == first.year) {
            if (second.month < first.month) {
                return 1;
            } else {
                if (second.month == first.month) {
                    if (second.day < first.day) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void dateSort(TDate *dates, unsigned int num) {
    TDate aux;
    int sorted = 0;
    while (sorted == 0) {
        sorted = 1;
        for (int i = 0; i < num - 1; i++) {
            if (dateCompare(dates[i], dates[i + 1]) == 1) {
                aux = dates[i];
                dates[i] = dates[i + 1];
                dates[i + 1] = aux;
                sorted = 0;
            }
        }
    }
}

void printDate(TDate date) {
    printf("%hhu ", date.day);
    switch (date.month) {
    case IAN:
        printf("ianuarie ");
        break;
    case FEB:
        printf("februarie ");
        break;
    case MAR:
        printf("martie ");
        break;
    case APR:
        printf("aprilie ");
        break;
    case MAI:
        printf("mai ");
        break;
    case JUN:
        printf("iunie ");
        break;
    case JUL:
        printf("iulie ");
        break;
    case AUG:
        printf("august ");
        break;
    case SEPT:
        printf("septembrie ");
        break;
    case OCT:
        printf("octombrie ");
        break;
    case NOV:
        printf("noiembrie ");
        break;
    case DEC:
        printf("decembrie ");
        break;
    default:
        break;
    }
    printf("%u\n", date.year);
}

int main() {
    unsigned int task = 0;
    scanf("%u", &task);
    if (task == JUL) {
        unsigned int N = 0, bitDate = 0;
        scanf("%u", &N);
        TDate *bitDates = calloc(N, sizeof(TDate));
        for (int i = 0; i < N; i++) {
            scanf("%u", &bitDate);
            bitDates[i].day = bitDate & LONGMONTH;
            bitDate = (bitDate >> MAI);
            bitDates[i].month = bitDate & CINSPE;
            bitDate = (bitDate >> APR);
            bitDates[i].year = bitDate & SAISTREI;
            bitDates[i].year = bitDates[i].year + ANULHRISTOS;
        }
        dateSort(bitDates, N);
        for (int i = 0; i < N; i ++) {
            printDate(bitDates[i]);
        }
    }
    if (task == AUG) {
        unsigned int numDates = 0;
        unsigned int totalInts = 0;
        unsigned int totalCtrlInts = 0;
        unsigned int intBitCount = 0;
        unsigned int ctrlBitVerify = 0;
        unsigned int countDay = 0, countMonth = 0, countYear = 0;
        unsigned int currentInt = 0;
        unsigned int currentBit = 0;

        // citim nr de date
        scanf("%u", &numDates);
        unsigned int totalBits = numDates * CINSPE;

        // aflam cate inturi cu date avem
        if (totalBits % BITNUM == 0) {
            totalInts = totalBits / BITNUM;
        } else {
            totalInts = totalBits / BITNUM + 1;
        }

        // alocam vector inturi de date
        IntVerified *intsVect = calloc(totalInts, sizeof(IntVerified));

        // aflam cate inturi de control avem
        if (totalInts % BITNUM == 0) {
            totalCtrlInts = totalInts / BITNUM;
        } else {
            totalCtrlInts = totalInts / BITNUM + 1;
        }

        // alocam vector inturi de control
        unsigned int *ctrlVect = calloc(totalCtrlInts, sizeof(unsigned int));

        // citim inturi de date si le calculam bitul de control
        for (int i = 0; i < totalInts; i ++) {
            intBitCount = 0;
            scanf("%u", &intsVect[i].dataInt);
            intsVect[i].corrupted = 0;
            for (int j = 0; j < BITNUM; j ++) {
                if (((intsVect[i].dataInt >> j) & 1) == 1) {
                    intBitCount = intBitCount + 1;
                }
            }
            intsVect[i].ctrlBit = intBitCount % 2;
        }

        // citim inturi de control
        for (int i = 0; i < totalCtrlInts; i ++) {
            scanf("%u", &ctrlVect[i]);
        }

        // comparam bitul de control
        for (int i = 0; i < totalInts; i ++) {
            ctrlBitVerify = ((ctrlVect[i / BITNUM] >> (i % BITNUM)) & 1);
            if (ctrlBitVerify != intsVect[i].ctrlBit) {
                intsVect[i].corrupted = 1;
            }
        }

        // intocmim un vector de date
        PackedDate *dataVect = calloc(numDates, sizeof(PackedDate));
        for (int i = 0; i < numDates; i ++) {
            // setam corrupted la 0
            dataVect[i].corrupted = 0;
            // aflam in ce bit incepe si se termina data
            dataVect[i].bitBegin = (i * CINSPE) % BITNUM;
            dataVect[i].bitEnd = ((i + 1) * CINSPE) % BITNUM;
            // aflam in ce int incepe data
            dataVect[i].intBegin = (i * CINSPE) / BITNUM;
            // aflam in ce int se termina data
            if (dataVect[i].bitEnd < dataVect[i].bitBegin) {
                dataVect[i].intEnd = dataVect[i].intBegin + 1;
            } else {
                dataVect[i].intEnd = dataVect[i].intBegin;
            }
            // verificam daca data e corupta
            if (intsVect[dataVect[i].intBegin].corrupted == 1) {
                dataVect[i].corrupted = 1;
            }
            if (intsVect[dataVect[i].intEnd].corrupted == 1) {
                if (dataVect[i].bitEnd != 0) {
                    dataVect[i].corrupted = 1;
                }
            }
        }

        // despachetam datele
        TDate *unpackedDates = calloc(numDates, sizeof(TDate));
        for (int i = 0; i < numDates; i ++) {
            // verificam daca data e corupta
            if (dataVect[i].corrupted == 1) {
                // dam datelor corupte o valoare imposibila
                unpackedDates[i].day = 1;
                unpackedDates[i].month = 1;
                unpackedDates[i].year = ANTIHRISTUL;
            } else {
                // initializam data
                unpackedDates[i].day = 0;
                unpackedDates[i].month = 0;
                unpackedDates[i].year = ANULHRISTOS;
                // initializam countere
                countDay = 0;
                countMonth = 0;
                countYear = 0;
                currentInt = dataVect[i].intBegin;
                currentBit = dataVect[i].bitBegin;
                for (int bit = 0; bit < CINSPE; bit ++) {
                    // verificam daca am trecut in intul urmator
                    if ((dataVect[i].bitBegin + bit) >= BITNUM) {
                        currentInt = dataVect[i].intEnd;
                        currentBit = (dataVect[i].bitBegin + bit) % BITNUM;
                    } else {
                        currentBit = dataVect[i].bitBegin + bit;
                    }
                    // adunam la data in functie de bit
                    switch (bit) {
                    case 0:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countDay = countDay + POWER_ZERO;
                        }
                        break;
                    case 1:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countDay = countDay + POWER_I;
                        }
                        break;
                    case FEB:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countDay = countDay + POWER_II;
                        }
                        break;
                    case MAR:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countDay = countDay + POWER_III;
                        }
                        break;
                    case APR:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countDay = countDay + POWER_IV;
                        }
                        break;
                    case MAI:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countMonth = countMonth + POWER_ZERO;
                        }
                        break;
                    case JUN:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countMonth = countMonth + POWER_I;
                        }
                        break;
                    case JUL:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countMonth = countMonth + POWER_II;
                        }
                        break;
                    case AUG:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countMonth = countMonth + POWER_III;
                        }
                        break;
                    case SEPT:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countYear = countYear + POWER_ZERO;
                        }
                        break;
                    case OCT:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countYear = countYear + POWER_I;
                        }
                        break;
                    case NOV:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countYear = countYear + POWER_II;
                        }
                        break;
                    case DEC:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countYear = countYear + POWER_III;
                        }
                        break;
                    case TREISPE:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countYear = countYear + POWER_IV;
                        }
                        break;
                    case PAISPE:
                        if (((intsVect[currentInt].dataInt >> currentBit) & 1) == 1) {
                            countYear = countYear + POWER_V;
                        }
                        break;
                    default:
                        break;
                    }
                }
                unpackedDates[i].day = unpackedDates[i].day + countDay;
                unpackedDates[i].month = unpackedDates[i].month + countMonth;
                unpackedDates[i].year = unpackedDates[i].year + countYear;
                // printf("%u %u %u\n", countDay, countMonth, countYear);
            }
        }

        // sortam datele
        dateSort(unpackedDates, numDates);
        for (int i = 0; i < numDates; i ++) {
            if (unpackedDates[i].year != ANTIHRISTUL) {
                printDate(unpackedDates[i]);
            }
        }
    }

    return 0;
}
