#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_SW_BLOCKS 10
#define MAX_PARAMETERS 3

typedef struct SWBlock {
    char name[20];
    pid_t pid;
    int restartCount;
    time_t lastRestartTime;
    char reason[50];
    char params[MAX_PARAMETERS][20];
} SWBlock;

int swBlockCount;
char *timeString;
SWBlock blocks[MAX_SW_BLOCKS];

void runSwBlock(SWBlock *block, int init) {
    if (init) {
        printf("%s is initializing...\n", block->name);
    } else {
        printf("%s is restarting...\n", block->name);
    }
    block->lastRestartTime = time(NULL);
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) { // 자식 프로세스
        srand(time(NULL) * getpid());
        int sleepTime = rand() % 5 + 1;
        sleep(sleepTime);

        int signals[] = {SIGINT, SIGUSR1, SIGQUIT, SIGTERM, SIGSTOP};
        int randIndex = rand() % (sizeof(signals) / sizeof(int));
        int randSignal = signals[randIndex];
        kill(getpid(), randSignal);
    } else { // 부모 프로세스
        block->pid = pid;
    }
}

void trim(char *str) {
    char *start = str;
    char *end = str + (strlen(str) - 1);

    while (isspace(*start)) start++;
    while (isspace(*end)) end--;

    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

void printLog() {
    printf("\n\nS/W Block Name | Restart Count |       Start Time       | Reason\n");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < swBlockCount; i++) {
        timeString = ctime(&blocks[i].lastRestartTime);
        timeString[strlen(timeString) - 1] = '\0';

        printf("%-17s%-15d%s   %s\n", blocks[i].name, blocks[i].restartCount, timeString, blocks[i].reason);
    }
    printf("\n\n");
}

void signalHandler(int signum) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // 종료된 자식 프로세스에 대한 정보를 찾음
        SWBlock *block = NULL;
        for (int i = 0; i < swBlockCount; i++) {
            if (blocks[i].pid == pid) {
                block = &blocks[i];
                break;
            }
        }

        if (block) {
            block->restartCount++;
            block->lastRestartTime = time(NULL);

            if (WIFEXITED(status)) { // 자식프로세스가 정상종료된 경우
                snprintf(block->reason, sizeof(block->reason), "Exited with status %d",
                         WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) { // 자식프로세스가 시그널에 의해 종료된 경우
                snprintf(block->reason, sizeof(block->reason), "%s", strsignal(WTERMSIG(status)));
            } else {
                strcpy(block->reason, "Unknown");
            }

            FILE *log = fopen("./log/restart.txt", "a");
            if (log == NULL) {
                perror("fopen");
                exit(1);
            }

            timeString = ctime(&block->lastRestartTime);
            timeString[strlen(timeString) - 1] = '\0';

            fprintf(log, "%-17s%-15d%s   %s\n", block->name, block->restartCount, timeString, block->reason);
            fclose(log);

            printLog();

            // 블록 재초기화
            runSwBlock(block, 0);
        } else {
            printf("Child process %d terminated\n", pid);
        }
    }
}

void initSigaction() {
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, 0) == -1) {
        perror("sigaction");
        exit(1);
    }
}

int readSwBlocks(FILE *file, SWBlock *blocks) {
    char buf[256];
    int idx = 0;
    while (idx < MAX_SW_BLOCKS && fgets(buf, sizeof(buf), file)) {
        char *token = strtok(buf, ";");
        trim(token);
        strcpy(blocks[idx].name, token);
        for (int paramIdx = 0; paramIdx < MAX_PARAMETERS; paramIdx++) {
            token = strtok(NULL, ";");
            if (token) {
                trim(token);
                strcpy(blocks[idx].params[paramIdx], token);
            }
        }
        idx++;
    }
    return idx;
}

FILE *openFile() {
    mkdir("log", 0755);
    FILE *log = fopen("./log/restart.txt", "w");
    if (log == NULL) {
        perror("fopen");
        exit(1);
    } else {
        fprintf(log, "S/W Block Name | Restart Count |       Start Time       | Reason\n");
        fprintf(log, "-----------------------------------------------------------------\n");
    }
    return log;
}

int main() {
    // 로그파일 생성 및 초기화
    FILE *initialLog = openFile();
    fclose(initialLog);

    // SwBlock 초기화
    FILE *file = fopen("FileList.txt", "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    swBlockCount = readSwBlocks(file, blocks);
    fclose(file);

    // SwBlock 프로세스 생성
    initSigaction();
    for (int i = 0; i < swBlockCount; i++) {
        runSwBlock(&blocks[i], 1);
    }

    // Signal 대기
    while (1) {
        sleep(1);
    }
}
