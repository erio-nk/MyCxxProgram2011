// Rakugaki_MikuMikuDance_2.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "Rakugaki_MikuMikuDance_2.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

ApplicationFrame g_app;

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAKUGAKI_MIKUMIKUDANCE_2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAKUGAKI_MIKUMIKUDANCE_2));

	// ���C�����[�v�B
	for (;;)
	{
		if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) 
			{
				break;
			}
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} 
		else 
		{
			g_app.UpdateFrame();
		}
	}
	
	g_app.Release();

	return (int) msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAKUGAKI_MIKUMIKUDANCE_2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RAKUGAKI_MIKUMIKUDANCE_2);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   if (Marda::failed(g_app.Create(hWnd)))
   {
     return FALSE;
   }

   return TRUE;
}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// ----------------------------------------------------------------------------
using namespace Marda;
using namespace Direct3D10;
using namespace D3DXHelpers;

// ----------------------------------------------------------------------------
class MyRenderer : public Renderer
{
public:
	enum SHADER_KIND
	{
		PMD_DEFAULT,	//!< ���_�J���[�̂݁B
		PMD_TEXTURE,	//!< �e�N�X�`���}�b�s���O�L��B

		SHADER_KIND_MAX
	};

	struct ShaderInput
	{
		D3DXVECTOR3 position;
		float color[3];
		D3DXVECTOR2 uv;
	};
		
	bool Create(Device& device)
	{
		if (failed(Renderer::Create(device)))
		{
			return false;
		}
		
		// �V�F�[�_�̍쐬�B
		const TCHAR* SHADER_FILE_PATH = _T("..\\Rakugaki_MikuMikuDance_2\\pmd.sh");
		_shaderDependencyObjects.resize(SHADER_KIND_MAX);

		ShaderDependencyObjects& shaderDefault = _shaderDependencyObjects[PMD_DEFAULT];
		shaderDefault._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS", "vs_4_0", 
			NULL, NULL,
			"PS_default", "ps_4_0"
			);
		if (shaderDefault._shader == ShaderHandle::INVALID)
		{
			return false;
		}
		
		ShaderDependencyObjects& shaderTexture = _shaderDependencyObjects[PMD_TEXTURE];
		shaderTexture._shader = device.CreateShader(
			SHADER_FILE_PATH,
			"VS", "vs_4_0", 
			NULL, NULL,
			"PS_texture", "ps_4_0"
			);
		if (shaderTexture._shader == ShaderHandle::INVALID)
		{
			return false;
		}
		
		// ���̓��C�A�E�g�̍쐬�B
		D3D10_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInput, position), D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(ShaderInput, color), D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(ShaderInput, uv), D3D10_INPUT_PER_VERTEX_DATA, 0},
		};
		
		InputLayoutHandle inputLayout = 
			device.CreateInputLayout(
				layout,
				_countof(layout),
				shaderDefault._shader
				);
		if (inputLayout == InputLayoutHandle::INVALID)
			return false;

		shaderDefault._inputLayout = inputLayout;
		shaderTexture._inputLayout = inputLayout;

		return true;
	}
	
	ShaderHandle GetShader(int shaderKind) const
	{
		return _shaderDependencyObjects[shaderKind]._shader;
	}
	
	InputLayoutHandle GetInputLayout(int shaderKind) const
	{
		return _shaderDependencyObjects[shaderKind]._inputLayout;
	}
	
	void SetShaderViewProjection(D3DXMATRIX mViewSrc, D3DXMATRIX mProjSrc)
	{
		D3DXMatrixTranspose(&mViewSrc, &mViewSrc);
		D3DXMatrixTranspose(&mProjSrc, &mProjSrc);
		
		*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(PMD_DEFAULT), "proj")) = mViewSrc;
		*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(PMD_DEFAULT), "view")) = mProjSrc;

		*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(PMD_TEXTURE), "proj")) = mViewSrc;
		*static_cast<D3DXMATRIX*>(GetShaderGlobalVariable(GetShader(PMD_TEXTURE), "view")) = mProjSrc;
	}

private:
	struct ShaderDependencyObjects
	{
		ShaderHandle _shader;
		InputLayoutHandle _inputLayout;
	};
	
	std::vector<ShaderDependencyObjects> _shaderDependencyObjects;
	
};	// end struct MyRenderer

// ----------------------------------------------------------------------------
// PMD ���f���`��p�B

namespace
{
	static const float MOTION_FPS = 1.f / 30.f;	// MMD �� 30FPS�B
}	// end namespace

