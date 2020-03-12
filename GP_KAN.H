/*

グラフィック画面漢字表示用特殊関数／シリアバージョン Ver-0.01 1995/10/28

必要 include ファイル
#include <gp_ini.h>
#include <dos.h>

*/


/* 漢字ＧＰ表示／１文字 */
/******************************************************************************
グラフィック画面に漢字を１文字表示します。

kanji_code		表示する漢字のコード
kanji_color		表示する漢字の色 □□□□□□□□
								 | | | | E G R B - 表示色プレーン
								 E G R B --------- 無視プレーン
kanji_address	表示するアドレス (X+Y*80)
kanji_mode		表示するモード □□□□□□□□
							   |       | | | +- 太文字
							   |       | | +--- 縦２倍
							   |       | +----- 横２倍
							   |       +------- ポジネガ反転
							   +--------------- 高速表示

*/
void SIL_gpko0(unsigned int kanji_code,
					unsigned char kanji_color,
					unsigned int kanji_address,
					unsigned char kanji_mode)
{
	unsigned char *kanji_work;
	unsigned char work_memory[32*2];
	union REGS ioregs;
	kanji_work = work_memory;
	
	_asm			PUSH	ES
	_asm			PUSH	SI
	/* モードチェック／高速表示 */
	_asm			TEST	kanji_mode,080h
	_asm			JE		ASMJ21
	/* 高速表示切り替え */
	_asm			MOV		AX,01B01h
	_asm			INT		018h
	/* 漢字コードの変換 kanji_code -> AX */
	_asm	ASMJ21:	MOV		AX,kanji_code
	_asm			XCHG	AL,AH
	_asm			SHL		AH,001h
	_asm			SUB		AL,01Fh
	_asm			JS		ASMJ01
	_asm			CMP		AL,061h
	_asm			ADC		AL,0DEh
	_asm	ASMJ01:	ADD		AX,01FA1h
	_asm			AND		AX,07F7Fh
	/* 漢字フォントの取り出し AX -> kanji_work */
	_asm			MOV		DX,AX
	_asm			MOV		AH,014h
	_asm			MOV		BX,DS
	_asm			MOV		CX,kanji_work
	_asm			INT		018h
	/* モードチェック／太文字 */
	_asm			TEST	kanji_mode,001h
	_asm			JE		ASMJ20
	/* 強調文字変換 kanji_work -> kanji_work */
	_asm			MOV		BX,kanji_work
	_asm			MOV		CX,16
	_asm	ASML01:	ADD		BX,00002h
	_asm			MOV		AX,DS:[BX]
	_asm			XCHG	AH,AL
	_asm			SHL		AX,1
	_asm			XCHG	AH,AL
	_asm			OR		DS:[BX],AX
	_asm			LOOP	ASML01
	/* Ｂプレーン書き込み */
	_asm	ASMJ20:	MOV		AX,0A800h
	_asm			MOV		ES,AX
	_asm			MOV		AL,kanji_color
	_asm			AND		AL,010h
	_asm			JNE		ASMJ03
	_asm			MOV		AL,kanji_color
	_asm			AND		AL,001h
	_asm			JE		ASMJ02
	_asm			CALL	ASMC01
	_asm			JMP		ASMJ03
	_asm	ASMJ02:	CALL	ASMC02
	/* Ｒプレーン書き込み */
	_asm	ASMJ03:	MOV		AX,0B000h
	_asm			MOV		ES,AX
	_asm			MOV		AL,kanji_color
	_asm			AND		AL,020h
	_asm			JNE		ASMJ05
	_asm			MOV		AL,kanji_color
	_asm			AND		AL,002h
	_asm			JE		ASMJ04
	_asm			CALL	ASMC01
	_asm			JMP		ASMJ05
	_asm	ASMJ04:	CALL	ASMC02
	/* Ｇプレーン書き込み */
	_asm	ASMJ05:	MOV		AX,0B800h
	_asm			MOV		ES,AX
	_asm			MOV		AL,kanji_color
	_asm			AND		AL,040h
	_asm			JNE		ASMJ07
	_asm			MOV		AL,kanji_color
	_asm			AND		AL,004h
	_asm			JE		ASMJ06
	_asm			CALL	ASMC01
	_asm			JMP		ASMJ07
	_asm	ASMJ06:	CALL	ASMC02
	/* Ｅプレーン書き込み */
	_asm	ASMJ07:	MOV		AX,0E000h
	_asm			MOV		ES,AX
	_asm			MOV		AL,kanji_color
	_asm			AND		AL,080h
	_asm			JNE		ASMJ15
	_asm			MOV		AL,kanji_color
	_asm			AND		AL,008h
	_asm			JE		ASMJ08
	_asm			CALL	ASMC01
	_asm			JMP		ASMJ09
	_asm	ASMJ08:	CALL	ASMC02
	_asm	ASMJ15:	JMP		ASMJ09
	/* ＶＲＡＭ書き込み（白） */
	_asm	ASMC01:	MOV		SI,kanji_work
	_asm			MOV		BX,kanji_address
	_asm			MOV		DX,00000h
	_asm			MOV		CX,16
	_asm	ASML02:	ADD		SI,2
	_asm			MOV		AX,DS:[SI]
	
	/* モードチェック */
	_asm			TEST	kanji_mode,008h
	_asm			JE		ASMJ17
	_asm			XOR		AX,0FFFFh
	
	_asm	ASMJ17:	TEST	kanji_mode,004h
	_asm			JE		ASMJ12
	_asm			CALL	ASMC03
	_asm			MOV		AL,AH
	_asm			PUSH	DX
	_asm			CALL	ASMC03
	_asm			POP		AX
	_asm	ASMJ12:	OR		ES:[BX],AX
	_asm			OR		ES:[BX+2],DX
	_asm			TEST	kanji_mode,002h
	_asm			JE		ASMJ10
	_asm			ADD		BX,80
	_asm			OR		ES:[BX],AX
	_asm			OR		ES:[BX+2],DX
	_asm	ASMJ10:	ADD		BX,80
	_asm			LOOP	ASML02
	_asm			RET
	/* ＶＲＡＭ書き込み（黒） */
	_asm	ASMC02:	MOV		SI,kanji_work
	_asm			MOV		BX,kanji_address
	_asm			MOV		DX,00000h
	_asm			MOV		CX,16
	_asm	ASML03:	ADD		SI,2
	_asm			MOV		AX,DS:[SI]
	
	_asm			TEST	kanji_mode,008h
	_asm			JE		ASMJ16
	_asm			XOR		AX,0FFFFh
	
	_asm	ASMJ16:	TEST	kanji_mode,004h
	_asm			JE		ASMJ13
	_asm			CALL	ASMC03
	_asm			MOV		AL,AH
	_asm			PUSH	DX
	_asm			CALL	ASMC03
	_asm			POP		AX
	_asm	ASMJ13:	OR		ES:[BX],AX
	_asm			XOR		AX,0FFFFh
	_asm			XOR		DX,0FFFFh
	_asm			AND		ES:[BX],AX
	_asm			AND		ES:[BX+2],DX
	_asm			TEST	kanji_mode,002h
	_asm			JE		ASMJ11
	_asm			ADD		BX,80
	_asm			AND		ES:[BX],AX
	_asm			AND		ES:[BX+2],DX
	_asm	ASMJ11:	ADD		BX,80
	_asm			LOOP	ASML03
	_asm			RET
	_asm	ASMC03:	PUSH	BX
	_asm			PUSH	CX
	_asm			MOV		BX,0C000h
	_asm			MOV		CX,08002h
	_asm			XOR		DX,DX
	_asm	ASML04:	TEST	AL,CH
	_asm			JE		ASMJ14
	_asm			OR		DX,BX
	_asm	ASMJ14:	SHR		BX,CL
	_asm			SHR		CH,01
	_asm			JNE		ASML04
	_asm			XCHG	DL,DH
	_asm			POP		CX
	_asm			POP		BX
	_asm			RET
	/* モードチェック／高速表示 */
	_asm	ASMJ09:	TEST	kanji_mode,080h
	_asm			JE		ASMJ22
	/* 高速表示から戻る */
	_asm			MOV		AX,01B00h
	_asm			INT		018h
	/* 終了処理 */
	_asm	ASMJ22:	POP		SI
	_asm			POP		ES
	
	ioregs.x.ax=0x1B00;
	int86(0x18,&ioregs,&ioregs);
	
	return;
}


