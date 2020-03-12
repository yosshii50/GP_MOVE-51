
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <GP_INI.H>
#include <GP_KAN.H>

long colors[]={	0x000000L,
				0x0F0F0AL,
				0x0E0E09L,
				0x0D0D08L,
				0x0C0C07L,
				0x0B0B06L,
				0x0A0A05L,
				0x090904L,
				0x080803L,
				0x070702L,
				0x060601L,
				0x050500L,
				0x0F0F0AL,
				0x0D0D08L,
				0x0B0B06L,
				0x090904L};

/*

入力： plane   - 移動させるＧＶＲＡＭのプレーン番号 1 - 4
       direct  - 移動させる方向 1-下 2-上
       jmpsize - 移動させるライン数 1 - 256
*/
void gp_move001(int plane,int direct,int jmpsize)
{
	unsigned int address;
	jmpsize=jmpsize*0x50;
	switch(plane)
	{
	case(1):
		address=0xA800;
		break;
	case(2):
		address=0xB000;
		break;
	case(3):
		address=0xB800;
		break;
	default:
		address=0xE000;
		break;
	}
	switch(direct)
	{
	case(1):
		_asm			PUSH	DS
		_asm			PUSH	AX
		_asm			PUSH	BX
		_asm			PUSH	CX
		_asm			MOV		AX,address
		_asm			MOV		DS,AX
		_asm			MOV		BX,07D00h
		_asm			SUB		BX,jmpsize
		_asm	loop01:	DEC		BX
		_asm			DEC		BX
		_asm			MOV		AX,DS:[BX]
		_asm			ADD		BX,jmpsize
		_asm			MOV		DS:[BX],AX
		_asm			SUB		BX,jmpsize
		_asm			CMP		BX,00000h
		_asm			JNE		loop01
		_asm			POP		CX
		_asm			POP		BX
		_asm			POP		AX
		_asm			POP		DS
		_setcolor(0);
		_rectangle(_GFILLINTERIOR,0,0,639,jmpsize/0x50);
		break;
	default:
		_asm			PUSH	DS
		_asm			PUSH	AX
		_asm			PUSH	BX
		_asm			PUSH	CX
		_asm			MOV		AX,address
		_asm			MOV		DS,AX
		_asm			MOV		BX,jmpsize
		_asm			SUB		BX,2
		_asm	loop02:	INC		BX
		_asm			INC		BX
		_asm			MOV		AX,DS:[BX]
		_asm			SUB		BX,jmpsize
		_asm			MOV		DS:[BX],AX
		_asm			ADD		BX,jmpsize
		_asm			CMP		BX,07CFEh
		_asm			JNE		loop02
		_asm			POP		CX
		_asm			POP		BX
		_asm			POP		AX
		_asm			POP		DS
		_setcolor(0);
		_rectangle(_GFILLINTERIOR,0,400,639,400-jmpsize/0x50);
		break;
	}
	return;
}

void main(void)
{
	
	/* グラフィック使用開始 */
	SIL_gpin0();
	
	/* 画面消去 */
	_clearscreen(_GCLEARSCREEN);
	_remapallpalette(colors);
	
	/* キー入力が有るまで繰り返し */
	while(kbhit()==0)
	{
		/* ランダムに箱を描く */
		_setcolor(rand()/(32768/16));
		_rectangle(_GFILLINTERIOR,
			rand()/(32768/640),rand()/(32768/400),
			rand()/(32768/640),rand()/(32768/400));
		
		gp_move001(1,1,2);
		gp_move001(2,2,2);
		gp_move001(3,1,4);
		gp_move001(4,2,4);
		
		_setcolor(rand()/(32768/16));
		_rectangle(_GBORDER,
			rand()/(32768/640),rand()/(32768/400),
			rand()/(32768/640),rand()/(32768/400));
		_rectangle(_GBORDER,
			rand()/(32768/640),rand()/(32768/400),
			rand()/(32768/640),rand()/(32768/400));
		gp_move001(1,1,2);
		gp_move001(2,2,2);
		gp_move001(3,1,4);
		gp_move001(4,2,4);
		
//		gp_move001(1,1,2);
//		gp_move001(2,2,2);
//		gp_move001(3,1,4);
//		gp_move001(4,2,4);
	}
	
	/* グラフィック使用終了／画面残す */
	SIL_gpen1();
	
	return;
}