class DrawablePMD
{
	struct BoneInfo;
	struct BoneKeyFrame;
public:
	//! �������B
	bool Create(Device& device, const TCHAR* pmdPath, const TCHAR* vmdPath)
	{
		// ���f���f�[�^�̓ǂݍ��݁B
		MMD::PMD pmd;

		if (failed(LoadPMD(pmd, pmdPath)))
			return false;

		static TCHAR dirPath[256];
		GetDirectoryPath(pmdPath, dirPath, _countof(dirPath));
		
		// ���b�V���̒��_���̃R�s�[�B
		std::vector<MyRenderer::ShaderInput> meshVertices;
		meshVertices.resize(pmd.vert_count);
		for (unsigned int i = 0; i < pmd.vert_count; ++i)
		{
			meshVertices[i].position = ToD3DXVECTOR3(pmd.vertex[i].pos);
			meshVertices[i].uv = ToD3DXVECTOR2(pmd.vertex[i].uv);
		}
		
		// �C���f�b�N�X�o�b�t�@���쐬����B
		_indexBuffer = device.CreateIndexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(unsigned short), pmd.face_vert_count, &pmd.face_vert_index[0]);
		if (_indexBuffer == IndexBufferHandle::INVALID)
			return false;
		_numIndices = pmd.face_vert_count;
		
		// �}�e���A�����̉�́B
		_drawPlans.push_back(DrawPlan());
		DrawPlan* pPlan = &_drawPlans.back();
		pPlan->_shader = -1;
		pPlan->_startIndex = 0;
		pPlan->_texture = -1;
		unsigned int total = 0;
		for (unsigned int i = 0, k = 0; i < pmd.material_count; ++i)
		{
			MMD::PMD::Material& material = pmd.material[i];

			// �}�e���A���̑Ή����钸�_�ɃJ���[��ݒ肷��B
			total += material.face_vert_count;
			for (; k < total; ++k)
			{
				MyRenderer::ShaderInput& vertex = meshVertices[pmd.face_vert_index[k]];
				vertex.color[0] = material.diffuse_color[0];
				vertex.color[1] = material.diffuse_color[1];
				vertex.color[2] = material.diffuse_color[2];
			}
			
			// �e�N�X�`������Ȃ��ŕ`��v�����𕪂���B
			int shader = MyRenderer::PMD_DEFAULT;
			int textureIndex = -1;
			if (strlen(material.texture_file_name) > 0)
			{
				shader = MyRenderer::PMD_TEXTURE;
				
				// �e�N�X�`����ǂݍ��ށB
				TString fileName;
				StringToTString(material.texture_file_name, fileName);
				TString texPath = dirPath;
				texPath += fileName;
				DEBUG_TRACE(texPath.c_str());
				TextureHandle tex = device.CreateTextureFromFile(texPath.c_str());
				if (tex == TextureHandle::INVALID)
					return false;
				_textures.push_back(tex);

				textureIndex = static_cast<int>(_textures.size() - 1);
			}
			
			if (pPlan->_shader < 0)
			{
				pPlan->_shader = shader;
			}
			else
			if (pPlan->_shader != shader || pPlan->_texture != textureIndex)
			{
				_drawPlans.push_back(*pPlan);
				pPlan = &_drawPlans.back();
				pPlan->_shader = shader;
				pPlan->_startIndex = pPlan->_startIndex + pPlan->_numIndices;
				pPlan->_numIndices = 0;
			}
			pPlan->_numIndices += material.face_vert_count;
			pPlan->_texture = textureIndex;
		}
		
