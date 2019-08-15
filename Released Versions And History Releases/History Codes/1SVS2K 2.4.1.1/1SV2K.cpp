#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<string>
#include<ctime>
#include<iomanip>
#define NUM_MAXMAPNUM 5
#define NUM_MAXDIFFICULTY 3
#define NUM_MAXPLAYERTYPE 4
#define NUM_MAXABILITYNUM 3
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;
COORD posBegin= {20,7},posPause= {21,9},size= {80,25};
long startTime,nowTime;
void* voidptr;
FILE *maper,*debuger,*scoreboard,*achievement;
int decay=1,bloodkill,historyMaxScore,NUM_KILLERS,NUM_VIEWMINRADIUS=2,Floor=0,
	ability,NUM_VIEWMAXRADIUS,level,beatKillerScore,winGameScore=0,specialScore=0,
													timescore,mapG[22][22][9],debugTimes=0,killerPosX[20],killerPosY[20],
																			  killerPosZ[20],killerDirection[20],trapPosX[40],trapPosY[40],trapPosZ[40],
																			  playerPosX,playerPosY,playerPosZ,direction=2,playerType,gameOver,special=0,
																											   score=0,difficulty=0,killsWithoutHurt=0,killsByBlock=0,killsByTraps=0,
																											   killersAlive,steps[22][22][9],mapID=49+abs(rand()*time(NULL))%NUM_MAXMAPNUM,
																																			 historyX,historyY,historyZ;
bool achieveNotHurt=false,achieveBlock=false,achieveTraps=false,start=false,
	 debugging=false,result,watchable[44][44],killersSleep,paused,debugEnable=false,
	 resetedStep[22][22][9];
unsigned long long achievementsUnlockedNow=0,totalScore;
string mapNames;
COORD posStart= {0,0};
HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);


void _debug(const char* func,int line)
{
	if(!debugEnable)return;
	FILE* fileptr;
	fileptr=fopen(func,"a");
	fprintf(fileptr,"[%2d:%2d:%2d]",nowTime/3600,(nowTime/60)%60,nowTime%60);
	fprintf(fileptr,":%d,playerType=%d",line,playerType);
	fprintf(fileptr,"\n");
	fclose(fileptr);
}

bool color(WORD wAttr)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hConsole == INVALID_HANDLE_VALUE)
		return false;
	return SetConsoleTextAttribute(hConsole, wAttr);
}

void _debug(const char* func,int line,const char *__format,...)
{
	if(!debugEnable)return;
	FILE* fileptr;
	fileptr=fopen(func,"a");
	fprintf(fileptr,"[%2d:%2d:%2d]",nowTime/3600,(nowTime/60)%60,nowTime%60);
	fprintf(fileptr,":%d,",line);
	fprintf(fileptr," message:");
	va_list vpr;
	va_start(vpr,__format);
	vfprintf(fileptr,__format,vpr);
	va_end(vpr);
	fprintf(fileptr,"\n");
	fclose(fileptr);
}

bool isKillerHere(int PosX,int PosY,int layer)
{
	for(int i=0; i<NUM_KILLERS; i++)
		if(PosX==killerPosX[i]&&PosY==killerPosY[i]&&layer==killerPosZ[i])return true;
	return false;
}

int killerHereID(int PosX,int PosY,int layer)
{
	for(int i=0; i<NUM_KILLERS; i++)
		if(PosX==killerPosX[i]&&PosY==killerPosY[i]&&layer==killerPosZ[i])return i;
	return -1;
}

bool isAnotherTrapHere(int PosX,int PosY,int layer,int ID)
{
	for(int i=0; i<=NUM_KILLERS*2; i++)
		if(PosX==trapPosX[i]&&PosY==trapPosY[i]&&layer==trapPosZ[i]&&ID!=i)return true;
	return false;
}

bool isAnotherKillerHere(int PosX,int PosY,int layer,int ID)
{
	for(int i=0; i<NUM_KILLERS; i++)
		if(PosX==killerPosX[i]&&PosY==killerPosY[i]&&layer==killerPosZ[i]
				&&ID!=i)return true;
	return false;
}

bool isPlayerAround(int playerX,int playerY,int playerLayer,int PosX,int PosY,
					int layer)
{
	return playerLayer==layer&&((PosX==playerX&&abs(PosY-playerY)<2)||(PosY==playerY
								&&abs(PosX-playerX)<2));
}

void _debug(const char* func,int line,int x,int y)
{
	if(!debugEnable)return;
	FILE* fileptr;
	fileptr=fopen(func,"a");
	fprintf(fileptr,"              [%2d:%2d:%2d]                  \n",nowTime/3600,
			(nowTime/60)%60,nowTime%60);
	for(int l=1; l<=Floor; l++)
	{
		for(int i=0; i<22; i++)
		{
			for(int j=0; j<22; j++)
			{
				if(playerPosX==i&&playerPosY==j&&l==playerPosZ)
				{
					fprintf(fileptr,"><");
					continue;
				}
				else if(mapG[i][j][l]==-1||((!(isKillerHere(i,j,l)||isAnotherTrapHere(i,j,l,-1)
											   ||watchable[i-playerPosX+22][j-playerPosY+22]))&&(!ability))
						||((!watchable[i-playerPosX+22][j-playerPosY+22])&&ability&&difficulty>0))
				{
					fprintf(fileptr,"■");
					continue;
				}
				else if(isKillerHere(i,j,l))
				{
					fprintf(fileptr,"K ");
					continue;
				}
				else if(isAnotherTrapHere(i,j,l,-1))
				{
					fprintf(fileptr,"O ");
					continue;
				}
				else if(mapG[i][j][l]==1)
				{
					fprintf(fileptr,"□");
					continue;
				}
				else if(mapG[i][j][l]>=2&&mapG[i][j][l]<=9)
				{
					fprintf(fileptr,"%dF",10-mapG[i][j][l]);
					continue;
				}
				else
				{
					if(steps[i][j][l]>=100)fprintf(fileptr,"XX");
					else fprintf(fileptr,"%2d",steps[i][j][l]);
					continue;
				}
			}
			fprintf(fileptr,"\n");
		}
		fprintf(fileptr,"%d楼\n",l);
	}
	fprintf(fileptr,"\n\n\n");
	fclose(fileptr);
}

#define DBG _debug(__FUNCTION__,__LINE__)
#define Debug( e ,...) _debug(__FUNCTION__,__LINE__, e ,#__VA_ARGS__)
#define DEBUG( x , y ) _debug(__FUNCTION__,__LINE__, x , y )

void printAchieveAchievement(int achievementID)
{
	system("cls");
	printf("恭喜：您已解锁成就：\n");
	switch(achievementID)
	{
		case 1:
			printf("            这么简单，有难度吗？\n");
			printf("解锁条件：赢得一局胜利\n");
			break;
		case 2:
			printf("               陷阱是我家\n");
			printf("解锁条件：连续两次用陷阱杀死杀手\n");
			break;
		case 3:
			printf("               没意思，太简单\n");
			printf("解锁条件：赢得一局中等模式\n");
			break;
		case 4:
			printf("               大力士力架\n");
			printf("解锁条件：连续两次用方块杀死杀手\n");
			break;
		case 5:
			printf("               有本事再难点\n");
			printf("解锁条件：赢得一局困难模式\n");
			break;
		case 6:
			printf("              我自带光环怕过谁\n");
			printf("解锁条件：在不被攻击的情况下击杀两位杀手\n");
			break;
		case 7:
			printf("            恕我直言这是个垃圾游戏\n");
			printf("解锁条件：赢得一局地狱模式\n");
			break;
		case 8:
			printf("            一刀斩对我的光环无效\n");
			printf("解锁条件：用默哀赢得一局地狱模式\n");
			break;
		case 9:
			printf("                 痛死个人\n");
			printf("解锁条件：用一起摇摆赢得一局地狱模式\n");
			break;
		case 10:
			printf("                 医者仁心\n");
			printf("解锁条件：用默哀和医生赢得一局地狱模式\n");
			break;
		case 11:
			printf("                 虽败犹荣\n");
			printf("解锁条件：一局地狱模式坚持6分钟以上并失败\n");
			break;
		case 12:
			printf("                 艰苦卓绝\n");
			printf("解锁条件：一局地狱模式坚持6分钟以上并获胜\n");
			break;
		case 13:
			printf("          小学生都可以玩过的游戏\n");
			printf("解锁条件：用默哀和小学生赢得一局地狱模式\n");
	}
	system("pause");
}

