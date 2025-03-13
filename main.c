#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_TILES 14
#define MAX_TOTAL 15       
#define MAX_FLAG 2         

#define SUIT_TYPES 9
#define HONOR_TYPES 7

#define MAX_SUIT_CONFIGS 15000
#define MAX_HONOR_CONFIGS 1500

typedef struct {
    int counts[SUIT_TYPES];  

typedef struct {
    int counts[HONOR_TYPES]; 
} HonorConfig;

SuitConfig suitConfigs[MAX_TOTAL][MAX_FLAG][MAX_SUIT_CONFIGS];
int suitConfigCount[MAX_TOTAL][MAX_FLAG] = {0};

HonorConfig honorConfigs[MAX_TOTAL][MAX_FLAG][MAX_HONOR_CONFIGS];
int honorConfigCount[MAX_TOTAL][MAX_FLAG] = {0};

int checkMeldSuit(int counts[SUIT_TYPES]) {
    int i;
    for (i = 0; i < SUIT_TYPES; i++) {
        if (counts[i] > 0)
            break;
    }
    if (i == SUIT_TYPES)
        return 1;
    if (counts[i] >= 3) {
        counts[i] -= 3;
        if (checkMeldSuit(counts)) {
            counts[i] += 3;
            return 1;
        }
        counts[i] += 3;
    }
    if (i <= 6 && counts[i+1] > 0 && counts[i+2] > 0) {
        counts[i]--; counts[i+1]--; counts[i+2]--;
        if (checkMeldSuit(counts)) {
            counts[i]++; counts[i+1]++; counts[i+2]++;
            return 1;
        }
        counts[i]++; counts[i+1]++; counts[i+2]++;
    }
    return 0;
}

int checkPairSuit(int counts[SUIT_TYPES]) {
    for (int i = 0; i < SUIT_TYPES; i++) {
        if (counts[i] >= 2) {
            counts[i] -= 2;
            if (checkMeldSuit(counts)) {
                counts[i] += 2;
                return 1;
            }
            counts[i] += 2;
        }
    }
    return 0;
}

int checkMeldHonor(int counts[HONOR_TYPES]) {
    int i;
    for (i = 0; i < HONOR_TYPES; i++) {
        if (counts[i] > 0)
            break;
    }
    if (i == HONOR_TYPES)
        return 1;
    if (counts[i] >= 3) {
        counts[i] -= 3;
        if (checkMeldHonor(counts)) {
            counts[i] += 3;
            return 1;
        }
        counts[i] += 3;
    }
    return 0;
}

int checkPairHonor(int counts[HONOR_TYPES]) {
    for (int i = 0; i < HONOR_TYPES; i++) {
        if (counts[i] >= 2) {
            counts[i] -= 2;
            if (checkMeldHonor(counts)) {
                counts[i] += 2;
                return 1;
            }
            counts[i] += 2;
        }
    }
    return 0;
}

void enumerateSuitRec(int idx, int currentSum, int target, int counts[SUIT_TYPES]) {
    if (idx == SUIT_TYPES) {
        if (currentSum <= target) {
            if (currentSum % 3 == 0) {
                int temp[SUIT_TYPES];
                memcpy(temp, counts, sizeof(int) * SUIT_TYPES);
                if (checkMeldSuit(temp)) {
                    int t = currentSum;
                    int flag = 0;
                    if (suitConfigCount[t][flag] < MAX_SUIT_CONFIGS) {
                        memcpy(suitConfigs[t][flag][suitConfigCount[t][flag]].counts, counts, sizeof(int) * SUIT_TYPES);
                        suitConfigCount[t][flag]++;
                    }
                }
            }
            if (currentSum % 3 == 2) {
                int temp[SUIT_TYPES];
                memcpy(temp, counts, sizeof(int) * SUIT_TYPES);
                if (checkPairSuit(temp)) {
                    int t = currentSum;
                    int flag = 1;
                    if (suitConfigCount[t][flag] < MAX_SUIT_CONFIGS) {
                        memcpy(suitConfigs[t][flag][suitConfigCount[t][flag]].counts, counts, sizeof(int) * SUIT_TYPES);
                        suitConfigCount[t][flag]++;
                    }
                }
            }
        }
        return;
    }
    for (int c = 0; c <= 4; c++) {
        if (currentSum + c > target)
            break;
        counts[idx] = c;
        enumerateSuitRec(idx + 1, currentSum + c, target, counts);
    }
}

void enumerateSuit() {
    int counts[SUIT_TYPES] = {0};
    enumerateSuitRec(0, 0, TOTAL_TILES, counts);
}

void enumerateHonorRec(int idx, int currentSum, int target, int counts[HONOR_TYPES]) {
    if (idx == HONOR_TYPES) {
        if (currentSum <= target) {
            if (currentSum % 3 == 0) {
                int temp[HONOR_TYPES];
                memcpy(temp, counts, sizeof(int) * HONOR_TYPES);
                if (checkMeldHonor(temp)) {
                    int t = currentSum;
                    int flag = 0;
                    if (honorConfigCount[t][flag] < MAX_HONOR_CONFIGS) {
                        memcpy(honorConfigs[t][flag][honorConfigCount[t][flag]].counts, counts, sizeof(int) * HONOR_TYPES);
                        honorConfigCount[t][flag]++;
                    }
                }
            }
            if (currentSum % 3 == 2) {
                int temp[HONOR_TYPES];
                memcpy(temp, counts, sizeof(int) * HONOR_TYPES);
                if (checkPairHonor(temp)) {
                    int t = currentSum;
                    int flag = 1;
                    if (honorConfigCount[t][flag] < MAX_HONOR_CONFIGS) {
                        memcpy(honorConfigs[t][flag][honorConfigCount[t][flag]].counts, counts, sizeof(int) * HONOR_TYPES);
                        honorConfigCount[t][flag]++;
                    }
                }
            }
        }
        return;
    }
    for (int c = 0; c <= 4; c++) {
        if (currentSum + c > target)
            break;
        counts[idx] = c;
        enumerateHonorRec(idx + 1, currentSum + c, target, counts);
    }
}

void enumerateHonor() {
    int counts[HONOR_TYPES] = {0};
    enumerateHonorRec(0, 0, TOTAL_TILES, counts);
}

unsigned long long computeCompleteHands() {
    unsigned long long totalCompleteHands = 0;
    for (int t0 = 0; t0 <= TOTAL_TILES; t0++) {
        for (int t1 = 0; t1 <= TOTAL_TILES; t1++) {
            for (int t2 = 0; t2 <= TOTAL_TILES; t2++) {
                int t3 = TOTAL_TILES - t0 - t1 - t2;
                if (t3 < 0 || t3 > TOTAL_TILES)
                    continue;
                for (int pairGroup = 0; pairGroup < 4; pairGroup++) {
                    int valid = 1;
                    int meldCount = 0;
                    int ts[4] = {t0, t1, t2, t3};
                    int reqFlag[4];
                    for (int g = 0; g < 4; g++) {
                        if (g == pairGroup) {
                            if (ts[g] % 3 != 2) { valid = 0; break; }
                            reqFlag[g] = 1;
                            meldCount += (ts[g] - 2) / 3;
                        } else {
                            if (ts[g] % 3 != 0) { valid = 0; break; }
                            reqFlag[g] = 0;
                            meldCount += ts[g] / 3;
                        }
                    }
                    if (!valid || meldCount != 4)
                        continue;
                    unsigned long long ways = 1;
                    ways *= suitConfigCount[t0][reqFlag[0]];
                    ways *= suitConfigCount[t1][reqFlag[1]];
                    ways *= suitConfigCount[t2][reqFlag[2]];
                    ways *= honorConfigCount[t3][reqFlag[3]];
                    totalCompleteHands += ways;
                }
            }
        }
    }
    return totalCompleteHands;
}

unsigned long long nCr(int n, int r) {
    unsigned long long result = 1;
    for (int i = 1; i <= r; i++) {
        result = result * (n - r + i) / i;
    }
    return result;
}

unsigned long long computeChiitoitsu() {
    return nCr(34, 7);
}

unsigned long long computeKokushiMusou() {
    return 13;
}

unsigned long long computeTotalHands() {
    const int n_tile_types = 34;
    const int hand_size = 14;
    unsigned long long dp[35][15] = {0};
    dp[0][0] = 1ULL;
    for (int i = 1; i <= n_tile_types; i++) {
        for (int j = 0; j <= hand_size; j++) {
            for (int used = 0; used <= 4; used++) {
                if (used <= j)
                    dp[i][j] += dp[i - 1][j - used];
            }
        }
    }
    return dp[n_tile_types][hand_size];
}

void writeStandardWinningHands(FILE *fp) {
    for (int t0 = 0; t0 <= TOTAL_TILES; t0++) {
        for (int t1 = 0; t1 <= TOTAL_TILES; t1++) {
            for (int t2 = 0; t2 <= TOTAL_TILES; t2++) {
                int t3 = TOTAL_TILES - t0 - t1 - t2;
                if (t3 < 0 || t3 > TOTAL_TILES)
                    continue;
                for (int pairGroup = 0; pairGroup < 4; pairGroup++) {
                    int valid = 1;
                    int meldCount = 0;
                    int ts[4] = {t0, t1, t2, t3};
                    int reqFlag[4];
                    for (int g = 0; g < 4; g++) {
                        if (g == pairGroup) {
                            if (ts[g] % 3 != 2) { valid = 0; break; }
                            reqFlag[g] = 1;
                            meldCount += (ts[g] - 2) / 3;
                        } else {
                            if (ts[g] % 3 != 0) { valid = 0; break; }
                            reqFlag[g] = 0;
                            meldCount += ts[g] / 3;
                        }
                    }
                    if (!valid || meldCount != 4)
                        continue;
                    for (int i0 = 0; i0 < suitConfigCount[ts[0]][reqFlag[0]]; i0++) {
                        for (int i1 = 0; i1 < suitConfigCount[ts[1]][reqFlag[1]]; i1++) {
                            for (int i2 = 0; i2 < suitConfigCount[ts[2]][reqFlag[2]]; i2++) {
                                for (int i3 = 0; i3 < honorConfigCount[ts[3]][reqFlag[3]]; i3++) {
                                    int hand[34] = {0};
                                    for (int k = 0; k < SUIT_TYPES; k++) {
                                        hand[k] = suitConfigs[ts[0]][reqFlag[0]][i0].counts[k];
                                    }
                                    for (int k = 0; k < SUIT_TYPES; k++) {
                                        hand[SUIT_TYPES + k] = suitConfigs[ts[1]][reqFlag[1]][i1].counts[k];
                                    }
                                    for (int k = 0; k < SUIT_TYPES; k++) {
                                        hand[2 * SUIT_TYPES + k] = suitConfigs[ts[2]][reqFlag[2]][i2].counts[k];
                                    }
                                    for (int k = 0; k < HONOR_TYPES; k++) {
                                        hand[3 * SUIT_TYPES + k] = honorConfigs[ts[3]][reqFlag[3]][i3].counts[k];
                                    }
                                    for (int j = 0; j < 34; j++) {
                                        fprintf(fp, "%d%s", hand[j], (j < 33 ? "," : "\n"));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void writeChiitoitsuHands(FILE *fp) {
    int subset[7] = {0};
    void rec(int start, int depth) {
        if (depth == 7) {
            int hand[34] = {0};
            for (int i = 0; i < 7; i++) {
                hand[subset[i]] = 2;
            }
            for (int j = 0; j < 34; j++) {
                fprintf(fp, "%d%s", hand[j], (j < 33 ? "," : "\n"));
            }
            return;
        }
        for (int i = start; i < 34; i++) {
            subset[depth] = i;
            rec(i + 1, depth + 1);
        }
    }
    rec(0, 0);
}


void writeKokushiMusouHands(FILE *fp) {
    int terminals[13] = {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33};
    for (int extra = 0; extra < 13; extra++) {
        int hand[34] = {0};
        for (int i = 0; i < 13; i++) {
            hand[terminals[i]] = 1;
        }
        hand[terminals[extra]] = 2;
        for (int j = 0; j < 34; j++) {
            fprintf(fp, "%d%s", hand[j], (j < 33 ? "," : "\n"));
        }
    }
}

int main(void) {
    unsigned long long totalHands = computeTotalHands();
    
    enumerateSuit();
    enumerateHonor();
    
    unsigned long long standardComplete = computeCompleteHands();
    unsigned long long chiitoitsu = computeChiitoitsu();
    unsigned long long kokushi = computeKokushiMusou();
    
    unsigned long long overallWinning = standardComplete + chiitoitsu + kokushi;
    double tenhouRate = (double) overallWinning / totalHands;
    
    FILE *fp = fopen("winning_hands.csv", "w");
    if (!fp) {
        perror("Error opening CSV file");
        return 1;
    }
    
    printf("Metric, Value\n");
    printf("Total 14-tile hands: %llu\n", totalHands);
    printf("Standard winning hands: %llu\n", standardComplete);
    printf("Chiitoitsu: %llu\n", chiitoitsu);
    printf("Kokushi Musou: %llu\n", kokushi);
    printf("Overall winning hands: %llu\n", overallWinning);
    printf("Tenhou Rate: %f\n", tenhouRate);
    
    printf("Writing standard winning hands...\n");
    writeStandardWinningHands(fp);
    
    printf("Writing chiitoitsu hands...\n");
    writeChiitoitsuHands(fp);
    
    printf("Writing kokushi musou hands...\n");
    writeKokushiMusouHands(fp);
    
    fclose(fp);
    printf("All data written to winning_hands.csv\n");


    return 0;
}
