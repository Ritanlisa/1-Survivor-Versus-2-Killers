#include<bits/stdc++.h>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include<windows.h>
#include<conio.h>
#include<string>
#include<ctime>
#include<iomanip>
#define NUM_MAXMAPNUM 3
#define NUM_MAXDIFFICULTY 3
#define NUM_MAXPLAYERTYPE 3
#define NUM_MAXABILITYNUM 3
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;



namespace offlineSurvivor{
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
COORD posStart={0,0};
bool debugEnable=false;
HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);


void _debug(const char* func,int line){
	if(!debugEnable)return;
	FILE* fileptr;
	fileptr=fopen(func,"a");
	fprintf(fileptr,"[%2d:%2d:%2d]",nowTime/3600,(nowTime/60)%60,nowTime%60);
	fprintf(fileptr,":%d,killers:%d",line,killersAlive);
	fprintf(fileptr,"\n");
	fclose(fileptr);
}

BOOL color(WORD wAttr){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hConsole == INVALID_HANDLE_VALUE)			return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttr);
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
				fprintf(fileptr,"��");}
				else if(mapG[i][j]==1){
				fprintf(fileptr,"��");}
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
	printf("��ϲ�����ѽ����ɾͣ�\n");
	switch(achievementID)
	{
		case 1:
		printf("            ��ô�򵥣����Ѷ���\n");
		printf("����������Ӯ��һ��ʤ��\n");
		break;
		case 2:
		printf("               �������Ҽ�\n");
		printf("������������������������ɱ��ɱ��\n");
		break;
		case 3:
		printf("               û��˼��̫��\n");
		printf("����������Ӯ��һ���е�ģʽ\n");
		break;
		case 4:
		printf("               ����ʿ����\n");
		printf("�������������������÷���ɱ��ɱ��\n");
		break;
		case 5:
		printf("               �б������ѵ�\n");
		printf("����������Ӯ��һ������ģʽ\n");
		break;
		case 6:
		printf("              ���Դ��⻷�¹�˭\n");
		printf("�����������ڲ�������������»�ɱ��λɱ��\n");
		break;
		case 7:
		printf("            ˡ��ֱ�����Ǹ�������Ϸ\n");
		printf("����������Ӯ��һ�ֵ���ģʽ\n");
		break;
		case 8:
		printf("            һ��ն���ҵĹ⻷��Ч\n");
		printf("������������Ĭ��Ӯ��һ�ֵ���ģʽ\n");
		break;
		case 9:
		printf("                 ʹ������\n");
		printf("������������һ��ҡ��Ӯ��һ�ֵ���ģʽ\n");
		break;
		case 10:
		printf("                 ҽ������\n");
		printf("������������Ĭ����ҽ��Ӯ��һ�ֵ���ģʽ\n");
		break;
		case 11:
		printf("                 �������\n");
		printf("����������һ�ֵ���ģʽ���6�������ϲ�ʧ��\n");
		break;
		case 12:
		printf("                 ���׿��\n");
		printf("����������һ�ֵ���ģʽ���6�������ϲ���ʤ\n");
		break;
		case 13:
		printf("          Сѧ���������������Ϸ\n");
		printf("������������Ĭ����Сѧ��Ӯ��һ�ֵ���ģʽ\n");
	}
system("pause");
}

void showPlayerType(int playerType)
{
	system("cls");
	printf("��\"A\"��\"D\"�л���ɫ,��Enter��ȷ��\n");
	switch(playerType){
	case 0:
	printf("                 Сѧ��\n");
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
	printf("   OO^ `*  <��>         <��>     OOO    \n");
	printf("   ,^ =            \\\\           =OO     \n");
	printf("      |              \\\\          `        \n");
	printf("      *             ==           /   \n");
	printf("    * =                               \n");
	printf("    ,`**          /====\\     ,`**=    \n");
	printf("      *,**        \\====/     ****=`     \n");
	printf("      \\*\\***               ***        \n");
	printf("    `o^*,/oo]******        *****  ^       \n");
	printf("���ܣ���\n");
	printf("��飺ֻ��Ϲ�ܵĽ�ɫ");
	break;
	case 1:
	printf("                  ��ũ\n");
	printf("                . @@@@@@@@@@@@@@\\\n");
	printf("              ,@@@@@@@@@@@@@@@@@@@ `\n");
	printf("             ,@@@@@@@@@@@@@@@@@@@@@@`\n");
	printf("           .]@@@@@@@@@@@@@@@@@@@@@@@@\\.\n");
	printf("          .@@@@@@@@@@@@@   @ @@@@@@@   .\n");
	printf("           @ @@@@@@@@@@          @@@@  .\n");
	printf("          . @@@@@@@  =====   ====  @@@ ^.\n");
	printf("           ,@@@@@@@   <��>   <��>    @@/\n");
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
	printf("���ܣ��ռ�/���÷���\n");
	printf("��飺������ѳ�ϰ�ߵĽ�ɫ");
	break;
	case 2:
	printf("                  ҽ��\n");
	printf("                      =@@@@@@@@@@@@@@]\n");
	printf("                     @@@@@@@@@@@@@@@@@\\\n");
	printf("                  /@@@@@@  @@@@@@@@@@@@@\n");
	printf("                 \\@@@@       @@@@@@@@@@@@\n");
	printf("                 @@@@         @@@@@@@@@@@@\n");
	printf("                =@@@---\\* */---@@@@@@@@@@@`\n");
	printf("                =@@@<��>/   <��>@@@@@@@@@@^\n");
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
	printf("���ܣ�����\n");
	printf("��飺ϰ�����Ʊ��˵Ľ�ɫ");
	break;
	case 3:	
	printf("                  ����\n");
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
	printf("���ܣ���������\n");
	printf("��飺ϰ��ɱ���Ľ�ɫ");
	}
}

