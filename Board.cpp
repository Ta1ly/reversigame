#include"Board.h"
#include <vector>
//#include <fstream>  
using namespace std;
int modelmap[8][8]={};

BlockStatus (*Board::getBoard())[8] {
	return m_board;
}



Board::Board() : isGameOver(false), isFull(false), isnoAvailable(false) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			m_board[i][j] =EMPTY;
		}
	}
	m_board[4][4] = m_board[3][3] = BLACK;
	m_board[3][4] = m_board[4][3] = WHITE;
	m_board[2][4] = m_board[3][5] = m_board[4][2] = m_board[5][3] = AVAILABLE;
}

Board::~Board() {}

/*
/*
* You should complete the functions below.
* The things you could consider:
* 1. Reverse the pieces to Black
* 1. Compute all available position for AI.
* 2. Design an algorithm to make the best choice. And then set corresponding position to "WHITE". Reverse the pieces afterwards.
* 3. Recompute all available position for player and set correspoding position to "AVAILABLE"
*/


void Board::mouseClick(int i, int j) {
	
	m_board[i][j] =BLACK;
	Reverse(i,j);
	checkifOver();	
}


void Board::restart() {
	Board::isFull=false;
	Board::isGameOver=false;
	Board::isnoAvailable=false;
	int i,j;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			m_board[i][j] =EMPTY;
		}
	}
	m_board[4][4] = m_board[3][3] =BLACK;
	m_board[3][4] = m_board[4][3] =WHITE;
	m_board[2][4] = m_board[3][5] = m_board[4][2] = m_board[5][3] =AVAILABLE;
}


void Board::Reverse(int i, int j) {
	int p,q;
	for(p=-1;p<2;p++){
		for(q=-1;q<2;q++){
			Board::Test(i,j,p,q);
		}
	}	
	for (int i=0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j){
			if(m_board[i][j]==AVAILABLE)
			m_board[i][j]=EMPTY;
		}
	}
}


void Board::Test(int i,int j,int m,int n){
	int p,q,r,s,count=0;bool flag=false;
	if(m_board[i][j]==BLACK){
		for(p=i+m,q=j+n;p>=0&&p<8&&q>=0&&q<8;p+=m,q+=n){
			if(m_board[p][q]==EMPTY) return;
			if(m_board[p][q]==BLACK){
			flag=true;
			break;
			}
			count++;
		}
		if(flag){
			for(r=i+m,s=j+n;r!=p||s!=q;r+=m,s+=n){
				m_board[r][s]=BLACK;
			}
		}
	}
	else {
		for(p=i+m,q=j+n;p>=0&&p<8&&q>=0&&q<8;p+=m,q+=n){
			if(m_board[p][q]==EMPTY) return;
			if(m_board[p][q]==WHITE){
			flag=true;
			break;
			}
			count++;
		}
		if(flag){
			for(r=i+m,s=j+n;r!=p||s!=q;r+=m,s+=n){
				m_board[r][s]=WHITE;
			}
		}
	}
}


void Board::modelTest(int i,int j,int m,int n){
	int p,q,r,s,count=0;bool flag=false;
	for(p=i+m,q=j+n;p>=0&&p<8&&q>=0&&q<8;p+=m,q+=n){
		if(modelmap[p][q]==0) return;
		else if(modelmap[p][q]==-1){
			flag=true;
			break;
			}
			count++;
	}
	if(flag){
		for(r=i+m,s=j+n;r!=p||s!=q;r+=m,s+=n){
			modelmap[r][s]=-1;
		}
	}
}


bool Board::checkmodelifAvailable(int i,int j,int m,int n){
	int p=i+m,q=j+n;bool flag=false;
	for(;p>=0&&p<8&&q>=0&&q<8;p+=m,q+=n){
		if(m*m+n*n==0)break;
		else if(flag==false){
				if(modelmap[p][q]==1||modelmap[p][q]==0)	return false;
				else	flag=true;

			}
		else if(modelmap[p][q]==0)	return false;
		else if(modelmap[p][q]==1) return true ;
		else continue;
	}
	return false;
}


void Board::robot() {	
	if(Board::isGameOver==true)return ;
	int point[8][8]={0},i,j,highest=0,hi=0,hj=0,m,n;
	bool flag=false;
	Board::isGameOver=true;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(m_board[i][j]==WHITE) Board::isGameOver=false;
			if(m_board[i][j]==EMPTY) {
				for(m=-1;m<2;m++){
					for(n=-1;n<2;n++){
						if(checkifAvailable(i,j,m,n,1)){
							flag=true;
							point[i][j]=500+countPoint(i,j);
						}
					}
				}
			}
		}
	}
	if(flag){
		for(i=0;i<8;i++){
			for(j=0;j<8;j++){
				if(point[i][j]>highest){
					hi=i;hj=j;
					highest=point[i][j];
				}
			}
		}
		m_board[hi][hj]=WHITE;
		Reverse(hi,hj);
		checkifOver();	
	}
	return ;	
}


void Board::checkifOver(){
	int i,j,status[4]={},m,n;bool flag1=true,flag2=true;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(m_board[i][j]==EMPTY){
				status[0]++;
				for(m=-1;m<2;m++){
					for(n=-1;n<2;n++){
						if(checkifAvailable(i,j,m,n,1)){
							flag1=false;
						}
						if(checkifAvailable(i,j,m,n,2)){
							flag2=false;
						}
					}
				}
			}
			else if(m_board[i][j]==BLACK)status[1]++;
			else if(m_board[i][j]==WHITE)status[2]++;
			else status[3]++;
			
		}
	}
	if((status[0]+status[3])*status[1]*status[2]==0) Board::isGameOver=true;
	if(flag1&&flag2&&(status[3]==0))Board::isGameOver=true;
}


