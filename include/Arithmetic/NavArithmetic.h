#pragma once
class AFX_EXT_CLASS  CNavArithmetic
{
public:
	CNavArithmetic(void);
	virtual ~CNavArithmetic(void);
	
	// MakeMappedMatrix
	void MMM_VS1toVS2(float in_VS1[4][3], float in_VS2[4][3], float out_Mateix[4][4]);
	void MDD_VS1toVS2(float in_VS1[4][3], float in_VS2[4][3], float out_Mateix[4][4]);

};

