#pragma once
#ifndef __STARTBUTTON_H__
#define __STARTBUTTON_H__

#include "Button.h"

class CStartButton : public CButton
{
public:
	void Render() override;
private:
	void OnEnter() override;
	void OnExit() override;
	void Execute() override;
};

#endif //__STARTBUTTON_H__