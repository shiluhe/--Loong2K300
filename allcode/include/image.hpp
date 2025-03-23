//数据类型声明（方便移植——移植的时候可以删掉，改成你自己的） 
typedef   signed          char int8;
typedef   signed short     int int16;
typedef   signed           int int32;
typedef unsigned          char uint8;
typedef unsigned short     int uint16;
typedef unsigned           int uint32;

#define image_h 120 //height
#define image_w 160 //width
#define white_pixel	255
#define black_pixel	0

#define border_max	image_w-2 //图像宽边界最大值
#define border_min	1	//图像宽边界最小值	

extern double image_process(void);