int choosePlayerTypes()
{	int playerType=0;
	while(true){
	showPlayerType(playerType);
	while(!(KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)||KEY_DOWN('F')||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_LEFT)));
		if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F')){_sleep(500);return playerType;}
		if((KEY_DOWN('A')||KEY_DOWN(VK_LEFT))&&playerType>0)playerType--;
		if((KEY_DOWN('D')||KEY_DOWN(VK_RIGHT))&&playerType<NUM_MAXPLAYERTYPE)playerType++;
		_sleep(150);
	}
}

void showAbilities()
{
	system("cls");
	printf("��\"A\"��\"D\"�л�����,��Enter��ȷ��\n");
	switch(ability){
	case -1:printf("           ��ѡ��\n");
	break;
	case 0:
	printf("           ��ä\n");
	printf("����Ч������ʾɱ��λ��,����λ��\n");
	printf("����Ч�������ɼ���Χ����");
	break;
	case 1:
	printf("          һ��ҡ��\n");
	printf("����Ч����Ѫ��Խ��ɱ���ƶ��ٶ�Խ��\n");
	printf("����Ч����ÿ��һ������1Ѫ����ÿ�α��������Ѫ�ٶȷ���");
	break;
	case 2:
	printf("            ѪծѪ��\n");
	printf("����Ч������������200Ѫ��\n");
	printf("����Ч����ɱ��ÿ�ɹ�����һ�Σ�������������25");
	break;
	case 3:	
	printf("             Ĭ��\n");
	printf("����Ч����ɱ��һλɱ������ɱ��Ĭ��5��(��������)\n");
	printf("����Ч����Ĭ����ÿλɱ�ֹ���������50%c(�ɵ��ӣ�������)",'%');
	}
}

int chooseAbilities()
{	ability=-1;
	while(true){
	showAbilities();
	while(!(KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)||KEY_DOWN('F')||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_LEFT)));
		if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F')){_sleep(500);return ability;}
		if((KEY_DOWN('A')||KEY_DOWN(VK_LEFT))&&ability>-1)ability--;
		if((KEY_DOWN('D')||KEY_DOWN(VK_RIGHT))&&ability<NUM_MAXABILITYNUM)ability++;
		_sleep(150);
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
	printf("��\"A\"��\"D\"�л��Ѷ�,��Enter��ȷ��\n");
	switch(difficulty){
	case -1:
	printf("����ģʽ\n");
	printf("��Ұ��ȫ��\n");
	printf("ɱ�������Զ���");
	system("color 5F");
	break;
	case 0:
	printf("��ģʽ\n");
	printf("��Ұ��ȫ��\n");
	printf("ɱ������2");
	system("color 2F");
	break;
	case 1:
	printf("�е�ģʽ\n");
	printf("��Ұ��ԲȦ���뾶10��\n");
	printf("ɱ������4");
	system("color 6F");
	break;
	case 2:
	printf("����ģʽ\n");
	printf("��Ұ��ԲȦ���뾶5��\n");
	printf("ɱ������8");
	system("color CF");
	break;
	case 3:	
	printf("����ģʽ\n");
	printf("��Ұ��1/4���Σ��뾶5��\n");
	printf("ɱ������10");
	system("color 0F");
	}
	if(difficulty>=(level/10)+1)printf("\n    �ȼ����㣡%d������:������Ϊ%d����",difficulty*10,level);
}

void chooseDifficulties()
{	difficulty=0;
	while(true){
	showDifficulties();
	while(!(KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)||KEY_DOWN(VK_LEFT)||KEY_DOWN(VK_RIGHT)||KEY_DOWN('F')));
			if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F'))
			if((difficulty<(level/10)+1)||(difficulty>-1&&debugEnable)){
			system("color F0");
			return;
			}
			if(KEY_DOWN(VK_LEFT)||KEY_DOWN('A'))if(difficulty>0||(difficulty>-1&&debugEnable))difficulty--;
			if(KEY_DOWN(VK_RIGHT)||KEY_DOWN('D'))if(difficulty<NUM_MAXDIFFICULTY)difficulty++;
			_sleep(150);
	}
}