void showPlayerType()
{
	system("cls");
	printf("按\"A\"和\"D\"切换角色,按Enter键确认\n");
	switch(playerType)
	{
		case 0:
			system("color 9F");
			printf("                 小学生\n");
			printf("        @@@@@@@@@@@@@@@@@@@@@@@       \n");
			printf("    O@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\     \n");
			printf("   OOO@@@@@@@@@@@@@@@@@@@@@@@@@@@OO`   \n");
			printf("  OOOOOOOOO@@OO@@@@@@@@@@@@@@O@@@OOO   \n");
			printf(" =O@@@@O@@@O@@@@OOOOOOO@OOOOOOOOOOO@\\  \n");
			printf(" @O@@@@@@@@@@@O@OO@@@@OO@OOOOOO@OOOOO` \n");
			printf("=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^ \n");
			printf("=@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\ \n");
			printf("=@@@@@@@@@@@@@@@@@@O@@@@@@@@@@@@@@@@@^ \n");
			printf("=@@@@@@@@O\\O/\\OOOO[\\OOOOOOO@@@@@@@@O@` \n");
			printf(" @O@@@@/**,/OOO]**   *,*]]]]* ,@@@@O^  \n");
			printf(" ,/OO@O^     ***               ,@@O/   \n");
			printf("   OO^ `*  <■>         <■>     OOO    \n");
			printf("   ,^ =            \\\\           =OO     \n");
			printf("      |              \\\\          `        \n");
			printf("      *             ==           /   \n");
			printf("    * =                               \n");
			printf("    ,`**          /====\\     ,`**=    \n");
			printf("      *,**        \\====/     ****=`     \n");
			printf("      \\*\\***               ***        \n");
			printf("技能：无\n");
			printf("简介：只会瞎跑的角色");
			break;
		case 1:
			system("color AC");
			printf("                  码农\n");
			printf("                . @@@@@@@@@@@@@@\\\n");
			printf("              ,@@@@@@@@@@@@@@@@@@@ `\n");
			printf("             ,@@@@@@@@@@@@@@@@@@@@@@`\n");
			printf("           .]@@@@@@@@@@@@@@@@@@@@@@@@\\.\n");
			printf("          .@@@@@@@@@@@@@   @ @@@@@@@   .\n");
			printf("           @ @@@@@@@@@@          @@@@  .\n");
			printf("          . @@@@@@@  =====   ====  @@@ ^.\n");
			printf("           ,@@@@@@@   <■>   <■>    @@/\n");
			printf("            ,@@@@@      /    \\  \\   @`\n");
			printf("           =   @@@      / -- \\    @/\n");
			printf("          ,   @@@@@   /\\      /   @@^\n");
			printf("         /      @@@@@/   \\ /      @@^\n");
			printf("        =@@@@    @@@@            @@@\\___-------`\n");
			printf("        .@@@@@@@@ @@@@          @@      \\    \\\\ \\ \\\n");
			printf("         =@@@@@@@@@@@@@ =======@@\\\\ \\  \\     \\\\ \\.\n");
			printf("         /@@@@@@@@@@@@@@       @^.=\\ ^ \\^ \\   \\\\\\\\\\\\\n");
			printf("      ,    @@@@@@@@@@@@@@@@@@@@`  .\\@ //\\/\\ \\[\\\\\\  \\\n");
			printf("   ./   @@@@@@@@@@@@@@/[/\\ [\\@@@@@@@@@ /\\\\`\\`[\\]\\ ,\n");
			printf(" ,      @@@^.[\\@@@@@   \\ @@@@@@@@@@@@@/  \\\\\\\\``,, \\\n");
			printf(".       @@ `   .   ,\\=  @@@@@@`..[@@@@    // /`,,*/ \n");
			printf("技能：收集/放置方块\n");
			printf("简介：码代码已成习惯的角色");
			break;
		case 2:
			system("color F0");
			printf("                  医生\n");
			printf("                      =@@@@@@@@@@@@@@]\n");
			printf("                     @@@@@@@@@@@@@@@@@\\\n");
			printf("                  /@@@@@@  @@@@@@@@@@@@@\n");
			printf("                 \\@@@@       @@@@@@@@@@@@\n");
			printf("                 @@@@         @@@@@@@@@@@@\n");
			printf("                =@@@---\\* */---@@@@@@@@@@@`\n");
			printf("                =@@@<■>/   <■>@@@@@@@@@@^\n");
			printf("                 @@@   /      @@@@@@@@@@@@^\n");
			printf("                ,@@@@@  /   \\     @@@@@@@@@\n");
			printf("                =@@@@               @@@@@@@`\n");
			printf("                  @@@  \\_____/      @@@@/,\n");
			printf("                  =/@               @@@@@\n");
			printf("                   \\\\           //    @@@       @@@/`\n");
			printf("                      ,[[*\\\\--///        @@@\\\\`\n");
			printf("                      ,=             @@@@\\\\*\n");
			printf("                        ,            @@@@@ `\n");
			printf("                        **           @@@@@@\\\\\n");
			printf("                       =***=         @\\\\@@@@@ \n");
			printf("                        ,**,[       /**=@@@@@^,\n");
			printf("                  ,/`  @  =]       ^  **\\\\\\@@^]]^\n");
			printf("技能：自疗\n");
			printf("简介：习惯治疗别人的角色");
			break;
		case 3:
			system("color 94");
			printf("                  猎人\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("                                                   //\n");
			printf("                                            /]]@/[/\n");
			printf("          ,/@@@@`                   /]/@/[`/\n");
			printf("         @@@@@@@@`          //@@@@@//`\n");
			printf("        =@@@@@@@@^    ,/@@@@/\\@@@@\n");
			printf("        @@@@@@@@@@]@@@@@\\`  =@@@`\n");
			printf("      /@@@@@@@@@@@@@@@`   ,@@@@@`\n");
			printf("   ,@@@@@@@@@@@@@@@@@\\/\\]/@@@@@@\n");
			printf(" ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@^\n");
			printf(" =@@@@@@@@@@@@@@@@@@@@@@@@@@@[\n");
			printf(" \\@@@@@@@@@@@@@@@@@@@@@@/`\n");
			printf("    \\@@@@@@@@@@@@@@@@@@^\n");
			printf("     \\@@@@@@@@@@@@@@@@@\\\n");
			printf("      @@@@@@@@@@@@@@@@@@^\n");
			printf("      =@@@@@@@@@@@@@@@@@@^\n");
			printf("技能：放置陷阱\n");
			printf("简介：习惯杀生的角色");
			break;
		case 4:
			system("color 5D");
			printf("                街头青年\n");
			printf("\n");
			printf("\n");
			printf("                                  /  /\\^\\^]       \n");
			printf("                                /           \\     \n");
			printf("                           ===\\/]________ =* S      \n");
			printf("                         ====== @\\*       =SSSSSS    \n");
			printf("                                  @       SS*SSSSS  \n");
			printf("                                  @      @SSSSSSSS   \n");
			printf("                                 @@@@@@@@@SSSSSSSS    \n");
			printf("                                 @@@@@@ @@@@@@@@    \n");
			printf("                               @@@@         @@@@    \n");
			printf("                           @@@@@            @@@@@    \n");
			printf("                         @@@@@@            @@@@@@     \n");
			printf("                          @@@@@@          @@@@@@@    \n");
			printf("                                    @@@@@@@@@@@@@  \n");
			printf("                                @@@@@@@@@@@@@@@@@  \n");
			printf("                              @@@@@@@@@@@@@@@@@@@  \n");
			printf("                           @@@@@@@@@@@@@@@@@@@@@@    \n");
			printf("                       @@@@@@@@@     @@@@@@@@@@@       \n");
			printf("                      @@@@@@@@@          @@@@@@        \n");
			printf("技能：闪现冲刺\n");
			printf("简介：习惯游走在街头的角色");
			break;
	}
	if(playerType>=(level/10)+1)printf("\n    等级不足！%d级解锁:您现在为%d级。",
										   playerType*10,level);
}

bool choosePlayerTypes()
{
	while(true)
	{
		showPlayerType();
		while(!(KEY_DOWN(VK_ESCAPE)||KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)
				||KEY_DOWN('F')||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_LEFT)));
		if(KEY_DOWN(VK_ESCAPE))return false;
		if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F'))if(playerType<(level/10)+1)return true;
		if((KEY_DOWN('A')||KEY_DOWN(VK_LEFT))&&playerType>0)
		{
			playerType--;
		}
		if((KEY_DOWN('D')||KEY_DOWN(VK_RIGHT))&&playerType<NUM_MAXPLAYERTYPE)
		{
			playerType++;
		}
		_sleep(150);
	}
}

