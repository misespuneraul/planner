#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

typedef struct {
    unsigned int timestampBegin;
    unsigned int duration;
} Interval;

typedef struct {
    char name[OCT];
    char timezone[MAI];
    TTimezone *tz;
    unsigned int intervalNum;
    Interval *intervals;
} Person;

typedef struct {
    unsigned int timestampBegin;
    unsigned int timestampEnd;
    char personID;
} FreeInterval;

typedef struct {
    unsigned int timestamp;
    unsigned int numComing;
    char *whosComing;
} PossibleInt;

int main() {
    unsigned T = 0, P = 0, K = 0, F = 0, eventDuration = 0;
    unsigned int totalIntervals = 0;
    unsigned int timeEnd = 0;
    unsigned int intervalCounter = 0;

    // citire timezones
    scanf("%u", &T);
    TTimezone *timezones = calloc(T, sizeof(TTimezone));
    for (int i = 0; i < T; i ++) {
        scanf("%s %hhd", timezones[i].name, &timezones[i].utc_hour_difference);
    }

    // citire persoane
    scanf("%u", &P);
    Person *people = calloc(P, sizeof(Person));
    for (int i = 0; i < P; i ++) {
        scanf("%s", people[i].name);
        scanf("%s", people[i].timezone);
        for (int j = 0; j < T; j ++) {
            if (strcmp(people[i].timezone, timezones[j].name) == 0) {
                people[i].tz = timezones + j;
            }
        }
        scanf("%u", &K);
        people[i].intervalNum = K;
        totalIntervals = totalIntervals + K;
        if (K != 0) {
            people[i].intervals = calloc(people[i].intervalNum, sizeof(Interval));
            for (int j = 0; j < K; j ++) {
                TDateTimeTZ personInterval;
                scanf("%u", &personInterval.date.year);
                scanf("%hhu", &personInterval.date.month);
                scanf("%hhu", &personInterval.date.day);
                scanf("%hhu", &personInterval.time.hour);
                personInterval.time.min = 0;
                personInterval.time.sec = 0;
                personInterval.tz = people[i].tz;
                scanf("%u", &people[i].intervals[j].duration);
                people[i].intervals[j].timestampBegin = convertDateTimeTZToUnixTimestamp(personInterval);
                // timestampBegin este convertit si inregistrat corect
                if (j > 0) {
                    // aici concatenam intervalele
                    unsigned int endOfEnds = people[i].intervals[j - 1].timestampBegin;
                    endOfEnds += people[i].intervals[j - 1].duration * SECSINHOUR;
                    if (endOfEnds == people[i].intervals[j].timestampBegin) {
                        people[i].intervals[j - 1].duration += people[i].intervals[j].duration;
                        j--;
                        K--;
                        totalIntervals--;
                        people[i].intervalNum--;
                    }
                }
            }
        } else {
            people[i].intervalNum = 1;
            totalIntervals = totalIntervals + 1;
            people[i].intervals = calloc(1, sizeof(Interval));
            people[i].intervals[0].duration = 0;
            people[i].intervals[0].timestampBegin = 0;
        }
    }

    // sortare persoane
    unsigned int pplSorted = 0;
    Person auxPerson;
    while (pplSorted == 0) {
        pplSorted = 1;
        for (int i = 0; i < P - 1; i ++) {
            if (strcmp(people[i].name, people[i + 1].name) > 0) {
                auxPerson = people[i];
                people[i] = people[i + 1];
                people[i + 1] = auxPerson;
                pplSorted = 0;
            }
        }
    }

    // citire eveniment
    scanf("%u%u", &F, &eventDuration);
    eventDuration = eventDuration * SECSINHOUR;

    FreeInterval *freeIntervals = calloc(totalIntervals, sizeof(FreeInterval));
    for (int i = 0; i < P; i ++) {
        // printf("\npersoana %u %s\n", i, people[i].name);
        for (int j = 0; j < people[i].intervalNum; j ++) {
            timeEnd = people[i].intervals[j].timestampBegin + SECSINHOUR * people[i].intervals[j].duration;
            freeIntervals[intervalCounter].timestampBegin = people[i].intervals[j].timestampBegin;
            freeIntervals[intervalCounter].timestampEnd = timeEnd;
            freeIntervals[intervalCounter].personID = (char) i;
            intervalCounter = intervalCounter + 1;
        }
    }

    // sortare intervale
    unsigned int sorted = 0;
    FreeInterval aux;
    while (sorted == 0) {
        sorted = 1;
        for (int i = 0; i < totalIntervals - 1; i ++) {
            if (freeIntervals[i].timestampBegin > freeIntervals[i + 1].timestampBegin) {
                aux  = freeIntervals[i];
                freeIntervals[i] = freeIntervals[i + 1];
                freeIntervals[i + 1] = aux;
                sorted = 0;
            }
        }
    }

    // gasire interval eveniment
    unsigned int lowestTS = 0;
    unsigned int highestTS = freeIntervals[totalIntervals - 1].timestampEnd;

    for (int i = 0; i < totalIntervals; i ++) {
        // gasim cel mai mic interval
        if (freeIntervals[i].timestampBegin > 0) {
            lowestTS = freeIntervals[i].timestampBegin;
            break;
        }
    }  // lowest si highest sunt setate corect

    // cream un vector cu toate intervalele posibile
    unsigned int possibleCount = 0;
    PossibleInt *possibleInts = calloc((highestTS - lowestTS) / SECSINHOUR, sizeof(PossibleInt));
    for (unsigned int i = lowestTS; i < highestTS; i += SECSINHOUR) {
        possibleInts[possibleCount].timestamp = i;
        possibleInts[possibleCount].numComing = 0;
        possibleInts[possibleCount].whosComing = calloc(P, sizeof(char));
        possibleCount = possibleCount + 1;
    }

    // vedem cati si cine vin in fiecare interval posibil
    unsigned int coming = 0;
    for (int i = 0; i < possibleCount; i ++) {
        for (int j = 0; j < totalIntervals; j ++) {
            if (freeIntervals[j].timestampEnd >= possibleInts[i].timestamp + eventDuration) {
                if (freeIntervals[j].timestampBegin <= possibleInts[i].timestamp) {
                    coming = possibleInts[i].numComing;
                    possibleInts[i].whosComing[coming] = freeIntervals[j].personID;
                    possibleInts[i].numComing = possibleInts[i].numComing + 1;
                }
            }
        }
    }

    // vedem cel mai devreme interval
    unsigned int possible = 0;
    unsigned int happening = 0;
    for (int i = 0; i < possibleCount; i ++) {
        if (possibleInts[i].numComing >= F) {
            happening = i;
            possible = 1;
            break;
        }
    }

    // printam imposibil
    if (possible == 0) {
        printf("imposibil");
    }

    // printare daca e posibil
    if (possible == 1) {
        unsigned int found = 0;
        unsigned int luckyTS = possibleInts[happening].timestamp;
        for (int i = 0; i < P; i ++) {
            printf("%s: ", people[i].name);
            found = 0;
            for (int j = 0; j < possibleInts[happening].numComing; j ++) {
                if (possibleInts[happening].whosComing[j] == i) {
                    TDateTimeTZ eventPpl = convertUnixTimestampToDateTimeTZ(luckyTS, people[i].tz, 0);
                    printDateTimeTZ(eventPpl);
                    printf("\n");
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                printf("invalid\n");
            }
        }
    }

    // eliberari
    free(freeIntervals);
    for (int i = 0; i < P; i ++) {
        free(people[i].intervals);
    }
    free(people);
    free(timezones);
    for (int i = 0; i < possibleCount; i ++) {
        free(possibleInts[i].whosComing);
    }
    free(possibleInts);

    return 0;
}
