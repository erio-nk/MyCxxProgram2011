/*!
	@file	VMD.cpp

	@author	E.Nakayama
*/

#include "VMD.h"

using namespace Marda;
using namespace MMD;

bool MMD::LoadVMD(VMD& vmd, const TCHAR* path)
{
	FILE* fp;
	if (0 != _tfopen_s(&fp, path, _T("rb")))
		return false;
	
	// ヘッダ。
	static const char SIGNATURE[] = "Vocaloid Motion Data 0002";
	fread(&vmd.header, sizeof(vmd.header), 1, fp);
	if (strcmp(vmd.header.VmdHeader, SIGNATURE) != 0)
	{
		return false;
	}

	// ボーン用モーションデータ。
	fread(&vmd.motion_count, sizeof(vmd.motion_count), 1, fp);

	vmd.motion.resize(vmd.motion_count);
	fread(&vmd.motion[0], sizeof(VMD::Motion), vmd.motion_count, fp);

	// 表情用アニメーションデータ。
	fread(&vmd.skin_count, sizeof(vmd.skin_count), 1, fp);

	vmd.skin.resize(vmd.skin_count);
	if (!vmd.skin.empty())
		fread(&vmd.skin[0], sizeof(VMD::Skin), vmd.skin_count, fp);
	
	fclose(fp);
	
	return true;
}