void showAbilities()
{
	system("cls");
	printf("按\"A\"和\"D\"切换技能,按Enter键确认\n");
	switch(ability)
	{
		case -1:
			system("color F0");
			printf("           不选择\n");
			break;
		case 0:
			system("color 08");
			printf("           眼盲\n");
			printf("正面效果：显示杀手位置,陷阱位置\n");
			printf("负面效果：仅可见周围方块");
			break;
		case 1:
			system("color 19");
			printf("          一起摇摆\n");
			printf("正面效果：血量越低杀手移动速度越慢\n");
			printf("负面效果：每走一步降低1血量，每次被攻击后掉血速度翻番");
			break;
		case 2:
			system("color 4C");
			printf("            血债血还\n");
			printf("正面效果：开局增加200血量\n");
			printf("负面效果：杀手每成功攻击一次，攻击力就上升25");
			break;
		case 3:
			system("color 5D");
			printf("             默哀\n");
			printf("正面效果：杀死一位杀手其他杀手默哀5秒(攻击反弹)\n");
			printf("负面效果：默哀后每位杀手攻击力增加50%c(可叠加，无上限)",'%');
	}
}

bool chooseAbilities()
{
	ability=-1;
	while(true)
	{
		showAbilities();
		while(!(KEY_DOWN(VK_ESCAPE)||KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)
				||KEY_DOWN('F')||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_LEFT)));
		if(KEY_DOWN(VK_ESCAPE))return false;
		if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F'))return true;
		if((KEY_DOWN('A')||KEY_DOWN(VK_LEFT))&&ability>-1)ability--;
		if((KEY_DOWN('D')||KEY_DOWN(VK_RIGHT))&&ability<NUM_MAXABILITYNUM)ability++;
		_sleep(150);
	}
}

void setTrap()
{
	int j,p;
	for(int i=0; i<=NUM_KILLERS+3; i++)
	{
		do
		{
			trapPosZ[i]=1+abs(rand()*time(NULL))%Floor;
			p=0;
			do
			{
				trapPosX[i]=1+abs(rand()*time(NULL))%20;
				j=0;
				do
				{
					trapPosY[i]=1+abs(rand()*time(NULL))%20;
					j++;
				}
				while(j<4&&(mapG[trapPosX[i]][trapPosY[i]][trapPosZ[i]]==1
							||mapG[trapPosX[i]][trapPosY[i]][trapPosZ[i]]==-1
							||isAnotherKillerHere(trapPosX[i],trapPosY[i],trapPosZ[i],-1)
							||isPlayerAround(playerPosX,playerPosY,playerPosZ,trapPosX[i],trapPosY[i],
											 trapPosZ[i])||isAnotherTrapHere(trapPosX[i],trapPosY[i],trapPosZ[i],i)));
			}
			while(p<4&&(mapG[trapPosX[i]][trapPosY[i]][trapPosZ[i]]==1
						||mapG[trapPosX[i]][trapPosY[i]][trapPosZ[i]]==-1
						||isAnotherKillerHere(trapPosX[i],trapPosY[i],trapPosZ[i],-1)
						||isPlayerAround(playerPosX,playerPosY,playerPosZ,trapPosX[i],trapPosY[i],
										 trapPosZ[i])||isAnotherTrapHere(trapPosX[i],trapPosY[i],trapPosZ[i],i)));
		}
		while(mapG[trapPosX[i]][trapPosY[i]][trapPosZ[i]]==1
				||mapG[trapPosX[i]][trapPosY[i]][trapPosZ[i]]==-1
				||isAnotherKillerHere(trapPosX[i],trapPosY[i],trapPosZ[i],-1)
				||isPlayerAround(playerPosX,playerPosY,playerPosZ,trapPosX[i],trapPosY[i],
								 trapPosZ[i])||isAnotherTrapHere(trapPosX[i],trapPosY[i],trapPosZ[i],i));
	}
}


void showDifficulties()
{
	system("cls");
	printf("按\"A\"和\"D\"切换难度,按Enter键确认\n");
	switch(difficulty)
	{
		case -2:
			system("color 5D");
			printf("调试模式\n");
			printf("视野：全屏\n");
			printf("杀手数：2");
			break;
		case -1:
			system("color 5D");
			printf("教程模式\n");
			break;
		case 0:
			system("color 2A");
			printf("简单模式\n");
			printf("视野：全屏\n");
			printf("杀手数：2");
			break;
		case 1:
			system("color 6E");
			printf("中等模式\n");
			printf("视野：圆圈（半径10格）\n");
			printf("杀手数：5");
			break;
		case 2:
			system("color C4");
			printf("困难模式\n");
			printf("视野：圆圈（半径5格）\n");
			printf("杀手数：10");
			break;
		case 3:
			system("color 08");
			printf("地狱模式\n");
			printf("视野：1/4扇形（半径5格）\n");
			printf("杀手数：15");
	}
	if(difficulty>=(level/10)+1)printf("\n    等级不足！%d级解锁:您现在为%d级。",
										   difficulty*10,level);
}

void chooseDifficulties()
{
	difficulty=-1;
	while(true)
	{
		showDifficulties();
		while(!(KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)||KEY_DOWN(VK_LEFT)
				||KEY_DOWN(VK_RIGHT)||KEY_DOWN('F')));
		if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F'))
			if((difficulty<(level/10)+1)||(difficulty>-2&&debugEnable))
			{
				system("color F0");
				return;
			}
		if(KEY_DOWN(VK_LEFT)||KEY_DOWN('A'))if(difficulty>-1||(difficulty>-2
												   &&debugEnable))difficulty--;
		if(KEY_DOWN(VK_RIGHT)
				||KEY_DOWN('D'))if(difficulty<NUM_MAXDIFFICULTY)difficulty++;
		_sleep(150);
	}
}

bool readMap()
{
	Floor=0;
	int tmp;
	memset(mapG,-1,sizeof(mapG));
	while(fscanf(maper,"F%d",&tmp)!=EOF)
	{
		Floor=tmp;
		for(int i=1; i<21; i++)
		{
			for(int j=1; j<21; j++)
			{
				fscanf(maper,"%d",&mapG[i][j][Floor]);
				if(mapG[i][j][Floor]>=10)
					return false;
			}
		}
	}
	if(Floor>=1&&Floor<=8)return true;
	return false;
}

bool inMap(int X,int Y)
{
	if(0<X&&X<22&&0<Y&&Y<22)return true;
	return false;
}

void setStep(int posX,int posY,int layer,int stepNum)
{
	if((!inMap(posX,posY))||layer<1||layer>Floor)return;
	if(resetedStep[posX][posY][layer]&&stepNum>=steps[posX][posY][layer])return;
	steps[posX][posY][layer]=stepNum;
	resetedStep[posX][posY][layer]=true;
	if(mapG[posX][posY][layer]==1||mapG[posX][posY][layer]==-1)stepNum+=500;
	if(isAnotherTrapHere(posX,posY,layer,-1))
	{
		if(killersAlive<difficulty+2)
			stepNum+=300+difficulty*50;
		else if(steps[posX][posY][layer]>20-difficulty*5)
			stepNum+=50+difficulty*20;
		else
			stepNum+=90+difficulty*30;
	}
	if(isKillerHere(posX,posY,layer))stepNum+=100+difficulty*15;
	setStep(posX+1,posY,layer,stepNum+1);
	setStep(posX-1,posY,layer,stepNum+1);
	setStep(posX,posY+1,layer,stepNum+1);
	setStep(posX,posY-1,layer,stepNum+1);
	if(mapG[posX][posY][layer]>=2&&mapG[posX][posY][layer]<=9){
		setStep(posX+1,posY,10-mapG[posX][posY][layer],stepNum+6);
		setStep(posX-1,posY,10-mapG[posX][posY][layer],stepNum+6);
		setStep(posX,posY+1,10-mapG[posX][posY][layer],stepNum+6);
		setStep(posX,posY-1,10-mapG[posX][posY][layer],stepNum+6);
	}
	
/*	if(Floor>1)
		for(int i=1; i<=Floor; i++)
		{
			if(mapG[posX][posY][i]==10-layer)
			{
				setStep(posX+1,posY,i,stepNum+6);
				setStep(posX-1,posY,i,stepNum+6);
				setStep(posX,posY+1,i,stepNum+6);
				setStep(posX,posY-1,i,stepNum+6);
			}
		}*/
}

