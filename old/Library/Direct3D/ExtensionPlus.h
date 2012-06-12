/*!
 	@file Direct3D/ExtensionPlus.h
 
 	D3DX�ɖ����Ă�����Ɨ~�����Ȃ�悤�Ȋ֐��B
 
 	@author Dila
 */


#pragma once

#include "Direct3D/Device.h"	// TODO:�ʂ̃w�b�_�̂��������ǂ����v����

namespace D3DXPlus
{
	//! @addtogroup Direct3D Direct3D
	//! @{

	// ���b�V��	------------------------------------------------------------ //

	/*!
 		�|�����b�V���쐬�֐�
 		@param	pDevice	Direct3D�f�o�C�X
 		@param	width	�|���̉���(x�������̒���)
 		@param	height	�|���̏c��(z�������̒���)
 		@param[out]	ppMesh	�쐬���ꂽ���b�V��
	*/
	HRESULT CreateBoard(IDirect3DDevice* pDevice, float width, float height, LPD3DXMESH* ppMesh);


	// �`��	---------------------------------------------------------------- //

	/*!
 		������`���֐�
 		@param	pDevice	Direct3D�f�o�C�X�C���^�t�F�[�X
		@param	v1		�����̎n�_���W
		@param	v2		�����̏I�_���W
		@param	color1	�n�_�̒��_�J���[
		@param	color2	�I�_�̒��_�J���[
	*/
	HRESULT DrawLine(IDirect3DDevice* pDevice, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, DWORD color1, DWORD color2);


	// TODO: �T�N�b�ƃe�L�X�g�������_�����O����֐�


	// �Z�p	---------------------------------------------------------------- //

	/*!
 		�x�N�g���������Ƃ�����]�s����擾����
 		@param[out]	pOut		��]�s��
 		@param		pDirection	�����Ƃ���x�N�g��
		@return		pOut�Ŏw�肵���s��̃|�C���^�B
					�߂�l�����̂܂܊֐��̈����Ɏg�p�o����B
	*/
	D3DXMATRIX* MatrixRotationVector(D3DXMATRIX* pOut, const D3DXVECTOR3* pvDirection);
	
	/*!
		�x�N�g���������Ƃ�����]�N�H�[�^�j�I�����擾����
		@param[out]	pOut		��]�N�H�[�^�j�I��
		@param		pvDirection	�����Ƃ���x�N�g��
		@return		pOut�Ŏw�肵���N�H�[�^�j�I���̃|�C���^�B
					�߂�l�����̂܂܊֐��̈����Ɏg�p�o����B
		@todo		�������Ȃ̂Ŏ�������B
	*/
	D3DXQUATERNION* QuaternionRotationVector(D3DXQUATERNION* pOut, const D3DXVECTOR3* pvDirection);

	/*!
		��]�N�H�[�^�j�I�����������\���x�N�g�����擾����
		@param[out]	pOut		������\���x�N�g��
		@param		pqRotation	��]�N�H�[�^�j�I��
		@return		pOut�Ŏw�肵���N�x�N�g���̃|�C���^�B
					�߂�l�����̂܂܊֐��̈����Ɏg�p�o����B
		@todo		�������Ȃ̂Ŏ�������B(0,0,1)�̃x�N�g������]�N�H�[�^�j�I���ŉ�]���������̂��o�͂���B
	 */
	D3DXVECTOR3* Vector3DirectionQuaternion(D3DXVECTOR3* pOut, const D3DXQUATERNION* pqRotation);

	/*!
		�x�N�g������]�N�H�[�^�j�I���ŉ�]������
		@param[out]	pOut		��]��̃x�N�g��
		@param		pqRotation	��]�N�H�[�^�j�I��
		@return		pOut�Ŏw�肵���N�x�N�g���̃|�C���^�B
					�߂�l�����̂܂܊֐��̈����Ɏg�p�o����B
		@todo		�������Ȃ̂Ŏ�������B
	 */
	D3DXVECTOR3* Vector3TransformQuaternion(D3DXVECTOR3* pOut, const D3DXQUATERNION* pqRotation);


	// �ϊ��s��	-------------------------------------------------------- //

	/*!
		�r���[�|�[�g�ϊ�����s����擾����
		(�v���W�F�N�V�����ϊ������x�N�g�����X�N���[�����W�ɕϊ�����s����擾����)
		@param[out]	pOut	�ϊ��s��
		@param		cx		�X�N���[���̉���
		@param		cy		�X�N���[���̏c��
		@return		pOut�Ŏw�肵���s��̃|�C���^�B
					�߂�l�����̂܂܊֐��̈����Ɏg�p�o����B
	*/
	D3DXMATRIX* MatrixViewport(D3DXMATRIX* pOut, float cx, float cy);


	/*!
		�r���{�[�h�����ׂ̎p���ϊ��s����擾����
		@param[out]	pOut			�ϊ��s��
		@param		pmViewMatrix	�r���[�ϊ��s��
		@return		pOut�Ŏw�肵���s��̃|�C���^�B
					�߂�l�����̂܂܊֐��̈����Ɏg�p�o����B
	 */
	D3DXMATRIX* MatrixBillboard(D3DXMATRIX* pOut, const D3DMATRIX* pmViewMatrix);

	//! @}
}
