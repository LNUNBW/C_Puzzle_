#include "myPuzzle.h"

// 터미널 입력 모드 설정
void set_input_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// 키 입력 확인 함수
int kbhit() {
    struct termios oldt, newt;
    int ch, oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

// 키 입력 함수
int getKey() {
    if (kbhit()) {
        int key = getchar();
        if(key == 'q' || key == 'Q'){
            return 'q';
        }else if(key == 'r' || key == 'R'){
            return 'r';
        }else if(key == 's' || key == 'S'){
            return 's';
        }else if(key == 27 && getchar() == '[') { // ESC [ 시퀀스 확인
            return getchar();  //방향키 입력 ('A', 'B', 'C', 'D')
        }
        
    }
    return 0;  // 입력이 없으면 0 반환
}

// 퍼즐 초기화 함수
int p2D_init(int temp1, MY_PUZZLE *puzzle) {
    int i, k;

    if(temp1 == 1){
        // 테스트용 초기화
        for(i = 0; i < puzzle->size * puzzle->size - 1; i++){
            puzzle->d1.p1D[i] = i + 1;
        }
        puzzle->d1.p1D[puzzle->size * puzzle->size - 1] = 0;
    } else {
        for(i = 0; i < puzzle->size * puzzle->size; i++){
            // 난수 생성
            puzzle->d1.p1D[i] = rand() % (puzzle->size * puzzle->size);

            // 중복 값 확인 및 처리
            for(k = 0; k < i; k++){
                if(puzzle->d1.p1D[i] == puzzle->d1.p1D[k]){
                    i--; // 중복된 i번째 다시 생성 처리
                    break;
                }
            }
        }
    }

    for(i = 0; i < puzzle->size * puzzle->size; i++){
        // 빈 칸의 위치 정보 저장
        if(puzzle->d1.p1D[i] == 0){
            puzzle->d2.row = i / puzzle->size;
            puzzle->d2.column = i % puzzle->size;
        }
    }
    return 1;
}

// 퍼즐 디스플레이 함수
int p2D_display(int count, MY_PUZZLE *puzzle) {
    int i, k;

    system("clear");



    // 상단 테두리 출력
    printf("┌");
    for (k = 0; k < puzzle->size; k++) {
        for (int j = 0; j < 3; j++) { // 각 박스 너비
            printf("─");
        }
        if (k < puzzle->size - 1) {
            printf("┬");
        }
    }
    printf("┐\n");

    for (i = 0; i < puzzle->size; i++) {
        // 각 행의 시작
        printf("│");
        for (k = 0; k < puzzle->size; k++) {
            if (puzzle->d1.p1D[i * puzzle->size + k] == 0) {
                printf(" * "); // 빈 공간을 '*'로 표시
            } else {
                printf("%2d ", puzzle->d1.p1D[i * puzzle->size + k]); // 숫자 출력
            }
            printf("│");
        }
        printf("\n");

        // 각 행의 하단 구분선 출력
        if (i < puzzle->size - 1) {
            printf("├");
            for (k = 0; k < puzzle->size; k++) {
                for (int j = 0; j < 3; j++) { // 각 박스 너비
                    printf("─");
                }
                if (k < puzzle->size - 1) {
                    printf("┼");
                }
            }
            printf("┤\n");
        }
    }

    // 마지막 하단 테두리 출력
    printf("└");
    for (k = 0; k < puzzle->size; k++) {
        for (int j = 0; j < 3; j++) { // 각 박스 너비
            printf("─");
        }
        if (k < puzzle->size - 1) {
            printf("┴");
        }
    }
    printf("┘\n");

    printf("Count : %d\n", count);

    return count;
}


// 퍼즐 플레이 함수
int p2D_play(int key, MY_PUZZLE *puzzle) {

    static int count = 0;
    int targetRow = puzzle->d2.row;
    int targetColumn = puzzle->d2.column;

    switch(key) {
        case KEY_UP: 
            if (puzzle->d2.row > 0) {
                targetRow--;
            }
            break;

        case KEY_DOWN: 
            if (puzzle->d2.row < puzzle->size - 1) {
                targetRow++;
            }
            break;

        case KEY_RIGHT: 
            if (puzzle->d2.column < puzzle->size - 1) {
                targetColumn++;
            }
            break;

        case KEY_LEFT: 
            if (puzzle->d2.column > 0) {
                targetColumn--;
            }
            break;

        default:
            break;
    }

    if (targetRow != puzzle->d2.row || targetColumn != puzzle->d2.column) {
        int temp = puzzle->d1.p1D[puzzle->d2.row * puzzle->size + puzzle->d2.column];
        puzzle->d1.p1D[puzzle->d2.row * puzzle->size + puzzle->d2.column] = puzzle->d1.p1D[targetRow * puzzle->size + targetColumn];
        puzzle->d1.p1D[targetRow * puzzle->size + targetColumn] = 0;

        puzzle->d2.row = targetRow;
        puzzle->d2.column = targetColumn;

        count++;
    }

    p2D_display(count, puzzle); // 매 이동 후 퍼즐 출력

    return count;
}

// 퍼즐 종료 확인 함수
int p2D_finish(int temp1, MY_PUZZLE *puzzle){
    int i;

    for(i = 0; i < puzzle->size * puzzle->size - 1; i++){
        if(puzzle->d1.p1D[i] != (i + 1)){
            return 0;   // 게임 계속
        }
    }
    return 1;   // 게임 종료
}

int p2D_config(int temp, MY_PUZZLE *puzzle){
    FILE *pConfig;

    char buffer[500];
    char *result;

    pConfig = fopen(CONFIG, "r");
    if(pConfig == NULL){
        puzzle->size = DEFAULT_SIZE;
        puzzle->test = 0;
        strcpy(puzzle->score, SCORE);
        return 0;
    }

    while(1){
        memset(buffer, 0x00, 500);

        result = fgets(buffer, 500, pConfig);
        if(result == NULL){
            break;
        }        
        if(buffer[strlen(buffer)-1] == '\n'){
            buffer[strlen(buffer)-1] = 0x00;
        }


        if(strncmp("#", buffer, 1) == 0){
            continue;
        }else if(strncmp("SIZE: ", buffer, 6) == 0){
            puzzle->size = atoi(&buffer[6]);
        }else if(strncmp("TEST: ", buffer, 6) == 0){
            puzzle->test = atoi(&buffer[6]);
        }else if(strncmp("SCORE: ", buffer, 7) == 0){
            memset(puzzle->score, 0x00, 200);
            strcpy(puzzle->score, &buffer[7]);
        }else if(strncmp("DIR: ", buffer, 5) == 0){
            memset(puzzle->dir, 0x00, 200);
            strcpy(puzzle->dir, &buffer[5]);
        }else if (strncmp("NAME: ", buffer, 6) == 0) {
            memset(puzzle->user, 0x00, 80);
            strcpy(puzzle->user, &buffer[6]);
        }
    }
    fclose(pConfig);
    return 1;
}

int p2D_recordScore(char *user, int count, char *dirName, char *fileName){

    FILE *pScore;

    time_t timer;
    struct tm *t;

    pScore = fopen(fileName, "r");
    if(pScore == NULL){
        pScore = fopen(fileName, "w");
        if(pScore == NULL){
            return 1;
        }
        fprintf(pScore, "================================================\n");
        fprintf(pScore, " %-13s  %-8s  %s\n", "NAME", "COUNT", "DATE");
        fprintf(pScore, "================================================\n");
    }
    fclose(pScore);
    pScore = NULL;

    pScore = fopen(fileName, "a");
    if(pScore == NULL){
        return 1;
    }
    timer = time(NULL);
    t = localtime(&timer);

    fprintf(pScore, " %-15s %-6d %4d-%02d-%02d (%02d:%02d:%02d) \n", user, count, t->tm_year+1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    fclose(pScore);
    return 0;
}

void p2D_printScore(char *dirName, char *fileName) {
    FILE *pScore;
    char buffer[500] = {0};
    size_t byte;

    system("clear");

    pScore = fopen(fileName, "r");

    if (pScore == NULL) {
        printf("에러: 파일을 열 수 없습니다.\n");
        return;
    }

    while((byte = fread(buffer, 1, sizeof(buffer) - 1, pScore)) > 0){    //sizeof(buffer)-1의 이유는 문자열 맨 뒤에는 널값이 들어가야하기 때문
        buffer[byte] = '\0';  // 읽은 데이터 뒤에 문자열 종료 추가
        printf("%s", buffer);      // 버퍼 내용 출력
    }

    if(ferror(pScore)){
        printf("에러: score.txt를 불러올 수 없습니다.\n");
    }

    fclose(pScore);
}

void displayStars(wchar_t grid[15][15]) {
    system("clear");  // 화면을 지우고
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            wprintf(L"%lc ", grid[i][j]);  // 현재 별 배열 상태 출력
        }
        printf("\n");
    }
    usleep(8000); //별 속도
}

void introStars(wchar_t grid[15][15], int step, wchar_t starChar) {
    int x = 15 / 2, y = 15 / 2;  // 중앙에서부터 시작
    int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};  // 상, 우, 하, 좌
    int d = 0, length = 1, moves = 0;   // d = 이동방향, length = 이동할 거리, moves = 이동한 횟수
    while (moves < step && x >= 0 && y >= 0 && x < 15 && y < 15) {
        grid[x][y] = starChar;   // 전달받은 문자로 채우기
        displayStars(grid);
        x += directions[d][0];
        y += directions[d][1];
        moves++;
        if (moves == length) {
            d = (d + 1) % 4;  // 방향 전환
            moves = 0;
            if (d == 0 || d == 2) length++;  // 한 방향 이동 후에는 이동 거리 증가
        }
    }
}

void p2D_intro(wchar_t grid[15][15]){
    introStars(grid, 15*15, L'★');
    introStars(grid, 15*15, L'☆');
    introStars(grid, 15*15, L' ');

}

