#include "opencv2\opencv.hpp"

#ifdef _WIN64

#ifdef _DEBUG
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x64\\vc12\\lib\\opencv_core2411d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x64\\vc12\\lib\\opencv_imgproc2411d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x64\\vc12\\lib\\opencv_highgui2411d.lib")
#else
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x64\\vc12\\lib\\opencv_core2411.lib")
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x64\\vc12\\lib\\opencv_imgproc2411.lib")
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x64\\vc12\\lib\\opencv_highgui2411.lib")
#endif

#else

#ifdef _DEBUG
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x86\\vc12\\lib\\opencv_core2411d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x86\\vc12\\lib\\opencv_imgproc2411d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x86\\vc12\\lib\\opencv_highgui2411d.lib")
#else
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x86\\vc12\\lib\\opencv_core2411.lib")
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x86\\vc12\\lib\\opencv_imgproc2411.lib")
	#pragma comment(lib,"C:\\opencv\\opencv2411\\build\\x86\\vc12\\lib\\opencv_highgui2411.lib")
#endif

#endif