		// ���b�V���̒��_�o�b�t�@�쐬�B
		_meshVertexBuffer = device.CreateVertexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(MyRenderer::ShaderInput), meshVertices.size(), &meshVertices[0]);
		if (_meshVertexBuffer == VertexBufferHandle::INVALID)
			return false;
		
		// �{�[���̉�́B
		_drawBoneVertices.resize(pmd.bone_count);

		std::vector<unsigned short> drawBoneIndices;
		drawBoneIndices.reserve(pmd.bone_count*2);

		unsigned short k = 0;
		for (unsigned int i = 0; i < pmd.bone_count; ++i)
		{
			MMD::PMD::Bone& boneHead = pmd.bone[i];

			// �{�[�����̓o�^�B
			BoneInfo boneInfo;
			boneInfo._name = boneHead.bone_name;
			
			Vector3 headPosParentSpace;
			
			if (boneHead.parent_bone_index != MMD::PMD::Bone::INVALID_PARENT)
			{
				// �e�{�[���Ɏq�{�[����o�^�B
				MMD::PMD::Bone& boneParent = pmd.bone[boneHead.parent_bone_index];
				BoneNameIndexMap::iterator found = _boneNameIndexMap.find(boneParent.bone_name);
				if (found == _boneNameIndexMap.end())
					continue;	// ����Ƃ߂�ǂ������c�B
				_bones[found->second]._childs.push_back(i);
				
				headPosParentSpace = boneHead.bone_head_pos - boneParent.bone_head_pos;
			}
			else
			{
				headPosParentSpace = boneHead.bone_head_pos;
				_rootBoneIndices.push_back(i);
			}

			D3DXMatrixTranslation(&boneInfo._transformToParentSpace, headPosParentSpace.x, headPosParentSpace.y, headPosParentSpace.z);
			D3DXMatrixIdentity(&boneInfo._transformLocalSpace);
			boneInfo._transformFixModelSpace = boneInfo._transformLocalSpace;

			_bones.push_back(boneInfo);
			_boneNameIndexMap.insert(std::make_pair(boneHead.bone_name, i));			

			// �`��p���_�̃Z�b�g�A�b�v�B
			MyRenderer::ShaderInput vertexHead;
			vertexHead.position = ToD3DXVECTOR3(boneHead.bone_head_pos);
			Color::WHITE.ToFloat3(vertexHead.color);
			vertexHead.uv = D3DXVECTOR2(0.f, 0.f);
			_drawBoneVertices[i] = vertexHead;

			if (boneHead.tail_pos_bone_index != MMD::PMD::Bone::INVALID_TAIL)
			{
				drawBoneIndices.push_back(i);
				drawBoneIndices.push_back(boneHead.tail_pos_bone_index);
			}
			
		}	// end for (unsigned int i = 0; ...)
		
		_drawBoneVertexBuffer = device.CreateVertexBuffer(D3D10_USAGE_DEFAULT, 0, sizeof(MyRenderer::ShaderInput), _drawBoneVertices.size(), &_drawBoneVertices[0]);
		if (_drawBoneVertexBuffer == VertexBufferHandle::INVALID)
			return false;

		_numDrawBoneVertices = static_cast<unsigned int>(drawBoneIndices.size());
		_drawBoneIndexBuffer = device.CreateIndexBuffer(D3D10_USAGE_IMMUTABLE, 0, sizeof(unsigned short), _numDrawBoneVertices, &drawBoneIndices[0]);
		if (_drawBoneIndexBuffer == IndexBufferHandle::INVALID)
			return false;

		// ���[�V�����f�[�^�̓ǂݍ��݁B�Ή�����{�[�����Ƃɐ������Ă����B
		MMD::VMD vmd;
		if (failed(LoadVMD(vmd, vmdPath)))
			return false;
		_allBoneKeyFrames.resize(pmd.bone_count);
		_keyFrameMax = 0;
		for (unsigned int i = 0; i < vmd.motion_count; ++i)
		{
			MMD::VMD::Motion& motion = vmd.motion[i];
			BoneNameIndexMap::iterator found = _boneNameIndexMap.find(motion.BoneName);
			if (found != _boneNameIndexMap.end())
			{
				BoneKeyFrames& keyFrames = _allBoneKeyFrames[found->second];

				BoneKeyFrame keyFrame;
				keyFrame._frame = motion.Frame;
				keyFrame._time = motion.Frame * MOTION_FPS;
				keyFrame._translation = ToD3DXVECTOR3(motion.Location);
				keyFrame._rotation = *reinterpret_cast<D3DXQUATERNION*>(motion.Rotation);

				keyFrame._interpolation_x.p1 = D3DXVECTOR2(motion.Interpolation[0],	 motion.Interpolation[4]);
				keyFrame._interpolation_x.p2 = D3DXVECTOR2(motion.Interpolation[8],	 motion.Interpolation[12]);
				keyFrame._interpolation_y.p1 = D3DXVECTOR2(motion.Interpolation[1],	 motion.Interpolation[5]);
				keyFrame._interpolation_y.p2 = D3DXVECTOR2(motion.Interpolation[9],	 motion.Interpolation[13]);
				keyFrame._interpolation_z.p1 = D3DXVECTOR2(motion.Interpolation[2],	 motion.Interpolation[6]);
				keyFrame._interpolation_z.p2 = D3DXVECTOR2(motion.Interpolation[10], motion.Interpolation[14]);
				keyFrame._interpolation_r.p1 = D3DXVECTOR2(motion.Interpolation[3],	 motion.Interpolation[7]);
				keyFrame._interpolation_r.p2 = D3DXVECTOR2(motion.Interpolation[11], motion.Interpolation[15]);

				keyFrames.insert(std::make_pair(motion.Frame, keyFrame));

				if (_keyFrameMax < motion.Frame)
					_keyFrameMax = motion.Frame;
			}
			
			#if defined(_DEBUG)
			TString bone_name;
			StringToTString(motion.BoneName, bone_name);
			DEBUG_TRACE(_T("Bone[%u] : name=%s, frame=%u, trans=(%.2f, %.2f, %.2f), rot=(%.2f, %.2f, %.2f, %.2f)"),
				i, bone_name.c_str(), motion.Frame, motion.Location.x, motion.Location.y, motion.Location.z,
				motion.Rotation[0], motion.Rotation[1], motion.Rotation[2], motion.Rotation[3]);
			#endif
		}

		SwitchToLinearInterpolation();
		
		// �T���v���B
		D3D10_SAMPLER_DESC sampDesc;
		sampDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
		sampDesc.MipLODBias = 0.0f;
		sampDesc.MaxAnisotropy = 16;
		sampDesc.ComparisonFunc = D3D10_COMPARISON_NEVER;
		sampDesc.BorderColor[0] = 0.f;
		sampDesc.BorderColor[1] = 0.f;
		sampDesc.BorderColor[2] = 0.f;
		sampDesc.BorderColor[3] = 0.f;
		sampDesc.MinLOD = 0.f;
		sampDesc.MaxLOD = FLT_MAX;
		_sampler = device.CreateSamplerState(sampDesc);
		if (_sampler == SamplerStateHandle::INVALID)
			return false;
		
		return true;
	}
	
	void SetMotion(float time)
	{
		D3DXMATRIX m;

		_motionTime = time;

		unsigned int keyFrame = GetMotionKeyFrame();
		float validMotionTime = GetMotionTime();
		
		for (size_t i = 0, sz = _allBoneKeyFrames.size(); i < sz; ++i)
		{
			GetMotionMatrix(keyFrame, validMotionTime, i, m);
			BoneInfo& bone = _bones[i];
			bone._transformLocalSpace = m;
		}
		
		D3DXMATRIX mIdentity;
		D3DXMatrixIdentity(&mIdentity);
		for (size_t i = 0, sz = _rootBoneIndices.size(); i < sz; ++i)
		{
			unsigned int boneIndex = _rootBoneIndices[i];
			BoneInfo& rootBone = _bones[boneIndex];
			Transform(rootBone, boneIndex, mIdentity);
		}
	}

	void GetMotionMatrix(unsigned int keyFrame, float motionTime, size_t boneIndex, D3DXMATRIX& m)
	{
		BoneKeyFrames& keyFrames = _allBoneKeyFrames[boneIndex];
		if (keyFrames.empty())
		{
			// �L�[�t���[�����X�g�����ۂɁA�K��1�v�f�ȏ�����悤�ɂ��Ă����Ε���s�v�B
			D3DXMatrixIdentity(&m);
			return;
		}
		
		BoneKeyFrames::const_iterator found = keyFrames.lower_bound(keyFrame);
		if (found == keyFrames.end() || found->first > keyFrame)
		{
			--found;
		}
		BoneKeyFrames::const_iterator next = found;
		++next;
		if (next == keyFrames.end())
		{
			const BoneKeyFrame& boneKeyFrame = found->second;
			D3DXMatrixTranslation(&m, boneKeyFrame._translation.x, boneKeyFrame._translation.y, boneKeyFrame._translation.z);
			D3DXMATRIX mRot;
			D3DXMatrixRotationQuaternion(&mRot, &boneKeyFrame._rotation);
			m = mRot * m;
			return;
		}

		const BoneKeyFrame& boneKeyFrame1 = found->second;
		const BoneKeyFrame& boneKeyFrame2 = next->second;

		(this->*_pfnInterpolateBoneKeyFrame)(boneKeyFrame1, boneKeyFrame2, motionTime, m);

		return;
	}

	// ��Ԗ����B
	void BoneKeyFrameNoInterpolation(const BoneKeyFrame& boneKeyFrame1, const BoneKeyFrame& /*boneKeyFrame2*/, float /*motionTime*/, D3DXMATRIX& m)
	{
		D3DXMATRIX mRot;
		D3DXMatrixTranslation(&m, boneKeyFrame1._translation.x, boneKeyFrame1._translation.y, boneKeyFrame1._translation.z);
		D3DXMatrixRotationQuaternion(&mRot, &boneKeyFrame1._rotation);
		m = mRot * m;
	}

	// ���`��ԁ����ʐ��`��ԁB
	void BoneKeyFrameLinearInterpolation(const BoneKeyFrame& boneKeyFrame1, const BoneKeyFrame& boneKeyFrame2, float motionTime, D3DXMATRIX& m)
	{
		D3DXMATRIX mRot;

		float t = (motionTime - boneKeyFrame1._time) / (boneKeyFrame2._time - boneKeyFrame1._time);
		
		D3DXVECTOR3 translation;
		D3DXVec3Lerp(&translation, &boneKeyFrame1._translation, &boneKeyFrame2._translation, t);
		D3DXMatrixTranslation(&m, translation.x, translation.y, translation.z);
		
		D3DXQUATERNION rotation;
		D3DXQuaternionSlerp(&rotation, &boneKeyFrame1._rotation, &boneKeyFrame2._rotation, t);
		D3DXMatrixRotationQuaternion(&mRot, &rotation);

		m = mRot * m;
	}
	
	// ���`��ԃp�����[�^t�̎Z�o�ɁA��ԃp�����[�^���g�p�����x�W�G�Ȑ����g������ԏ����B
	// ���ꂪ�{�ƂƓ��������Ȃ̂��͕s���Amiku_test4�Ŕ�ׂĂ݂�ƂȂ񂩖{�Ƃ̂ق��������ƃL��������B
	void BoneKeyFrameUseParameterInterpolation(const BoneKeyFrame& boneKeyFrame1, const BoneKeyFrame& boneKeyFrame2, float motionTime, D3DXMATRIX& m)
	{
		D3DXMATRIX mRot;

		float bt = (motionTime - boneKeyFrame1._time) / (boneKeyFrame2._time - boneKeyFrame1._time);
		float ltx = boneKeyFrame2._interpolation_x.Bezier(bt).y;	// boneKeyFrame2 �̃p�����[�^���g���B���`��Ԃ� t �ɂ� y ���g���B
		float lty = boneKeyFrame2._interpolation_y.Bezier(bt).y;	
		float ltz = boneKeyFrame2._interpolation_z.Bezier(bt).y;	
		float ltr = boneKeyFrame2._interpolation_r.Bezier(bt).y;
		float ltw = boneKeyFrame2._interpolation_x.Bezier(0.5).y;
		DEBUG_TRACE(_T("bt=%.2f, ltw=%.2f"), bt, ltw);
		
		D3DXVECTOR3 translation;
		#define LERP(x0, x1, t) ((x1 - x0) * t + x0) 
		translation.x = LERP(boneKeyFrame1._translation.x, boneKeyFrame2._translation.x, ltx);
		translation.y = LERP(boneKeyFrame1._translation.y, boneKeyFrame2._translation.y, lty);
		translation.z = LERP(boneKeyFrame1._translation.z, boneKeyFrame2._translation.z, ltz);
		D3DXMatrixTranslation(&m, translation.x, translation.y, translation.z);
		
		D3DXQUATERNION rotation;
		D3DXQuaternionSlerp(&rotation, &boneKeyFrame1._rotation, &boneKeyFrame2._rotation, ltr);
		D3DXMatrixRotationQuaternion(&mRot, &rotation);

		m = mRot * m;
	}
	
	// �{�[�������f����ԂɃg�����X�t�H�[������B
	void Transform(BoneInfo& bone, unsigned int boneIndex, const D3DXMATRIX& transformToParentModelSpace)
	{
		bone._transformFixModelSpace = bone._transformLocalSpace * bone._transformToParentSpace * transformToParentModelSpace;
		MyRenderer::ShaderInput& drawBoneVertex = _drawBoneVertices[boneIndex];
		drawBoneVertex.position.x = bone._transformFixModelSpace._41;
		drawBoneVertex.position.y = bone._transformFixModelSpace._42;
		drawBoneVertex.position.z = bone._transformFixModelSpace._43;
		
		for (size_t i = 0, sz = bone._childs.size(); i < sz; ++i)
		{
			unsigned int boneIndex = bone._childs[i];
			BoneInfo& childBone = _bones[boneIndex];
			Transform(childBone, boneIndex, bone._transformFixModelSpace);
		}
	}
	
	void SetMotionLoop(bool loop)
	{
		_flags.set(MOTION_LOOP, loop);
	}

	float GetMotionTime() const
	{
		if (GetMotionLoop())
			return fmod(_motionTime, _keyFrameMax * MOTION_FPS);
		return _motionTime;
	}
	
	unsigned int GetMotionKeyFrame() const
	{
		unsigned int motionFrame = static_cast<unsigned int>(_motionTime * (1.f/MOTION_FPS));
		if (GetMotionLoop())
			motionFrame %= _keyFrameMax;
		return motionFrame;
	}
	
	bool GetMotionLoop() const
	{
		return _flags.is(MOTION_LOOP);
	}
	void SwitchToNoInterpolation()
	{
		_pfnInterpolateBoneKeyFrame = &DrawablePMD::BoneKeyFrameNoInterpolation;
	}

	bool IsNoInterpolation() const
	{
		return _pfnInterpolateBoneKeyFrame == &DrawablePMD::BoneKeyFrameNoInterpolation;
	}

	void SwitchToLinearInterpolation()
	{
		_pfnInterpolateBoneKeyFrame = &DrawablePMD::BoneKeyFrameLinearInterpolation;
	}

	bool IsLinearInterpolation() const
	{
		return _pfnInterpolateBoneKeyFrame == &DrawablePMD::BoneKeyFrameLinearInterpolation;
	}

	void SwitchToUseParameterInterpolation()
	{
		_pfnInterpolateBoneKeyFrame = &DrawablePMD::BoneKeyFrameUseParameterInterpolation;
	}

	bool IsUseParameterInterpolation() const
	{
		return _pfnInterpolateBoneKeyFrame == &DrawablePMD::BoneKeyFrameUseParameterInterpolation;
	}
		
	void SetWorldMatrix(const D3DXMATRIX& mWorld)
	{
		_transformToWorld = mWorld;
	}

	void Draw(MyRenderer& renderer)
	{
		for (size_t i = 0; i < _drawPlans.size(); ++i)
		{
			DrawPlan& plan = _drawPlans[i];
			
			ShaderHandle shader = renderer.GetShader(plan._shader);
			renderer.SetShader(shader);
			renderer.SetInputLayout(renderer.GetInputLayout(plan._shader));

			D3DXMATRIX& mWorld = *static_cast<D3DXMATRIX*>(renderer.GetShaderGlobalVariable(shader, "world"));
			D3DXMatrixTranspose(&mWorld, &_transformToWorld);

			if (plan._texture >= 0)
			{
				// TODO: �o����΂�������ĕ��������͖������c�BNullHandle ����낤���ȁH�H
				renderer.SetShaderTexture(shader, "tex", _textures[plan._texture]);
				renderer.SetShaderSamplerState(shader, "samp", _sampler);
			}

			renderer.ApplyShader();

			auto_interface<Device::NativeDevice> pNativeDevice(renderer.GetNativeDevice());
			
			renderer.SetVertexBuffer(_meshVertexBuffer);
			renderer.SetIndexBuffer(_indexBuffer);
			pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			renderer.SetDefaultRenderTargets();

	//		renderer.SetRasterizerState(_rasterizerState);
			renderer.SetDefaultRasterizerState();
			renderer.SetDefaultBlendState();
			renderer.SetDefaultDepthStencilState();
			
			pNativeDevice->DrawIndexed(plan._numIndices, plan._startIndex, 0);
		}
	}
	
	// �{�[���`��B
	void DrawBone(MyRenderer& renderer)
	{
		ShaderHandle shader = renderer.GetShader(MyRenderer::PMD_DEFAULT);
		renderer.SetShader(shader);
		renderer.SetInputLayout(renderer.GetInputLayout(MyRenderer::PMD_DEFAULT));

		D3DXMATRIX& mWorld = *static_cast<D3DXMATRIX*>(renderer.GetShaderGlobalVariable(shader, "world"));
		D3DXMatrixTranspose(&mWorld, &_transformToWorld);

		renderer.ApplyShader();

		renderer.UpdateVertexBuffer(_drawBoneVertexBuffer, &_drawBoneVertices[0]);

		auto_interface<Device::NativeDevice> pNativeDevice(renderer.GetNativeDevice());
		
		renderer.SetVertexBuffer(_drawBoneVertexBuffer);
		renderer.SetIndexBuffer(_drawBoneIndexBuffer);
		pNativeDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		renderer.SetDefaultRenderTargets();

		renderer.SetDefaultRasterizerState();
		renderer.SetDefaultBlendState();
		renderer.SetDefaultDepthStencilState();
		
		pNativeDevice->DrawIndexed(_numDrawBoneVertices, 0, 0);
	}
	
