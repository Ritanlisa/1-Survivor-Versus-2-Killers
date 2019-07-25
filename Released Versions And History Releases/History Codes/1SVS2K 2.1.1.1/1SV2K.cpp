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
#define NUM_MAXMAPNUM 3
#define NUM_MAXDIFFICULTY 3
#define NUM_MAXPLAYERTYPE 3
#define NUM_MAXABILITYNUM 3
using namespace std;
bool debugEnable=false;

void* voidptr;
FILE *maper,*debuger,*scoreboard,*achievement;
int decay=1,bloodkill,historyMaxScore,NUM_KILLERS=4,NUM_VIEWMINRADIUS=2;
int ability,NUM_VIEWMAXRADIUS,level,totalScore,beatKillerScore,winGameScore=0;
int specialScore=0,timescore,mapG[22][22],debugTimes=0,killerPosX[10],killerPosY[10];
int trapPosX[10*2],trapPosY[10*2],playerPosX,playerPosY,direction=2,playerType,gameOver=300;
int special=0,score=0,difficulty=0,killsWithoutHurt=0,killsByBlock=0,killsByTraps=0;
bool achieveNotHurt=false,achieveBlock=false,achieveTraps=false,start=false,debugging=false,result,watchable[22][22],killersSleep;
long startTime,achievementsUnlockedNow=0;
int killersAlive,steps[22][22],mapID=49+abs(rand()*time(NULL))%NUM_MAXMAPNUM;
char mapNames[9]={'m','a','p',char(mapID),'.','m','a','p','\0'};
long nowTime;
void _debug(const char* func,int line){
	if(!debugEnable)return;
	FILE* fileptr;
	fileptr=fopen(func,"a");
	fprintf(fileptr,"[%2d:%2d:%2d]",nowTime/3600,(nowTime/60)%60,nowTime%60);
	fprintf(fileptr,":%d,killers:%d",line,killersAlive);
	fprintf(fileptr,"\n");
	fclose(fileptr);
}

void __debug(const char* func,int line,const char *__format,...){
	if(!debugEnable)return;
	FILE* fileptr;
	fileptr=fopen(func,"a");
	fprintf(fileptr,"[%2d:%2d:%2d]",nowTime/3600,(nowTime/60)%60,nowTime%60);
	fprintf(fileptr,":%d,killers:%d",line,killersAlive);
	fprintf(fileptr," message:");
	va_list vpr;
	va_start(vpr,__format);
	vfprintf(fileptr,__format,vpr);
	va_end(vpr);
	fprintf(fileptr,"\n");
	fclose(fileptr);
}

bool isKillerHere(int PosX,int PosY)
{
	for(int i=0;i<NUM_KILLERS ;i++)
	if((PosX==killerPosX[i])&&(PosY==killerPosY[i]))return true;
	return false;
}

bool isAnotherTrapHere(int PosX,int PosY,int ID)
{
	for(int i=0;i<=NUM_KILLERS*2;i++)
	if((PosX==trapPosX[i])&&(PosY==trapPosY[i])&&ID!=i)return true;
	return false;
}

bool isAnotherKillerHere(int PosX,int PosY,int ID)
{
	for(int i=0;i<NUM_KILLERS ;i++)
	if((PosX==killerPosX[i])&&(PosY==killerPosY[i])&&ID!=i)return true;
	return false;
}

bool isPlayerAround(int playerX,int playerY,int PosX,int PosY)
{
	return (PosX==playerX&&abs(PosY-playerY)<2)||(PosY==playerY&&abs(PosX-playerX)<2);
}

void ___debug(const char* func,int line,int x,int y){
	if(!debugEnable)return;
	FILE* fileptr;
	fileptr=fopen(func,"a");
	fprintf(fileptr,"[%2d:%2d:%2d]",nowTime/3600,(nowTime/60)%60,nowTime%60);
	fprintf(fileptr,":%d,killers:%d",line,killersAlive);
	fprintf(fileptr,"\n");
	for(int i=0;i<22;i++)
		{
			for(int j=0;j<22;j++)
			{
				if(i==x&&j==y){
					fprintf(fileptr,"X ");
				}
				else if(isKillerHere(i,j))
				{fprintf(fileptr,"K ");
				continue;
				}
				else if(isAnotherTrapHere(i,j,-1))
				{fprintf(fileptr,"O ");
				continue;
				}
				else if(mapG[i][j]>1){
				fprintf(fileptr,"■");}
				else if(mapG[i][j]==1){
				fprintf(fileptr,"□");}
				else if(playerPosX==i&&playerPosY==j)
				switch(direction)
			{
					case 0:fprintf(fileptr,"%c ",'\030');break;
					case 1:fprintf(fileptr,"%c ",'\033');break;
					case 2:fprintf(fileptr,"%c ",'\031');break;
					case 3:fprintf(fileptr,"%c ",'\032');break;
					default:exit(0);
				}
				else if(isKillerHere(i,j)){				
				fprintf(fileptr,"K ");}
				else if(isAnotherTrapHere(i,j,-1)){
				fprintf(fileptr,"O ");}
				else{ fprintf(fileptr,"  ");}
			}
		fprintf(fileptr,"\n");	
		}
	fclose(fileptr);
}

