#pragma once
#ifndef __ENDBUTTON_H__
#define __ENDBUTTON_H__

#include "Button.h"

class CEndButton : public CButton
{
public:
	void Render() override;
private:
	void OnEnter() override;
	void OnExit() override;
	void Execute() override;
};

#endif // !__ENDBUTTON_H__