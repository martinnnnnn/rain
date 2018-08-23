//#include "win32_command_queue.h"
//
//#include "win32_helpers.h"
//
//#include <cassert>
//
//
//
//CommandQueue::CommandQueue(Microsoft::WRL::ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
//    : m_FenceValue(0)
//    , m_CommandListType(type)
//    , m_d3d12Device(device)
//{
//    D3D12_COMMAND_QUEUE_DESC desc = {};
//    desc.Type = type;
//    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
//    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//    desc.NodeMask = 0;
//
//    ThrowIfFailed(m_d3d12Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_d3d12CommandQueue)));
//    ThrowIfFailed(m_d3d12Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_d3d12Fence)));
//
//    m_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
//    assert(m_FenceEvent && "Failed to create fence event handle.");
//}
//
//Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandQueue::CreateCommandAllocator()
//{
//    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
//    ThrowIfFailed(m_d3d12Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));
//
//    return commandAllocator;
//}
//
//Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CommandQueue::CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator)
//{
//    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;
//    ThrowIfFailed(m_d3d12Device->CreateCommandList(0, m_CommandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
//
//    return commandList;
//}
//
//Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> CommandQueue::GetCommandList()
//{
//    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
//    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList;
//
//    if (!m_CommandAllocatorQueue.empty() && IsFenceComplete(m_CommandAllocatorQueue.front().fenceValue))
//    {
//        commandAllocator = m_CommandAllocatorQueue.front().commandAllocator;
//        m_CommandAllocatorQueue.pop();
//
//        ThrowIfFailed(commandAllocator->Reset());
//    }
//    else
//    {
//        commandAllocator = CreateCommandAllocator();
//    }
//
//    if (!m_CommandListQueue.empty())
//    {
//        commandList = m_CommandListQueue.front();
//        m_CommandListQueue.pop();
//
//        ThrowIfFailed(commandList->Reset(commandAllocator.Get(), nullptr));
//    }
//    else
//    {
//        commandList = CreateCommandList(commandAllocator);
//    }
//    // Associate the command allocator with the command list so that it can be
//    // retrieved when the command list is executed.
//    ThrowIfFailed(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));
//
//    return commandList;
//}