#define DBG _debug(__FUNCTION__,__LINE__)
#define Debug( e ,...) __debug(__FUNCTION__,__LINE__, e ,#__VA_ARGS__)
#define DEBUG( x , y ) ___debug(__FUNCTION__,__LINE__, x , y )

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

void showPlayerType(int playerType)
{
	system("cls");
	printf("按\"A\"和\"D\"切换角色,按Enter键确认\n");
	switch(playerType){
	case 0:
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
	printf("    `o^*,/oo]******        *****  ^       \n");
	printf("技能：无\n");
	printf("简介：只会瞎跑的角色");
	break;
	case 1:
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
	printf("=     @@@@    \\`.  ,\\    @@@/.       =  / / // `,/= \n");     
	printf("技能：收集/放置方块\n");
	printf("简介：码代码已成习惯的角色");
	break;
	case 2:
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
	printf("          *  `  ,@[    @@ *^,`    ^  ***,@/@\\\n");                     
	printf("技能：自疗\n");
	printf("简介：习惯治疗别人的角色");
	break;
	case 3:	
	printf("                  猎人\n");
	printf("\n");
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
	}
}

int choosePlayerTypes()
{	int playerType=0;
	while(true){
	showPlayerType(playerType);
	while(!_kbhit());
		switch(_getch())
		{
			case 'f':
			case 'F':
			case VK_RETURN:return playerType;
			case 'a':
			case 'A':if(playerType>0)playerType--;
			break;
			case 'd':
			case 'D':if(playerType<NUM_MAXPLAYERTYPE)playerType++;
		}
	}
}

void showAbilities()
{
	system("cls");
	printf("按\"A\"和\"D\"切换技能,按Enter键确认\n");
	switch(ability){
	case -1:printf("           不选择\n");
	break;
	case 0:
	printf("           眼盲\n");
	printf("正面效果：显示杀手位置,陷阱位置\n");
	printf("负面效果：仅可见周围方块");
	break;
	case 1:
	printf("          一起摇摆\n");
	printf("正面效果：血量越低杀手移动速度越慢\n");
	printf("负面效果：每走一步降低1血量，每次被攻击后掉血速度翻番");
	break;
	case 2:
	printf("            血债血还\n");
	printf("正面效果：开局增加200血量\n");
	printf("负面效果：杀手每成功攻击一次，攻击力就上升25");
	break;
	case 3:	
	printf("             默哀\n");
	printf("正面效果：杀死一位杀手其他杀手默哀5秒(攻击反弹)\n");
	printf("负面效果：默哀后每位杀手攻击力增加50%c(可叠加，无上限)",'%');
	}
}

int chooseAbilities()
{	ability=-1;
	while(true){
	showAbilities();
	while(!_kbhit());
		switch(_getch())
		{
			case 'f':
			case 'F':
			case VK_RETURN:return ability;
			case 'a':
			case 'A':if(ability>-1)ability--;
			break;
			case 'd':
			case 'D':if(ability<NUM_MAXABILITYNUM)ability++;
		}
	}
}

void setTrap()
{
	int j;
	for(int i=0;i<=NUM_KILLERS+1;i++)
	while(mapG[trapPosX[i]][trapPosY[i]]||isAnotherKillerHere(trapPosX[i],trapPosY[i],-1)||isPlayerAround(playerPosX,playerPosY,trapPosX[i],trapPosY[i])||isAnotherTrapHere(trapPosX[i],trapPosY[i],i))
	{
	trapPosX[i]=1+abs(rand()*time(NULL))%20;
	j=0;
   do
   {
	trapPosY[i]=1+abs(rand()*time(NULL))%20;
	j++;
   }while(j<4&&(mapG[trapPosX[i]][trapPosY[i]]||isAnotherKillerHere(trapPosX[i],trapPosY[i],-1)||isPlayerAround(playerPosX,playerPosY,trapPosX[i],trapPosY[i])||isAnotherTrapHere(trapPosX[i],trapPosY[i],i)));
	}
}


