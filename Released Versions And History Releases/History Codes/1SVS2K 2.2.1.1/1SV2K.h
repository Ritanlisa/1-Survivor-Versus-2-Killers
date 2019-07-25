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

bool isPlayerAround(int playerX,int playerY,int PosX,int PosY)
{
	return (PosX==playerX&&abs(PosY-playerY)<2)||(PosY==playerY&&abs(PosX-playerX)<2);
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

