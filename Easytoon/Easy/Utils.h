static class Utils
{
public:
	static CBitmap* CopyBitmap(CBitmap* bmp);
	static CBitmap* ResizeBitmap(CBitmap* bmp, int x, int y);
	static void SaveBitmapToFile(CBitmap * bmp);
};