void showDifficulties()
{
	system("cls");
	printf("按\"A\"和\"D\"切换难度,按Enter键确认\n");
	switch(difficulty){
	case -1:
	printf("调试模式\n");
	printf("视野：全屏\n");
	printf("杀手数：自定义");
	system("color 5F");
	break;
	case 0:
	printf("简单模式\n");
	printf("视野：全屏\n");
	printf("杀手数：2");
	system("color 2F");
	break;
	case 1:
	printf("中等模式\n");
	printf("视野：圆圈（半径10格）\n");
	printf("杀手数：4");
	system("color 6F");
	break;
	case 2:
	printf("困难模式\n");
	printf("视野：圆圈（半径5格）\n");
	printf("杀手数：8");
	system("color CF");
	break;
	case 3:	
	printf("地狱模式\n");
	printf("视野：1/4扇形（半径5格）\n");
	printf("杀手数：10");
	system("color 0F");
	}
	if(difficulty>=(level/10)+1)printf("\n    等级不足！%d级解锁:您现在为%d级。",difficulty*10,level);
}

void chooseDifficulties()
{	difficulty=0;
	while(true){
	showDifficulties();
	while(!_kbhit());
		switch(_getch())
		{
			case 'f':
			case 'F':
			case VK_RETURN:if((difficulty<(level/10)+1)||(difficulty>-1&&debugEnable)){
			system("color F0");
			return;
			}
			break;
			case 'a':
			case 'A':if(difficulty>0||(difficulty>-1&&debugEnable))difficulty--;
			break;
			case 'd':
			case 'D':if(difficulty<NUM_MAXDIFFICULTY)difficulty++;
		}
	}
}

/*
void debug(int posLocation)
{
	if((!debugEnable)||(debugging))return;
	debugging=true;
	debugTimes++;
	long nowTime=time(NULL)-startTime;
	fprintf(debuger,"[%2d:%2d:%2d]%s,第%3d次,在第%d排调试,调试输出:\n",nowTime/3600,(nowTime/60)%60,nowTime%60,mapNames,debugTimes,posLocation);
	for(int i=0;i<22;i++)
		{
		for(int j=0;j<22;j++)
			{
				if(mapG[i][j]>=0)
				fprintf(debuger,"%10d ",mapG[i][j]);
				else if(playerPosX==i&&playerPosY==j)
				fprintf(debuger,"P         ");
				else if(isKillerHere(i,j))
				fprintf(debuger,"K         ");
				else if(isAnotherTrapHere(i,j,-1))
				fprintf(debuger,"O         ");
				else fprintf(debuger,"          ");
			}
		fprintf(debuger,"\n");	
		}
		fprintf(debuger,"剩余杀手数:%3d\n",killersAlive);
		fprintf(debuger,"HP:%3d",gameOver);
		for(int p=0;p<gameOver/100;p++)
		fprintf(debuger,"■");
		fprintf(debuger,"\n");
		switch(playerType)
		{
		case 0:break;
		case 1:fprintf(debuger,"可用方块数:%d \n", special); break;
		case 2:fprintf(debuger,"治疗进程:%d%c\n",special,'%');
		for(int p=0;p<special/10;p++)
			fprintf(debuger,"");
		break;
		}
		
	fprintf(debuger,"\nPlayer:(%d,%d)\n",playerPosX,playerPosY);
		for(int i=0;i<22;i++)
		{
		for(int j=0;j<22;j++)
			{if(watchable[i][j])
				fprintf(debuger,"■");
				else fprintf(debuger,"□");
			}
		fprintf(debuger,"\n\n");	
		}
		for(int i=0;i<22;i++)
		{
		for(int j=0;j<22;j++)
				fprintf(debuger,"%10d ",steps[i][j]);
		fprintf(debuger,"\n");	
		}

	debugging=false;
}
*/

bool readMap()
{
	for(int i=1;i<21;i++)
	{
		
		for(int j=1;j<21;j++)
		{
			
			fscanf(maper,"%d",&mapG[i][j]);
			if(mapG[i][j]>=10)
			return false;
		}
	}
return true;
}

void setStep(int posX,int posY,int stepNum)
{
	if(stepNum>=steps[posX][posY])return;
	if(mapG[posX][posY])return;
	if(isAnotherTrapHere(posX,posY,-1))stepNum+=20;
	if(isKillerHere(posX,posY))stepNum+=40;
	steps[posX][posY]=stepNum;
	setStep(posX+1,posY,stepNum+1);
	setStep(posX-1,posY,stepNum+1);
	setStep(posX,posY+1,stepNum+1);
	setStep(posX,posY-1,stepNum+1);
}

