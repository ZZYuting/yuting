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
void l_tai()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,499,150);
//		Action_write(2,480,150);//0
		Action_write(2,531,150);
		Action_write(3,262,150);
		Action_write(4,435,150);
		Action_write(5,583,300);
		Action_write(11,495,200);
//		Action_write(12,538,200);//0
		Action_write(12,484,200);
		Action_write(13,774,200);
		Action_write(14,574,200);
		Action_write(15,536,150);
		Action();
	}
}
void l_mai()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,499,200);
		Action_write(2,442,200);
		Action_write(3,342,200);
		Action_write(4,436,200);
		Action_write(5,582,200);
		Action_write(11,491,200);
		Action_write(12,482,200);
		Action_write(13,776,200);
		Action_write(14,580,200);
		Action_write(15,528,200);
		Action();
	}
}
//void l_mai()
//{
//	unsigned char i;
//	for(i=0;i<100;i++)
//	{
//		Action_write(1,524-10,200);//+25
//		Action_write(2,486,200);//-45
//		Action_write(3,282,200);//+20
//		Action_write(4,469+30,200);//+34
//		Action_write(5,572,200);//-11
//		Action_write(11,509,200);//-14
//		Action_write(12,463,200);//21
//		Action_write(13,693,200);//-81
//		Action_write(14,670-30,200-50);//+98
//		Action_write(15,536,200);
//		Action();
//	}
//}
void l_luo()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,500,200);
		Action_write(2,442,200);
		Action_write(3,345,200);
		Action_write(4,424,200);
		Action_write(5,498,230);
		Action_write(11,494,200);
		Action_write(12,485,200);
		Action_write(13,785,200);
		Action_write(14,577,200);
		Action_write(15,511,200);
		Action();
	}
}

void r_tai()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,497,200);
		Action_write(2,479,200);
		Action_write(3,303,200);
		Action_write(4,423,200);
		Action_write(5,483,300);
		Action_write(11,492,200);
		Action_write(12,493,200);
		Action_write(13,783,200);
		Action_write(14,585,200);
		Action_write(15,422,340);
		Action();
	}
}
//void r_tai()
//{
//	unsigned char i;
//	for(i=0;i<100;i++)
//	{
//		Action_write(1,497,200);
//		Action_write(2,479,200);
//		Action_write(3,303,200);
//		Action_write(4,423,200);
//		Action_write(5,483,300);
//		Action_write(11,492,200);
//		Action_write(12,493,200);
//		Action_write(13,783,200);
//		Action_write(14,585,200);
//		Action_write(15,422,240+80);
//		Action();
//	}
//}

void r_mai()
{
	unsigned char i;
	for(i=0;i<100;i++)
	{
		Action_write(1,499,200);
		Action_write(2,498,200);
		Action_write(3,356,200);
		Action_write(4,352,200);
		Action_write(5,480,200);
		Action_write(11,491,200);
		Action_write(12,528,200);
		Action_write(13,783,200);
		Action_write(14,545+20,200+40);
		Action_write(15,426,200+20);
		Action();
	}
}
//void l_luo()
//{
//	unsigned char i;
//	for(i=0;i<100;i++)
//	{
//		Action_write(1,525,200);
//		Action_write(2,454,200);
//		Action_write(3,280,200);
//		Action_write(4,467,200);
//		Action_write(5,511,200);
//		Action_write(11,510,200);
//		Action_write(12,495,230);
//		Action_write(13,690,200);
//		Action_write(14,640,200);
//		Action_write(15,521,200);
//		Action();
//	}
//}
//void r_tai()
//{
//	unsigned char i;
//	for(i=0;i<100;i++)
//	{
//		Action_write(1,501,200);
//		Action_write(2,438,200);
//		Action_write(3,346,200);
//		Action_write(4,422,200);
//		Action_write(5,482-30,200);
//		Action_write(11,490,200);
//		Action_write(12,499,200);
//		Action_write(13,783,200);
//		Action_write(14,576,200);
//		Action_write(15,445,240);
//		Action();
//	}
//}
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