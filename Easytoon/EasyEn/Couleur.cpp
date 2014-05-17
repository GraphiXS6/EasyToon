#include "stdafx.h"
#include "Couleur.h"

COLORREF Couleur::GetColorFromBrushNb(int i)
{
	if(i==16)
	{
		return RGB(250,250,250);
	}
	else
	{
		return RGB(i*16,i*16,i*16);
	}
};