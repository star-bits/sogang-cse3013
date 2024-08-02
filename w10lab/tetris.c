#include "tetris.h"
#include <unistd.h>

static struct sigaction act, oact;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _NODE {
    struct _NODE* link;
    char name[NAMELEN];
    int score;
} NODE;

NODE* head = NULL;

FILE* fp = NULL;

int score_number = 0;


#define max_depth 3


int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	createRankList();

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
    writeRankFile();
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	// nextBlock[0]=rand()%7;
	// nextBlock[1]=rand()%7;
	// nextBlock[2]=rand()%7;
    for (i=0; i<5; i++)
        nextBlock[i] = rand()%7;

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
    recStart();
	// DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
    DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	move(9,WIDTH+10);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	// DrawBox(10,WIDTH+10,1,8);
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	// move(11,WIDTH+11);
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for( i = 0; i < 4; i++ ){
		move(10+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	// 2d arr f: field 전체
	// currentBlock: 블록의 종류. 0~6
	// blockRotate: rotation state
	// blockY: 블록의 우상단 y
	// blockX: 블록의 우상단 x
	// 모든 블록은 4x4 2d arr
	for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[currentBlock][blockRotate][i][j]) {
                int y = blockY + i;
                int x = blockX + j;
                if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH || f[y][x])
				// boundary check + f[x][y]==1
				// y >= HEIGHT-4; x >= WIDTH-4; ??
				// ㄴㄴ i:0~4, j:0~4 for loop 두번 돌면서 다 체크함
                    return 0;
            }
        }
    }
    return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
	int prevRotate = blockRotate;
    int prevY = blockY;
    int prevX = blockX;

    DrawField();

	// 이전 블록의 정보
	// blockX, blockY는 이미 업데이트 된 상태로 들어옴
    switch (command) {
        case KEY_UP:
            prevRotate = (blockRotate + 3) % 4;
            break;
        case KEY_DOWN:
            prevY--;
            break;
        case KEY_LEFT:
            prevX++;
            break;
        case KEY_RIGHT:
            prevX--;
            break;
    }

	// 이전 블록 지우기
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[currentBlock][prevRotate][i][j] && i+prevY>=0) {
                move(i + prevY + 1, j + prevX + 1);
                printw(".");
            }
        }
    }

	// 이전 shadow 지우기
    int shadowY = prevY;
    while (CheckToMove(f, currentBlock, prevRotate, shadowY + 1, prevX)) {
        shadowY++;
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[currentBlock][prevRotate][i][j] == 1 && i + shadowY >= 0) {
                move(i + shadowY + 1, j + prevX + 1);
                printw(".");
            }
        }
    }

    // DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
    // move(HEIGHT, WIDTH + 10);
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);

	// int newShadowY = blockY;
    // while (CheckToMove(f, currentBlock, blockRotate, newShadowY + 1, blockX)) {
    //     newShadowY++;
    // }
    // DrawBlock(newShadowY, blockX, currentBlock, blockRotate, '/');
}

// void BlockDown(int sig){
// 	// user code

// 	//강의자료 p26-27의 플로우차트를 참고한다.
// 	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
//         blockY++;
//         DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
//     } else {
//         score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);

// 		if (blockY == -1) {
// 			gameOver = 1;
// 		}
//         // int gameEnded = 0;
// 		// for (int i = 0; i < WIDTH; i++) {
//     	// 	if (field[0][i] == 1) {
//         // 	gameEnded = 1;
//         // 	break;
//     	// 	}
// 		// }
//         // if (gameEnded) {
//         //     gameOver = 1;
//         // } 
// 		else {
//             score += DeleteLine(field);
//             nextBlock[0] = nextBlock[1];
//             nextBlock[1] = nextBlock[2];
//             nextBlock[2] = rand() % 7;
//             DrawNextBlock(nextBlock);
//             blockY = -1;
//             blockX = WIDTH / 2 - 2;
//             blockRotate = 0;
//             PrintScore(score);
//         }
//         DrawField();
//     }
//     timed_out = 0;
// }