void* resetStepThread(void * vpr)
{
	memset(steps,0x7f7f7f,sizeof(steps));
	while(gameOver>0&&killersAlive>0)
	{
		memset(resetedStep,false,sizeof(resetedStep));
		setStep(playerPosX,playerPosY,playerPosZ,0);
		_sleep(200);
	}
}

void lookAtMap()
{
	double m;
	if(historyX!=playerPosX||historyY!=playerPosY||historyZ!=playerPosZ)
	{
		historyX=playerPosX;
		historyY=playerPosY;
		historyZ=playerPosZ;
		for(int x=-21; x<21; x++)
		{
			for(int y=-21; y<21; y++)
			{
				if(difficulty<=2)
				{
					if(x*x+y*y<=NUM_VIEWMAXRADIUS*NUM_VIEWMAXRADIUS)
						watchable[x+22][y+22]=true;
					else watchable[x+22][y+22]=false;
				}
				else if(x*x+y*y<=NUM_VIEWMINRADIUS*NUM_VIEWMINRADIUS)
				{
					watchable[x+22][y+22]=true;
				}
				else if(mapG[x+playerPosX][y+playerPosY][playerPosZ]
						&&x*x+y*y<=NUM_VIEWMAXRADIUS*NUM_VIEWMAXRADIUS)
				{
					if(steps[x+playerPosX][y+playerPosY][playerPosZ]==abs(x)+abs(y))
						switch(direction)
						{
							case 0://-10<i<0,-10<j<10,PosY=(j/i)*x+playerPosY(i<x<0)
								if(y-playerPosY>=x-playerPosX&&y-playerPosY<=-x+playerPosX)
									watchable[x+22][y+22]=true;
								break;
							case 1://-10<i<10,-10<j<0,PosX=(i/j)*y+playerPosX(j<y<0)
								if(y-playerPosY<=x-playerPosX&&y-playerPosY<=-x+playerPosX)
									watchable[x+22][y+22]=true;
								break;
							case 2://0<i<10,-10<j<10,PosY=(j/i)*x+playerPosY(0<x<i)
								if(y-playerPosY<=x-playerPosX&&y-playerPosY>=-x+playerPosX)
									watchable[x+22][y+22]=true;
								break;
							case 3:
								if(y-playerPosY>=x-playerPosX&&y-playerPosY>=-x+playerPosX)
									watchable[x+22][y+22]=true;
								break;
						}
					else watchable[x+22][y+22]=false;
				}
				else watchable[x+22][y+22]=false;
			}
		}
	}
}

bool run()
{
	memset(watchable,false,sizeof(watchable));
	memset(steps,0x7f7f7f,sizeof(steps));
	lookAtMap();
	paused=false;
	switch(playerType)
	{
		case 1:
			special=difficulty*2+4;
		case 3:
			special=NUM_KILLERS+difficulty;
			break;

	}
	bloodkill=0;
	switch(ability)
	{
		case 0:
			NUM_VIEWMAXRADIUS=NUM_VIEWMINRADIUS=2;
			break;
		case 2:
			bloodkill=1;
			gameOver+=200;
			break;
	}
	start=true;
	pthread_t setStep;
	pthread_create(&setStep,NULL,resetStepThread,voidptr);
	while(gameOver>0&&killersAlive>0)
	{
		if(difficulty>2)
			lookAtMap();
		gameOver-=bloodkill;
		if(playerPosX<=0)playerPosX=1;
		if(playerPosY<=0)playerPosY=1;
		if(playerType==4&&special>0)
		{
			_sleep(150);
			special--;
			if(special==0)special=-60-difficulty*10;
		}
		else _sleep(500);

		char gotcha;
		while(!(KEY_DOWN(VK_ESCAPE)||KEY_DOWN(VK_LSHIFT)||KEY_DOWN('W')||KEY_DOWN('A')
				||KEY_DOWN('S')||KEY_DOWN('D')||KEY_DOWN(VK_SPACE)||KEY_DOWN(VK_LEFT)
				||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_UP)||KEY_DOWN(VK_DOWN)))if(gameOver<=0
							||killersAlive==0)return gameOver>0&&killersAlive==0;
		if(KEY_DOWN(VK_ESCAPE))
		{
			paused=true;
			if(MessageBox(NULL,"你要退出本局游戏吗？","提示",
						  MB_ICONINFORMATION|MB_YESNO)==IDYES)
			{
				gameOver=0;
				paused=false;
				return false;
			}
			paused=false;
		}
		if(KEY_DOWN(VK_UP)||KEY_DOWN('W'))
		{
			direction=0;
			if(mapG[playerPosX-1][playerPosY][playerPosZ]!=1
					&&mapG[playerPosX-1][playerPosY][playerPosZ]!=-1)
				playerPosX--;
		}
		if(KEY_DOWN(VK_LEFT)||KEY_DOWN('A'))
		{
			direction=1;
			if(mapG[playerPosX][playerPosY-1][playerPosZ]!=1
					&&mapG[playerPosX][playerPosY-1][playerPosZ]!=-1)
				playerPosY--;
		}
		if(KEY_DOWN(VK_DOWN)||KEY_DOWN('S'))
		{
			direction=2;
			if(mapG[playerPosX+1][playerPosY][playerPosZ]!=1
					&&mapG[playerPosX+1][playerPosY][playerPosZ]!=-1)
				playerPosX++;
		}
		if(KEY_DOWN(VK_RIGHT)||KEY_DOWN('D'))
		{
			direction=3;
			if(mapG[playerPosX][playerPosY+1][playerPosZ]!=1
					&&mapG[playerPosX][playerPosY+1][playerPosZ]!=-1)
				playerPosY++;
		}
		if(KEY_DOWN(VK_SPACE)||KEY_DOWN('F'))
		{
			switch(playerType)
			{
				case 0:
					break;
				case 1:
					switch(direction)
					{
						case 0:
							if(mapG[playerPosX-1][playerPosY][playerPosZ]==1)
							{
								mapG[playerPosX-1][playerPosY][playerPosZ]=0;
								special++;
								specialScore++;
								break;
							}
							if(mapG[playerPosX-1][playerPosY][playerPosZ]==0&&special)
							{
								mapG[playerPosX-1][playerPosY][playerPosZ]=1;
								special--;
								specialScore++;
								break;
							}
							break;
						case 1:
							if(mapG[playerPosX][playerPosY-1][playerPosZ]==1)
							{
								mapG[playerPosX][playerPosY-1][playerPosZ]=0;
								special++;
								specialScore++;
								break;
							}
							if(mapG[playerPosX][playerPosY-1][playerPosZ]==0&&special)
							{
								mapG[playerPosX][playerPosY-1][playerPosZ]=1;
								special--;
								specialScore++;
								break;
							}
							break;
						case 2:
							if(mapG[playerPosX+1][playerPosY][playerPosZ]==1)
							{
								mapG[playerPosX+1][playerPosY][playerPosZ]=0;
								special++;
								specialScore++;
								break;
							}
							if(mapG[playerPosX+1][playerPosY][playerPosZ]==0&&special)
							{
								mapG[playerPosX+1][playerPosY][playerPosZ]=1;
								special--;
								specialScore++;
								break;
							}
							break;
						case 3:
							if(mapG[playerPosX][playerPosY+1][playerPosZ]==1)
							{
								mapG[playerPosX][playerPosY+1][playerPosZ]=0;
								special++;
								specialScore++;
								break;
							}
							if(mapG[playerPosX][playerPosY+1][playerPosZ]==0&&special)
							{
								mapG[playerPosX][playerPosY+1][playerPosZ]=1;
								special--;
								specialScore++;
								break;
							}
							break;
					}
					break;
				case 2:
					{
						if(gameOver<800-100*difficulty)
						{
							specialScore+=10;
							special+=30+difficulty*10;
							if(special>=100)
							{
								gameOver+=(special/100)*100;
								special%=100;
							}
						}
						break;
					}
				case 3:
					if(special)
					{
						switch(direction)
						{
							case 0:
								if(mapG[playerPosX-1][playerPosY][playerPosZ]==0
										&&!isAnotherTrapHere(playerPosX-1,playerPosY,playerPosZ,-1))
								{
									trapPosX[2*NUM_KILLERS-special]=playerPosX-1;
									trapPosY[2*NUM_KILLERS-special]=playerPosY;
									trapPosZ[2*NUM_KILLERS-special]=playerPosZ;
									special--;
									specialScore++;
								}
								break;
							case 1:
								if(mapG[playerPosX][playerPosY-1][playerPosZ]==0
										&&!isAnotherTrapHere(playerPosX,playerPosY-1,playerPosZ,-1))
								{
									trapPosX[2*NUM_KILLERS-special]=playerPosX;
									trapPosY[2*NUM_KILLERS-special]=playerPosY-1;
									trapPosZ[2*NUM_KILLERS-special]=playerPosZ;
									special--;
									specialScore++;
								}
								break;
							case 2:
								if(mapG[playerPosX+1][playerPosY][playerPosZ]==0
										&&!isAnotherTrapHere(playerPosX+1,playerPosY,playerPosZ,-1))
								{
									trapPosX[2*NUM_KILLERS-special]=playerPosX+1;
									trapPosY[2*NUM_KILLERS-special]=playerPosY;
									trapPosZ[2*NUM_KILLERS-special]=playerPosZ;
									special--;
									specialScore++;
								}
								break;
							case 3:
								if(mapG[playerPosX][playerPosY+1][playerPosZ]==0
										&&!isAnotherTrapHere(playerPosX,playerPosY+1,playerPosZ,-1))
								{
									trapPosX[2*NUM_KILLERS-special]=playerPosX;
									trapPosY[2*NUM_KILLERS-special]=playerPosY+1;
									trapPosZ[2*NUM_KILLERS-special]=playerPosZ;
									special--;
									specialScore++;
								}
								break;
						}
						break;
					}
				case 4:
					if(special!=0)break;
					special=30+difficulty*5;
					specialScore+=10;
					int i=5+difficulty;
					switch(direction)
					{
						case 0:
							for(; playerPosX-i<=0||mapG[playerPosX-i][playerPosY][playerPosZ]==1
									||mapG[playerPosX-i][playerPosY][playerPosZ]==-1; i--);
							playerPosX-=i;
							break;
						case 1:
							for(; playerPosY-i<=0||mapG[playerPosX][playerPosY-i][playerPosZ]==1
									||mapG[playerPosX][playerPosY-i][playerPosZ]==-1; i--);
							playerPosY-=i;
							break;
						case 2:
							for(; playerPosX+i>=22||mapG[playerPosX+i][playerPosY][playerPosZ]==1
									||mapG[playerPosX+i][playerPosY][playerPosZ]==-1; i--);
							playerPosX+=i;
							break;
						case 3:
							for(; playerPosY+i>=22||mapG[playerPosX][playerPosY+i][playerPosZ]==1
									||mapG[playerPosX][playerPosY+i][playerPosZ]==-1; i--);
							playerPosY+=i;
							break;
					}
			}
			if(KEY_DOWN(VK_RETURN))
				if(difficulty==-1)
				{
					killersAlive=0;
					return true;
				}
		}
		switch(playerType)
		{
			case 2:
				if(special>=10)special-=10;
				else special=0;
				break;
		}
		if(mapG[playerPosX][playerPosY][playerPosZ]>=2
				&&mapG[playerPosX][playerPosY][playerPosZ]<=9)
		{
			playerPosZ=10-mapG[playerPosX][playerPosY][playerPosZ];
			if(mapG[playerPosX][playerPosY][playerPosZ]>=2
					&&mapG[playerPosX][playerPosY][playerPosZ]<=9)
				switch(direction)
				{
					case 0:
						playerPosX+=1;
						direction=2;
						break;
					case 1:
						playerPosY+=1;
						direction=3;
						break;
					case 2:
						playerPosX-=1;
						direction=0;
						break;
					case 3:
						playerPosY-=1;
						direction=1;
						break;
				}
			if(playerType==4&&special>0)
			{
				_sleep(150);
				special--;
				if(special==0)special=-60-difficulty*10;
			}
			else _sleep(500);
		}
	}
	if(gameOver<=0||killersAlive>0)
		return false;
	else return true;
}

