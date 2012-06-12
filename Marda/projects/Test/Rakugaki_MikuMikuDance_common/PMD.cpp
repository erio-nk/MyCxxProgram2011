/*!
	@file	PMD.cpp

	@author	E.Nakayama
*/

#include "PMD.h"

using namespace Marda;
using namespace MMD;

namespace
{
	static const char TEXTURE_FILE_NAME_DELIM = '*';
	
}	// end namespace

bool MMD::LoadPMD(PMD& pmd, const TCHAR* path)
{
	FILE* fp;
	if (0 != _tfopen_s(&fp, path, _T("rb")))
		return false;
	
	// ヘッダ。
	fread(&pmd.header, sizeof(pmd.header), 1, fp);
	if (strncmp(pmd.header.magic, "Pmd", 3) != 0)
	{
		return false;
	}
	if (pmd.header.version != 1.0f)
	{
		return false;
	}

	// 頂点。
	fread(&pmd.vert_count, sizeof(pmd.vert_count), 1, fp);

	pmd.vertex.resize(pmd.vert_count);
	fread(&pmd.vertex[0], sizeof(PMD::Vertex), pmd.vert_count, fp);
	
	// 面頂点。
	fread(&pmd.face_vert_count, sizeof(pmd.face_vert_count), 1, fp);

	pmd.face_vert_index.resize(pmd.face_vert_count);
	fread(&pmd.face_vert_index[0], sizeof(unsigned short), pmd.face_vert_count, fp);

	// マテリアル。
	fread(&pmd.material_count, sizeof(pmd.material_count), 1, fp);

	pmd.material.resize(pmd.material_count);
	fread(&pmd.material[0], sizeof(PMD::Material), pmd.material_count, fp);

	// ボーン。
	fread(&pmd.bone_count, sizeof(pmd.bone_count), 1, fp);

	pmd.bone.resize(pmd.bone_count);
	fread(&pmd.bone[0], sizeof(PMD::Bone), pmd.bone_count, fp);

	// IK。
	fread(&pmd.ik_data_count, sizeof(pmd.ik_data_count), 1, fp);

	pmd.ik_data.resize(pmd.ik_data_count);
	for (size_t i = 0, sz = pmd.ik_data.size(); i < sz; ++i)
	{
		fread(&pmd.ik_data[i], offsetof(PMD::IKData, ik_child_bone_index), 1, fp);
		pmd.ik_data[i].ik_child_bone_index.resize(pmd.ik_data[i].ik_chain_length);
		fread(&pmd.ik_data[i].ik_child_bone_index[0], sizeof(unsigned short), pmd.ik_data[i].ik_chain_length, fp);
	}

	// 表情。
	fread(&pmd.skin_count, sizeof(pmd.skin_count), 1, fp);

	pmd.skin.resize(pmd.skin_count);
	for (size_t i = 0, sz = pmd.skin.size(); i < sz; ++i)
	{
		fread(&pmd.skin[i], offsetof(PMD::Skin, skin_vert), 1, fp);
		pmd.skin[i].skin_vert.resize(pmd.skin[i].skin_vert_count);
		fread(&pmd.skin[i].skin_vert[0], sizeof(PMD::Skin::Vert), pmd.skin[i].skin_vert_count, fp);
	}

	fclose(fp);
	
	return true;
}

std::string
MMD::PMD::Material::GetTextureFileName() const
{
	char buf[sizeof(texture_file_name)];
	const char* found = strchr(texture_file_name, TEXTURE_FILE_NAME_DELIM);
	if (is_null(found))
	{
		if (is_not_null(StrRStr(texture_file_name, ".spa")))
			return "";
		return texture_file_name;
	}
	SubStr(buf, texture_file_name, 0, found - texture_file_name);
	return buf;
}

std::string
MMD::PMD::Material::GetSphereMapFileName() const
{
	char buf[sizeof(texture_file_name)];
	const char* found = strchr(texture_file_name, TEXTURE_FILE_NAME_DELIM);
	if (is_null(found))
	{
		if (is_null(StrRStr(texture_file_name, ".spa")))
			return "";
		return texture_file_name;
	}
	size_t found_pos = found - texture_file_name;
	SubStr(buf, texture_file_name, found_pos, strlen(texture_file_name) - found_pos);
	return buf;
}
