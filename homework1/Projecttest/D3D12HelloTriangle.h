#pragma once
#include "DXSample.h"

using namespace DirectX;

//注意，ComPtr是用来管理CPU资源的生命周期的，
//但它不了解GPU资源的生命周期。 
//应用程序必须考虑GPU资源的生命周期，
//以避免销毁可能仍被GPU引用的对象。  
using Microsoft::WRL::ComPtr;

class D3D12HelloTriangle :public DXSample
{
public:D3D12HelloTriangle(UINT width, UINT heigh, std::wstring name);


	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();

private:
	static const UINT FrameCount = 2;

	struct Vertex //顶点结构体：包含顶点的位置和颜色
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	//管线对象
	//CD3DX12是CD3D12的变体，龙书有说到
	CD3DX12_VIEWPORT m_viewport;//视口对象
	CD3DX12_RECT m_scissorRect; //初始化D3D12 _ RECT结构(裁剪矩形)

	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;//命令分配器
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;//根签名(函数签名?)
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;//描述符堆
	ComPtr<ID3D12PipelineState> m_pipelineState;
	UINT m_rtvDescriptorSize;

	// 应用程序资源
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	//同步对象
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;


	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();//填充命令列表
	void WaitForPreviousFrame();//等待上一帧
};