private:
	// ���b�V�����e�N�X�`���`��p�B
	VertexBufferHandle _meshVertexBuffer;
	IndexBufferHandle _indexBuffer;
	unsigned int _numIndices;
	std::vector<TextureHandle> _textures;
	
	SamplerStateHandle _sampler;

	// �{�[���`��p�B
	VertexBufferHandle _drawBoneVertexBuffer;
	IndexBufferHandle _drawBoneIndexBuffer;
	std::vector<MyRenderer::ShaderInput> _drawBoneVertices;
	unsigned int _numDrawBoneVertices;
	
	//! �{�[�����B
	struct BoneInfo
	{
		std::string _name;

		D3DXMATRIX _transformToParentSpace;
		D3DXMATRIX _transformLocalSpace;
		D3DXMATRIX _transformFixModelSpace;
		
		std::vector<unsigned int> _childs;	//!< �q�{�[���̃C���f�b�N�X�B
	};
	std::vector<BoneInfo> _bones;
	std::vector<unsigned int> _rootBoneIndices;
	
	typedef std::map<std::string, unsigned int> BoneNameIndexMap;
	BoneNameIndexMap _boneNameIndexMap;
	
	// ���[�V�������B
	struct BoneKeyFrame
	{
		unsigned int _frame;
		float _time;
		D3DXVECTOR3 _translation;
		D3DXQUATERNION _rotation;

		struct Interpolation
		{
			D3DXVECTOR2 p1;	//!< �R���x�W�F�Ȑ��̐���_ p1�B
			D3DXVECTOR2 p2;	//!< �R���x�W�F�Ȑ��̐���_ p2�B
			
			D3DXVECTOR2 Bezier(float t) const
			{
				#define SQ(x) ((x)*(x))
				#define CB(x) ((x)*(x)*(x))
				#define CALC(p0, p1, p2, p3, t)	(CB(1.f-t)*p0 + 3.f*SQ(1.f-t)*t*p1 + 3.f*(1.f-t)*SQ(t)*p2 + CB(t)*p3)
				D3DXVECTOR2 p0(0.f, 0.f);
				D3DXVECTOR2 p3(127.f, 127.f);
				return 
					D3DXVECTOR2(
						CALC(p0.x, p1.x, p2.x, p3.x, t) / 127.f,
						CALC(p0.y, p1.y, p2.y, p3.y, t) / 127.f
						);
			}
		};

		Interpolation _interpolation_x;
		Interpolation _interpolation_y;
		Interpolation _interpolation_z;
		Interpolation _interpolation_r;
	};
	
	typedef std::map<unsigned int, BoneKeyFrame> BoneKeyFrames;
	std::vector<BoneKeyFrames> _allBoneKeyFrames;
	unsigned int _keyFrameMax;
	
	float _motionTime;	//!< ���[�V�����̌��ݎ��ԁB

	void (DrawablePMD::*_pfnInterpolateBoneKeyFrame)(const BoneKeyFrame&, const BoneKeyFrame&, float, D3DXMATRIX&);

	//! ���f���̃��[���h�B
	D3DXMATRIX _transformToWorld;
	
	//! �`��v��B����̃V�F�[�_�ƃe�N�X�`���ŕ`��o���Ă��܂��C���f�b�N�X��S�Ă܂Ƃ߂Ă����A�o�������ŏ��̉񐔂ŕ`��o����l�ɂ���B
	struct DrawPlan
	{
		int _shader;
		unsigned int _startIndex;
		unsigned int _numIndices;
		int _texture;
	};
	std::vector<DrawPlan> _drawPlans;
	
	//! �t���O�B
	enum FLAGS
	{
		MOTION_LOOP	= 0x0001,
	};
	Flags _flags;


};	// end class PMDDrawer

