#include "VoxAnimation.h"

void VoxAnimation::ReadChunk(FILE* file) {
	fread(&temp_chunk, sizeof(temp_chunk), 1, file);
}

void VoxAnimation::RollbackChunk(FILE* file) {
	fseek(file, -12, SEEK_CUR);
}

bool VoxAnimation::ReadPack(FILE* file) {
	ReadChunk(file);
	if (strncmp(temp_chunk.id, "PACK", 4) != 0) {
		numModels = 1;
		return false;
	}
	fread(&numModels, sizeof(numModels), 1, file);
	return true;
}

bool VoxAnimation::ReadSize(FILE* file, VoxModel* voxModel) {
	ReadChunk(file);
	if (strncmp(temp_chunk.id, "SIZE", 4) != 0) {
		return false;
	}
	fread(&voxModel->sizeX, sizeof(voxModel->sizeX), 1, file);
	fread(&voxModel->sizeZ, sizeof(voxModel->sizeZ), 1, file);
	fread(&voxModel->sizeY, sizeof(voxModel->sizeY), 1, file);
	depthSize = max(depthSize, max(voxModel->sizeX, max(voxModel->sizeY, voxModel->sizeZ))) * 2.0f;
	return true;
}

bool VoxAnimation::ReadXYZI(FILE* file, VoxModel* voxModel) {
	ReadChunk(file);
	if (strncmp(temp_chunk.id, "XYZI", 4) != 0) {
		return false;
	}
	fread(&voxModel->numVoxels, sizeof(voxModel->numVoxels), 1, file);
	for (unsigned int i = 0; i < voxModel->numVoxels; i++) {
		unsigned char x; fread(&x, sizeof(x), 1, file);
		unsigned char z; fread(&z, sizeof(z), 1, file);
		unsigned char y; fread(&y, sizeof(y), 1, file);
		unsigned char c; fread(&c, sizeof(c), 1, file);
		voxModel->AddVoxel(x, y, z, c, false);
	}
	for (auto it = voxModel->voxVoxels.begin(); it != voxModel->voxVoxels.end(); it++) {
		VoxModel::VoxVoxel* voxVoxel = it->second;
		if (voxModel->IsVoxelAt(voxVoxel->x - 1, voxVoxel->y, voxVoxel->z) &&
			voxModel->IsVoxelAt(voxVoxel->x + 1, voxVoxel->y, voxVoxel->z) &&
			voxModel->IsVoxelAt(voxVoxel->x, voxVoxel->y - 1, voxVoxel->z) &&
			voxModel->IsVoxelAt(voxVoxel->x, voxVoxel->y + 1, voxVoxel->z) &&
			voxModel->IsVoxelAt(voxVoxel->x, voxVoxel->y, voxVoxel->z - 1) &&
			voxModel->IsVoxelAt(voxVoxel->x, voxVoxel->y, voxVoxel->z + 1)) {
			voxVoxel->invisible = true;
		}
	}
	return true;
}

bool VoxAnimation::ReadRGBA(FILE* file) {
	ReadChunk(file);
	if (strncmp(temp_chunk.id, "RGBA", 4) != 0) {
		return false;
	}
	for (int i = 0; i <= 254; i++) {
		unsigned char r; fread(&r, sizeof(r), 1, file);
		unsigned char g; fread(&g, sizeof(g), 1, file);
		unsigned char b; fread(&b, sizeof(b), 1, file);
		unsigned char a; fread(&a, sizeof(a), 1, file);
		palette[i + 1] = RGB(r, g, b);
	}
	return true;
}

bool VoxAnimation::ReadMatt(FILE* file) {
	return false;
}

bool VoxAnimation::ReadMain(FILE* file) {
	ReadChunk(file);
	if (strncmp(temp_chunk.id, "MAIN", 4) != 0) {
		return false;
	}
	if (!ReadPack(file)) {
		RollbackChunk(file);
	}
	voxModels = new VoxModel[numModels];
	for (unsigned int i = 0; i < numModels; i++) {
		VoxModel* voxModel = new VoxModel();
		if (!ReadSize(file, voxModel) || !ReadXYZI(file, voxModel)) {
			return false;
		}
		voxModels[i] = *voxModel;
	}
	depthSize = depthSize * 2.0f;
	if (!ReadRGBA(file)) {
		RollbackChunk(file);
	}
	if (!ReadMatt(file)) {
		RollbackChunk(file);
	}
	return true;
}

void VoxAnimation::LoadFromFile(mv* mV) {
	if (filename == "") {
		Reset();
		loaded = false;
		return;
	}
	if (currentFilename == filename) {
		loaded = true;
		return;
	}
	FILE *file = fopen(filename.c_str(), "rb");
	if (!file) {
		loaded = false;
		return;
	}
	char signature[4];
	fread(signature, sizeof(signature), 1, file);
	if (strncmp(signature, "VOX ", 4) != 0) {
		loaded = false;
		return;
	}
	unsigned int version;
	fread(&version, sizeof(version), 1, file);
	if (version != 150) {
		loaded = false;
		return;
	}
	bool success = ReadMain(file);
	if (success) {
		currentFilename = filename;
	}
	fclose(file);
	loaded = success;
}

void VoxAnimation::Destroy() {
	if (voxModels != nullptr) {
		delete[] voxModels;
		voxModels = nullptr;
	}
}

void VoxAnimation::Reset() {
	Destroy();
	depthSize = 1.0f;
	numModels = 0;
	currentFrame = 0.0f;
	minSpeed = 30;
	maxSpeed = 60;
	loaded = false;
}

VoxAnimation::VoxAnimation()
{
	Reset();
}

VoxAnimation::~VoxAnimation()
{
	Destroy();
}