bool run()
{
	switch(playerType)
	{
		case 3:
			special=NUM_KILLERS-1;
			break;
			
	}
	switch(ability)
	{
		case 0:
			NUM_VIEWMAXRADIUS=NUM_VIEWMINRADIUS=2;
			break;
		case 2:
			gameOver+=200;
			break;
	}
	system("pause");
	start=true;
	bloodkill=0;
	switch(ability)
	{
		case 1:
			bloodkill=1;
			break;
	}
	while(gameOver>0&&killersAlive>0)
	{
		gameOver-=bloodkill;
		if(playerPosX<=0)playerPosX=1;
		if(playerPosY<=0)playerPosY=1;
		_sleep(1000);
		char gotcha;
		while(!_kbhit());
		switch(_getch())
		{
			case VK_LSHIFT:
			debugEnable=true;
			break;
			case 'w':
			case 'W':
			case VK_UP:
			direction=0;
			if(!mapG[playerPosX-1][playerPosY])
			playerPosX--;
			break;
			case 'a':
			case 'A':
			case VK_LEFT:
			direction=1;
			if(!mapG[playerPosX][playerPosY-1])
			playerPosY--;
			break;
			case 's':
			case 'S':
			case VK_DOWN:
			direction=2;
			if(!mapG[playerPosX+1][playerPosY])
			playerPosX++;
			break;
			case 'd':
			case 'D':
			case VK_RIGHT:
			direction=3;
			if(!mapG[playerPosX][playerPosY+1])
			playerPosY++;
			break;
			case 'f':
			case 'F':
			case VK_SPACE:
			switch(playerType)
			{
				case 0:break;
				case 1:switch(direction)
						{
							case 0:if(mapG[playerPosX-1][playerPosY]==1)
									{
									mapG[playerPosX-1][playerPosY]=0;
									special++;
									specialScore++;
									break;
									}
								   if(mapG[playerPosX-1][playerPosY]==0&&special)
								   {
								   mapG[playerPosX-1][playerPosY]=1;
									special--;
									specialScore++;
								   break;
									}
								   break;
							case 1:if(mapG[playerPosX][playerPosY-1]==1)
									{
									mapG[playerPosX][playerPosY-1]=0;
									special++;
									specialScore++;
								   break;
									}
								   if(mapG[playerPosX][playerPosY-1]==0&&special)
								   {
								   mapG[playerPosX][playerPosY-1]=1;
									special--;
									specialScore++;
								   break;
									}
								   break;
							case 2:if(mapG[playerPosX+1][playerPosY]==1)
									{
									mapG[playerPosX+1][playerPosY]=0;
									special++;
									specialScore++;
								   break;
									}
								   if(mapG[playerPosX+1][playerPosY]==0&&special)
								   {
								   mapG[playerPosX+1][playerPosY]=1;
									special--;
									specialScore++;
								   break;
									}
								   break;
							case 3:if(mapG[playerPosX][playerPosY+1]==1)
									{
									mapG[playerPosX][playerPosY+1]=0;
									special++;
									specialScore++;
								   break;
									}
								   if(mapG[playerPosX][playerPosY+1]==0&&special)
								   {
								   mapG[playerPosX][playerPosY+1]=1;
									special--;
									specialScore++;
									break;
									}
						break;
						}break;
				case 2:{
					if(gameOver<400)
					{
					specialScore++;
				if(special<85)
				special+=35;
				else{
					gameOver+=100;
					special=0;
				}}
				break;
				}
				case 3:if(special)
					{switch(direction)
						{
							case 0:if(mapG[playerPosX-1][playerPosY]==0&&!isAnotherTrapHere(playerPosX-1,playerPosY,-1))
									{
									trapPosX[2*NUM_KILLERS-special]=playerPosX-1;
									trapPosY[2*NUM_KILLERS-special]=playerPosY;
									special--;
									specialScore++;
									}
								   break;
							case 1:if(mapG[playerPosX][playerPosY-1]==0&&!isAnotherTrapHere(playerPosX,playerPosY-1,-1))
									{
									trapPosX[2*NUM_KILLERS-special]=playerPosX;
									trapPosY[2*NUM_KILLERS-special]=playerPosY-1;
									special--;
									specialScore++;
									}
								   break;
							case 2:if(mapG[playerPosX+1][playerPosY]==0&&!isAnotherTrapHere(playerPosX+1,playerPosY,-1))
									{
									trapPosX[2*NUM_KILLERS-special]=playerPosX+1;
									trapPosY[2*NUM_KILLERS-special]=playerPosY;
									special--;
									specialScore++;
									}
								   break;
							case 3:if(mapG[playerPosX][playerPosY+1]==0&&!isAnotherTrapHere(playerPosX,playerPosY+1,-1))
									{
									trapPosX[2*NUM_KILLERS-special]=playerPosX;
									trapPosY[2*NUM_KILLERS-special]=playerPosY+1;
									special--;
									specialScore++;
									}
						break;
						}break;
					}
			}
			case VK_RETURN:
			if(difficulty==-1)
			{	
			killersAlive=0;
			return true;
			}
		}
		
		switch(playerType)
		{
		
			case 2:if(special>=10)special-=10;
			else special=0;
			break;
		}
		memset(steps,0x7f7f7f,sizeof(steps));
		setStep(playerPosX,playerPosY,1);
	}
	if(gameOver<=0)
	return false;
	else return true;
}