// ----------------------------------------------------------------------------
// �A�v���P�[�V�����t���[���N���X�̎������B
struct ApplicationFrame::Impl
{
	Window _window;
	Device _device;
	MyRenderer _renderer;
	
	FrameRateController _frameRateController;

	enum FLAGS
	{
		SCREEN_OCCLUDED			= 0x01,
		FRAME_SKIP				= 0x02,
		INVALID_MESH_DRAW		= 0x04,
		INVALID_BONE_DRAW		= 0x08,
	};
	Flags _flags;
	
	RasterizerStateHandle _rasterizerState;

	DrawablePMD _pmd;
	Timer _motionTimer;

	Impl()
	{
	}
	
	~Impl()
	{
	}

	void Exit(int exitCode)
	{
		PostQuitMessage(exitCode);
	}

	// �I�������B
	void Release()
	{
		_renderer.Release();
		_device.Release();
	}

	// �������B
	bool Create(HWND hWnd)
	{
		if (failed(_window.Create(hWnd)))
		{
			return false;
		}
		SIZE sz;
		_window.GetClientSize(sz);

		{
			using namespace std::placeholders;
			_window.AddMessageHandler(WM_SIZE, std::bind(&Impl::OnWindowSizeMessage, this, _1, _2, _3, _4));
			_window.AddMessageHandler(WM_KEYUP, std::bind(&Impl::OnWindowKeyUpMessage, this, _1, _2, _3, _4));
		}
		
		if (failed(_device.Create(hWnd)))
		{
			return false;
		}
		
		if (failed(_renderer.Create(_device)))
		{
			return false;
		}
			
		if (failed(CreateModel()))
		{
			return false;
		}

		ResetProjectionTransform(sz);

		return true;
	}
	