void BlockDown(int sig) {
    // user code

	//강의자료 p26-27의 플로우차트를 참고한다.
    if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    } else {
        // check if the block can be placed at the current position
        if (!CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX)) {
            timed_out = 0;
            gameOver = 1;
            return;
        }
        
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (block[nextBlock[0]][blockRotate][i][j] && (blockY + i < 0)) {
                    timed_out = 0;
                    gameOver = 1;
                    return;
                }
            }
        }

        score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
        score += DeleteLine(field);

        nextBlock[0] = nextBlock[1];
        nextBlock[1] = nextBlock[2];
        nextBlock[2] = rand() % 7;

        DrawNextBlock(nextBlock);

        blockX = WIDTH / 2 - 2;
        blockY = -1;
        blockRotate = 0;

        recStart();
        PrintScore(score);
        DrawField();
    }

    timed_out = 0;
}


// int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
// 	// user code

// 	//Block이 추가된 영역의 필드값을 바꾼다.
// 	int touched = 0;
// 	for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
// 			// if the cell is filled
// 			// transfer cell info from block to field
//             if (block[currentBlock][blockRotate][i][j]) {
//                 if (0 <= i+blockY && i+blockY < HEIGHT && 0 <= j+blockX && j+blockX < WIDTH) {
//                     f[i + blockY][j + blockX] = 1;

// 					// cell이 1이고 bottom row일 때
//                     if (HEIGHT-1 == i+blockY) touched++;
//                 }
//             }
//         }
//     }
// 	return touched * 10;

// }

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
    int touched = 0; 

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // if the cell is filled
            if (block[currentBlock][blockRotate][i][j] == 1) {
                // transfer cell info from block to field
                f[blockY + i][blockX + j] = 1;
                
                // check if the block cell is on the bottom row or directly above another block
                if (blockY + i >= HEIGHT - 1 || f[blockY + i + 1][blockX + j] == 1) {
                    touched++;
                }
            }
        }
    }

    return touched * 10;  // Return the score (10 points per filled cell)
}


int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int count = 0;
    for (int i = 0; i < HEIGHT; i++) {
        int filled = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (!f[i][j]) {
                filled = 0;
                break;
            }
        }
        if (filled) {
            count++;
			// shift lines above filled down by 1
            for (int y = i; y >= 1; y--) { // from the current row to the first row
                for (int x = 0; x < WIDTH; x++) {
                    f[y][x] = f[y - 1][x];
                }
            }
            i--;
        }
    }
    return count * count * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
    int shadowY = y;
    while (CheckToMove(field, blockID, blockRotate, shadowY + 1, x)) {
        shadowY++;
    }
    DrawBlock(shadowY, x, blockID, blockRotate, '/');

}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
    DrawRecommend(recY, recX, blockID, recR);
    DrawShadow(y, x, blockID, blockRotate);
    DrawBlock(y, x, blockID, blockRotate, ' ');
}

// void createRankList(){
// 	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
// 	// 1. "rank.txt"열기
// 	// 2. 파일에서 랭킹정보 읽어오기
// 	// 3. LinkedList로 저장
// 	// 4. 파일 닫기
// 	FILE *fp;
// 	int i, j;

// 	//1. 파일 열기
// 	fp = fopen("rank.txt", "r");

// 	// 2. 정보읽어오기
// 	/* int fscanf(FILE* stream, const char* format, ...);
// 	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
// 	format: 형식지정자 등등
// 	변수의 주소: 포인터
// 	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
// 	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
// 	if (fscanf() != EOF) {




// 	}
// 	else {


// 	}
// 	// 4. 파일닫기
// 	fclose(fp);
// }