bool cannotgo(int i,int ID)
{
	switch (i){
		case 0:if(!mapG[killerPosX[ID]-1][killerPosY[ID]]&&!isKillerHere(killerPosX[ID]-1,killerPosY[ID]))return false;
		case 1:if(!mapG[killerPosX[ID]][killerPosY[ID]-1]&&!isKillerHere(killerPosX[ID],killerPosY[ID]-1))return false;
		case 2:if(!mapG[killerPosX[ID]+1][killerPosY[ID]]&&!isKillerHere(killerPosX[ID]+1,killerPosY[ID]))return false;
		case 3:if(!mapG[killerPosX[ID]][killerPosY[ID]+1]&&!isKillerHere(killerPosX[ID],killerPosY[ID]+1))return false;
	}return true;
}

int min4(int a,int b,int c,int d)
{
	return min(min(a,b),min(c,d));
}


int killerChooseWhereToGo(int ID)
{
	int choice[4]={steps[killerPosX[ID]-1][killerPosY[ID]],steps[killerPosX[ID]][killerPosY[ID]-1],steps[killerPosX[ID]+1][killerPosY[ID]],steps[killerPosX[ID]][killerPosY[ID]+1]};
	int minDirection=-1,minChoice=2139062143,i;
	for(i=0;i<4;i++)
		if(choice[i]<minChoice)
		{
			minChoice=choice[i];
			minDirection=i;
		}
	return minDirection;
}


void *killer(void *threadid)
{
	int attackForce=100,ID=*((int*)threadid),moving=0,j,decay=1;
	bool alive=true;
	while(mapG[killerPosX[ID]][killerPosY[ID]]||isAnotherKillerHere(killerPosX[ID],killerPosY[ID],ID)||isPlayerAround(playerPosX,playerPosY,killerPosX[ID],killerPosY[ID])||isAnotherTrapHere(killerPosX[ID],killerPosY[ID],-1))
	{
	killerPosX[ID]=1+abs(rand()*time(NULL))%20; 
	j=0; 
   do
   {
	killerPosY[ID]=1+abs(rand()*time(NULL))%20; 
	j++; 
   }while(j<4&&(mapG[killerPosX[ID]][killerPosY[ID]]||isAnotherKillerHere(killerPosX[ID],killerPosY[ID],ID)||isPlayerAround(playerPosX,playerPosY,killerPosX[ID],killerPosY[ID])||isAnotherTrapHere(killerPosX[ID],killerPosY[ID],-1)));
	}
	while(!start); 
	while(gameOver>0&&alive)
	{ 
		if(ability==1)
			if(gameOver>=400) 
				decay=0;
			else
				decay=(400-gameOver)/100;
		if(killersSleep)
		{ 
			while(killersSleep)
			if(mapG[killerPosX[ID]][killerPosY[ID]]){ 
			mapG[killerPosX[ID]][killerPosY[ID]]=0;gameOver-=100;
			}attackForce=int(1.5*attackForce);
		}
	moving=killerChooseWhereToGo(ID); 
		_sleep(decay*1000); 
		switch(moving)
		{
			case 0: 
			if(!mapG[killerPosX[ID]-1][killerPosY[ID]]&&!isKillerHere(killerPosX[ID]-1,killerPosY[ID]))
			{
			killerPosX[ID]--;
			break; 
			}
			case 1:
			if(!mapG[killerPosX[ID]][killerPosY[ID]-1]&&!isKillerHere(killerPosX[ID],killerPosY[ID]-1))
			killerPosY[ID]--; 
			break;
			case 2:
			if(!mapG[killerPosX[ID]+1][killerPosY[ID]]&&!isKillerHere(killerPosX[ID]+1,killerPosY[ID]))
			killerPosX[ID]++; 
			break;
			case 3:
			if(!mapG[killerPosX[ID]][killerPosY[ID]+1]&&!isKillerHere(killerPosX[ID],killerPosY[ID]+1))
			killerPosY[ID]++; 
			break;
		}
		if(isPlayerAround(playerPosX,playerPosY,killerPosX[ID],killerPosY[ID]))
		{
			gameOver-=attackForce; 
			bloodkill*=2; 
			if(ability==2)
				attackForce+=25; 
			_sleep(20000*decay);
			if(killsWithoutHurt>=2&&(achievementsUnlockedNow/32)%2==0)
				achieveNotHurt=true;
			killsWithoutHurt=0;
		}
		if(mapG[killerPosX[ID]][killerPosY[ID]]||isAnotherTrapHere(killerPosX[ID],killerPosY[ID],-1))
		{
			if(isAnotherTrapHere(killerPosX[ID],killerPosY[ID],-1))
			{ 
			killsByTraps++;
			if(killsByBlock>=2&&(achievementsUnlockedNow/8)%2==0)
				achieveBlock=true;
			killsByBlock=0;
			int i;
			for(i=0;!(trapPosX[i]==killerPosX[ID]&&trapPosY[i]==killerPosY[ID]);i++);
			trapPosX[i]=-1;
			trapPosY[i]=-1;
			}
			else 
			killsByBlock++;
			if(killsByTraps>=2&&(achievementsUnlockedNow/2)%2==0)
				achieveTraps=true;
			killsByTraps=0;
			killersAlive--;
			killerPosX[ID]=-1;
			killerPosY[ID]=-1;
			alive=false;
			gameOver+=100;
			return voidptr;
			}
			}
		moving=killerChooseWhereToGo(ID);
		_sleep(decay*1000);
		switch(moving)
		{
			case 0:
			if(!mapG[killerPosX[ID]-1][killerPosY[ID]]&&!isKillerHere(killerPosX[ID]-1,killerPosY[ID]))
			{
			killerPosX[ID]--;
			break;
			}
			case 1:
			if(!mapG[killerPosX[ID]][killerPosY[ID]-1]&&!isKillerHere(killerPosX[ID],killerPosY[ID]-1))
			killerPosY[ID]--;
			break;
			case 2:
			if(!mapG[killerPosX[ID]+1][killerPosY[ID]]&&!isKillerHere(killerPosX[ID]+1,killerPosY[ID]))
			killerPosX[ID]++;
			break;
			case 3:
			if(!mapG[killerPosX[ID]][killerPosY[ID]+1]&&!isKillerHere(killerPosX[ID],killerPosY[ID]+1))
			killerPosY[ID]++;
			break;
		}
		if(isPlayerAround(playerPosX,playerPosY,killerPosX[ID],killerPosY[ID]))
		{
			gameOver-=attackForce;
			bloodkill*=2;
			if(ability==2)
				attackForce+=25;
			_sleep(2000*decay);
			if(killsWithoutHurt>=2&&(achievementsUnlockedNow/32)%2==0)
				achieveNotHurt=true;
			killsWithoutHurt=0;
		}
		if(mapG[killerPosX[ID]][killerPosY[ID]]||isAnotherTrapHere(killerPosX[ID],killerPosY[ID],-1))
		{
			if(isAnotherTrapHere(killerPosX[ID],killerPosY[ID],-1))
			{
			killsByTraps++;
			if(killsByBlock>=2&&(achievementsUnlockedNow/8)%2==0)
				achieveBlock=true;
			killsByBlock=0;
			int i;
			for(i=0;!(trapPosX[i]==killerPosX[ID]&&trapPosY[i]==killerPosY[ID]);i++);
			trapPosX[i]=-1;
			trapPosY[i]=-1;
			}
			else 
			killsByBlock++;
			if(killsByTraps>=2&&(achievementsUnlockedNow/2)%2==0)
				achieveTraps=true;
			killsByTraps=0;
			killersAlive--;
			killerPosX[ID]=-1;
			killerPosY[ID]=-1;
			alive=false;
			gameOver+=100;
			}
}