	void ResetProjectionTransform(SIZE sz)
	{
		D3DXMATRIX mProj;
		D3DXMATRIX mView;

		D3DXMatrixPerspectiveFovLH(&mProj,
				(float)D3DXToRadian(60.0f),		// ����p60��
			static_cast<float>(sz.cx) / static_cast<float>(sz.cy), // �A�X�y�N�g��
				1.0f,							// �O�����e�ʂ܂ł̋���
				1000.0f);							// ������e�ʂ܂ł̋���

		D3DXVECTOR3 vEye(0.0f, 12.0f, -25.0f);
		D3DXVECTOR3 vAt = vEye + D3DXVECTOR3(0.f, -0.5f, 5.f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.f);
		D3DXMatrixLookAtLH(&mView, &vEye, &vAt, &vUp);

		_renderer.SetShaderViewProjection(mProj, mView);
	}
	
	bool CreateModel()
	{
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\cube.pmd"))))
		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\�����~�N.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\�����߂�.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\�����~�N.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test5.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\�����~�N.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test4.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\�����~�N.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test3.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\�����~�N.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test2.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\�����~�N.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test.vmd"))))
//		if (failed(_pmd.Create(_device, _T("..\\Rakugaki_MikuMikuDance_dat\\�s�N�s�N\\�s�N�s�N.pmd"), _T("..\\Rakugaki_MikuMikuDance_dat\\miku_test2.vmd"))))
			return false;
		_motionTimer.Start();
		return true;
	}
	
	
	// �t���[���X�V�B
	void UpdateFrame()
	{
		_flags.reset(FRAME_SKIP);
//		if (_frameRateController.GetElapsedTime() > _frameRateController.GetFrameTime() * 2)
//		{
//			// TODO: ���܂�ɂ����e���ȏ����c�����ƍl����B
//			_flags.set(FRAME_SKIP);
//		}

		Draw();
		
		_frameRateController.ChangeFrame();
	}
	
