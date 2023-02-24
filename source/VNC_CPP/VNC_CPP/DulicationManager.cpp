#pragma once

#include <boost/detail/winapi/handles.hpp>
#include <boost/detail/winapi/semaphore.hpp>
#include <boost/detail/winapi/basic_types.hpp>

#include <d3d11.h>
#include <dxgi1_2.h>

using namespace boost::detail::winapi;

class DulicationManager {

public:
    DulicationManager() : m_DeskDupl(nullptr),
        m_OutputNumber(0),
        m_AcquiredDesktopImage(nullptr),
        m_Device(nullptr),
        m_DeviceContext(nullptr) {
        RtlZeroMemory(&m_OutputDesc, sizeof(m_OutputDesc));
    }

    ~DulicationManager() {
        if (this->m_DeskDupl)
        {
            this->m_DeskDupl->Release();
            this->m_DeskDupl = nullptr;
        }

        if (m_Device)
        {
            m_Device->Release();
            m_Device = nullptr;
        }

        if (m_DeviceContext)
        {
            m_DeviceContext->Release();
            m_DeviceContext = nullptr;
        }

        if (m_AcquiredDesktopImage)
        {
            m_AcquiredDesktopImage->Release();
            m_AcquiredDesktopImage = nullptr;
        }
    }

private:
    IDXGIOutputDuplication* m_DeskDupl;
    unsigned int m_OutputNumber;
    DXGI_OUTPUT_DESC m_OutputDesc;
    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    ID3D11Texture2D* m_AcquiredDesktopImage;

public:
    void InitDupl() {
        // Take a reference on the device
        this->m_Device->AddRef();

        // Get DXGI device
        IDXGIDevice* DxgiDevice = nullptr;
        HRESULT hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DxgiDevice));
        if (FAILED(hr))
        {
            // return ProcessFailure(nullptr, L"Failed to QI for DXGI Device", L"Error", hr);
            // 创建idgi设备失败
        }

        // Get DXGI adapter
        IDXGIAdapter* DxgiAdapter = nullptr;
        hr = DxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DxgiAdapter));
        DxgiDevice->Release();
        DxgiDevice = nullptr;
        if (FAILED(hr))
        {
            // return ProcessFailure(m_Device, L"Failed to get parent DXGI Adapter", L"Error", hr, SystemTransitionsExpectedErrors);
            // 创建idgi设备失败
        }

        // Get output
        IDXGIOutput* DxgiOutput = nullptr;
        hr = DxgiAdapter->EnumOutputs(0, &DxgiOutput);
        DxgiAdapter->Release();
        DxgiAdapter = nullptr;
        if (FAILED(hr))
        {
            // return ProcessFailure(m_Device, L"Failed to get specified output in DUPLICATIONMANAGER", L"Error", hr, EnumOutputsExpectedErrors);
        }

        DxgiOutput->GetDesc(&m_OutputDesc);

        // QI for Output 1
        IDXGIOutput1* DxgiOutput1 = nullptr;
        hr = DxgiOutput->QueryInterface(__uuidof(DxgiOutput1), reinterpret_cast<void**>(&DxgiOutput1));
        DxgiOutput->Release();
        DxgiOutput = nullptr;
        if (FAILED(hr))
        {
            // return ProcessFailure(nullptr, L"Failed to QI for DxgiOutput1 in DUPLICATIONMANAGER", L"Error", hr);
        }

        // Create desktop duplication
        hr = DxgiOutput1->DuplicateOutput(m_Device, &m_DeskDupl);
        DxgiOutput1->Release();
        DxgiOutput1 = nullptr;
        if (FAILED(hr))
        {
            if (hr == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE)
            {
                MessageBoxW(nullptr, L"There is already the maximum number of applications using the Desktop Duplication API running, please close one of those applications and then try again.", L"Error", MB_OK);
                //return DUPL_RETURN_ERROR_UNEXPECTED;
            }
            //return ProcessFailure(m_Device, L"Failed to get duplicate output in DUPLICATIONMANAGER", L"Error", hr, CreateDuplicationExpectedErrors);
        }

    }

    /*
     * 初始化dx
     */
    void InitDX() {
        HRESULT hr = S_OK;
        // Driver types supported
        D3D_DRIVER_TYPE DriverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT NumDriverTypes = ARRAYSIZE(DriverTypes);

        // Feature levels supported
        D3D_FEATURE_LEVEL FeatureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_1
        };
        UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

        D3D_FEATURE_LEVEL FeatureLevel;

        // Create device
        for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; ++DriverTypeIndex)
        {
            hr = D3D11CreateDevice(nullptr,  // *pAdapter,
                DriverTypes[DriverTypeIndex],  // DriverType
                nullptr,  // Software
                0,  // Flags
                FeatureLevels,  // *pFeatureLevels,
                NumFeatureLevels, // FeatureLevels,
                D3D11_SDK_VERSION,  // SDKVersion
                &this->m_Device,  // **ppDevice,
                &FeatureLevel,  // *pFeatureLevel,
                &this->m_DeviceContext); // **ppImmediateContext
            if (SUCCEEDED(hr))
            {
                // Device creation success, no need to loop anymore
                break;
            }
        }
        if (FAILED(hr))
        {
            // return ProcessFailure(nullptr, L"Failed to create device in InitializeDx", L"Error", hr);
            // 创建d3d失败
        }

    }

    void GetFrame() {
        //HRESULT_ hr = S_OK;

        IDXGIResource* DesktopResource = NULL;
        DXGI_OUTDUPL_FRAME_INFO FrameInfo;

        // Get new frame
        HRESULT hr = m_DeskDupl->AcquireNextFrame(500, &FrameInfo, &DesktopResource);
        if (hr == DXGI_ERROR_WAIT_TIMEOUT)
        {
            //*Timeout = true;
            //return DUPL_RETURN_SUCCESS;
        }
        //*Timeout = false;
        if (FAILED(hr))
        {
            //return ProcessFailure(m_Device, L"Failed to acquire next frame in DUPLICATIONMANAGER", L"Error", hr, FrameInfoExpectedErrors);
        }

        //// If still holding old frame, destroy it
        //if (m_AcquiredDesktopImage)
        //{
        //    m_AcquiredDesktopImage->Release();
        //    m_AcquiredDesktopImage = nullptr;
        //}

        //// QI for IDXGIResource
        //hr = DesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_AcquiredDesktopImage));
        //DesktopResource->Release();
        //DesktopResource = nullptr;
        //if (FAILED(hr))
        //{
        //    //return ProcessFailure(nullptr, L"Failed to QI for ID3D11Texture2D from acquired IDXGIResource in DUPLICATIONMANAGER", L"Error", hr);
        //}

    }

};

// https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/DXGIDesktopDuplication
// https://github.com/GERD0GDU/dxgi_desktop_capture#:~:text=From%20the%20command-line%2C%20run%20dxgi_desktop_capture.exe%20-s%204%20-x,-o%20.%5Coutput_file_name.jpg%20to%20produce%20a%20jpeg.%20More%20items
// https://github.com/microsoft/DirectXTK/wiki/Adding-the-DirectX-Tool-Kit
// https://learn.microsoft.com/zh-cn/windows/win32/direct3ddxgi/desktop-dup-api