bool cannotgo(int i,int ID)
{
	switch (i)
	{
		case 0:
			if(!mapG[killerPosX[ID]-1][killerPosY[ID]][killerPosZ[ID]]
					&&!isKillerHere(killerPosX[ID]-1,killerPosY[ID],killerPosZ[ID]))return false;
		case 1:
			if(!mapG[killerPosX[ID]][killerPosY[ID]-1][killerPosZ[ID]]
					&&!isKillerHere(killerPosX[ID],killerPosY[ID]-1,killerPosZ[ID]))return false;
		case 2:
			if(!mapG[killerPosX[ID]+1][killerPosY[ID]][killerPosZ[ID]]
					&&!isKillerHere(killerPosX[ID]+1,killerPosY[ID],killerPosZ[ID]))return false;
		case 3:
			if(!mapG[killerPosX[ID]][killerPosY[ID]+1][killerPosZ[ID]]
					&&!isKillerHere(killerPosX[ID],killerPosY[ID]+1,killerPosZ[ID]))return false;
	}
	return true;
}

int min4(int a,int b,int c,int d)
{
	return min(min(a,b),min(c,d));
}


int killerChooseWhereToGo(int ID)
{
	int Steps[4]= {	steps[killerPosX[ID]-1][killerPosY[ID]][killerPosZ[ID]],
					steps[killerPosX[ID]][killerPosY[ID]-1][killerPosZ[ID]],
					steps[killerPosX[ID]+1][killerPosY[ID]][killerPosZ[ID]],
					steps[killerPosX[ID]][killerPosY[ID]+1][killerPosZ[ID]]
				  };
	int maps[4]= {	mapG[killerPosX[ID]-1][killerPosY[ID]][killerPosZ[ID]],
					mapG[killerPosX[ID]][killerPosY[ID]-1][killerPosZ[ID]],
					mapG[killerPosX[ID]+1][killerPosY[ID]][killerPosZ[ID]],
					mapG[killerPosX[ID]][killerPosY[ID]+1][killerPosZ[ID]]
				 };
	int minDirection=-1,minChoice=INT_MAX,i;
	for(i=0; i<4; i++)
		if(Steps[i]<minChoice&&maps[i]!=-1&&maps[i]!=1)
		{
			minChoice=Steps[i];
			minDirection=i;
		}
	return minDirection;
}

