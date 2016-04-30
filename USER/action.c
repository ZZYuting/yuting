#include "uart5.h"
#include "action.h"
#include "uart5.h"
#include "action.h"
void zhili()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,501,150);
		Action_write(2,535,150);
		Action_write(3,262,150);
		Action_write(4,437,150);
		Action_write(5,505,150);
		Action_write(11,496,150);
		Action_write(12,485,150);
		Action_write(13,774,150);
		Action_write(14,575,150);
		Action_write(15,511,150);
		Action();
	}
}
void l_0tai()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,499,150);
		Action_write(2,531,150);
		Action_write(3,262,150);
		Action_write(4,435,150);
		Action_write(5,628,300);
		Action_write(11,495,200);
		Action_write(12,484,200);
		Action_write(13,774,200);
		Action_write(14,574,200);
		Action_write(15,545,270);
		Action();
	}
}
void l_0mai_1()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,498,200);
		Action_write(2,477,300);
		Action_write(3,274,200);
		Action_write(4,466,600);
		Action_write(5,626,200);
		Action_write(11,498,200);
		Action_write(12,482,200);
		Action_write(13,775,200);
		Action_write(14,583-11,200);
		Action_write(15,543+20,200);
		Action();
	}
}
void l_0mai_2()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,498,200);
		Action_write(2,477,300);
		Action_write(3,274,200);
		Action_write(4,473,600);
		Action_write(5,587,200);
		Action_write(11,498,200);
		Action_write(12,482,200);
		Action_write(13,783,200);
		Action_write(14,583-11,200);
		Action_write(15,543,200);
		Action();
	}
}
//void l_mai_2()
//{
//	unsigned char i;
//	for(i=0;i<100;i++)
//	{
//		Action_write(1,498,200);
//		Action_write(2,477,300);
//		Action_write(3,274,200);
//		Action_write(4,466,600);
//		Action_write(5,626,200);
//		Action_write(11,498,200);
//		Action_write(12,482,200);
//		Action_write(13,775,200);
//		Action_write(14,583-11,200);
//		Action_write(15,543,200);
//		Action();
//	}
//}

void l_luo()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,500-1,200);
		Action_write(2,477,200);
		Action_write(3,274,200);
		Action_write(4,460,200);
		Action_write(5,507,230);
		Action_write(11,498,200);
		Action_write(12,483,200);
		Action_write(13,792,200);
		Action_write(14,571,200);
		Action_write(15,518,200);
		Action();
	}
}

void r_tai()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,504,200);
		Action_write(2,435,200);
		Action_write(3,272,200);
		Action_write(4,467,200);
		Action_write(5,478,330);
		Action_write(11,498,200);
		Action_write(12,530,200);
		Action_write(13,790,200);
		Action_write(14,569,200);
		Action_write(15,417,300);
		Action();
	}
}

void r_mai1()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,534,150-20);
		Action_write(2,471,150-20);
		Action_write(3,321,150-20);
		Action_write(4,420,150-20);
		Action_write(5,486-20,150-20);
		Action_write(11,540,150-20);
		Action_write(12,560,150-20);
		Action_write(13,725,100);
		Action_write(14,560,150-20);
		Action_write(15,418,150-20);
		Action();
	}
}

void r_mai2()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,534,200);
		Action_write(2,471,200);
		Action_write(3,321,200);
		Action_write(4,420,200);
		Action_write(5,486,200);
		Action_write(11,540,200);
		Action_write(12,560,200);
		Action_write(13,725,200);
		Action_write(14,560,200);
		Action_write(15,443,200);
		Action();
	}
}
void r_luo()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,537,200);
		Action_write(2,468,200);
		Action_write(3,321,200);
		Action_write(4,398,200);
		Action_write(5,500,200);
		Action_write(11,530,200);
		Action_write(12,559,230);
		Action_write(13,725,200);
		Action_write(14,588,200);
		Action_write(15,509,200);
		Action();
	}
}
void l_tai()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,536,150);
		Action_write(2,468,150);
		Action_write(3,321,150);
		Action_write(4,398,150);
		Action_write(5,617,300);
		Action_write(11,526,200);
		Action_write(12,558,200);
		Action_write(13,722,200);
		Action_write(14,588,200);
		Action_write(15,536,270);
		Action();
	}
}
//void r_mai()
//{
//	unsigned char i;
//	for(i=0;i<100;i++)
//	{
//		Action_write(1,539,200);
//		Action_write(2,457,200);
//		Action_write(3,270,200);
//		Action_write(4,451,200);
//		Action_write(5,481,200);
//		Action_write(11,533,200);
//		Action_write(12,519,200);
//		Action_write(13,790,200);
//		Action_write(14,584,200);
//		Action_write(15,433,200);
//		Action();
//	}
//}
/*
4.27 17µã25 ³¢ÊÔ
*/
//void r_begin()
//{
//	unsigned char i;
//	for(i=0;i<100;i++)
//	{
//		Action_write(1,499,200);
////		Action_write(2,465,200);
//		Action_write(2,437,200);
//		Action_write(3,414,200);
//		Action_write(4,374,200);
//		Action_write(5,490,200);   //478
//		Action_write(11,490,200);
//		Action_write(12,469,200);
//		Action_write(13,784,200);
//		Action_write(14,586,200);
//		Action_write(15,501,200);      //404
//		Action();
//	}
//}