/*

グラフィック初期化用特殊関数／シリアバージョン Ver-0.01 1995/10/28

必要 include ファイル
なし

*/

#include <graph.h>

/* パレット情報 */
long SIL_gpal[]={	0x000000L,
					0x0C0000L,
					0x00000CL,
					0x0C000CL,
					0x000C00L,
					0x0C0C00L,
					0x000C0CL,
					0x070707L,
					0x0C0C0CL,
					0x0F0000L,
					0x00000FL,
					0x0F000FL,
					0x000F00L,
					0x0F0F00L,
					0x000F0FL,
					0x0F0F0FL};


/* グラフィック使用開始 */
/******************************************************************************
グラフィック使用開始・モード／画面を初期化します。
*/
void SIL_gpin0(void)
{
	if(_setvideomode(_98RES16COLOR)==0)
	{
		printf("ビデオモードの設定に失敗しました。");
		exit(1);
	}
	
	/* パレットのセット */
//	while( (inp(0x60) & 0x20)!=0x20 );
	_remapallpalette(SIL_gpal);
	
	_setactivepage(0);
	_clearscreen(_GCLEARSCREEN);
	_setvisualpage(0);
	return;
}


/* グラフィック使用終了 */
/******************************************************************************
グラフィック使用終了。
*/
void SIL_gpen0(void)
{
	_setvideomode(_DEFAULTMODE);
	printf("\x1B[>1l");
	return;
}


/* グラフィック使用終了／画面残す */
/******************************************************************************
グラフィック使用終了。
*/
void SIL_gpen1(void)
{
	_asm			MOV		AH,041h
	_asm			INT		018h
	_asm			MOV		AH,042h
	_asm			MOV		CH,11000000b
	_asm			INT		018h
	_asm			MOV		AH,00Ch
	_asm			INT		018h
	printf("\x1B[>3l\x1B[>5l\x1B[>1l");
	return;
}

