/*

�O���t�B�b�N�������p����֐��^�V���A�o�[�W���� Ver-0.01 1995/10/28

�K�v include �t�@�C��
�Ȃ�

*/

#include <graph.h>

/* �p���b�g��� */
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


/* �O���t�B�b�N�g�p�J�n */
/******************************************************************************
�O���t�B�b�N�g�p�J�n�E���[�h�^��ʂ����������܂��B
*/
void SIL_gpin0(void)
{
	if(_setvideomode(_98RES16COLOR)==0)
	{
		printf("�r�f�I���[�h�̐ݒ�Ɏ��s���܂����B");
		exit(1);
	}
	
	/* �p���b�g�̃Z�b�g */
//	while( (inp(0x60) & 0x20)!=0x20 );
	_remapallpalette(SIL_gpal);
	
	_setactivepage(0);
	_clearscreen(_GCLEARSCREEN);
	_setvisualpage(0);
	return;
}


/* �O���t�B�b�N�g�p�I�� */
/******************************************************************************
�O���t�B�b�N�g�p�I���B
*/
void SIL_gpen0(void)
{
	_setvideomode(_DEFAULTMODE);
	printf("\x1B[>1l");
	return;
}


/* �O���t�B�b�N�g�p�I���^��ʎc�� */
/******************************************************************************
�O���t�B�b�N�g�p�I���B
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
