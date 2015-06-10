#include "opencv2\opencv.hpp"

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"C:\\opencv\\opencv300\\build\\x64\\vc12\\lib\\opencv_ts300d.lib")
#pragma comment(lib,"C:\\opencv\\opencv300\\build\\x64\\vc12\\lib\\opencv_world300d.lib")
#else
#pragma comment(lib,"C:\\opencv\\opencv300\\build\\x64\\vc12\\lib\\opencv_ts300.lib")
#pragma comment(lib,"C:\\opencv\\opencv300\\build\\x64\\vc12\\lib\\opencv_world300.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib,"C:\\opencv\\opencv300\\build\\x86\\vc12\\lib\\opencv_ts300d.lib")
#pragma comment(lib,"C:\\opencv\\opencv300\\build\\x86\\vc12\\lib\\opencv_world300d.lib")
#else
#pragma comment(lib,"C:\\opencv\\opencv300\\build\\x86\\vc12\\lib\\opencv_ts300.lib")
#pragma comment(lib,"C:\\opencv\\opencv300\\build\\x86\\vc12\\lib\\opencv_world300.lib")

#endif

#endif