	// �`��B
	void Draw()
	{
		if (_flags.is(FRAME_SKIP) || _device.IsScreenOccluded())
		{
			// noop
		}
		else
		{
			_renderer.Clear(true, true, false, Color::BLUE);

			D3DXMATRIX mWorld;
			{
				_pmd.SetMotion(_motionTimer.GetTime() * 1.0f);
				
				//D3DXMatrixIdentity(&mWorld);
				static float angle = 0.f;
//				angle += _frameRateController.GetElapsedTime() * 0.5f;
				D3DXMatrixRotationY(&mWorld, angle);
				_pmd.SetWorldMatrix(mWorld);
				
				if (!_flags.is(INVALID_MESH_DRAW))
					_pmd.Draw(_renderer);
				if (!_flags.is(INVALID_BONE_DRAW))
					_pmd.DrawBone(_renderer);
			}
			
			int y = 0, h = _device.GetFontHeight(_renderer.GetDefaultFont());

			_renderer.DrawText(0, y, Color::GREEN, _T("FPS=%.2f (%.2fms)"), 
				_frameRateController.GetCurrentFPS(),
				_frameRateController.GetElapsedTime() * 1000.f
				);
			y += h;

			_renderer.DrawText(0, y, Color::WHITE, _T("Frame=%u / Time=%.2f"), _pmd.GetMotionKeyFrame(), _pmd.GetMotionTime());
			y += h;
			_renderer.DrawText(0, y, Color::WHITE, _T("[F2] ���b�V���`��=") TFMT_BOOL, format_bool(!_flags.is(INVALID_MESH_DRAW)));
			y += h;
			_renderer.DrawText(0, y, Color::WHITE, _T("[F3] �{�[���`��=") TFMT_BOOL, format_bool(!_flags.is(INVALID_BONE_DRAW)));
			y += h;
			_renderer.DrawText(0, y, Color::WHITE, _T("[F4] ���[�V�������X�^�[�g"));
			y += h;
			_renderer.DrawText(0, y, Color::WHITE, _T("[F5] ���[�V�������[�v=") TFMT_BOOL, format_bool(_pmd.GetMotionLoop()));
			y += h;

			const TCHAR* pInterpolationMethod = _T("����");
			if (_pmd.IsLinearInterpolation()) pInterpolationMethod = _T("���`���");
			if (_pmd.IsUseParameterInterpolation()) pInterpolationMethod = _T("�p�����[�^�g�p�i�x�W�F�Ȑ��{���`��ԁj");
			_renderer.DrawText(0, y, Color::WHITE, _T("[F6] ���[�V�������=%s"), pInterpolationMethod);
			y += h;
		}

		_renderer.Present();
	}
	
