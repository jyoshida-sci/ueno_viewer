#include "opencv2\opencv.hpp"

#ifdef _WIN64

#ifdef _DEBUG
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_core249d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_imgproc249d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_highgui249d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_ml249d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_objdetect249d.lib")
#else
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_core249.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_imgproc249.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_highgui249.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_ml249.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x64\\vc10\\lib\\opencv_objdetect249.lib")
#endif

#else

#ifdef _DEBUG
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_core249d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_imgproc249d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_highgui249d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_ml249d.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_objdetect249d.lib")
#else
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_core249.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_imgproc249.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_highgui249.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_ml249.lib")
	#pragma comment(lib,"C:\\opencv\\opencv249\\build\\x86\\vc10\\lib\\opencv_objdetect249.lib")
#endif

#endif