//���������ϲ�ͬλ�ö����Ӱ���Ȩ��ȥ�������λ�� 
//�ο�https://www.dreamwings.cn/reversi/3013.html �и����ļ�ͼ 
/*int Board::countPoint(int i,int j){
	int point[8][8]={{90,-60,10,10,10,10,-60,90},{-60,-80,5,5,5,5,-80,-60},
	{10,5,1,1,1,1,5,10},{10,5,1,1,1,1,5,10},{10,5,1,1,1,1,5,10},{10,5,1,1,1,1,5,10},
	{-60,-80,5,5,5,5,-80,-60},{90,-60,10,10,10,10,-60,90}};
	return point [i][j];
} */

int Board::countPoint(int s,int r){
	int point[8][8]={{90,-60,10,10,10,10,-60,90},{-60,-80,5,5,5,5,-80,-60},
	{10,5,1,1,1,1,5,10},{10,5,1,1,1,1,5,10},{10,5,1,1,1,1,5,10},{10,5,1,1,1,1,5,10},
	{-60,-80,5,5,5,5,-80,-60},{90,-60,10,10,10,10,-60,90}};
	int i=0,j=0,m,n,p,q,sum=0,availablepoint=0;
	bool flag=false;
	copymap();
	modelmap[s][r]=-1;
	for(p=-1;p<2;p++){
		for(q=-1;q<2;q++){
			modelTest(s,r,p,q);
		}
	}
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(modelmap[i][j]==0)
			for(m=-1;m<2;m++){
				for(n=-1;n<2;n++){
					if(checkmodelifAvailable(i,j,m,n)) flag=true;
				}
			}
			if(flag) { 
				if(point[i][j]>sum) sum=point[i][j];
			}
			flag=false;
		}
	}
	return point[s][r]-sum;
}


void Board::copymap(){
	int i,j;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
		if(m_board[i][j]==WHITE)modelmap[i][j]=-1;
		else if(m_board[i][j]==BLACK) modelmap[i][j]=1;
		else modelmap[i][j]=0;
		}
	}
	return ; 
}
	

void Board::setAvailable() {
	int i,j,m,n;
	Board::isFull=true;
	Board::isnoAvailable=true;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(m_board[i][j]==EMPTY){
				Board::isFull=false;
				for(m=-1;m<2;m++){
					for(n=-1;n<2;n++){
						if(checkifAvailable(i,j,m,n,2)){
							m_board[i][j]=AVAILABLE;
							Board::isnoAvailable=false;
						}
					}
				}
			}
		}
	}
	checkifOver();
}



/*  check if point(i,j) could be linked with an available point
    return true when find an available point
    */

bool Board::checkifAvailable(int i,int j,int m,int n,int cmd){
	int p,q;bool flag=false;
	//case : white
	if(cmd==1){
		for(p=i+m,q=j+n;p>=0&&p<8&&q>=0&&q<8;p+=m,q+=n){
			if(flag==false){
				if(m_board[p][q]==WHITE||m_board[p][q]==EMPTY||m_board[p][q]==AVAILABLE)	return false;
				else	flag=true;
			}
			else if(m_board[p][q]==EMPTY)	return false;
			else if(m_board[p][q]==AVAILABLE)	return false;
			else if(m_board[p][q]==WHITE) return true ;
			else continue;
		}
	}
	//case : black
	else if(cmd==2){
		for(p=i+m,q=j+n;p>=0&&p<8&&q>=0&&q<8;p+=m,q+=n){
			if(flag==false){
				if(m_board[p][q]==BLACK||m_board[p][q]==EMPTY||m_board[p][q]==AVAILABLE)	return false;
				else	flag=true;
			}
			else if(m_board[p][q]==EMPTY)	return false;
			else if(m_board[p][q]==AVAILABLE)	return false;
			else if(m_board[p][q]==BLACK) return true ;
			else continue;
		}
	}
	return false;
}

/*bool Board::checkifAvailableWhite(int i,int j,int m,int n){
	int p,q;bool flag=false;
	for(p=i+m,q=j+n;p>=0&&p<8&&q>=0&&q<8;p+=m,q+=n){
		if(flag==false){
//			m_board[7][7]=AVAILABLE;
			if(m_board[p][q]==BLACK)	flag=true;
			else	return false;
		}
		else if(m_board[p][q]==EMPTY)	return false;
		else if(m_board[p][q]==AVAILABLE)	return false;
		else if(m_board[p][q]==BLACK) continue ;
		else {
			return true;
		}
	}
	return false;
}
int p,q,m,n,r,s,result=0;bool flag=false;
	for(m=-1;m<2;m++){
		for(n=-1;n<2;n++){
			for(p=i+m,q=j+n;p>=0&&p<8&&q>=0&&q<8;p+=m,q+=n){
				if(m_board[p][q]==EMPTY) break;
				if(m_board[p][q]==WHITE){
					flag=true;
					break;
				}
			}
			if(flag){
				for(r=i+m,s=j+n;r!=p||s!=q;r+=m,s+=n){
					result+=point[r][s];
				}
			}
			flag=false;	
		}
	}*/	
