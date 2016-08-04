#pragma once

#include "..\opencv2411.h"
#include "..\camparams.h"
#include <vector>


struct pixel{
	int x, y, b;
	bool isClusterd;//not clusterd yet = false, already clusterd = true
};

struct cluster2d{
	double x, y;
	int z;
	int brisum;
	std::vector<pixel> vp;
	bool isClusterd;//not clusterd yet = false, already clusterd = true
};

struct cluster3d{
	double x, y, z;
	int brisum;
	double dist;
	std::vector<cluster2d> vc;
	bool isClusterd;//not clusterd yet = false, already clusterd = true
};





class Clustring2D
{
public:
	Clustring2D(void);
	~Clustring2D(void);

	std::vector<cluster2d> DoClustring2D(const int z, const cv::Mat src, const double d);
	int Clustring2D::GetBlockID(int val);

private:
	std::vector<std::vector<std::vector<pixel*>>> hash_pix;//hash_pix[iblockY][iblockX][i]


};



class Clustring3D
{
public:
	Clustring3D(void);
	~Clustring3D(void);

	std::vector<cluster3d> DoClustring3D(std::vector<cluster2d> vc, const double dxy, const double dz);
	int GetBlockIDXY(int val);
	int GetBlockIDZ(int val);

private:
	std::vector<std::vector<std::vector<std::vector<cluster2d*>>>> hash_cls;//hash_cls[iblockZ][iblockY][iblockX][i]


};

