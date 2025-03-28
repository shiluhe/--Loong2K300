//八临域代码
#include "image.hpp"

// 定义 bin_image 为全局变量
uint8_t bin_image[image_h][image_w];  

/*
函数名称：int my_abs(int value)
功能说明：求绝对值
参数说明：
函数返回：绝对值
修改时间：2022年9月8日
备    注：
example：  my_abs( x)；
 */
int my_abs(int value)
{
if(value>=0) return value;
else return -value;
}

/*
函数名称：void get_start_point(uint8 start_row)
功能说明：寻找两个边界的边界点作为八邻域循环的起始点
参数说明：输入任意行数
函数返回：无
example：  get_start_point(image_h-2)
*/
uint8 start_point_l[2] = { 0 };//左边起点的x，y值
uint8 start_point_r[2] = { 0 };//右边起点的x，y值
uint8 get_start_point(uint8 start_row)
{
	uint8 i = 0,l_found = 0,r_found = 0;
	//清零
	start_point_l[0] = 0;//x
	start_point_l[1] = 0;//y

	start_point_r[0] = 0;//x
	start_point_r[1] = 0;//y

		//从中间往左边，先找起点
	for (i = image_w / 2; i > border_min; i--)
	{
		start_point_l[0] = i;//x
		start_point_l[1] = start_row;//y
		if (bin_image[start_row][i] == 1 && bin_image[start_row][i - 1] == 0)
		{
			printf("找到左边起点image[%d][%d]\n", start_row,i);
			l_found = 1;
			break;
		}
	}

	for (i = image_w / 2; i < border_max; i++)
	{
		start_point_r[0] = i;//x
		start_point_r[1] = start_row;//y
		if (bin_image[start_row][i] == 1 && bin_image[start_row][i + 1] == 0)
		{
			printf("找到右边起点image[%d][%d]\n",start_row, i);
			r_found = 1;
			break;
		}
	}

	if(l_found&&r_found)return 1;
	else {
		printf("未找到起点\n");
		return 0;
	} 
}

/*
函数名称：void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
							uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r			：最多需要循环的次数
(*image)[image_w]		：需要进行找点的图像数组，必须是二值图,填入数组名称即可
					   特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic				：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic				：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x				：左边起点横坐标
l_start_y				：左边起点纵坐标
r_start_x				：右边起点横坐标
r_start_y				：右边起点纵坐标
hightest				：循环结束所得到的最高高度
example：
	search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
				start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
 */
#define USE_num image_h*3

 //存放点的x，y坐标
