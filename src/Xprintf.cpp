#include <DxLib.h>
#include <string.h>
#include "Xprintf.h"
#include "Image.h"

static int size;
static int color;
static int position;

/************************************************************
�֐��� : Xprintf_Init
����   : �Ȃ�
�ԋp�l : �Ȃ�
���l   : ������
*************************************************************/
void Xprintf_Init()
{
	size = 1;
	color = 0;
	position = 0;
}

/************************************************************
�֐��� : Xprintf
����   : ������,x���W,y���W
�ԋp�l : �Ȃ�
�⑫   : ������******
		 # : �c�@�}�[�N
         @ : (c)�}�[�N
		 ^ : (p)�}�[�N
���l   : �g�p����ۂ́AXprintf_Init��K���Ăяo������Ɏg���Ă��������B
*************************************************************/
void Xprintf(const char* aText,int aX,int aY) {
	int textnum;
	int between = size * 8;
	int kazu = strlen(aText);
	// �\���ʒu
	switch (position) {
	case 0:
		aX += between / 2;
		break;
	case 1:
		aX += between / 2;
		aX -= kazu * between;
		break;
	case 2:
		aX -= between / 2;
		aX -= (kazu * between / 2) - between;
		break;
	}
	aY += between / 2;
	// �\��
	for (int i = 0; aText[i] != '\0'; ++i) {
		switch (aText[i]) {
		case 'A':
			textnum = 0;
			break;
		case'B':
			textnum = 1;
			break;
		case'C':
			textnum = 2;
			break;
		case'D':
			textnum = 3;
			break;
		case'E':
			textnum = 4;
			break;
		case'F':
			textnum = 5;
			break;
		case'G':
			textnum = 6;
			break;
		case'H':
			textnum = 7;
			break;
		case'I':
			textnum = 8;
			break;
		case'J':
			textnum = 9;
			break;
		case'K':
			textnum = 10;
			break;
		case'L':
			textnum = 11;
			break;
		case'M':
			textnum = 12;
			break;
		case'N':
			textnum = 13;
			break;
		case'O':
			textnum = 14;
			break;
		case'P':
			textnum = 15;
			break;
		case'Q':
			textnum = 16;
			break;
		case'R':
			textnum = 17;
			break;
		case'S':
			textnum = 18;
			break;
		case'T':
			textnum = 19;
			break;
		case'U':
			textnum = 20;
			break;
		case'V':
			textnum = 21;
			break;
		case'W':
			textnum = 22;
			break;
		case'X':
			textnum = 23;
			break;
		case'Y':
			textnum = 24;
			break;
		case'Z':
			textnum = 25;
			break;
		case'0':
			textnum = 26;
			break;
		case'1':
			textnum = 27;
			break;
		case'2':
			textnum = 28;
			break;
		case'3':
			textnum = 29;
			break;
		case'4':
			textnum = 30;
			break;
		case'5':
			textnum = 31;
			break;
		case'6':
			textnum = 32;
			break;
		case'7':
			textnum = 33;
			break;
		case'8':
			textnum = 34;
			break;
		case'9':
			textnum = 35;
			break;
		case'a':
			textnum = 36;
			break;
		case'b':
			textnum = 37;
			break;
		case'c':
			textnum = 38;
			break;
		case'd':
			textnum = 39;
			break;
		case'e':
			textnum = 40;
			break;
		case'f':
			textnum = 41;
			break;
		case'g':
			textnum = 42;
			break;
		case'h':
			textnum = 43;
			break;
		case'i':
			textnum = 44;
			break;
		case'j':
			textnum = 45;
			break;
		case'k':
			textnum = 46;
			break;
		case'l':
			textnum = 47;
			break;
		case'm':
			textnum = 48;
			break;
		case'n':
			textnum = 49;
			break;
		case'o':
			textnum = 50;
			break;
		case'p':
			textnum = 51;
			break;
		case'q':
			textnum = 52;
			break;
		case'r':
			textnum = 53;
			break;
		case's':
			textnum = 54;
			break;
		case't':
			textnum = 55;
			break;
		case'u':
			textnum = 56;
			break;
		case'v':
			textnum = 57;
			break;
		case'w':
			textnum = 58;
			break;
		case'x':
			textnum = 59;
			break;
		case'y':
			textnum = 60;
			break;
		case'z':
			textnum = 61;
			break;
		case'#':
			textnum = 62;
			break;
		case'&':
			textnum = 63;
			break;
		case' ':
			textnum = 64;
			break;
		case'.':
			textnum = 72;
			break;
		case'!':
			textnum = 81;
			break;
		case '/':
			textnum = 91;
			break;
		case':':
			textnum = 92;
			break;
		case'[':
			textnum = 97;
			break;
		case']':
			textnum = 98;
			break;
		case'@':
			textnum = 104;
			break;
		case'^':
			textnum = 121;
			break;
		case'~':
			textnum = 65;
			break;
		case'<':
			textnum = 122;
			break;
		case'{':
			textnum = 123;
			break;
		case'}':
			textnum = 124;
			break;
		case'>':
			textnum = 125;
			break;
		default:
			textnum = 64;
			break;
		}
		// �\��
		switch (color) {
		case 0:
			DrawRotaGraph(aX + (i * between), aY, size, 0, Image::getIns()->getImage(Image_Font_a, textnum), TRUE);
			break;
		case 1:
			DrawRotaGraph(aX + (i * between), aY, size, 0, Image::getIns()->getImage(Image_Font_b, textnum), TRUE);
			break;
		default:
			break;
		}
	}
}

/************************************************************
�֐��� : XNprintf
����   : int�^�ϐ�,x���W,y���W
�ԋp�l : �Ȃ�
�⑫   : �Ȃ�
���l   : �g�p����ۂ́AXprintf_Init��K���Ăяo������Ɏg���Ă��������B
*************************************************************/
void XNprintf(int aVar, int aX, int aY) {
    char Text[256];
	sprintf_s(Text, 16, "%d", aVar);
	Xprintf(Text, aX, aY);
}

/************************************************************
�֐��� : Xprintf_Size
����   : �T�C�Y
�ԋp�l : �Ȃ�
�⑫   : �T�C�Y���Z�b�g�ł��܂�
���l   : �g�p����ۂ́AXprintf_Init��K���Ăяo������Ɏg���Ă��������B
*************************************************************/
void Xprintf_Size(int aSize) {
	size = aSize;
}

/************************************************************
�֐��� : Xprintf_Color
����   : �F
�ԋp�l : �Ȃ�
�⑫   : 
		 �F**********
		 0 : ��
		 1 : �I�����W
���l   : �g�p����ۂ́AXprintf_Init��K���Ăяo������Ɏg���Ă��������B
*************************************************************/
void Xprintf_Color(int aColor) {
	color = aColor;
}

/************************************************************
�֐��� : Xprintf_Position
����   : �\���ʒu
�ԋp�l : �Ȃ�
�⑫   : �\���ʒu****
		 0 : ���l��
		 1 : �����l��
		 2 : �E�l��
���l   : �g�p����ۂ́AXprintf_Init��K���Ăяo������Ɏg���Ă��������B
*************************************************************/
void Xprintf_Position(int aPosition) {
	position = aPosition;
}