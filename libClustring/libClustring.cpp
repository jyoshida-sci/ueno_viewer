#include "libClustring.h"


Clustring2D::Clustring2D(void)
{
}


Clustring2D::~Clustring2D(void)
{
}


int Clustring2D::GetBlockID(int val){
	const int HashSize = 4;
	return (int)(val / HashSize);
}



std::vector<cluster2d> Clustring2D::DoClustring2D(const int z, const cv::Mat src, const double d)
{


	//making hash table
	hash_pix.clear();//hash_pix[iblockY][iblockX][i]
	int NblockX = GetBlockID(src.cols);
	int NblockY = GetBlockID(src.rows);

	for (int y = 0; y<NblockY; y++){
		std::vector<std::vector<pixel*>> hash_pix_tmp;//hash_pix_tmp[iblockY][i]
		for (int x = 0; x<NblockX; x++){
			std::vector<pixel*> vpix_tmp;
			hash_pix_tmp.push_back(vpix_tmp);
		}
		hash_pix.push_back(hash_pix_tmp);
	}


	//fill data
	std::vector<pixel*> vpix;
	for (int y = 0; y<src.rows; y++){
		for (int x = 0; x<src.cols; x++){

			int b = src.data[y * src.step + x * src.elemSize() + 0];
			if ( b == 0) continue;
			pixel* p = new pixel;
			p->x = x;
			p->y = y;
			p->b = b;
			p->isClusterd = false;//not clusterd
			vpix.push_back(p);

			int iblockX = GetBlockID(x);
			int iblockY = GetBlockID(y);
			hash_pix[iblockY][iblockX].push_back(p);
		}
	}


	//sort
	std::sort(vpix.begin(), vpix.end(),
		[](const pixel* L, const pixel* R)
	{
		return L->b > R->b;
	});


	//clustering
	std::vector<cluster2d> vc;
	for (int i = 0; i<vpix.size(); i++){

		if (vpix[i]->isClusterd) continue;//It is already a member of a cluster 

		cluster2d c;
		vpix[i]->isClusterd = true;//It is core of a new cluster 
		c.vp.push_back(*vpix[i]);

		//making candidates
		std::vector<pixel*> vpixj;
		int iblockX = GetBlockID(vpix[i]->x);
		int iblockY = GetBlockID(vpix[i]->y);
		for (int iy = iblockY - 1; iy <= iblockY + 1; iy++){
			if (iblockY < 0) continue;
			if (iblockY > NblockY) continue;
			for (int ix = iblockX - 1; ix <= iblockX + 1; ix++){
				if (iblockX < 0) continue;
				if (iblockX > NblockX) continue;
				for (int p = 0; p<hash_pix[iblockY][iblockX].size(); p++){
					vpixj.push_back(hash_pix[iblockY][iblockX][p]);
				}
			}
		}


		//distance
		for (int j = 0; j<vpixj.size(); j++){
			if (vpixj[j]->isClusterd) continue;
			double rsq = (vpix[i]->x - vpixj[j]->x)*(vpix[i]->x - vpixj[j]->x) + (vpix[i]->y - vpixj[j]->y)*(vpix[i]->y - vpixj[j]->y);
			if (rsq > d*d) continue;
			vpixj[j]->isClusterd = true;
			c.vp.push_back(*vpixj[j]);
		}


		//calc centroid
		c.brisum = 0;
		c.x = 0.0;
		c.y = 0.0;
		c.z = z*1.0;
		c.isClusterd = false;
		for (int pi = 0; pi<c.vp.size(); pi++){
			c.brisum += c.vp[pi].b;
			c.x += c.vp[pi].x * c.vp[pi].b;
			c.y += c.vp[pi].y * c.vp[pi].b;
		}
		c.x /= c.brisum;
		c.y /= c.brisum;
		vc.push_back(c);
	}//vpix[i]


	//release
	for (int i = 0; i<vpix.size(); i++){
		delete vpix[i];
	}


	return vc;
}





Clustring3D::Clustring3D(void)
{
}


Clustring3D::~Clustring3D(void)
{
}


int Clustring3D::GetBlockIDXY(int val)
{
	const int HashSize = 4;
	return (int)(val / HashSize);
}


int Clustring3D::GetBlockIDZ(int val)
{
	const int HashSize = 2;
	return (int)(val / HashSize);
}