/* 漢字ＧＰ表示／文字列 */
/******************************************************************************
グラフィック画面に漢字文字列を表示します。

kanji_str		表示する漢字文字列の格納ポインタ
kanji_color		表示する漢字の色 □□□□□□□□
								 | | | | E G R B - 表示色プレーン
								 E G R B --------- 無視プレーン
kanji_address	表示するアドレス (X+Y*80)
kanji_mode		表示するモード □□□□□□□□
									   | | | +- 太文字
									   | | +--- 縦２倍
									   | +----- 横２倍
									   +------- ポジネガ反転

*/
void SIL_gpks0( unsigned char *kanji_str,
				unsigned char kanji_color,
				unsigned int kanji_address,
				unsigned char kanji_mode)
{
	union
	{
		unsigned int full;
		struct
		{
			unsigned char h;
			unsigned char l;
		}h;
	}kan;
	
	while(*kanji_str!=0x00)
	{
		kan.h.h=*kanji_str;
		kanji_str++;
		kan.h.l=*kanji_str;
		kanji_str++;
		SIL_gpko0(kan.full,kanji_color,kanji_address,kanji_mode);
		kanji_address+=2;
		if((kanji_mode&0x04)==0x04)
		{
			kanji_address+=2;
		}
	}
	
	return;
}
