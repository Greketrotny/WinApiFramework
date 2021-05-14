# WinApiFramework
Simple Win32 OOP wrapper with basic windows, panels, controls and graphics.

![screen1](https://user-images.githubusercontent.com/38960244/118317385-48ba7380-b4f8-11eb-8670-ca67cf207c55.png)
![screen2](https://user-images.githubusercontent.com/38960244/118318027-1bba9080-b4f9-11eb-88ab-156bb4b5ae7d.png)

## Usage

- Include WinapiFramework:
    ```
    #include "winapi_framework.h"
    namespace WAF = WinapiFramework;
    auto& Winwork = WAF::Framework::GetInstance();
    ```
- Create first window:
    ```
    int WINAPI wWinMain(
        HINSTANCE hInstance, 
        HINSTANCE hPrevInstance, 
        LPWSTR args, INT ncmd)
    {
        WAF::Window *window = Winwork.CreateNewWindow(
            WAF::ConStruct<WAF::Window>(
	        	L"WAF Window",
	        	WAF::Rect(50, 50, 1200, 700)));
    ```
- [Optionally] Set callback function (called every time after message queue is processed)
    ```
        Winwork.SetCallbackFunction(YourCallBackFunction);
    ```
- Start message loop:
    ```
        return Winwork.ProcessMessages();
    }
    ```
    
## Features
- Windows with different styles and confgurations
- Controls:
    - Button
    - CheckBox
    - EditBox
    - Label
    - ProgressBar
    - TrackBar
    - GroupBox
    - Panel
    - ComboBox
    - GraphicsBox - Direct2D1 based. Capable of rendering text, some shapes and bitmaps.
- keyboard and mouse input
- event and loop driven system

## Requirements
WinapiFramework uses small graphics library developed separately. Can be found in top folder in the repository.