	// �E�B���h�E���T�C�Y�B
	LRESULT OnWindowSizeMessage(HWND /*hWnd*/, UINT /*msg*/, WPARAM wParam, LPARAM lParam)
	{
		WindowSizeMessageParameters params(wParam, lParam);
		SIZE sz;
		sz.cx = params.GetClientWidth();
		sz.cy = params.GetClientHeight();

		if (params.GetType() == SIZE_MINIMIZED)
		{
			return 0;
		}
		
		// �Ȃ񂩁��̏������Ȃ��Ă���肭�ł��Ă�悤�Ɍ������ł����ǁc�B
		if (failed(_device.ResizeBackBuffer(sz)))
		{
			Exit(0);
			return 0;
		}
		
		// �ˉe�g�����X�t�H�[�������Z�b�g�B
		ResetProjectionTransform(sz);
		return 0;
	}
	
	// �L�[��������ė����ꂽ�B
	LRESULT OnWindowKeyUpMessage(HWND /*hWnd*/, UINT /*msg*/, WPARAM wParam, LPARAM lParam)
	{
		WindowKeyMessageParameters params(wParam, lParam);
		switch (params.GetVirtKey())
		{
		case VK_F2:
			_flags = _flags.not(INVALID_MESH_DRAW);
			break;
		case VK_F3:
			_flags = _flags.not(INVALID_BONE_DRAW);
			break;
		case VK_F4:
			_motionTimer.Reset();
			_motionTimer.Start();
			break;
		case VK_F5:
			_pmd.SetMotionLoop(!_pmd.GetMotionLoop());
			break;
		case VK_F6:
			if (_pmd.IsNoInterpolation())
			{
				_pmd.SwitchToLinearInterpolation();
			}
			else if (_pmd.IsLinearInterpolation())
			{
				_pmd.SwitchToUseParameterInterpolation();
			}
			else if (_pmd.IsUseParameterInterpolation())
			{
				_pmd.SwitchToNoInterpolation();
			}
			break;
		}
		return 0;
	}

};	// end struct ApplicationFrame::Impl

// �A�v���P�[�V�����t���[���N���X�̓]�����B
ApplicationFrame::ApplicationFrame()
	: _pimpl(new Impl())
{
}

ApplicationFrame::~ApplicationFrame()
{
}

bool
ApplicationFrame::Create(HWND hWnd)
{
	return _pimpl->Create(hWnd);
}

void
ApplicationFrame::Release()
{
	_pimpl->Release();
}

void
ApplicationFrame::UpdateFrame()
{
	_pimpl->UpdateFrame();
}

