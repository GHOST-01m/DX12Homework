#pragma once
#include "DXSample.h"

using namespace DirectX;

//ע�⣬ComPtr����������CPU��Դ���������ڵģ�
//�������˽�GPU��Դ���������ڡ� 
//Ӧ�ó�����뿼��GPU��Դ���������ڣ�
//�Ա������ٿ����Ա�GPU���õĶ���  
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

	struct Vertex //����ṹ�壺���������λ�ú���ɫ
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	//���߶���
	//CD3DX12��CD3D12�ı��壬������˵��
	CD3DX12_VIEWPORT m_viewport;//�ӿڶ���
	CD3DX12_RECT m_scissorRect; //��ʼ��D3D12 _ RECT�ṹ(�ü�����)

	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;//���������
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;//��ǩ��(����ǩ��?)
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;//��������
	ComPtr<ID3D12PipelineState> m_pipelineState;
	UINT m_rtvDescriptorSize;

	// Ӧ�ó�����Դ
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	//ͬ������
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;


	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();//��������б�
	void WaitForPreviousFrame();//�ȴ���һ֡
};
