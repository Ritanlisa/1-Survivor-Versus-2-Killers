#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include <math.h>
#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#define NUM_MAXMAPNUM 3
#define NUM_MAXDIFFICULTY 3
#define NUM_MAXPLAYERTYPE 3
#define NUM_MAXABILITYNUM 3


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

bool isPlayerAround(int playerX,int playerY,int PosX,int PosY)
{
	return (PosX==playerX&&abs(PosY-playerY)<2)||(PosY==playerY&&abs(PosX-playerX)<2);
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

int chooseAbilities()
{	int ability=-1;
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

