/*!
	@file	Graphics/Direct3D10/Misc.h
	
	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	
	#define DEPTH_MAX	1.f
	
	namespace Direct3D10
	{
		//! ハンドルが無効だった事を示す例外。
		struct InvalidHandleException : std::exception
		{
		};
		
		//! デバイス依存オブジェクトのハンドル。
		#define DECLARE_HANDLE_CLASS(name)		\
			class name 							\
			{									\
			public:								\
				static const int INVALID = -1;	\
				name () : _val(INVALID) {}					\
				name (const name& src) : _val(src._val) {}	\
				name (int val) : _val(val) {}	\
				bool operator == (const name& rhs)			\
				{ return _val == rhs._val; }				\
				bool operator != (const name& rhs)			\
				{ return _val != rhs._val; }				\
				int Value() const				\
				{								\
					return _val;				\
				}								\
			private:							\
				int _val;						\
			};
			
		DECLARE_HANDLE_CLASS(TextureHandle)
		DECLARE_HANDLE_CLASS(RenderTargetHandle)
		DECLARE_HANDLE_CLASS(DepthStencilBufferHandle)
		DECLARE_HANDLE_CLASS(InputLayoutHandle)
		DECLARE_HANDLE_CLASS(BlendStateHandle)
		DECLARE_HANDLE_CLASS(RasterizerStateHandle)
		DECLARE_HANDLE_CLASS(DepthStencilStateHandle)
		DECLARE_HANDLE_CLASS(VertexBufferHandle)
		DECLARE_HANDLE_CLASS(IndexBufferHandle)
		DECLARE_HANDLE_CLASS(ShaderHandle)
		DECLARE_HANDLE_CLASS(SamplerStateHandle)
		DECLARE_HANDLE_CLASS(FontHandle)
		
		#undef DECLARE_HANDLE

		//! バッファへのポインタを表す型。
		class BufferPtr : private Uncopyable
		{
		public:
			BufferPtr() : _ptr(NULL), _pInterface(NULL) {}
			~BufferPtr()
			{
				if (is_not_null(_pInterface))
					_pInterface->Unmap();
			}
				
			void* Get() 
			{ 
				return _ptr; 
			}
				
		private:
			friend class Core;
			void* _ptr;
			ID3D10Buffer* _pInterface;
		};	// end class BufferPtr

		//! フォント詳細。
		struct FontDesc
		{
			INT Height;
			UINT Width;
			UINT Weight;
			UINT MipLevels;
			BOOL Italic;
			UINT CharSet;
			UINT OutputPrecision;
			UINT Quality;
			UINT PitchAndFamily;
			LPCTSTR pFaceName;
		};	// end struct FontDesc
		
	}	// end namespace Direct3D
}	// end namespace Marda