/*
void debug(int posLocation)
{
	if((!debugEnable)||(debugging))return;
	debugging=true;
	debugTimes++;
	long nowTime=time(NULL)-startTime;
	fprintf(debuger,"[%2d:%2d:%2d]%s,��%3d��,�ڵ�%d�ŵ���,�������:\n",nowTime/3600,(nowTime/60)%60,nowTime%60,mapNames,debugTimes,posLocation);
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
		fprintf(debuger,"ʣ��ɱ����:%3d\n",killersAlive);
		fprintf(debuger,"HP:%3d",gameOver);
		for(int p=0;p<gameOver/100;p++)
		fprintf(debuger,"��");
		fprintf(debuger,"\n");
		switch(playerType)
		{
		case 0:break;
		case 1:fprintf(debuger,"���÷�����:%d \n", special); break;
		case 2:fprintf(debuger,"���ƽ���:%d%c\n",special,'%');
		for(int p=0;p<special/10;p++)
			fprintf(debuger,"");
		break;
		}
		
	fprintf(debuger,"\nPlayer:(%d,%d)\n",playerPosX,playerPosY);
		for(int i=0;i<22;i++)
		{
		for(int j=0;j<22;j++)
			{if(watchable[i][j])
				fprintf(debuger,"��");
				else fprintf(debuger,"��");
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
	steps[posX][posY]=stepNum;
	if(mapG[posX][posY])return;
	if(isAnotherTrapHere(posX,posY,-1))stepNum+=200;
	if(isKillerHere(posX,posY))stepNum+=100;
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
	start=true;
	bloodkill=0;
	switch(ability)
	{
		case 1:
			bloodkill=1;
			break;
	}
	system("cls");
	while(gameOver>0&&killersAlive>0)
	{
		gameOver-=bloodkill;
		if(playerPosX<=0)playerPosX=1;
		if(playerPosY<=0)playerPosY=1;
		_sleep(1000);
		char gotcha;
		while(!(KEY_DOWN(VK_LSHIFT)||KEY_DOWN('W')||KEY_DOWN('A')||KEY_DOWN('S')||KEY_DOWN('D')||KEY_DOWN(VK_SPACE)||KEY_DOWN(VK_LEFT)||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_UP)||KEY_DOWN(VK_DOWN)));
			if(KEY_DOWN(VK_LSHIFT))debugEnable=true;
			
			if(KEY_DOWN(VK_UP)||KEY_DOWN('W')){
			direction=0;
			if(!mapG[playerPosX-1][playerPosY])
			playerPosX--;}
			
			if(KEY_DOWN(VK_LEFT)||KEY_DOWN('A')){
			direction=1;
			if(!mapG[playerPosX][playerPosY-1])
			playerPosY--;}
			
			if(KEY_DOWN(VK_DOWN)||KEY_DOWN('S')){
			direction=2;
			if(!mapG[playerPosX+1][playerPosY])
			playerPosX++;
			}
			
			if(KEY_DOWN(VK_RIGHT)||KEY_DOWN('D')){
			direction=3;
			if(!mapG[playerPosX][playerPosY+1])
			playerPosY++;
			}
			
			if(KEY_DOWN(VK_SPACE)||KEY_DOWN('F')){
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
		}
			if(KEY_DOWN(VK_RETURN))
			if(difficulty==-1)
			{	
			killersAlive=0;
			return true;
			}
		switch(playerType)
		{
		
			case 2:if(special>=10)special-=10;
			else special=0;
			break;
		}
		memset(steps,0x7f7f7f,sizeof(steps));
		setStep(playerPosX,playerPosY,0);
	}
	if(gameOver<=0||killersAlive>0)
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
	int choice[4]={	steps[killerPosX[ID]-1][killerPosY[ID]]*(mapG[killerPosX[ID]-1][killerPosY[ID]]==0?1:100000),
					steps[killerPosX[ID]][killerPosY[ID]-1]*(mapG[killerPosX[ID]][killerPosY[ID]-1]==0?1:100000),
					steps[killerPosX[ID]+1][killerPosY[ID]]*(mapG[killerPosX[ID]+1][killerPosY[ID]]==0?1:100000),
					steps[killerPosX[ID]][killerPosY[ID]+1]*(mapG[killerPosX[ID]][killerPosY[ID]+1]==0?1:100000)};
	int minDirection=-1,minChoice=INT_MAX,i;
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
				if(steps[playerPosX+i][playerPosY+j]==abs(i)+abs(j))
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
	system("cls");

	while(gameOver>0&&killersAlive>0)
	{
		//system("cls");
		SetConsoleCursorPosition(hOut,posStart);
		lookAtMap();
		nowTime=time(NULL)-startTime;
		printf("            [%2d:%2d:%2d]                  \n",nowTime/3600,(nowTime/60)%60,nowTime%60);
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
				printf("��");}
				else if(mapG[i][j]==1){
				printf("��");}
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
				else{
					if(debugEnable&&difficulty==-1){
						if(steps[i][j]>=100)printf("XX");
						else printf("%2d",steps[i][j]);
					}
					else printf("  ");}
			}
		printf("\n");	
		}
		printf("ʣ��ɱ����:%3d     ",killersAlive);
		printf("HP:%3d ",gameOver);
		for(int p=0;p<gameOver/100;p++)
		printf("��");
		printf("              \n");
		switch(playerType)
		{
		case 0:break;
		case 1:printf("���÷�����:%d     ",special); break;
		case 2:printf("���ƽ���:%d%c    ",special,'%');
		for(int p=0;p<special/10;p++)
			printf("��");printf("                         ");break;
		case 3:printf("ʣ��������:%d     ",special);break;
		}
		if(gameOver==100)system("color 4C");
		if(gameOver==200)system("color C0");
		if(gameOver==300)system("color F4");
		if(gameOver>=400)system("color F0");
		_sleep(1000);
	}
	system("cls");
}
 
void offlineSurvivor(){
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
	_sleep(500);
	chooseDifficulties();
	_sleep(500);
	switch(difficulty)
	{
		case -1:
			system("cls");
			printf("������ɱ����:"); 
			scanf("%d",&NUM_KILLERS);
			printf("�����������Ұ�뾶:"); 
			scanf("%d",&NUM_VIEWMAXRADIUS);
			printf("��������С��Ұ�뾶:"); 
			scanf("%d",&NUM_VIEWMINRADIUS);
			printf("������Զ����ʼ��:ɱ����:%d �����Ұ�뾶:%d ��С��Ұ�뾶:%d \n",NUM_KILLERS,NUM_VIEWMAXRADIUS,NUM_VIEWMINRADIUS); 
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
	killersAlive=NUM_KILLERS;
	playerType=choosePlayerTypes();
	_sleep(500);
	chooseAbilities();
	system("cls");
	printf("��WASD�ƶ���ɫ,��F����ո�ʹ�ü���\n");
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
    i=1;
	system("cls"); 
	printf("loading...\n");
	pthread_create(&threads[i],NULL,showMap,(void *)&(indexes[i]));
    startTime=time(NULL);
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
   }
	result=run();
	if(result&&gameOver>0)
	{
	_sleep(1000);
	system("cls");
	printf("��Ӯ�ˣ�\n");
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
	_sleep(1000);
	system("cls");
	printf("�����ˣ�\n");
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
	timescore=(log(600)-log(time(NULL)-startTime))*500*(1+difficulty);
	winGameScore=3000*(1+difficulty);
	}else timescore=log(time(NULL)-startTime)*500*(1+difficulty);
	beatKillerScore=(NUM_KILLERS-killersAlive)*1000*(1+difficulty);
	specialScore*=100*(1+difficulty);
	gameOver*=10*(1+difficulty);
	if(gameOver<0)gameOver=0;
	system("pause");
	for(score=0;timescore>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�%d\n����ɱ�ֵ÷֣�%d\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,timescore,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=50;
		timescore-=50;}
	for(;timescore>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�%d\n����ɱ�ֵ÷֣�%d\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,timescore,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=1;
		timescore-=1;}
	for(;beatKillerScore>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�%d\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=50;
		beatKillerScore-=50;}
	for(;beatKillerScore>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�%d\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=1;
		beatKillerScore-=1;}
	for(;specialScore>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,specialScore,winGameScore,gameOver);
		score+=50;
		specialScore-=50;}
	for(;specialScore>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,specialScore,winGameScore,gameOver);
		score+=1;
		specialScore-=1;}
	for(;winGameScore>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�0\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,winGameScore,gameOver);
		score+=50;
		winGameScore-=50;}
	for(;winGameScore>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�0\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,winGameScore,gameOver);
		score+=0;
		winGameScore-=0;}
	for(;gameOver>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�0\nӮ����Ϸ�÷֣�0\nѪ���÷֣�%d\n",score,gameOver);
		score+=50;
		gameOver-=50;}
	for(;gameOver>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�0\nӮ����Ϸ�÷֣�0\nѪ���÷֣�%d\n",score,gameOver);
		score+=1;
		gameOver-=1;}
	int scoremiddle=score,print;
	fscanf(scoreboard,"%d%d%d",&historyMaxScore,&totalScore,&level);
	for(;scoremiddle>0;)
	{
	system("cls");
	if(historyMaxScore<=score)
	{
	printf("\n      �¼�¼��\n");
	system("color CF");
	historyMaxScore=score;}
	printf("��ĵȼ���%d\n��ı��ε÷֣�%d\n�����ߵ÷ּ�¼��%d\n%d/%d:",level,score,historyMaxScore,totalScore,int(1000*pow(1.2,level)));
	print=int(10*totalScore/(1000*pow(1.2,level)));
	while(print>0)
	{
		printf("��");
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
}
}


namespace offlineKiller{
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
COORD posStart={0,0};
bool debugEnable=false;
HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);


void _debug(const char* func,int line){
	if(!debugEnable)return;
	FILE* fileptr;
	fileptr=fopen(func,"a");
	fprintf(fileptr,"[%2d:%2d:%2d]",nowTime/3600,(nowTime/60)%60,nowTime%60);
	fprintf(fileptr,":%d,killers:%d",line,killersAlive);
	fprintf(fileptr,"\n");
	fclose(fileptr);
}

BOOL color(WORD wAttr){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hConsole == INVALID_HANDLE_VALUE)			return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttr);
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
				fprintf(fileptr,"��");}
				else if(mapG[i][j]==1){
				fprintf(fileptr,"��");}
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
	printf("��ϲ�����ѽ����ɾͣ�\n");
	switch(achievementID)
	{
		case 1:
		printf("            ��ô�򵥣����Ѷ���\n");
		printf("����������Ӯ��һ��ʤ��\n");
		break;
		case 2:
		printf("               �������Ҽ�\n");
		printf("������������������������ɱ��ɱ��\n");
		break;
		case 3:
		printf("               û��˼��̫��\n");
		printf("����������Ӯ��һ���е�ģʽ\n");
		break;
		case 4:
		printf("               ����ʿ����\n");
		printf("�������������������÷���ɱ��ɱ��\n");
		break;
		case 5:
		printf("               �б������ѵ�\n");
		printf("����������Ӯ��һ������ģʽ\n");
		break;
		case 6:
		printf("              ���Դ��⻷�¹�˭\n");
		printf("�����������ڲ�������������»�ɱ��λɱ��\n");
		break;
		case 7:
		printf("            ˡ��ֱ�����Ǹ�������Ϸ\n");
		printf("����������Ӯ��һ�ֵ���ģʽ\n");
		break;
		case 8:
		printf("            һ��ն���ҵĹ⻷��Ч\n");
		printf("������������Ĭ��Ӯ��һ�ֵ���ģʽ\n");
		break;
		case 9:
		printf("                 ʹ������\n");
		printf("������������һ��ҡ��Ӯ��һ�ֵ���ģʽ\n");
		break;
		case 10:
		printf("                 ҽ������\n");
		printf("������������Ĭ����ҽ��Ӯ��һ�ֵ���ģʽ\n");
		break;
		case 11:
		printf("                 �������\n");
		printf("����������һ�ֵ���ģʽ���6�������ϲ�ʧ��\n");
		break;
		case 12:
		printf("                 ���׿��\n");
		printf("����������һ�ֵ���ģʽ���6�������ϲ���ʤ\n");
		break;
		case 13:
		printf("          Сѧ���������������Ϸ\n");
		printf("������������Ĭ����Сѧ��Ӯ��һ�ֵ���ģʽ\n");
	}
system("pause");
}

void showPlayerType(int playerType)
{
	system("cls");
	printf("��\"A\"��\"D\"�л���ɫ,��Enter��ȷ��\n");
	switch(playerType){
	case 0:
	printf("                 Сѧ��\n");
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
	printf("   OO^ `*  <��>         <��>     OOO    \n");
	printf("   ,^ =            \\\\           =OO     \n");
	printf("      |              \\\\          `        \n");
	printf("      *             ==           /   \n");
	printf("    * =                               \n");
	printf("    ,`**          /====\\     ,`**=    \n");
	printf("      *,**        \\====/     ****=`     \n");
	printf("      \\*\\***               ***        \n");
	printf("    `o^*,/oo]******        *****  ^       \n");
	printf("���ܣ���\n");
	printf("��飺ֻ��Ϲ�ܵĽ�ɫ");
	break;
	case 1:
	printf("                  ��ũ\n");
	printf("                . @@@@@@@@@@@@@@\\\n");
	printf("              ,@@@@@@@@@@@@@@@@@@@ `\n");
	printf("             ,@@@@@@@@@@@@@@@@@@@@@@`\n");
	printf("           .]@@@@@@@@@@@@@@@@@@@@@@@@\\.\n");
	printf("          .@@@@@@@@@@@@@   @ @@@@@@@   .\n");
	printf("           @ @@@@@@@@@@          @@@@  .\n");
	printf("          . @@@@@@@  =====   ====  @@@ ^.\n");
	printf("           ,@@@@@@@   <��>   <��>    @@/\n");
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
	printf("���ܣ��ռ�/���÷���\n");
	printf("��飺������ѳ�ϰ�ߵĽ�ɫ");
	break;
	case 2:
	printf("                  ҽ��\n");
	printf("                      =@@@@@@@@@@@@@@]\n");
	printf("                     @@@@@@@@@@@@@@@@@\\\n");
	printf("                  /@@@@@@  @@@@@@@@@@@@@\n");
	printf("                 \\@@@@       @@@@@@@@@@@@\n");
	printf("                 @@@@         @@@@@@@@@@@@\n");
	printf("                =@@@---\\* */---@@@@@@@@@@@`\n");
	printf("                =@@@<��>/   <��>@@@@@@@@@@^\n");
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
	printf("���ܣ�����\n");
	printf("��飺ϰ�����Ʊ��˵Ľ�ɫ");
	break;
	case 3:	
	printf("                  ����\n");
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
	printf("���ܣ���������\n");
	printf("��飺ϰ��ɱ���Ľ�ɫ");
	}
}

int choosePlayerTypes()
{	int playerType=0;
	while(true){
	showPlayerType(playerType);
	while(!(KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)||KEY_DOWN('F')||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_LEFT)));
		if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F')){_sleep(500);return playerType;}
		if((KEY_DOWN('A')||KEY_DOWN(VK_LEFT))&&playerType>0)playerType--;
		if((KEY_DOWN('D')||KEY_DOWN(VK_RIGHT))&&playerType<NUM_MAXPLAYERTYPE)playerType++;
		_sleep(150);
	}
}

void showAbilities()
{
	system("cls");
	printf("��\"A\"��\"D\"�л�����,��Enter��ȷ��\n");
	switch(ability){
	case -1:printf("           ��ѡ��\n");
	break;
	case 0:
	printf("           ��ä\n");
	printf("����Ч������ʾɱ��λ��,����λ��\n");
	printf("����Ч�������ɼ���Χ����");
	break;
	case 1:
	printf("          һ��ҡ��\n");
	printf("����Ч����Ѫ��Խ��ɱ���ƶ��ٶ�Խ��\n");
	printf("����Ч����ÿ��һ������1Ѫ����ÿ�α��������Ѫ�ٶȷ���");
	break;
	case 2:
	printf("            ѪծѪ��\n");
	printf("����Ч������������200Ѫ��\n");
	printf("����Ч����ɱ��ÿ�ɹ�����һ�Σ�������������25");
	break;
	case 3:	
	printf("             Ĭ��\n");
	printf("����Ч����ɱ��һλɱ������ɱ��Ĭ��5��(��������)\n");
	printf("����Ч����Ĭ����ÿλɱ�ֹ���������50%c(�ɵ��ӣ�������)",'%');
	}
}

int chooseAbilities()
{	ability=-1;
	while(true){
	showAbilities();
	while(!(KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)||KEY_DOWN('F')||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_LEFT)));
		if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F')){_sleep(500);return ability;}
		if((KEY_DOWN('A')||KEY_DOWN(VK_LEFT))&&ability>-1)ability--;
		if((KEY_DOWN('D')||KEY_DOWN(VK_RIGHT))&&ability<NUM_MAXABILITYNUM)ability++;
		_sleep(150);
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
	printf("��\"A\"��\"D\"�л��Ѷ�,��Enter��ȷ��\n");
	switch(difficulty){
	case -1:
	printf("����ģʽ\n");
	printf("��Ұ��ȫ��\n");
	printf("ɱ�������Զ���");
	system("color 5F");
	break;
	case 0:
	printf("��ģʽ\n");
	printf("��Ұ��ȫ��\n");
	printf("ɱ������2");
	system("color 2F");
	break;
	case 1:
	printf("�е�ģʽ\n");
	printf("��Ұ��ԲȦ���뾶10��\n");
	printf("ɱ������4");
	system("color 6F");
	break;
	case 2:
	printf("����ģʽ\n");
	printf("��Ұ��ԲȦ���뾶5��\n");
	printf("ɱ������8");
	system("color CF");
	break;
	case 3:	
	printf("����ģʽ\n");
	printf("��Ұ��1/4���Σ��뾶5��\n");
	printf("ɱ������10");
	system("color 0F");
	}
	if(difficulty>=(level/10)+1)printf("\n    �ȼ����㣡%d������:������Ϊ%d����",difficulty*10,level);
}

void chooseDifficulties()
{	difficulty=0;
	while(true){
	showDifficulties();
	while(!(KEY_DOWN('A')||KEY_DOWN('D')||KEY_DOWN(VK_RETURN)||KEY_DOWN(VK_LEFT)||KEY_DOWN(VK_RIGHT)||KEY_DOWN('F')));
			if(KEY_DOWN(VK_RETURN)||KEY_DOWN('F'))
			if((difficulty<(level/10)+1)||(difficulty>-1&&debugEnable)){
			system("color F0");
			return;
			}
			if(KEY_DOWN(VK_LEFT)||KEY_DOWN('A'))if(difficulty>0||(difficulty>-1&&debugEnable))difficulty--;
			if(KEY_DOWN(VK_RIGHT)||KEY_DOWN('D'))if(difficulty<NUM_MAXDIFFICULTY)difficulty++;
			_sleep(150);
	}
}

/*
void debug(int posLocation)
{
	if((!debugEnable)||(debugging))return;
	debugging=true;
	debugTimes++;
	long nowTime=time(NULL)-startTime;
	fprintf(debuger,"[%2d:%2d:%2d]%s,��%3d��,�ڵ�%d�ŵ���,�������:\n",nowTime/3600,(nowTime/60)%60,nowTime%60,mapNames,debugTimes,posLocation);
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
		fprintf(debuger,"ʣ��ɱ����:%3d\n",killersAlive);
		fprintf(debuger,"HP:%3d",gameOver);
		for(int p=0;p<gameOver/100;p++)
		fprintf(debuger,"��");
		fprintf(debuger,"\n");
		switch(playerType)
		{
		case 0:break;
		case 1:fprintf(debuger,"���÷�����:%d \n", special); break;
		case 2:fprintf(debuger,"���ƽ���:%d%c\n",special,'%');
		for(int p=0;p<special/10;p++)
			fprintf(debuger,"");
		break;
		}
		
	fprintf(debuger,"\nPlayer:(%d,%d)\n",playerPosX,playerPosY);
		for(int i=0;i<22;i++)
		{
		for(int j=0;j<22;j++)
			{if(watchable[i][j])
				fprintf(debuger,"��");
				else fprintf(debuger,"��");
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
	steps[posX][posY]=stepNum;
	if(mapG[posX][posY])return;
	if(isAnotherTrapHere(posX,posY,-1))stepNum+=200;
	if(isKillerHere(posX,posY))stepNum+=100;
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
	start=true;
	bloodkill=0;
	switch(ability)
	{
		case 1:
			bloodkill=1;
			break;
	}
	system("cls");
	while(gameOver>0&&killersAlive>0)
	{
		gameOver-=bloodkill;
		if(playerPosX<=0)playerPosX=1;
		if(playerPosY<=0)playerPosY=1;
		_sleep(1000);
		char gotcha;
		while(!(KEY_DOWN(VK_LSHIFT)||KEY_DOWN('W')||KEY_DOWN('A')||KEY_DOWN('S')||KEY_DOWN('D')||KEY_DOWN(VK_SPACE)||KEY_DOWN(VK_LEFT)||KEY_DOWN(VK_RIGHT)||KEY_DOWN(VK_UP)||KEY_DOWN(VK_DOWN)));
			if(KEY_DOWN(VK_LSHIFT))debugEnable=true;
			
			if(KEY_DOWN(VK_UP)||KEY_DOWN('W')){
			direction=0;
			if(!mapG[playerPosX-1][playerPosY])
			playerPosX--;}
			
			if(KEY_DOWN(VK_LEFT)||KEY_DOWN('A')){
			direction=1;
			if(!mapG[playerPosX][playerPosY-1])
			playerPosY--;}
			
			if(KEY_DOWN(VK_DOWN)||KEY_DOWN('S')){
			direction=2;
			if(!mapG[playerPosX+1][playerPosY])
			playerPosX++;
			}
			
			if(KEY_DOWN(VK_RIGHT)||KEY_DOWN('D')){
			direction=3;
			if(!mapG[playerPosX][playerPosY+1])
			playerPosY++;
			}
			
			if(KEY_DOWN(VK_SPACE)||KEY_DOWN('F')){
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
		}
			if(KEY_DOWN(VK_RETURN))
			if(difficulty==-1)
			{	
			killersAlive=0;
			return true;
			}
		switch(playerType)
		{
		
			case 2:if(special>=10)special-=10;
			else special=0;
			break;
		}
		memset(steps,0x7f7f7f,sizeof(steps));
		setStep(playerPosX,playerPosY,0);
	}
	if(gameOver<=0||killersAlive>0)
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
	int choice[4]={	steps[killerPosX[ID]-1][killerPosY[ID]]*(mapG[killerPosX[ID]-1][killerPosY[ID]]==0?1:100000),
					steps[killerPosX[ID]][killerPosY[ID]-1]*(mapG[killerPosX[ID]][killerPosY[ID]-1]==0?1:100000),
					steps[killerPosX[ID]+1][killerPosY[ID]]*(mapG[killerPosX[ID]+1][killerPosY[ID]]==0?1:100000),
					steps[killerPosX[ID]][killerPosY[ID]+1]*(mapG[killerPosX[ID]][killerPosY[ID]+1]==0?1:100000)};
	int minDirection=-1,minChoice=INT_MAX,i;
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
				if(steps[playerPosX+i][playerPosY+j]==abs(i)+abs(j))
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
	system("cls");
	while(gameOver>0&&killersAlive>0)
	{
		//system("cls");
		SetConsoleCursorPosition(hOut,posStart);
		lookAtMap();
		nowTime=time(NULL)-startTime;
		printf("            [%2d:%2d:%2d]                  \n",nowTime/3600,(nowTime/60)%60,nowTime%60);
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
				printf("��");}
				else if(mapG[i][j]==1){
				printf("��");}
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
				else{
					if(debugEnable&&difficulty==-1){
						if(steps[i][j]>=100)printf("XX");
						else printf("%2d",steps[i][j]);
					}
					else printf("  ");}
			}
		printf("\n");	
		}
		printf("ʣ��ɱ����:%3d     ",killersAlive);
		printf("HP:%3d ",gameOver);
		for(int p=0;p<gameOver/100;p++)
		printf("��");
		printf("              \n");
		switch(playerType)
		{
		case 0:break;
		case 1:printf("���÷�����:%d     ",special); break;
		case 2:printf("���ƽ���:%d%c    ",special,'%');
		for(int p=0;p<special/10;p++)
			printf("��");printf("                         ");break;
		case 3:printf("ʣ��������:%d     ",special);break;
		}
		if(gameOver==100)system("color 4C");
		if(gameOver==200)system("color C0");
		if(gameOver==300)system("color F4");
		if(gameOver>=400)system("color F0");
		_sleep(1000);
	}
	system("cls");
}
 
void offlineSurvivor(){
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
	_sleep(500);
	chooseDifficulties();
	_sleep(500);
	switch(difficulty)
	{
		case -1:
			system("cls");
			printf("������ɱ����:"); 
			scanf("%d",&NUM_KILLERS);
			printf("�����������Ұ�뾶:"); 
			scanf("%d",&NUM_VIEWMAXRADIUS);
			printf("��������С��Ұ�뾶:"); 
			scanf("%d",&NUM_VIEWMINRADIUS);
			printf("������Զ����ʼ��:ɱ����:%d �����Ұ�뾶:%d ��С��Ұ�뾶:%d \n",NUM_KILLERS,NUM_VIEWMAXRADIUS,NUM_VIEWMINRADIUS); 
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
	killersAlive=NUM_KILLERS;
	playerType=choosePlayerTypes();
	_sleep(500);
	chooseAbilities();
	system("cls");
	printf("��WASD�ƶ���ɫ,��F����ո�ʹ�ü���\n");
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
    i=1;
	system("cls"); 
	printf("loading...\n");
	pthread_create(&threads[i],NULL,showMap,(void *)&(indexes[i]));
    startTime=time(NULL);
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
   }
	result=run();
	if(result&&gameOver>0)
	{
	_sleep(1000);
	system("cls");
	printf("��Ӯ�ˣ�\n");
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
	_sleep(1000);
	system("cls");
	printf("�����ˣ�\n");
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
	timescore=(log(600)-log(time(NULL)-startTime))*500*(1+difficulty);
	winGameScore=3000*(1+difficulty);
	}else timescore=log(time(NULL)-startTime)*500*(1+difficulty);
	beatKillerScore=(NUM_KILLERS-killersAlive)*1000*(1+difficulty);
	specialScore*=100*(1+difficulty);
	gameOver*=10*(1+difficulty);
	if(gameOver<0)gameOver=0;
	system("pause");
	for(score=0;timescore>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�%d\n����ɱ�ֵ÷֣�%d\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,timescore,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=50;
		timescore-=50;}
	for(;timescore>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�%d\n����ɱ�ֵ÷֣�%d\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,timescore,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=1;
		timescore-=1;}
	for(;beatKillerScore>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�%d\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=50;
		beatKillerScore-=50;}
	for(;beatKillerScore>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�%d\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,beatKillerScore,specialScore,winGameScore,gameOver);
		score+=1;
		beatKillerScore-=1;}
	for(;specialScore>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,specialScore,winGameScore,gameOver);
		score+=50;
		specialScore-=50;}
	for(;specialScore>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�%d\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,specialScore,winGameScore,gameOver);
		score+=1;
		specialScore-=1;}
	for(;winGameScore>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�0\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,winGameScore,gameOver);
		score+=50;
		winGameScore-=50;}
	for(;winGameScore>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�0\nӮ����Ϸ�÷֣�%d\nѪ���÷֣�%d\n",score,winGameScore,gameOver);
		score+=0;
		winGameScore-=0;}
	for(;gameOver>=50;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�0\nӮ����Ϸ�÷֣�0\nѪ���÷֣�%d\n",score,gameOver);
		score+=50;
		gameOver-=50;}
	for(;gameOver>0;)
	{
		system("cls");
		printf("��ĵ÷֣�%d\nʱ��÷֣�0\n����ɱ�ֵ÷֣�0\n���ܵ÷֣�0\nӮ����Ϸ�÷֣�0\nѪ���÷֣�%d\n",score,gameOver);
		score+=1;
		gameOver-=1;}
	int scoremiddle=score,print;
	fscanf(scoreboard,"%d%d%d",&historyMaxScore,&totalScore,&level);
	for(;scoremiddle>0;)
	{
	system("cls");
	if(historyMaxScore<=score)
	{
	printf("\n      �¼�¼��\n");
	system("color CF");
	historyMaxScore=score;}
	printf("��ĵȼ���%d\n��ı��ε÷֣�%d\n�����ߵ÷ּ�¼��%d\n%d/%d:",level,score,historyMaxScore,totalScore,int(1000*pow(1.2,level)));
	print=int(10*totalScore/(1000*pow(1.2,level)));
	while(print>0)
	{
		printf("��");
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
}
}
