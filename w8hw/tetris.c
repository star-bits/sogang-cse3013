﻿#include "tetris.h"
#include <unistd.h>

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
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

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
        blockY++;
        DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
    } else {
        score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);

		if (blockY == -1) {
			gameOver = 1;
		}
        // int gameEnded = 0;
		// for (int i = 0; i < WIDTH; i++) {
    	// 	if (field[0][i] == 1) {
        // 	gameEnded = 1;
        // 	break;
    	// 	}
		// }
        // if (gameEnded) {
        //     gameOver = 1;
        // } 
		else {
            score += DeleteLine(field);
            nextBlock[0] = nextBlock[1];
            nextBlock[1] = nextBlock[2];
            nextBlock[2] = rand() % 7;
            DrawNextBlock(nextBlock);
            blockY = -1;
            blockX = WIDTH / 2 - 2;
            blockRotate = 0;
            PrintScore(score);
        }
        DrawField();
    }
    timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
	int touched = 0;
	for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
			// if the cell is filled
			// transfer cell info from block to field
            if (block[currentBlock][blockRotate][i][j]) {
                if (0 <= i+blockY && i+blockY < HEIGHT && 0 <= j+blockX && j+blockX < WIDTH) {
                    f[i + blockY][j + blockX] = 1;

					// cell이 1이고 bottom row일 때
                    if (HEIGHT-1 == i+blockY) touched++;
                }
            }
        }
    }
	return touched * 10;

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
    DrawShadow(y, x, blockID, blockRotate);
    DrawBlock(y, x, blockID, blockRotate, ' ');
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
