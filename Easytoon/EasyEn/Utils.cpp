#include "stdafx.h"
#include "Utils.h"
#include <atlimage.h>
#include <Gdiplusimaging.h>

static int counter=0;
CBitmap* Utils::CopyBitmap(CBitmap* bmp)
{
	CBitmap* aMemBtmp = new CBitmap();
		
	WORD	 wFrameSize = bmp->GetBitmapDimension().cy * ( bmp->GetBitmapDimension().cx / 8 + (bmp->GetBitmapDimension().cx & 15 ? 2 : 0) );

	BYTE * aBmpBytes = new BYTE[wFrameSize];
	BYTE * aMemBtmpBytes = new BYTE[wFrameSize];

	aMemBtmp->CreateBitmap(bmp->GetBitmapDimension().cx,bmp->GetBitmapDimension().cy,1,1,aMemBtmpBytes);
	bmp->GetBitmapBits(wFrameSize,aBmpBytes);

	aMemBtmp->SetBitmapBits(wFrameSize,aBmpBytes);
	aMemBtmp->SetBitmapDimension(bmp->GetBitmapDimension().cx,bmp->GetBitmapDimension().cy);
	
	delete[] aBmpBytes;
	delete[] aMemBtmpBytes;

	return aMemBtmp;
	
}

CBitmap* Utils::ResizeBitmap(CBitmap *bmp, int x, int y)
{
	CDC TempDc;
	CDC ResizeDc;
	
	CBitmap* ResizeBmp= new CBitmap();
			
	WORD	 wFrameSize = y* (x / 8 + (x & 15 ? 2 : 0) );
	BYTE * aBmpBytes = new BYTE[wFrameSize];
	
	ResizeDc.CreateCompatibleDC(NULL);
	TempDc.CreateCompatibleDC(NULL);

	TempDc.SelectObject(bmp);

	ResizeBmp->CreateBitmap(x,y,1,1,aBmpBytes);
	ResizeDc.SelectObject(ResizeBmp);
	
	ResizeDc.StretchBlt(0,0,x,y,&TempDc,0,0,bmp->GetBitmapDimension().cx-1,bmp->GetBitmapDimension().cy-1, SRCCOPY   );
	
	ResizeDc.SelectStockObject(NULL_BRUSH);
	ResizeDc.Rectangle(0,0,x,y);

	delete bmp;
	delete[] aBmpBytes;

	return ResizeBmp;
}

void Utils::SaveBitmapToFile(CBitmap * bmp)
{
	CImage image;
	CString Path;
	
	Path.Format("C:\\test%d.bmp", counter);
    image.Attach(*bmp);

    image.Save(Path, Gdiplus::ImageFormatBMP);
	image.Detach();
	counter++;
}