uint16 points_l[(uint16)USE_num][2] = { {  0 } };//左线
uint16 points_r[(uint16)USE_num][2] = { {  0 } };//右线
uint16 dir_r[(uint16)USE_num] = { 0 };//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = { 0 };//用来存储左边生长方向
uint16 data_stastics_l = 0;//统计左边找到点的个数
uint16 data_stastics_r = 0;//统计右边找到点的个数
uint8 hightest = 0;//最高点
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest)
{

	uint8 i = 0, j = 0;

	//左边变量
	uint8 search_filds_l[8][2] = { {  0 } };
	uint8 index_l = 0;
	uint8 temp_l[8][2] = { {  0 } };
	uint8 center_point_l[2] = {  0 };
	uint16 l_data_statics;//统计左边
	//定义八个邻域
	static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是顺时针

	//右边变量
	uint8 search_filds_r[8][2] = { {  0 } };
	uint8 center_point_r[2] = { 0 };//中心坐标点
	uint8 index_r = 0;//索引下标
	uint8 temp_r[8][2] = { {  0 } };
	uint16 r_data_statics;//统计右边
	//定义八个邻域
	static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是逆时针

	l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
	r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

	//第一次更新坐标点  将找到的起点值传进来
	center_point_l[0] = l_start_x;//x
	center_point_l[1] = l_start_y;//y
	center_point_r[0] = r_start_x;//x
	center_point_r[1] = r_start_y;//y

		//开启邻域循环
	while (break_flag--)
	{

		//左边
		for (i = 0; i < 8; i++)//传递8F坐标
		{
			search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
			search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
		}
		//中心坐标点填充到已经找到的点内
		points_l[l_data_statics][0] = center_point_l[0];//x
		points_l[l_data_statics][1] = center_point_l[1];//y
		l_data_statics++;//索引加一

		//右边
		for (i = 0; i < 8; i++)//传递8F坐标
		{
			search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
			search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
		}
		//中心坐标点填充到已经找到的点内
		points_r[r_data_statics][0] = center_point_r[0];//x
		points_r[r_data_statics][1] = center_point_r[1];//y

		index_l = 0;//先清零，后使用
		for (i = 0; i < 8; i++)
		{
			temp_l[i][0] = 0;//先清零，后使用
			temp_l[i][1] = 0;//先清零，后使用
		}

		//左边判断
		for (i = 0; i < 8; i++)
		{
			if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
				&& image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
			{
				temp_l[index_l][0] = search_filds_l[(i)][0];
				temp_l[index_l][1] = search_filds_l[(i)][1];
				index_l++;
				dir_l[l_data_statics - 1] = (i);//记录生长方向
			}

			if (index_l)
			{
				//更新坐标点
				center_point_l[0] = temp_l[0][0];//x
				center_point_l[1] = temp_l[0][1];//y
				for (j = 0; j < index_l; j++)
				{
					if (center_point_l[1] > temp_l[j][1])
					{
						center_point_l[0] = temp_l[j][0];//x
						center_point_l[1] = temp_l[j][1];//y
					}
				}
			}

		}
		if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
			&& points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
			||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
				&& points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
		{
			//printf("三次进入同一个点，退出\n");
			break;
		}
		if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
			&& my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
			)
		{
			//printf("\n左右相遇退出\n");	
			*hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
			//printf("\n在y=%d处退出\n",*hightest);
			break;
		}
		if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
		{
			printf("\n如果左边比右边高了,左边等待右边\n");	
			continue;//如果左边比右边高了，左边等待右边
		}
		if (dir_l[l_data_statics - 1] == 7
			&& (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
		{
			//printf("\n左边开始向下了，等待右边，等待中... \n");
			center_point_l[0] = points_l[l_data_statics - 1][0];//x
			center_point_l[1] = points_l[l_data_statics - 1][1];//y
			l_data_statics--;
		}
		r_data_statics++;//索引加一

		index_r = 0;//先清零，后使用
		for (i = 0; i < 8; i++)
		{
			temp_r[i][0] = 0;//先清零，后使用
			temp_r[i][1] = 0;//先清零，后使用
		}

		//右边判断
		for (i = 0; i < 8; i++)
		{
			if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
				&& image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
			{
				temp_r[index_r][0] = search_filds_r[(i)][0];
				temp_r[index_r][1] = search_filds_r[(i)][1];
				index_r++;//索引加一
				dir_r[r_data_statics - 1] = (i);//记录生长方向
				//printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
			}
			if (index_r)
			{

				//更新坐标点
				center_point_r[0] = temp_r[0][0];//x
				center_point_r[1] = temp_r[0][1];//y
				for (j = 0; j < index_r; j++)
				{
					if (center_point_r[1] > temp_r[j][1])
					{
						center_point_r[0] = temp_r[j][0];//x
						center_point_r[1] = temp_r[j][1];//y
					}
				}

			}
		}


	}

	//取出循环次数
	*l_stastic = l_data_statics;
	*r_stastic = r_data_statics;
}

/*
函数名称：void get_left(uint16 total_L)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_L	：找到的点的总数
example： get_left(data_stastics_l );
 */
uint8 l_border[image_h];//左线数组
uint8 r_border[image_h];//右线数组
uint8 center_line[image_h];//中线数组
void get_left(uint16 total_L)
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	//初始化
	for (i = 0;i<image_h;i++)
	{
		l_border[i] = border_min;
	}
	h = image_h - 2;
	//左边
	for (j = 0; j < total_L; j++)
	{
		//printf("%d\n", j);
		if (points_l[j][1] == h)
		{
			l_border[h] = points_l[j][0]+1;
		}
		else continue; //每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0) 
		{
			break;//到最后一行退出
		}
	}
}
/*
函数名称：void get_right(uint16 total_R)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_R  ：找到的点的总数
example：get_right(data_stastics_r);
 */
void get_right(uint16 total_R)
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	for (i = 0; i < image_h; i++)
	{
		r_border[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
	}
	h = image_h - 2;
	//右边
	for (j = 0; j < total_R; j++)
	{
		if (points_r[j][1] == h)
		{
			r_border[h] = points_r[j][0] - 1;
		}
		else continue;//每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0)break;//到最后一行退出
	}
}