std::vector<cluster3d> Clustring3D::DoClustring3D(std::vector<cluster2d> vc, const double dxy, const double dz)
{


	//making hash table
	hash_cls.clear();//hash_pix[iblockZ][iblockY][iblockX][i]
	int NblockX = GetBlockIDXY(sony_wi);
	int NblockY = GetBlockIDXY(sony_he);
	int NblockZ = GetBlockIDZ(54) + 1;

	for (int z = 0; z<NblockZ; z++){
		std::vector<std::vector<std::vector<cluster2d*>>> hash_cls_tmpy;//hash_pix_tmp[iblockY]
		for (int y = 0; y<NblockY; y++){
			std::vector<std::vector<cluster2d*>> hash_cls_tmpx;//hash_pix_tmp[iblockX][i]
			for (int x = 0; x<NblockX; x++){
				std::vector<cluster2d*> vcls_tmp;
				hash_cls_tmpx.push_back(vcls_tmp);
			}
			hash_cls_tmpy.push_back(hash_cls_tmpx);
		}
		hash_cls.push_back(hash_cls_tmpy);
	}


	//fill data
	std::vector<cluster2d*> vcls;
	for (int c = 0; c<vc.size(); c++){
		cluster2d* c2d = &(vc[c]);
		vcls.push_back(c2d);

		int iblockX = GetBlockIDXY(vc[c].x);
		int iblockY = GetBlockIDXY(vc[c].y);
		int iblockZ = GetBlockIDZ(vc[c].z);
		hash_cls[iblockZ][iblockY][iblockX].push_back(c2d);
	}


	//sort
	std::sort(vcls.begin(), vcls.end(),
		[](const cluster2d* L, const cluster2d* R)
	{
		return L->brisum > R->brisum;
	});


	//clustering
	std::vector<cluster3d> vc3d;
	for (int i = 0; i<vcls.size(); i++){

		if (vcls[i]->isClusterd) continue;//It is already a member of a cluster 

		cluster3d c3d;
		vcls[i]->isClusterd = true;//It is core of a new cluster 
		c3d.vc.push_back(*vcls[i]);

		//making candidates
		std::vector<cluster2d*> vc2j;
		int iblockX = GetBlockIDXY(vcls[i]->x);
		int iblockY = GetBlockIDXY(vcls[i]->y);
		int iblockZ = GetBlockIDXY(vcls[i]->z);

		for (int iz = iblockZ - 1; iz <= iblockZ + 1; iz++){
			if (iblockZ < 0) continue;
			if (iblockZ > NblockZ) continue;
			for (int iy = iblockY - 1; iy <= iblockY + 1; iy++){
				if (iblockY < 0) continue;
				if (iblockY > NblockY) continue;
				for (int ix = iblockX - 1; ix <= iblockX + 1; ix++){
					if (iblockX < 0) continue;
					if (iblockX > NblockX) continue;
					for (int p = 0; p<hash_cls[iblockZ][iblockY][iblockX].size(); p++){
						vc2j.push_back(hash_cls[iblockZ][iblockY][iblockX][p]);
					}
				}
			}
		}


		//distance
		for (int j = 0; j<vc2j.size(); j++){
			if (vc2j[j]->isClusterd) continue;
			double rsq = (vcls[i]->x - vc2j[j]->x)*(vcls[i]->x - vc2j[j]->x) + (vcls[i]->y - vc2j[j]->y)*(vcls[i]->y - vc2j[j]->y);
			if (rsq > dxy*dxy + dxy*dxy + dz*dz) continue;
			vc2j[j]->isClusterd = true;
			c3d.vc.push_back(*vc2j[j]);
		}


		//calc centroid
		c3d.brisum = 0;
		c3d.x = 0.0;
		c3d.y = 0.0;
		c3d.z = 0.0;

		for (int pi = 0; pi<c3d.vc.size(); pi++){
			c3d.brisum += c3d.vc[pi].brisum;
			c3d.x += c3d.vc[pi].x * c3d.vc[pi].brisum;
			c3d.y += c3d.vc[pi].y * c3d.vc[pi].brisum;
			c3d.z += c3d.vc[pi].z * c3d.vc[pi].brisum;
		}
		c3d.x /= c3d.brisum;
		c3d.y /= c3d.brisum;
		c3d.z /= c3d.brisum;
		vc3d.push_back(c3d);
	}//vpix[i]

	return vc3d;
}