void createRankList() {

    FILE *fp = fopen("rank.txt", "r");
    if (fp == NULL) {
        return;
    }

    fscanf(fp, "%d", &score_number); // number of scores at the top of rank.txt

    for (int i = 0; i < score_number; i++) {
        NODE* pNew = (NODE*)malloc(sizeof(NODE));
        fscanf(fp, "%s %d", pNew->name, &(pNew->score)); // read name and score
        pNew->link = NULL;

        if (head == NULL) { // list is empty
            head = pNew;
        } else {
            NODE* p = head; // p is there to traverse the linked list
            NODE* prev = NULL; // makes insertion easier
            
            while (p != NULL && p->score > pNew->score) { // find the correct pos
                prev = p; 
                p = p->link; 
            }

            if (prev == NULL) { // insert at the head
                pNew->link = head;
                head = pNew;
            } else { // insert in the middle or at the end
                pNew->link = prev->link; // link pNew to the next node
                prev->link = pNew; // link the previous node to pNew

                // ... -> N2 (prev) -> N3 (p) -> ...
                
                // pNew->link = prev->link
                // prev->link는 N3를 point함. 그래서 pNew->link도 이제 N3를 point함

                // prev->link = pNew
                // prev->link는 이제 (N3가 아니라) pNew를 point함
            }
        }
    }

    fclose(fp);
}

// void rank(){
// 	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
// 	//1. 문자열 초기화
// 	int X=1, Y=score_number, ch, i, j;
// 	clear();

// 	//2. printw()로 3개의 메뉴출력


// 	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장


// 	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
// 	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
// 	if (ch == '1') {

// 	}

// 	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
// 	else if ( ch == '2') {
// 		char str[NAMELEN+1];
// 		int check = 0;


// 	}

// 	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
// 	else if ( ch == '3') {
// 		int num;

// 	}
// 	getch();

// }

void rank() {
    int X = 1, Y = score_number, ch;
    char inputX[10], inputY[10];
    clear();

    printw("1. list ranks from X to Y\n");
    printw("2. list ranks by a specific name\n");
    printw("3. delete a specific rank\n");

    ch = wgetch(stdscr);

    switch(ch) {
        case '1':
            echo();
            printw("X: ");
            scanw("%d", &X);
            // if (scanw("%s", inputX) == 1) {
            //     X = atoi(inputX);
            // } else {
            //     X = 1;
            // }
            printw("Y: ");
            scanw("%d", &Y);
            // if (scanw("%s", inputY) == 1) {
            //     Y = atoi(inputY);
            // } else {
            //     Y = 5;
            // }
            noecho();
            
            if (X > Y || X <= 0 || Y <= 0 || X > score_number) {
                printw("\nsearch failure: no rank in the list\n");
            } 
            
            else {
                printw(" name | score\n");
                printw("--------------------------------------------\n");
                NODE* p = head;
                for (int i = 1; i <= Y && p != NULL; i++) {
                    if (i >= X) {
                        printw(" %-17s| %d\n", p->name, p->score);
                    }
                    p = p->link;
                }                
            }
            break;
        case '2':
            searchRankByName();
            break;
        case '3':
            deleteRank();
            break;
        default:
            break;
    }

    getch();
}

void searchRankByName() {
    char str[NAMELEN + 1];
    echo();
    printw("input the name: ");
    scanw("%s", str);
    noecho();

    int found = 0;
    NODE* p = head;
    while (p != NULL) {
        if (strcmp(p->name, str) == 0) {
            printw(" %-17s| %d\n", p->name, p->score);
            found = 1;
        }
        p = p->link;
    }

    if (!found) {
        printw("\nsearch failure: no name in the list\n");
    }
}

void deleteRank() {
    int count;
    echo();
    printw("input the rank: ");
    scanw("%d", &count);
    noecho();

    if (count < 1 || count > score_number || head == NULL) {
        printw("\nsearch failure: the rank not in the list\n");
        return;
    }

    NODE* p = head;
    NODE* prev = NULL;
    for (int i = 1; i < count; i++) {
        prev = p;
        p = p->link;
    }

    if (p == head) { // delete the head node
        head = p->link;
    } else { // delete non-head node
        prev->link = p->link;
    }

    free(p);
    score_number--;

    // Save the updated rank list to rank.txt
    writeRankFile();

    printw("\nresult: the rank deleted\n");
}