void *killer(void *threadid)
{
	int attackForce,ID=*((int*)threadid),j,k,decay=0,invisibleTime=8-difficulty*1;
	bool alive=true;
	do
	{
		killerPosZ[ID]=1+abs(rand()*time(NULL))%Floor;
		k=0;
		do
		{
			killerPosX[ID]=1+abs(rand()*time(NULL))%20;
			j=0;
			k++;
			do
			{
				killerPosY[ID]=1+abs(rand()*time(NULL))%20;
				j++;
			}
			while(k<4&&((!inMap(killerPosX[ID],killerPosY[ID]))
						||(mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==1
						   ||mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==-1
						   ||isAnotherKillerHere(killerPosX[ID],killerPosY[ID],killerPosZ[ID],ID)
						   ||isPlayerAround(playerPosX,playerPosY,playerPosZ,killerPosX[ID],killerPosY[ID],
											killerPosZ[ID])
						   ||isAnotherTrapHere(killerPosX[ID],killerPosY[ID],killerPosZ[ID],-1))));
		}
		while(k<4&&((!inMap(killerPosX[ID],killerPosY[ID]))
					||(mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==1
					   ||mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==-1
					   ||isAnotherKillerHere(killerPosX[ID],killerPosY[ID],killerPosZ[ID],ID)
					   ||isPlayerAround(playerPosX,playerPosY,playerPosZ,killerPosX[ID],killerPosY[ID],
										killerPosZ[ID])
					   ||isAnotherTrapHere(killerPosX[ID],killerPosY[ID],killerPosZ[ID],-1))));
	}
	while((!inMap(killerPosX[ID],killerPosY[ID]))
			||(mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==1
			   ||mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==-1
			   ||isAnotherKillerHere(killerPosX[ID],killerPosY[ID],killerPosZ[ID],ID)
			   ||isPlayerAround(playerPosX,playerPosY,playerPosZ,killerPosX[ID],killerPosY[ID],
								killerPosZ[ID])
			   ||isAnotherTrapHere(killerPosX[ID],killerPosY[ID],killerPosZ[ID],-1)));
	while(!start);
	_sleep(5000);
	while(gameOver>0&&alive)
	{
		attackForce=90+20*difficulty+(NUM_KILLERS-killersAlive)*(2+difficulty);
		if(invisibleTime>0)
			invisibleTime--;
		while(paused);
		if(ability==1)
			if(gameOver<400)
				decay=(400-gameOver)/100;
		if(killersSleep)
		{
			while(killersSleep)
				if(mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==1
						||mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==-1)
				{
					mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]=0;
					gameOver-=100;
				}
			attackForce=int(1.5*attackForce);
		}
		killerDirection[ID]=killerChooseWhereToGo(ID);
		switch(killerDirection[ID])
		{
			case 0:
				if(mapG[killerPosX[ID]-1][killerPosY[ID]][killerPosZ[ID]]!=1
						&&mapG[killerPosX[ID]-1][killerPosY[ID]][killerPosZ[ID]]!=-1
						&&(!isKillerHere(killerPosX[ID]-1,killerPosY[ID],killerPosZ[ID])))
					killerPosX[ID]--;
				break;
			case 1:
				if(mapG[killerPosX[ID]][killerPosY[ID]-1][killerPosZ[ID]]!=1
						&&mapG[killerPosX[ID]][killerPosY[ID]-1][killerPosZ[ID]]!=-1
						&&(!isKillerHere(killerPosX[ID],killerPosY[ID]-1,killerPosZ[ID])))
					killerPosY[ID]--;
				break;
			case 2:
				if(mapG[killerPosX[ID]+1][killerPosY[ID]][killerPosZ[ID]]!=1
						&&mapG[killerPosX[ID]+1][killerPosY[ID]][killerPosZ[ID]]!=-1
						&&(!isKillerHere(killerPosX[ID]+1,killerPosY[ID],killerPosZ[ID])))
					killerPosX[ID]++;
				break;
			case 3:
				if(mapG[killerPosX[ID]][killerPosY[ID]+1][killerPosZ[ID]]!=1
						&&mapG[killerPosX[ID]][killerPosY[ID]+1][killerPosZ[ID]]!=-1
						&&(!isKillerHere(killerPosX[ID],killerPosY[ID]+1,killerPosZ[ID])))
					killerPosY[ID]++;
				break;
		}
		if(mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]>=2
				&&mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]<=9)
		{
			killerPosZ[ID]=10-mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]];
			if(mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]>=2
					&&mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]<=9)
				switch(killerDirection[ID])
				{
					case 0:
						playerPosX+=1;
						killerDirection[ID]=2;
						break;
					case 1:
						playerPosY+=1;
						killerDirection[ID]=3;
						break;
					case 2:
						playerPosX-=1;
						killerDirection[ID]=0;
						break;
					case 3:
						playerPosY-=1;
						killerDirection[ID]=1;
						break;
				}
			_sleep(3000-difficulty*500);
		}
		_sleep(600-difficulty*100);
		if(isAnotherTrapHere(killerPosX[ID],killerPosY[ID],killerPosZ[ID],-1)
				&&(!invisibleTime))
		{
			killsByTraps++;
			if(killsByBlock>=2&&(achievementsUnlockedNow/8)%2==0)
				achieveBlock=true;
			killsByBlock=0;
			int i;
			for(i=0; !(trapPosX[i]==killerPosX[ID]&&trapPosY[i]==killerPosY[ID]
					   &&trapPosZ[i]==killerPosZ[ID]); i++);
			trapPosX[i]=-1;
			trapPosY[i]=-1;
			trapPosZ[i]=-1;
			killersAlive--;
			killerPosX[ID]=-1;
			killerPosY[ID]=-1;
			killerPosZ[ID]=-1;
			alive=false;
			gameOver+=100;
			return voidptr;
		}
		if(isPlayerAround(playerPosX,playerPosY,playerPosZ,killerPosX[ID],
						  killerPosY[ID],killerPosZ[ID])&&(!invisibleTime))
		{
			gameOver-=attackForce;
			bloodkill*=2;
			if(ability==2)
				attackForce+=25;
			_sleep(15000-difficulty*2000);
			if(killsWithoutHurt>=2&&(achievementsUnlockedNow/32)%2==0)
				achieveNotHurt=true;
			killsWithoutHurt=0;
		}
		if(isPlayerAround(playerPosX,playerPosY,playerPosZ,killerPosX[ID],
						  killerPosY[ID],killerPosZ[ID])&&invisibleTime)
		{
			_sleep(5000);
			invisibleTime=0;
		}
		if((mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==1
				||mapG[killerPosX[ID]][killerPosY[ID]][killerPosZ[ID]]==-1)&&(!invisibleTime))
		{
			killsByBlock++;
			if(killsByTraps>=2&&(achievementsUnlockedNow/2)%2==0)
				achieveTraps=true;
			killsByTraps=0;
			killersAlive--;
			killerPosX[ID]=-1;
			killerPosY[ID]=-1;
			killerPosZ[ID]=-1;
			alive=false;
			gameOver+=100;
			return voidptr;
		}
	}

}

