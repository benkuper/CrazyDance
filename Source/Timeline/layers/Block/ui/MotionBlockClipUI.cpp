/*
  ==============================================================================

	MotionBlockClipUI.cpp
	Created: 17 Apr 2018 7:20:43pm
	Author:  Ben

  ==============================================================================
*/

#include "MotionBlockClipUI.h"
#include "../MotionBlockLayer.h"
#include "MotionBlock/model/library/ui/MotionBlockModelTreeUI.h"

MotionBlockClipUI::MotionBlockClipUI(MotionBlockClip * _clip) :
	LayerBlockUI(_clip),
	//Thread(_clip->niceName + "_preview"),
	clip(_clip),
	fadeInHandle(ImageCache::getFromMemory(BinaryData::fadeIn_png, BinaryData::fadeIn_pngSize)),
	fadeOutHandle(ImageCache::getFromMemory(BinaryData::fadeOut_png, BinaryData::fadeOut_pngSize)),
	shouldUpdateImage(true),
    imageIsReady(false),
    isDraggingModel(false)
{
	
	acceptedDropTypes.add("MotionBlockModel");

	bgColor = BG_COLOR.brighter().withAlpha(.5f);

	setHighlightOnMouseOver(true);
	fillColorOnSelected = true;

	addChildComponent(&fadeInHandle, 0);
	addChildComponent(&fadeOutHandle, 0);
	
	fadeInHandle.setVisible(!clip->autoFade->boolValue());
	fadeOutHandle.setVisible(!clip->autoFade->boolValue());

	fadeInHandle.addMouseListener(this, false);
	fadeOutHandle.addMouseListener(this, false);

	generatePreview();
	//startThread();
}

MotionBlockClipUI::~MotionBlockClipUI()
{
	//signalThreadShouldExit();
	//waitForThreadToExit(300);
}

void MotionBlockClipUI::paint(Graphics & g)
{
	LayerBlockUI::paint(g);

	if (inspectable.wasObjectDeleted()) return;
	
	imgLock.enter();
	g.setColour(Colours::white.withAlpha(automationUI != nullptr ? .3f : .6f));
	g.drawImage(previewImage, getCoreBounds().toFloat(), RectanglePlacement::centred);
	imgLock.exit();

	g.setColour(bgColor.brighter());
	g.drawText((clip->currentBlock != nullptr && clip->currentBlock->provider != nullptr) ? clip->currentBlock->provider->getTypeString() : "No Light block selected", getCoreBounds().removeFromBottom(20).reduced(8).toFloat(), Justification::centred);


	if (automationUI != nullptr)
	{
		g.setFont(g.getCurrentFont().withHeight(jmin(getHeight() - 20, 20)).boldened());
		g.setColour(Colours::white.withAlpha(.5f));
	}

	Colour fColor = (clip->autoFade->boolValue() ? BLUE_COLOR : Colours::yellow).withAlpha(.5f);

	if (clip->fadeIn->floatValue() > 0)
	{
		g.setColour(fColor);
		g.drawLine(0, getHeight(), getWidth()*(clip->fadeIn->floatValue() / clip->getTotalLength()), fadeInHandle.getY() + fadeInHandle.getHeight() / 2, 2);
	}

	if (clip->fadeOut->floatValue() > 0)
	{
		g.setColour(fColor);
		g.drawLine(getWidth()*(1 - (clip->fadeOut->floatValue() / clip->getTotalLength())), fadeOutHandle.getY() + fadeOutHandle.getHeight() / 2, getWidth(), getHeight(), 2);
	}
}

void MotionBlockClipUI::paintOverChildren(Graphics & g)
{
	LayerBlockUI::paintOverChildren(g);
	
	if (isDraggingModel) g.fillAll(BLUE_COLOR.withAlpha(.3f));
}

void MotionBlockClipUI::resizedBlockInternal()
{
	//if (!imageIsReady) generatePreview();
	if (automationUI != nullptr) automationUI->setBounds(getCoreBounds());

	//Rectangle<int> r = getLocalBounds();
	fadeInHandle.setCentrePosition((clip->fadeIn->floatValue() / clip->getTotalLength())*getWidth(), fadeInHandle.getHeight() / 2);
	fadeOutHandle.setCentrePosition((1 - clip->fadeOut->floatValue() / clip->getTotalLength())*getWidth(), fadeOutHandle.getHeight() / 2);
}

