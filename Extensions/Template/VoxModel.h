#pragma once
#include <vector>
#include <map>
#include <unordered_map>
class VoxModel
{
public:
	struct VoxVoxel {
		unsigned char x;
		unsigned char y;
		unsigned char z;
		unsigned char i;
		bool invisible;
	};

	unsigned int sizeX;
	unsigned int sizeY;
	unsigned int sizeZ;
	unsigned int numVoxels;
	std::unordered_map<int, VoxVoxel*> voxVoxels;

	void AddVoxel(unsigned char x, unsigned char y, unsigned char z, unsigned char c, bool invisible);
	bool IsVoxelAt(unsigned char x, unsigned char y, unsigned char z);
	bool GetVoxelAt(unsigned char x, unsigned char y, unsigned char z, VoxModel::VoxVoxel* result);
	void Reset();
	void Destroy();

	VoxModel();
	~VoxModel();
private:
	static int GetHash(unsigned char x, unsigned char y, unsigned char z);
};

