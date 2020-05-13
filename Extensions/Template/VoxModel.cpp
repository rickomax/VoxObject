#include "VoxModel.h"

int VoxModel::GetHash(unsigned char x, unsigned char y, unsigned char z) {
	int result = int(x);
	result = (result << 8) + y;
	result = (result << 8) + z;
	return result;
}

void VoxModel::AddVoxel(unsigned char x, unsigned char y, unsigned char z, unsigned char c, bool invisible) {
	VoxVoxel* voxVoxel = new VoxVoxel();
	voxVoxel->x = x;
	voxVoxel->y = y;
	voxVoxel->z = z;
	voxVoxel->i = c;
	voxVoxel->invisible = invisible;
	int hash = GetHash(x, y, z);
	voxVoxels.insert(std::make_pair(hash, voxVoxel));
}

bool VoxModel::GetVoxelAt(unsigned char x, unsigned char y, unsigned char z, VoxModel::VoxVoxel* result) {
	try {
		auto voxVoxel = voxVoxels.at(GetHash(x, y, z));
		result = voxVoxel;
		return true;
	} catch (std::exception e) {
		
	}
	return false;
}

bool VoxModel::IsVoxelAt(unsigned char x, unsigned char y, unsigned char z) {
	auto it = voxVoxels.find(GetHash(x, y, z));
	if (it != voxVoxels.end())
	{
		return true;
	}
	return false;
}

void VoxModel::Destroy() {
	//for (auto it = voxVoxels.begin(); it != voxVoxels.end(); ++it) {
	//	delete it->second;
	//}
	voxVoxels.clear();
}

void VoxModel::Reset() {
	Destroy();
	sizeX = 0;
	sizeY = 0;
	sizeZ = 0;
	numVoxels = 0;
}

VoxModel::VoxModel()
{
	Reset();
}

VoxModel::~VoxModel()
{
	Destroy();
}
