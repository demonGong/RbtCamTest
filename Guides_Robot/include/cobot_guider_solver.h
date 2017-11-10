//
// Created by lhc on 17-5-19.
// Copyright (c) 2017 Wuhan Collaborative Robot Technology Co.,Ltd. All rights reserved.
//

#pragma once

#ifdef  DLL_COBOTGUIDERSOLVER_API
#else
#define DLL_COBOTGUIDERSOLVER_API _declspec(dllimport)
#endif

#include <vector>

namespace cobot_guider_solver {
	bool DLL_COBOTGUIDERSOLVER_API init(int argc, char** argv);//init
	void DLL_COBOTGUIDERSOLVER_API exit();//exit

	bool DLL_COBOTGUIDERSOLVER_API start();//start
	void DLL_COBOTGUIDERSOLVER_API stop();//stop

	int DLL_COBOTGUIDERSOLVER_API enable(bool state, int opt);//enable

	int DLL_COBOTGUIDERSOLVER_API solve(std::vector<double> &curQ, std::vector<double> &tarQ);

}

