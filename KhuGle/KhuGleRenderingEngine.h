#pragma once
#include "KhuGleScene.h"
#include "KhuGleBase.h"
#include "KhuGleComponent.h"


class KhuGleRenderingEngine : public CKhuGleComponent
{
public:
	bool m_bInit;
	int m_nW, m_nH;

	unsigned char** m_ImageR, ** m_ImageG, ** m_ImageB;
	KgColor24 m_bgColor;

	KhuGleRenderingEngine(int nW, int nH, KgColor24 bgColor);
	~KhuGleRenderingEngine();

	void SetBackgroundImage(int nW, int nH, KgColor24 bgColor);
	void ResetBackgroundImage();
	void SetBgColor(KgColor24 bgColor);

	virtual void Render();
	virtual std::vector<double> getRenderInform();

};