bool inMap(int X,int Y)
{
	if(0<X&&X<22&&0<Y&&Y<22)return true;
	return false;
}

void lookAtMap()
{
	double m;
	memset(watchable,false,sizeof(watchable));
	for(int i=-NUM_VIEWMAXRADIUS;i<=NUM_VIEWMAXRADIUS;i++)
		for(int j=-NUM_VIEWMAXRADIUS;j<=NUM_VIEWMAXRADIUS;j++)
			{if(!inMap(playerPosX+i,playerPosY+j))continue;
			if(difficulty<=2){if(sqrt(i*i+j*j)<=NUM_VIEWMAXRADIUS)
			watchable[playerPosX+i][playerPosY+j]=true;
			continue;}
			if(sqrt(i*i+j*j)<=NUM_VIEWMINRADIUS){watchable[playerPosX+i][playerPosY+j]=true;continue;}
				else if((steps[playerPosX+i][playerPosY+j]<100||mapG[playerPosX+i][playerPosY+j])&&sqrt(i*i+j*j)<=NUM_VIEWMAXRADIUS)
				switch(direction)
				{
					case 0://-10<i<0,-10<j<10,PosY=(j/i)*x+playerPosY(i<x<0)
						if(j>=i&&j<=-i)
						watchable[playerPosX+i][playerPosY+j]=true;
						break;
					case 1://-10<i<10,-10<j<0,PosX=(i/j)*y+playerPosX(j<y<0)
						if(j<=i&&j<=-i)
						watchable[playerPosX+i][playerPosY+j]=true;
						break;
					case 2://0<i<10,-10<j<10,PosY=(j/i)*x+playerPosY(0<x<i)
						if(j<=i&&j>=-i)
						watchable[playerPosX+i][playerPosY+j]=true;
						break;
					case 3:
						if(j>=i&&j>=-i)
						watchable[playerPosX+i][playerPosY+j]=true;
						break;
				}


}
}