void *showMap(void *id)
{
	system("cls");
	system("color F0");
	printf("              [加载中..]                  \n");
	for(int i=0; i<22; i++)
	{
		for(int j=0; j<22; j++)
		{
			if(playerPosX==i&&playerPosY==j)
			{
				switch(direction)
				{
					case 0:
						printf("%c ",'\030');
						break;
					case 1:
						printf("%c ",'\033');
						break;
					case 2:
						printf("%c ",'\031');
						break;
					case 3:
						printf("%c ",'\032');
						break;
					default:
						exit(0);
				}
				continue;
			}
			else if(mapG[i][j][playerPosZ]>=2&&mapG[i][j][playerPosZ]<=9)
			{
				printf("%dF",10-mapG[i][j][playerPosZ]);
				continue;
			}
			else if(mapG[i][j][playerPosZ]==-1||((!(isKillerHere(i,j,playerPosZ)
													||isAnotherTrapHere(i,j,playerPosZ,-1)
													||watchable[i-playerPosX+22][j-playerPosY+22]))&&(!ability))
					||((!watchable[i-playerPosX+22][j-playerPosY+22])&&ability&&difficulty>0))
			{
				printf("■");
				continue;
			}
			else if(isKillerHere(i,j,playerPosZ))
			{
				if(difficulty==-2)
				{
					switch(killerDirection[killerHereID(i,j,playerPosZ)])
					{
						case 0:
							printf("%c ",'\030');
							break;
						case 1:
							printf("%c ",'\033');
							break;
						case 2:
							printf("%c ",'\031');
							break;
						case 3:
							printf("%c ",'\032');
							break;
						default:
							exit(0);
					}
				}
				else
					printf("K ");
				continue;
			}
			else if(isAnotherTrapHere(i,j,playerPosZ,-1))
			{
				printf("O ");
				continue;
			}
			else if(mapG[i][j][playerPosZ]==1)
			{
				printf("□");
				continue;
			}
			else if(debugEnable&&difficulty==-2)
			{
				if(steps[i][j][playerPosZ]>=100)printf("XX");
				else printf("%2d",steps[i][j][playerPosZ]);
				continue;
			}
			else printf("  ");
		}
		printf("\n");
	}
	if(Floor>1)
		printf("你在%d楼,",playerPosZ);
	printf("剩余杀手数:%3d     ",killersAlive);
	printf("HP:%3d ",gameOver);
	for(int p=0; p<=1+gameOver/200; p++)
		printf("■");
	printf("              \n");
	switch(playerType)
	{
		case 0:
			break;
		case 1:
			printf("可用方块数:%d     ",special);
			break;
		case 2:
			printf("治疗进程:%d%c    ",special,'%');
			for(int p=0; p<special/10; p++)
				printf("■");
			printf("                         ");
			break;
		case 3:
			printf("剩余陷阱数:%d     ",special);
			break;
		case 4:
			if(special<0)
			{
				printf("技能冷却中:");
				for(int p=0; p<-special/6; p++)
					printf("■");
				for(int p=20; p>=-special/6; p--)printf("  ");
			}
			else if(special>0)
			{
				printf("冲刺剩余:");
				for(int p=0; p<special/3; p++)
					printf("■");
				for(int p=20; p>=special/3; p--)printf("  ");
			}
			else printf("闪现冲刺可用。     ");
			break;
		default:
			printf("无效的角色类型%d！",playerType);
	}
	if(gameOver<200)system("color 4C");
	else if(gameOver<300)system("color C0");
	else if(gameOver<400)system("color F0");
	else if(gameOver<500)system("color 2F");
	else system("color 2A");
	startTime=time(NULL);
	while(gameOver>0&&killersAlive>0)
	{
		SetConsoleCursorPosition(hOut,posStart);
		nowTime=time(NULL)-startTime;
		printf("              [%2d:%2d:%2d]                  \n",nowTime/3600,
			   (nowTime/60)%60,nowTime%60);
		for(int i=0; i<22; i++)
		{
			for(int j=0; j<22; j++)
			{
				if(playerPosX==i&&playerPosY==j)
				{
					switch(direction)
					{
						case 0:
							printf("%c ",'\030');
							break;
						case 1:
							printf("%c ",'\033');
							break;
						case 2:
							printf("%c ",'\031');
							break;
						case 3:
							printf("%c ",'\032');
							break;
						default:
							exit(0);
					}
					continue;
				}
				else if(mapG[i][j][playerPosZ]==-1||((!(isKillerHere(i,j,playerPosZ)
														||isAnotherTrapHere(i,j,playerPosZ,-1)
														||watchable[i-playerPosX+22][j-playerPosY+22]))&&(!ability))
						||((!watchable[i-playerPosX+22][j-playerPosY+22])&&ability&&difficulty>0))
				{
					printf("■");
					continue;
				}
				else if(isKillerHere(i,j,playerPosZ))
				{
					printf("K ");
					continue;
				}
				else if(isAnotherTrapHere(i,j,playerPosZ,-1))
				{
					printf("O ");
					continue;
				}
				else if(mapG[i][j][playerPosZ]==1)
				{
					printf("□");
					continue;
				}
				else if(mapG[i][j][playerPosZ]>=2&&mapG[i][j][playerPosZ]<=9)
				{
					printf("%dF",10-mapG[i][j][playerPosZ]);
					continue;
				}
				else if(debugEnable&&difficulty==-2)
				{
					if(steps[i][j][playerPosZ]>=100)printf("XX");
					else printf("%2d",steps[i][j][playerPosZ]);
					continue;
				}
				else printf("  ");
			}
			printf("\n");
		}
		printf("你在%d楼,剩余杀手数:%3d     ",playerPosZ,killersAlive);
		printf("HP:%3d ",gameOver);
		for(int p=0; p<gameOver/100; p++)
			printf("■");
		printf("                      \n");
		switch(playerType)
		{
			case 0:
				break;
			case 1:
				printf("可用方块数:%d     ",special);
				break;
			case 2:
				printf("治疗进程:%d%c    ",special,'%');
				for(int p=0; p<special/10; p++)
					printf("■");
				printf("                         ");
				break;
			case 3:
				printf("剩余陷阱数:%d     ",special);
				break;
			case 4:
				if(special<0)
				{
					printf("技能冷却中:");
					for(int p=0; p<-special/6; p++)
						printf("■");
					for(int p=20; p>=-special/6; p--)printf("  ");
				}
				else if(special>0)
				{
					printf("冲刺剩余:");
					for(int p=0; p<special/3; p++)
						printf("■");
					for(int p=20; p>=special/3; p--)printf("  ");
				}
				else printf("闪现冲刺可用。     ");
				break;
			default:
				printf("无效的角色类型%d！",playerType);
		}
		if(gameOver<200)system("color 4C");
		else if(gameOver<300)system("color C0");
		else if(gameOver<400)system("color F0");
		else if(gameOver<500)system("color 2F");
		else system("color 2A");
		if(playerType==4&&special<0)
		{
			special++;
		}
		_sleep(50);
		while(paused)
		{
			startTime=time(NULL)-nowTime;
		}
	}
	system("cls");
}

void teachingLesson()
{
	system("cls");
	COORD posLesson= {12,10};
	SetConsoleCursorPosition(hOut,posLesson);
	printf("欢迎玩 1 Survivor Versus 2 Killers!  ");
	_sleep(2500);
	SetConsoleCursorPosition(hOut,posLesson);
	printf("在这个游戏里，你要面对一大群杀手。         ");
	_sleep(2500);
	SetConsoleCursorPosition(hOut,posLesson);
	printf("杀手在游戏中会以字符 \"K\"来表示。             ");
	_sleep(2500);
	SetConsoleCursorPosition(hOut,posLesson);
	printf("游戏中你会被作为箭头标记，使用WASD或上下左右移动。     ");
	_sleep(2500);
	SetConsoleCursorPosition(hOut,posLesson);
	printf("游戏中会随机产生陷阱,用字符\"O\"表示。               ");
	_sleep(2500);
	SetConsoleCursorPosition(hOut,posLesson);
	printf("你不会被陷阱伤害,但杀手会被陷阱杀死,同时陷阱被破坏。         ");
	_sleep(2500);
	SetConsoleCursorPosition(hOut,posLesson);
	printf("每名角色都有自己的主动技能,用F或Space激活。             ");
	_sleep(2500);
	SetConsoleCursorPosition(hOut,posLesson);
	printf("你也可以携带被动技能,但同时也要受到被动技能的负面效果。     ");
	_sleep(2500);
	SetConsoleCursorPosition(hOut,posLesson);
	printf("游戏分成四个难度，快来玩吧！                           ");
	_sleep(2500);
}

