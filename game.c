#include "myPuzzle.h"

int main(){
    set_input_mode();

    int i, count = 0, key, goStop = 1, shutdown;
    MY_PUZZLE puzzle;

    bool scoreDisplay = false;

    setlocale(LC_CTYPE, "");

    wchar_t grid[15][15] = {L' '};

    p2D_intro(grid);

    if (!p2D_config(0, &puzzle)) {
        printf("설정 파일을 로드하지 못했습니다.\n");
    }
    if (puzzle.size < MIN_PUZZLE_SIZE || puzzle.size > MAX_PUZZLE_SIZE) {
        printf("퍼즐 크기는 %d와 %d 사이여야 합니다.\n", MIN_PUZZLE_SIZE, MAX_PUZZLE_SIZE);
        return 1;
    }

    puzzle.d1.p1D = (int *)malloc(puzzle.size * puzzle.size * sizeof(int));

    int (*func[4])(int, MY_PUZZLE *) = {p2D_init, p2D_play, p2D_display, p2D_finish};

    srand(time(NULL));
    p2D_init(puzzle.test, &puzzle);
    p2D_display(0, &puzzle);

    while(goStop){
        key = getKey();
        if(key == 'q'){
            printf("프로그램을 종료합니다\n");
            goStop = 0;
            shutdown = 1;
        }else if(key == 'r'){
            p2D_init(puzzle.test, &puzzle);
            p2D_display(0, &puzzle);
        }else if(key == 's'){
            if (!scoreDisplay) {
                p2D_printScore(SCOREDIR, SCORE);
                scoreDisplay = true;
            } else {
                scoreDisplay = false;
                p2D_display(count, &puzzle);
            }
        }else if(key != 0) {
            count = p2D_play(key, &puzzle);
            if(p2D_finish(0, &puzzle)){
                goStop = 0;
            }
            
        }  
        usleep(100000);
    }
    
    if (p2D_finish(0, &puzzle) && shutdown != 1) {
        p2D_recordScore(puzzle.user, count, SCOREDIR, SCORE); 
    }

    printf("\n\n%s!\n", FINISH_MESSAGE);

    free(puzzle.d1.p1D);

    return 0;
}