void *showMap(void *id)
{
	while(gameOver>0&&killersAlive>0)
	{
		system("cls");
		lookAtMap();
		nowTime=time(NULL)-startTime;
		printf("            [%2d:%2d:%2d]\n",nowTime/3600,(nowTime/60)%60,nowTime%60);
		for(int i=0;i<22;i++)
		{
			for(int j=0;j<22;j++)
			{
				if(isKillerHere(i,j)&&!ability)
				{printf("K ");
				continue;
				}
				else if(isAnotherTrapHere(i,j,-1)&&!ability&&!(i==0&&j==0))
				{printf("O ");
				continue;
				}
				else if(!watchable[i][j]||mapG[i][j]>1){
				printf("■");}
				else if(mapG[i][j]==1){
				printf("□");}
				else if(playerPosX==i&&playerPosY==j)
				switch(direction)
			{
					case 0:printf("%c ",'\030');break;
					case 1:printf("%c ",'\033');break;
					case 2:printf("%c ",'\031');break;
					case 3:printf("%c ",'\032');break;
					default:exit(0);
				}
				else if(isKillerHere(i,j)){				
				printf("K ");}
				else if(isAnotherTrapHere(i,j,-1)){
				printf("O ");}
				else{ printf("  ");}
			}
		printf("\n");	
		}
		printf("剩余杀手数:%3d     ",killersAlive);
		printf("HP:%3d",gameOver);
		for(int p=0;p<gameOver/100;p++)
		printf("■");
		printf("\n");
		switch(playerType)
		{
		case 0:break;
		case 1:printf("可用方块数:%d ",special); break;
		case 2:printf("治疗进程:%d%c",special,'%');
		for(int p=0;p<special/10;p++)
			printf("■");break;
		case 3:printf("剩余陷阱数:%d ",special);break;
		}
		if(gameOver==0)system("color F4");
		if(gameOver==100)system("color 4C");
		if(gameOver==200)system("color C0");
		if(gameOver>=300)system("color F0");
		_sleep(1000);
	}
}
 