// void writeRankFile(){
// 	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
// 	int sn, i;
// 	//1. "rank.txt" 연다
// 	FILE *fp = fopen("rank.txt", "r");

// 	//2. 랭킹 정보들의 수를 "rank.txt"에 기록

// 	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
// 	if ( sn == score_number) return;
// 	else {


// 	}
// 	for ( i= 1; i < score_number+1 ; i++) {
// 		free(a.rank_name[i]);
// 	}
// 	free(a.rank_name);
// 	free(a.rank_score);
// }

void writeRankFile() {
    FILE* fp = fopen("rank.txt", "w");
    if (fp == NULL) {
        return;
    }

    fprintf(fp, "%d\n", score_number);

    NODE* p = head;
    while (p != NULL) {
        fprintf(fp, "%s %d\n", p->name, p->score);
        p = p->link;
    }

    fclose(fp);
}

// void newRank(int score){
// 	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
// 	char str[NAMELEN+1];
// 	int i, j;
// 	clear();
// 	//1. 사용자 이름을 입력받음

// 	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
// 	if() {

// 	}
// 	else {

// 	}
// 	writeRankFile();
// }

void newRank(int score) {
    char str[NAMELEN + 1];
    clear();
    echo();
    printw("input name: ");
    scanw("%s", str);
    noecho();

    NODE* pNew = (NODE*)malloc(sizeof(NODE));
    strcpy(pNew->name, str);
    pNew->score = score;
    pNew->link = NULL;

    if (head == NULL || head->score <= score) {
        pNew->link = head;
        head = pNew;
    } else {
        NODE* p = head;
        while (p->link != NULL && p->link->score > score) {
            p = p->link;
        }
        pNew->link = p->link;
        p->link = pNew;
    }

    score_number++;
    writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
    DrawBlock(y, x, blockID, blockRotate, 'R');
}

// int recommend(RecNode *root){
// 	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

// 	// user code

// 	return max;
// }


int recommend(NodePTR root) {
    RecNode node;
    NodePTR child = &node;
    int maxScore = -1;

    for (int rot = 0; rot < 4; rot++) {
        for (int x = -2; x < 13; x++) {
            if (!CheckToMove(root->recField, nextBlock[root->lv], rot, 0, x)) {
                continue;
            }

            // copy current field to child node
            memcpy(child->recField, root->recField, sizeof(child->recField));
            child->lv = root->lv + 1;


            int y = 0;
            for (; y < HEIGHT; y++) {
                bool collision = false;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (block[nextBlock[root->lv]][rot][i][j]) {
                            if ((y + i >= HEIGHT) || (y + i >= 0 && child->recField[y + i][x + j])) {
                                collision = true;
                                break;
                            }
                        }
                    }
                    if (collision) break;
                }
                if (collision) break;
            }
            y -= 1;


            int score = AddBlockToField(child->recField, nextBlock[root->lv], rot, y, x);
            score += DeleteLine(child->recField);

            // if not at max depth
            if (child->lv < max_depth) {
                score += recommend(child);
            }

            // update max score
            if (score > maxScore) {
                if (root->lv == 0 && (y > recY || score > maxScore)) {
                    recX = x;
                    recY = y;
                    recR = rot;
                }
                maxScore = score;
            }
        }
    }

    return maxScore;
}


void recStart() {
    RecNode node;
    NodePTR root = &node;

    // copy initial field state
    memcpy(root->recField, field, sizeof(root->recField));
    root->lv = 0;

    recX = 0;
    recY = 0;
    recR = 0;

    recommend(root);
}


void recommendedPlay(){
	// user code
}
