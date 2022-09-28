#include <stdio.h>
#include <graphics.h>		// 图形库的头文件

#define HEIGHT		8		// 转换后的高度
#define WIDTH		8		// 转换后的宽度
#define ASCII1		32		// 转换ASCII数量

struct ASCII
{
	char asc[ASCII1];		// ASCII码的范围
	int gray[ASCII1];		// 灰度值的范围

}as{ ' ', '`', '.', '^', ',', ':', '~', '"', '<', '!', 'c', 't', '+', '{', 'i', '7', '?',
	 'u', '3', '0', 'p', 'w', '4', 'A', '8', 'D', 'X', '%', '#', 'H', 'W', 'M',
	 0, 5, 7, 9, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37,
	 39, 41, 43, 45, 47, 49, 51, 53, 55, 59, 61, 63, 66, 68, 70 };


// 使用二分查找来计算字符与灰度值的匹配
char AsciiToGray(struct ASCII as, int gray)
{
	int low = 0;		// 低
	int mid;			// 中
	int high = ASCII1;	// 高

	// 
	if (gray <= as.gray[0])
	{
		return as.asc[0];		// 返回最低端Ascii码
	}
	else if (gray >= as.asc[ASCII1 - 1])
	{
		return as.asc[ASCII1 - 1];		// 返回最高端Ascii码
	}
	else
	{

		// 二分查找的核心
		while ((high - low) > 1)
		{
			mid = (low + high) / 2;			// 去中间值
			if (gray > as.gray[mid])		// 传入的灰度值大于结构体灰度值
			{
				low = mid + 1;				// 移动低标记
			}
			else
			{
				high = mid - 1;				// 移动高标记
			}
		}
		return as.asc[low];					// 返回ASCII码用于写入文本文件
	}
}

// 将图片转换为字符
void PhotoToAscii(struct ASCII as, char filename[])
{
	int height, windth, gray;		//  图像的高、宽、灰度值
	IMAGE img;

	// 加载图片文件
	loadimage(&img, filename);
	SetWorkingImage(&img);

	// 得到转换后图像的宽和高
	height = getheight() / HEIGHT;
	windth = getwidth() / WIDTH;

	// 打开文件 r 读  w 写
	FILE* file;
	if ((file = fopen("字符图案.txt", "w")) == NULL)
	{
		printf("文件打开失败！\n");
		exit(0);
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < windth; j++)
		{
			gray = 0;

			// 扫描每一块像素的灰度值进行累加
			for (int h = 0; h < HEIGHT; h++)
			{
				for (int w = 0; w < WIDTH; w++)
				{
					gray += GetRValue(RGBtoGRAY(getpixel(j * WIDTH + w, i * HEIGHT + h)));
				}
			}

			// 求像素点平均灰度值
			gray /= (HEIGHT * WIDTH);
			gray = (255 - gray) * 90 / 255;

			fputc(AsciiToGray(as, gray), file);
		}
		// 写入换行
		fputc('\n', file);
	}
	// 关闭打开的文件
	fclose(file);
}


int main()
{
	// 文件名的字符数组
	char filename[256];

	printf("请输入文件名（包含扩展名）：");		// 提示
	gets_s(filename);							// 获取文件名
	PhotoToAscii(as, filename);					// 将图片转换为字符
	printf("转换成功！");

	return 0;
}
