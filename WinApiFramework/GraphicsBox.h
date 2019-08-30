#ifndef GRAPHICS_BOX_H
#define GRAPHICS_BOX_H

#include "WindowControl.h"

#include "Color.h"
#include "Bitmap.h"
#include "Point2D.h"

#include <d2d1helper.h>
#include <d2d1effects.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

namespace G = Graphics;


namespace WinApiFramework
{
	class GraphicsBox : public WindowControl
	{
		// -- fields -- //
	public:
		enum RenderType
		{
			RenderTypeDefault,
			RenderTypeSoftware,
			RenderTypeHardware
		};
	private:
		class Graphics
		{
			// -- fields -- //
		private:
			GraphicsBox *control;
			unsigned int width = 0u, height = 0u;
			G::Bitmap* bitmap;
			GraphicsBox::RenderType renderType;

			ID2D1Factory *D2DFactory = nullptr;
			ID2D1HwndRenderTarget *RT = nullptr;
			ID2D1BitmapRenderTarget *BRT = nullptr;
			ID2D1Bitmap *pixelBitmap = nullptr, *compBitmap = nullptr;
			ID2D1SolidColorBrush *brush = nullptr;
			ID2D1BitmapBrush *bitmapBrush = nullptr;


			// -- contructor -- //
		private:
			Graphics(GraphicsBox *control, RenderType renderType);
			~Graphics();


			// -- methods -- //
		private:
			bool InitGraphics();
			template <class T> void SafeRelease(T **ppT)
			{
				if (*ppT)
				{
					(*ppT)->Release();
					*ppT = NULL;
				}
			}
			void Resize(unsigned int newWidth, unsigned int newHeight);
		public:
			void Render();
			void SetPixel(const unsigned int& x, const unsigned int& y, const unsigned int& colorValue);
			void SetPixel(const unsigned int& x, const unsigned int& y, const unsigned char& r, const unsigned char& g, const unsigned char& b);
			void SetPixel(const unsigned int& x, const unsigned int& y, const G::Color& color);
			void SetBitmap(const G::Bitmap& newBitmap);
			void ClearPixelMap(const unsigned char& r, const unsigned char& g, const unsigned char& b);
			void ClearPixelMap(const G::Color& color);
			void ClearGraphicsMap(const unsigned char& r, const unsigned char& g, const unsigned char& b);
			void ClearGraphicsMap(const G::Color& color);
			void Clear(const unsigned char& r, const unsigned char& g, const unsigned char& b);
			void Clear(const G::Color& color);

			void SetBrushColor(const G::Color& color);
			void DrawLine(const G::Point2D<float>& point0, const G::Point2D<float>& point1, const float& width = 1.0f);
			void DrawEllipse(const G::Point2D<float>& center, const G::Point2D<float>& size, const float& width = 1.0f);
			void FillEllipse(const G::Point2D<float>& center, const G::Point2D<float>& size);
			void DrawRectangle(const G::Point2D<float>& point, const G::Point2D<float>& size, const float& brushWidth = 1.0f);
			void FillRectangle(const G::Point2D<float>& point, const G::Point2D<float>& size);


			// -- properties -- //
		public:
			const unsigned int& Width;
			const unsigned int& Height;


			// -- friends -- //
		public:
			friend class GraphicsBox;
		};
	public:
		struct Config : public WindowControl::Config
		{
			RenderType renderType = RenderType::RenderTypeDefault;
		};
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4,
			};
			Type type;

			Event()
			{
				type = Invalid;
			}
			Event(GraphicsBox::Event::Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler<GraphicsBox::Event>
		{
			virtual void HandleEvent(GraphicsBox::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
					// cases
				default:
					break;
				}
			}
			// virtual functions
		};
	private:
		WindowControl::Events<GraphicsBox::Event> events;
		Graphics graphics;


		// -- constructors -- //
	public:
		GraphicsBox(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox(const GraphicsBox&& graphicsBox) = delete;
		GraphicsBox(const Config &config);
		~GraphicsBox();


		// -- operators -- //
	public:
		GraphicsBox& operator=(const GraphicsBox& graphicsBox) = delete;
		GraphicsBox& operator=(const GraphicsBox&& graphicsBox) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			// no GraphicsBox events yet
		}
	public:
		void Resize(unsigned int newWidth, unsigned int newHeight);


		// -- propetry fields -- //
	public:
		Graphics& Gfx;
	};
}

#endif // !GRAPHICS_BOX_H

