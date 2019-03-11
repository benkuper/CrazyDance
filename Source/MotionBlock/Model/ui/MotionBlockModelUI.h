/*
  ==============================================================================

    MotionBlockModelUI.h
    Created: 10 Apr 2018 8:10:04pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../MotionBlockModel.h"

class MotionBlockModelUI :
	public BaseItemMinimalUI<MotionBlockModel>,
	public MotionBlockModel::AsyncListener,
	public DragAndDropContainer
{
public:
	MotionBlockModelUI(MotionBlockModel * model);
	~MotionBlockModelUI();

	Image modelImage;
	
	static const Identifier dragAndDropID;

	void paint(Graphics &g) override;
	void resized() override;

	virtual void updateThumbnail();

	virtual void editBlock() {} //to be overriden by child classes

	void newMessage(const MotionBlockModel::ModelEvent &e) override;

	void mouseDown(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;

};