/*
函数名称：void image_draw_rectan(uint8(*image)[image_w])
功能说明：给图像画一个黑框
参数说明：uint8(*image)[image_w]	图像首地址
example： image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8(*image)[image_w])
{

	uint8 i = 0;
	for (i = 0; i < image_h; i++)
	{
		image[i][0] = 0;
		image[i][1] = 0;
		image[i][image_w - 1] = 0;
		image[i][image_w - 2] = 0;

	}
	for (i = 0; i < image_w; i++)
	{
		image[0][i] = 0;
		image[1][i] = 0;
		//image[image_h-1][i] = 0;

	}
//////////////调试打印////////////////////////////
	    //     for (int i = 0; i < image_h; i += 2) {
        //     for (int j = 0; j < image_w; j += 2) {
        //         printf("%d ", bin_image[i][j]);
        //     }
        //     printf("\n");
        // }
////////////////////////////////////////////////////////
}

Mat capture_frame() {
    static VideoCapture cap(0);  // 使用static保持摄像头持续打开
    if (!cap.isOpened()) {
        cerr << "Error opening video stream" << endl;
        return Mat();  // 返回空的Mat对象
    }

    // 设置摄像头参数
    cap.set(CAP_PROP_FRAME_WIDTH, image_w);
    cap.set(CAP_PROP_FRAME_HEIGHT, image_h);
    cap.set(CAP_PROP_FPS, 60);
    double fps = cap.get(CAP_PROP_FPS);  // 读取摄像头实际帧率
    cout << "摄像头当前帧率: " << fps << " FPS" << endl;

    Mat frame;
    cap.read(frame);

    if (frame.empty()) {
        cerr << "Error reading frame" << endl;
        return Mat();  // 返回空的Mat对象
    }
    
    return frame.clone();  // 返回帧的副本以确保数据安全
}

/*
函数名称：void Cam_Address()
功能说明：opencv二值化
example:Cam_Address()
*/
void Cam_Address(Mat frame)
{
    // VideoCapture cap(0);
    // if (!cap.isOpened())
    // {
    //     cerr << "Error opening video stream" << endl;
    //     return;
    // }

    // // 设置摄像头参数
    // cap.set(CAP_PROP_FRAME_WIDTH, image_w);
    // cap.set(CAP_PROP_FRAME_HEIGHT, image_h);
    // cap.set(CAP_PROP_FPS, 60);
	// double fps = cap.get(CAP_PROP_FPS);  // 读取摄像头实际帧率

    // cout << "摄像头当前帧率: " << fps << " FPS" << endl;

    // Mat frame, gray, binary, morph;

    // // 定义 3x3 结构元素
	// //形态学滤波（膨胀与腐蚀）
    // // Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

    // while (true)
    // {
    //     cap.read(frame);
	// 	//imshow("frame", frame);
	// 	// cv::waitkey(0);

    //     if (frame.empty())
    //     {
    //         cerr << "Error reading frame" << endl;
    //         break;
    //     }
		Mat gray, binary, morph;
        // 转灰度
        cvtColor(frame, gray, COLOR_BGR2GRAY);
		gray.convertTo(gray, -1, 1.5, 50); // alpha=1.5（对比度）, beta=50（亮度）
        // 二值化
        threshold(gray, binary, 75, 255, THRESH_BINARY | THRESH_OTSU);
		//adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 2);
		printf("二值化完成\n");

		// 形态学处理：先腐蚀再膨胀（开运算）
        // erode(binary, morph, kernel);
        // dilate(morph, morph, kernel);

        // 将 OpenCV Mat 转换为 C 数组
        // for (int i = 0; i < image_h; i++)
        // {
        //     for (int j = 0; j < image_w; j++)
        //     {
        //         bin_image[i][j] = morph.at<uchar>(i, j) > 0 ? 1 : 0; // 0/1 表示黑白
        //     }
        // }
	for (int i = 0; i < image_h; i++)
        {
            for (int j = 0; j < image_w; j++)
            {
                bin_image[i][j] = (binary.at<uchar>(i, j) > 0) ? 1 : 0; // 0: 黑, 1: 白
            }
        }
        //这里可以选择调试打印
        // for (int i = 0; i < image_h; i += 2) {
        //     for (int j = 0; j < image_w; j += 2) {
        //         printf("%d ", bin_image[i][j]);
        //     }
        //     printf("\n");
        // }

        //break; // 只读取一帧就返回
    //}

    //cap.release();
}

/*
TFT屏幕：展示八临域代码的边线与中线
*/
// void TFT3lmr(){
//     TFTSPI_Init(0); // LCD初始化    0：横屏     1：竖屏
//     cout << "TFTSPI_Init" << endl;
// }
/*
函数名称：void image_process(void)
功能说明：最终处理函数
参数说明：无
example： image_process();
 */