void MotionBlockClipUI::generatePreview()
{
	//if (isMouseButtonDown()) return;
	//shouldUpdateImage = true;
	if (clip->currentBlock != nullptr)
	{
		int numBytes;
		const char * imgData = BinaryData::getNamedResource((StringUtil::toShortName(clip->currentBlock->provider->getTypeString()) + "_png").getCharPointer(), numBytes);
		previewImage = ImageCache::getFromMemory(imgData, numBytes);
		repaint();
	}
	else
	{
		previewImage = Image();
		repaint();
	}
}

void MotionBlockClipUI::setTargetAutomation(ParameterAutomation * a)
{
	if (automationUI != nullptr)
	{
		removeChildComponent(automationUI);
		automationUI = nullptr;
	}

	if (a == nullptr) return;

	automationUI = new AutomationUI(dynamic_cast<Automation *>(a->automationContainer));
	addAndMakeVisible(automationUI);
	resized();
	repaint();
	automationUI->updateROI();
	automationUI->addMouseListener(this, true);
}

void MotionBlockClipUI::mouseDown(const MouseEvent & e)
{
	LayerBlockUI::mouseDown(e);

	if (e.eventComponent == &fadeInHandle) fadeValueAtMouseDown = clip->fadeIn->floatValue();
	else if (e.eventComponent == &fadeOutHandle) fadeValueAtMouseDown = clip->fadeOut->floatValue();


	if (e.eventComponent == automationUI && e.mods.isLeftButtonDown()) //because recursive mouseListener is removed to have special handling of automation
	{
		item->selectThis();
	}

	if (e.mods.isRightButtonDown() && (e.eventComponent == this || e.eventComponent == automationUI))
	{
		if (clip->currentBlock != nullptr)
		{

			PopupMenu p;
			p.addItem(1, "Clear automation editor");

			PopupMenu ap;

			Array<WeakReference<Parameter>> params = clip->currentBlock->paramsContainer.getAllParameters();



			int index = 2;
			for (auto &pa : params)
			{
				ap.addItem(index, pa->niceName, true, pa->controlMode == Parameter::ControlMode::AUTOMATION);
				index++;
			}

			p.addSubMenu("Edit...", ap);

			int result = p.show();
			if (result > 0)
			{
				if (result == 1) setTargetAutomation(nullptr);
				else
				{
					WeakReference<Parameter> pa = params[result - 2];
					if (pa->controlMode != Parameter::ControlMode::AUTOMATION)
					{
						pa->setControlMode(Parameter::ControlMode::AUTOMATION);
						pa->automation->setManualMode(true);
					}

					if (!pa.wasObjectDeleted()) setTargetAutomation(pa->automation.get());
				}
			}
		}
	}
}

void MotionBlockClipUI::mouseDrag(const MouseEvent & e)
{
	LayerBlockUI::mouseDrag(e);

	if (e.eventComponent == &fadeInHandle)
	{
		clip->fadeIn->setValue((getMouseXYRelative().x * 1.0f / getWidth())*clip->getTotalLength());
		resized();
	}
	else if (e.eventComponent == &fadeOutHandle)
	{
		clip->fadeOut->setValue((1 - (getMouseXYRelative().x *1.0f / getWidth()))*clip->getTotalLength());
		resized();
	}
	/*
	else if (automationUI != nullptr)
	{
		generatePreview();
	}
	*/
}

void MotionBlockClipUI::mouseUp(const MouseEvent & e)
{
	LayerBlockUI::mouseUp(e);

	if (e.eventComponent == &fadeInHandle)
	{
		clip->fadeIn->setUndoableValue(fadeValueAtMouseDown, clip->fadeIn->floatValue());
		resized();
	}
	else if (e.eventComponent == &fadeOutHandle)
	{
		clip->fadeOut->setUndoableValue(fadeValueAtMouseDown, clip->fadeOut->floatValue());
		resized();
	}
}

Rectangle<int> MotionBlockClipUI::getDragBounds()
{
	return LayerBlockUI::getLocalBounds();
}

void MotionBlockClipUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	LayerBlockUI::controllableFeedbackUpdateInternal(c);

	//if (c == clip->coreLength || c == clip->loopLength || c == clip->activeProvider || c == clip->fadeIn || c == clip->fadeOut) generatePreview();
	//else if (clip->currentBlock != nullptr && c->parentContainer == &clip->currentBlock->paramsContainer && c->type != Controllable::TRIGGER && ((Parameter *)c)->controlMode == Parameter::MANUAL) generatePreview();
	/*else if (dynamic_cast<AutomationKey *>(c->parentContainer) != nullptr)
	{
		generatePreview();
	}*/

	if (c == clip->activeProvider)
	{
		generatePreview();
	}
	
	if (c == clip->autoFade)
	{
		fadeInHandle.setVisible(!clip->autoFade->boolValue());
		fadeOutHandle.setVisible(!clip->autoFade->boolValue());
		repaint();
	}
	else if (c == clip->fadeIn || c == clip->fadeOut)
	{
		resized();
		repaint();
	}
}

void MotionBlockClipUI::itemDragEnter(const SourceDetails & source)
{
	isDraggingModel = true;
	repaint();
}

void MotionBlockClipUI::itemDragExit(const SourceDetails & source)
{
	isDraggingModel = false;
	repaint();
}

void MotionBlockClipUI::itemDragMove(const SourceDetails & source)
{
	
}

void MotionBlockClipUI::itemDropped(const SourceDetails & source)
{
	MotionBlockModelTreeUI * modelUI = dynamic_cast<MotionBlockModelTreeUI *>(source.sourceComponent.get());

	if (modelUI != nullptr)
	{
		MotionBlockDataProvider * provider = modelUI->model;

		bool shift = KeyPress::isKeyCurrentlyDown(16);
		if (shift)
		{
			PopupMenu m;
			m.addItem(-1, "Default");
			m.addSeparator();
			int index = 1;
			for (auto &p : modelUI->model->presetManager.items) m.addItem(index++, p->niceName);
			int result = m.show();
			if (result >= 1) provider = modelUI->model->presetManager.items[result - 1];
		}

		clip->activeProvider->setValueFromTarget(provider, true); 
	}

	isDraggingModel = false;
	repaint();
}

/*
void MotionBlockClipUI::run()
{
	
	while (!threadShouldExit())
	{
		sleep(20);

		if (!shouldUpdateImage || clip->currentBlock == nullptr)
		{
			sleep(20);
			continue;

		}

		shouldUpdateImage = false;
		imageIsReady = false;

		const int resX = jmin(getWidth(), 600);
		const int resY = 32; //to make dynamic

		if (resX == 0)
		{
			sleep(20);
			continue;
		}

		Image tmpImg = Image(Image::RGB, resX, resY, true);

		int id = clip->layer->previewID->intValue();

		//previewProp.globalID->setValue(id, true);
		//previewProp.resolution->setValue(resY, true);

		var params = new DynamicObject();
		params.getDynamicObject()->setProperty("updateAutomation", false);

		float start = clip->time->floatValue();
		float length = clip->getTotalLength();
		float coreLength = clip->coreLength->floatValue();

		for (int i = 0; i < resX; i++)
		{
			if (threadShouldExit()) return;

			float relTotal = i * length / resX;
			float absT = start + relTotal;
			Array<Colour> c = clip->getMotionData(&previewProp, absT, params);
			for (int ty = 0; ty < resY; ty++)
			{
				if (relTotal > coreLength) c.set(ty, c[ty].darker());// c[ty].darker();
				tmpImg.setPixelAt(i, ty, c[ty]);
			}
		}

		imgLock.enter();
		previewImage = tmpImg.createCopy();
		imgLock.exit();
		imageIsReady = true;

		{
			MessageManagerLock mmLock;
			repaint();
		}

	}
}
*/

MotionBlockFadeHandle::MotionBlockFadeHandle(const Image & image) :
	img(image)
{
	setSize(12, 12);
}

void MotionBlockFadeHandle::paint(Graphics & g)
{
	g.setColour(Colours::orange);
	g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(2,2), 2);
	//g.drawImage(img, getLocalBounds().toFloat().reduced(2));
}