void offlineSurvivor()
{
	memset(mapG,0x7f,sizeof(mapG));
	special=0,score=0,difficulty=0,killsWithoutHurt=0,killsByBlock=0,killsByTraps=0;
	scoreboard=fopen("playerScoreboard","r");
	achievement=fopen("achievement","r");
	fscanf(achievement,"%lld",&achievementsUnlockedNow);
	fscanf(scoreboard,"%d%lld%d",&historyMaxScore,&totalScore,&level);
	debuger=fopen("DebugOutPut.txt","r");
	if(debuger)
	{
		debugEnable=true;
		fclose(debuger);
		debuger=fopen("DebugOutPut.txt","w");
	}
cd:
	_sleep(1000);
	chooseDifficulties();
	switch(difficulty)
	{
		case -2:
			NUM_KILLERS=2;
			NUM_VIEWMAXRADIUS=30;
			NUM_VIEWMINRADIUS=30;
			break;
		case -1:
			teachingLesson();
			return;
		case 0:
			NUM_KILLERS=2;
			NUM_VIEWMAXRADIUS=30;
			break;
		case 1:
			NUM_KILLERS=5;
			NUM_VIEWMAXRADIUS=10;
			break;
		case 2:
			NUM_KILLERS=10;
			NUM_VIEWMAXRADIUS=5;
			break;
		case 3:
			NUM_KILLERS=15;
			NUM_VIEWMAXRADIUS=5;
	}
	killersAlive=NUM_KILLERS;
ct:
	_sleep(1000);
	if(!choosePlayerTypes())goto cd;
	_sleep(1000);
	if(!chooseAbilities())goto ct;
	system("cls");
	printf("按WASD移动角色,按F键或空格使用技能\n");
	pthread_t threads[20];
	int indexes[20],i,loopTimes,loopTimesZ;
	mapID=49+abs(rand()*time(NULL))%NUM_MAXMAPNUM;
	i=0;
	do
	{
		i++;
		mapNames="map";
		mapNames+=char(mapID);
		mapNames+=".map";
		maper=fopen(mapNames.c_str(),"r");
	}
	while(!maper||i<20);
	if(!readMap())
	{
		printf("错误!地图文件损坏!\n请与我们联系以获得地图文件!\n");
		system("pause");
		exit(-1);
	}
	fclose(maper);
	i=1;
	system("cls");
	printf("加载中...\n");
	playerPosX=0,playerPosY=0,playerPosZ=1;
	do
	{
		playerPosX=1+abs(rand()*time(NULL))%20;
		loopTimes=0;
		loopTimesZ++;
		do
		{
			playerPosY=1+abs(rand()*time(NULL))%20;
			loopTimes++;
		}
		while(mapG[playerPosX][playerPosY][playerPosZ]||loopTimes<5);
	}
	while(mapG[playerPosX][playerPosY][playerPosZ]);
	gameOver=700-75*difficulty;
	pthread_create(&threads[i],NULL,showMap,voidptr);
	setTrap();
	for(i=0; i<NUM_KILLERS; i++)
	{
		indexes[i] = i;
		pthread_create(&threads[i],NULL,killer,(void*)(indexes+i));
	}
	result=run();
	_sleep(1000);
	if(result&&gameOver>0)
	{
		system("color 2A");
		printf("游戏结束，你赢了！\n");
	}
	else
	{
		system("color 4C");
		printf("游戏结束，你输了！\n");
	}
	_sleep(5000);
	system("cls");
	if(result&&gameOver>0)
	{
		if(achievementsUnlockedNow%2==0)
		{
			achievementsUnlockedNow+=1;
			printAchieveAchievement(1);
		}
		else if((achievementsUnlockedNow/4)%2==0&&difficulty==1)
		{
			achievementsUnlockedNow+=4;
			printAchieveAchievement(3);
		}
		else if((achievementsUnlockedNow/16)%2==0&&difficulty==2)
		{
			achievementsUnlockedNow+=16;
			printAchieveAchievement(5);
		}
		else if(difficulty==3)
		{
			if((achievementsUnlockedNow/64)%2==0)
			{
				achievementsUnlockedNow+=64;
				printAchieveAchievement(7);
			}
			if((achievementsUnlockedNow/128)%2==0&&special==3)
			{
				achievementsUnlockedNow+=128;
				printAchieveAchievement(8);
			}
			if((achievementsUnlockedNow/256)%2==0&&special==1)
			{
				achievementsUnlockedNow+=256;
				printAchieveAchievement(9);
			}
			if((achievementsUnlockedNow/512)%2==0&&special==1&&playerType==2)
			{
				achievementsUnlockedNow+=512;
				printAchieveAchievement(10);
			}
			if((achievementsUnlockedNow/2048)%2==0&&time(NULL)-startTime>=360)
			{
				achievementsUnlockedNow+=2048;
				printAchieveAchievement(12);
			}
			if((achievementsUnlockedNow/4096)%2==0&&playerType==0)
			{
				achievementsUnlockedNow+=4096;
				printAchieveAchievement(13);
			}
		}
	}
	else
	{
		if((achievementsUnlockedNow/512)%2==0&&difficulty==3
				&&(time(NULL)-startTime)>=600)
		{
			achievementsUnlockedNow+=512;
			printAchieveAchievement(11);
		}
	}
	if(achieveBlock||(killsByBlock>=2&&(achievementsUnlockedNow/8)%2==0))
	{
		achievementsUnlockedNow+=8;
		printAchieveAchievement(4);
	}
	if(achieveTraps||(killsByTraps>=2&&(achievementsUnlockedNow/2)%2==0))
	{
		achievementsUnlockedNow+=2;
		printAchieveAchievement(2);
	}
	if(achieveNotHurt)
	{
		achievementsUnlockedNow+=32;
		printAchieveAchievement(6);
	}
	if(result)
	{
		timescore=(log(600)-log(time(NULL)-startTime))*500*(1+difficulty);
		winGameScore=3000*(1+difficulty);
	}
	else timescore=log(time(NULL)-startTime)*500*(1+difficulty);
	beatKillerScore=(NUM_KILLERS-killersAlive)*1000*(1+difficulty);
	specialScore*=100*(1+difficulty);
	gameOver*=10*(1+difficulty);
	_sleep(1000);
	system("cls");
	if(gameOver<0)gameOver=0;
	for(score=0; timescore>=50;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d                    \n时间得分：%d                         \n击败杀手得分：%d                     \n技能得分：%d                      \n赢得游戏得分：%d                     \n血量得分：%d                    \n",
			   score,timescore,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=50;
		timescore-=50;
	}
	for(; timescore>0;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：%d                     \n击败杀手得分：%d              \n技能得分：%d              \n赢得游戏得分：%d              \n血量得分：%d              \n",
			   score,timescore,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=1;
		timescore-=1;
	}
	for(; beatKillerScore>=50;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：0                     \n击败杀手得分：%d              \n技能得分：%d              \n赢得游戏得分：%d              \n血量得分：%d              \n",
			   score,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=50;
		beatKillerScore-=50;
	}
	for(; beatKillerScore>0;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：0                  \n击败杀手得分：%d              \n技能得分：%d              \n赢得游戏得分：%d              \n血量得分：%d              \n",
			   score,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=1;
		beatKillerScore-=1;
	}
	for(; specialScore>=50;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：0                 \n击败杀手得分：0              \n技能得分：%d              \n赢得游戏得分：%d              \n血量得分：%d              \n",
			   score,specialScore,winGameScore,gameOver);
		score+=50;
		specialScore-=50;
	}
	for(; specialScore>0;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：0                   \n击败杀手得分：0              \n技能得分：%d              \n赢得游戏得分：%d              \n血量得分：%d              \n",
			   score,specialScore,winGameScore,gameOver);
		score+=1;
		specialScore-=1;
	}
	for(; winGameScore>=50;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：0                  \n击败杀手得分：0              \n技能得分：0              \n赢得游戏得分：%d              \n血量得分：%d              \n",
			   score,winGameScore,gameOver);
		score+=50;
		winGameScore-=50;
	}
	for(; winGameScore>0;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：0              \n击败杀手得分：0              \n技能得分：0              \n赢得游戏得分：%d              \n血量得分：%d              \n",
			   score,winGameScore,gameOver);
		score+=0;
		winGameScore-=0;
	}
	for(; gameOver>=50;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：0              \n击败杀手得分：0              \n技能得分：0              \n赢得游戏得分：0              \n血量得分：%d              \n",
			   score,gameOver);
		score+=50;
		gameOver-=50;
	}
	for(; gameOver>0;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		printf("你的得分：%d              \n时间得分：0              \n击败杀手得分：0              \n技能得分：0              \n赢得游戏得分：0              \n血量得分：%d              \n",
			   score,gameOver);
		score+=1;
		gameOver-=1;
	}
	int scoremiddle=score,print;
	fscanf(scoreboard,"%d%d%d",&historyMaxScore,&totalScore,&level);
	system("cls");
	for(; scoremiddle>0;)
	{
		SetConsoleCursorPosition(hOut,posStart);
		if(historyMaxScore<=score)
		{
			printf("\n      新记录！\n");
			system("color CF");
			historyMaxScore=score;
		}
		printf("你的等级：%d              \n你的本次得分：%d              \n你的最高得分记录：%d              \n%lld/%lld:",
			   level,score,historyMaxScore,totalScore,(long long)1000*pow(1.1,level));
		print=int(10*totalScore/(1000*pow(1.1,level)));
		while(print>0)
		{
			printf("■");
			print--;
		}
		printf("                  ");
		if(scoremiddle>=500)
		{
			totalScore+=500;
			scoremiddle-=500;
		}
		else if(scoremiddle>=50)
		{
			totalScore+=50;
			scoremiddle-=50;
		}
		else totalScore+=1;
		scoremiddle-=1;
		if(totalScore>=(long long)1000*pow(1.1,level))
		{
			totalScore-=(long long)1000*pow(1.1,level);
			level++;
		}
	}
	fclose(achievement);
	fclose(scoreboard);
	achievement=fopen("achievement","w+");
	scoreboard=fopen("playerScoreboard","w+");
	fprintf(scoreboard,"%d %lld %d",historyMaxScore,totalScore,level);
	fprintf(achievement,"%lld",achievementsUnlockedNow);
	printf("\n               \n");
	system("pause");
}


int main()
{
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info= {1,0};
	SetConsoleCursorInfo(hOut, &cursor_info);
	SetConsoleTitle("某人的欢乐小游戏:\"一熊战二狗\"");
	SetConsoleScreenBufferSize(hOut,size);
restart:
	system("color F0");
	system("cls");
	SetConsoleCursorPosition(hOut,posBegin);
	printf("1 Survivor VS 2 Killers");
	SetConsoleCursorPosition(hOut,posPause);
	system("pause");
	if(KEY_DOWN(VK_ESCAPE))if(MessageBox(NULL,"你确定要退出吗？","提示",
											 MB_ICONQUESTION|MB_YESNO)==IDYES)return 0;
	offlineSurvivor();
	goto restart;
	return 0;
}