double image_process(Mat frame)
{
uint16 i;
uint8 hightest = 0;//定义一个最高行，tip：这里的最高指的是y值的最小
double error = 0;
double sum_error = 0;
double avg_error = 0;
int count = 0;
// /*这是离线调试用的（总钻风）*/
// Get_image(mt9v03x_image);
// turn_to_bin();
// /*提取赛道边界*/
// image_filter(bin_image);//滤波
Cam_Address(frame);
image_draw_rectan(bin_image);//预处理
//清零
data_stastics_l = 0;
data_stastics_r = 0;
///////////////////////////////////////TFT屏幕显示二值化图像///////////////////////////////////////////
TFTSPI_Init(0); // LCD初始化    0：横屏     1：竖屏
cout << "TFTSPI_Init" << endl;
//1.新TFT调用显示二值化
uint8_t *tft_buffer = (uint8_t *)malloc(image_w * image_h * 2);
BinToTFTFormat((uint8_t*)bin_image, image_w, image_h, tft_buffer);
TFTSPI_Show_Pic2(0, 0, image_w, image_h, tft_buffer);
free(tft_buffer);
////////////////////////////////////////////////////////////////////////////////////////////////////////
if (get_start_point(image_h - 2))//找到起点了，再执行八领域，没找到就一直找
{
	printf("正在开始八领域\n");
	search_l_r((uint16)USE_num, bin_image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);
	printf("八邻域已结束\n");
	// 从爬取的边界线内提取边线 ， 这个才是最终有用的边线
	get_left(data_stastics_l);
	get_right(data_stastics_r);
	//元素处理函数放这里，不要放到if外面去了，不要放到if外面去了，不要放到if外面去了，重要的事说三遍

}
else{
	printf("没有进入八领域\n");
}

///////////////////////////////////////2.TFT显示左右边界点//////////////////////////////////////
// for (i = 0; i < data_stastics_l; i++) {
//     TFTSPI_Draw_Dot(points_l[i][0] + 2, points_l[i][1], u16BLUE);
// }
// for (i = 0; i < data_stastics_r; i++) {
//     TFTSPI_Draw_Dot(points_r[i][0] - 2, points_r[i][1], u16RED);
// }
//////////////////////////////////////////////////////////////////////////////////////////////////

	//计算赛道中线 center_line[i] 及误差 error
    for (i = hightest; i < image_h - 1; i++)
    {
        center_line[i] = (l_border[i] + r_border[i]) >> 1; // 求中线

    // TFTSPI_Draw_Dot(center_line[i], i, u16GREEN);// 画中线（绿色）
    // TFTSPI_Draw_Dot(l_border[i], i, u16BLUE);// 画左边线（蓝色）
    // TFTSPI_Draw_Dot(r_border[i], i, u16RED);// 画右边线（红色）

        // 计算误差 error
        if (i >= (image_h * 2.0 / 3.0)) // 只计算 image_h * 2/3 到 image_h 之间的误差
        {
            error = center_line[i] - (image_w / 2.0); // 赛道中线 - 摄像头中心 (>0：小车右转；<0:小车左转)
            sum_error += error;
            count++;
        }
	}
    // 计算平均误差
    if (count > 0)
    {
        avg_error = sum_error / (double) count;
    }
    
    // 输出最终的平均误差
    printf("图像处理完成！error: %.2f\n", avg_error);
	return avg_error;
}


// //显示图像   改成你自己的就行 等后期足够自信了，显示关掉，显示屏挺占资源的
// ips154_displayimage032_zoom(bin_image[0], image_w, image_h, image_w, image_h,0,0);

// 	//根据最终循环次数画出边界点
// 	for (i = 0; i < data_stastics_l; i++)
// 	{
// 		ips154_drawpoint(points_l[i][0]+2, points_l[i][1], uesr_BLUE);//显示起点
// 	}
// 	for (i = 0; i < data_stastics_r; i++)
// 	{
// 		ips154_drawpoint(points_r[i][0]-2, points_r[i][1], uesr_RED);//显示起点
// 	}

// 	for (i = hightest; i < image_h-1; i++)
// 	{
// 		center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
// 		//求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
// 		//当然也有多组边线的找法，但是个人感觉很繁琐，不建议
// 		ips154_drawpoint(center_line[i], i, uesr_GREEN);//显示起点 显示中线	
// 		ips154_drawpoint(l_border[i], i, uesr_GREEN);//显示起点 显示左边线
// 		ips154_drawpoint(r_border[i], i, uesr_GREEN);//显示起点 显示右边线
// 	}