int main()
{
	system("title 某人的欢乐小游戏:\"一熊战二狗\"");
	system("color F0");
	memset(mapG,0x7f,sizeof(mapG));
	scoreboard=fopen("playerScoreboard","r");
	achievement=fopen("achievement","r");
	fscanf(achievement,"%lld",&achievementsUnlockedNow);
	fscanf(scoreboard,"%d%d%d",&historyMaxScore,&totalScore,&level);
	debuger=fopen("DebugOutPut.txt","r");
	if(debuger)
	{
		debugEnable=true;
		fclose(debuger);
		debuger=fopen("DebugOutPut.txt","w");
	}
	chooseDifficulties();
	switch(difficulty)
	{
		case -1:
			system("cls");
			printf("请输入杀手数:"); 
			scanf("%d",&NUM_KILLERS);
			printf("请输入最大视野半径:"); 
			scanf("%d",&NUM_VIEWMAXRADIUS);
			printf("请输入最小视野半径:"); 
			scanf("%d",&NUM_VIEWMINRADIUS);
			printf("已完成自定义初始化:杀手数:%d 最大视野半径:%d 最小视野半径:%d \n",NUM_KILLERS,NUM_VIEWMAXRADIUS,NUM_VIEWMINRADIUS); 
			system("pause");
			break;
		case 0:
			NUM_KILLERS=2;
			NUM_VIEWMAXRADIUS=30;
			break;
		case 1:
			NUM_KILLERS=4;
			NUM_VIEWMAXRADIUS=10;
			break;
		case 2:
			NUM_KILLERS=8;
			NUM_VIEWMAXRADIUS=5;
			break;
		case 3:
			NUM_KILLERS=10;
			NUM_VIEWMAXRADIUS=5;
	}
	if(difficulty>=0)
		killersAlive=NUM_KILLERS;
	else 
		killersAlive=1;
	playerType=choosePlayerTypes();
	chooseAbilities(); 
   pthread_t threads[10+1+1];
   int indexes[10+1],i,loopTimes;
   mapID=49+abs(rand()*time(NULL))%NUM_MAXMAPNUM;
    do{
   maper=fopen(mapNames,"r");
	}while(!maper);
    
   if(!readMap())
   {
   		printf("Error!map Was Damaged!\nConnect with us to get the map!\n");
   		system("pause");
   		exit(-1);
   }
    
   while(mapG[playerPosX][playerPosY]){
   playerPosX=1+abs(rand()*time(NULL))%20;
   loopTimes=0;
    
   do
   {
	playerPosY=1+abs(rand()*time(NULL))%20;
	loopTimes++;
   }while(mapG[playerPosX][playerPosY]&&loopTimes<5);
   }
    
    setTrap();
    
   for(i=0;i<NUM_KILLERS;i++)
   {
      indexes[i] = i; 
    pthread_create(&threads[i],NULL,killer,(void *)&(indexes[i]));
   }i=1;
	system("cls"); 
	printf("按WASD移动角色,按F键或空格使用技能\n");
	pthread_create(&threads[i],NULL,showMap,(void *)&(indexes[i]));
    startTime=time(NULL);
	result=run();
	if(result&&gameOver>0)
	{
	_sleep(1000);
	system("cls");
	printf("你赢了！\n");
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
	else 
	{
	_sleep(1000);
	system("cls");
	printf("你输了！\n");
	if((achievementsUnlockedNow/512)%2==0&&difficulty==3&&(time(NULL)-startTime)>=600)
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
	if(result){
	timescore=(log(600)-log(time(NULL)-startTime))*50*(10+difficulty);
	winGameScore=3000*(1+difficulty);
	}else timescore=log(time(NULL)-startTime)*50*(10+difficulty);
	beatKillerScore=(NUM_KILLERS-killersAlive)*100*(10+difficulty);
	specialScore*=10+difficulty;
	gameOver*=100*(1+difficulty);
	system("pause");
	for(score=0;timescore>=50;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：%d\n击败杀手得分：%d\n技能得分：%d\n赢得游戏得分：%d\n血量得分：%d\n",score,timescore,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=50;
		timescore-=50;}
	for(;timescore>0;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：%d\n击败杀手得分：%d\n技能得分：%d\n赢得游戏得分：%d\n血量得分：%d\n",score,timescore,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=1;
		timescore-=1;}
	for(;beatKillerScore>=50;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：0\n击败杀手得分：%d\n技能得分：%d\n赢得游戏得分：%d\n血量得分：%d\n",score,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=50;
		beatKillerScore-=50;}
	for(;beatKillerScore>0;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：0\n击败杀手得分：%d\n技能得分：%d\n赢得游戏得分：%d\n血量得分：%d\n",score,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=1;
		beatKillerScore-=1;}
	for(;specialScore>=50;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：0\n击败杀手得分：0\n技能得分：%d\n赢得游戏得分：%d\n血量得分：%d\n",score,specialScore,winGameScore,gameOver);
		score+=50;
		specialScore-=50;}
	for(;specialScore>0;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：0\n击败杀手得分：0\n技能得分：%d\n赢得游戏得分：%d\n血量得分：%d\n",score,specialScore,winGameScore,gameOver);
		score+=1;
		specialScore-=1;}
	for(;winGameScore>=50;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：0\n击败杀手得分：0\n技能得分：0\n赢得游戏得分：%d\n血量得分：%d\n",score,winGameScore,gameOver);
		score+=50;
		winGameScore-=50;}
	for(;winGameScore>0;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：0\n击败杀手得分：0\n技能得分：0\n赢得游戏得分：%d\n血量得分：%d\n",score,winGameScore,gameOver);
		score+=0;
		winGameScore-=0;}
	for(;gameOver>=50;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：0\n击败杀手得分：0\n技能得分：0\n赢得游戏得分：0\n血量得分：%d\n",score,gameOver);
		score+=50;
		gameOver-=50;}
	for(;gameOver>0;)
	{
		system("cls");
		printf("你的得分：%d\n时间得分：0\n击败杀手得分：0\n技能得分：0\n赢得游戏得分：0\n血量得分：%d\n",score,gameOver);
		score+=1;
		gameOver-=1;}
	int scoremiddle=score,print;
	fscanf(scoreboard,"%d%d%d",&historyMaxScore,&totalScore,&level);
	for(;scoremiddle>0;)
	{
	system("cls");
	if(historyMaxScore<=score)
	{
	printf("\n      新记录！\n");
	system("color CF");
	historyMaxScore=score;}
	printf("你的等级：%d\n你的本次得分：%d\n你的最高得分记录：%d\n%d/%d:",level,score,historyMaxScore,totalScore,int(1000*pow(1.2,level)));
	print=int(10*totalScore/(1000*pow(1.2,level)));
	while(print>0)
	{
		printf("■");
		print--;
	}
	
	if(scoremiddle>=50)
	{totalScore+=50;
	scoremiddle-=50;}
	else totalScore+=1;
	scoremiddle-=1;
		if(totalScore>=int(1000*pow(1.2,level)))
	{
		totalScore-=int(1000*pow(1.2,level));
		level++;
	}
		}
	fclose(achievement);
	fclose(scoreboard);
	achievement=fopen("achievement","w");
	scoreboard=fopen("playerScoreboard","w");
	fprintf(scoreboard,"%d %d %d",historyMaxScore,totalScore,level);
	fprintf(achievement,"%lld",achievementsUnlockedNow);
	printf("\n");
	system("pause");
	return 0;
}
