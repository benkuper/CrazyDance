/*
  ==============================================================================

	Drone.cpp
	Created: 10 Apr 2018 6:59:25pm
	Author:  Ben

  ==============================================================================
*/

#include "Drone.h"
#include "MotionBlock/model/library/MotionBlockModelLibrary.h"
#include "DroneManager.h"

Drone::Drone(StringRef name, StringRef familyName, var) :
	BaseItem(name),
	Thread("Drone " + name),
    generalCC("General"),
    controlCC("Controls"),
	flightCC("Flight"),
	lightCC("Light"),
	currentBlock(nullptr),
	previousID(-1),
	updateRate(50),
	droneNotifier(50)
{
	editorIsCollapsed = true;

	saveAndLoadRecursiveData = true;
	Random r(Time::currentTimeMillis());

	uiColor = addColorParameter("UI Color", "Color in UI", Colours::grey);	
	uiColor->setColor(Colour::fromHSV(r.nextFloat(), 1, .5f, 1));

	addChildControllableContainer(&controlCC);
	takeOff = controlCC.addTrigger("Take off", "Take off");
	land = controlCC.addTrigger("Land", "Land");

	addChildControllableContainer(&generalCC);
	droneID = generalCC.addIntParameter("Drone ID", "ID of the physical drone", 0);
	globalID = generalCC.addIntParameter("Global ID", "The Global Drone ID, it is a unique ID but it can be swapped between Drones", 0, 0, 100);
	state = generalCC.addEnumParameter("State", "State of the drone");
	for (int i = 0; i < MAX_STATES; i++) state->addOption(stateStrings[i], (State)i);

	battery = generalCC.addFloatParameter("Battery", "The battery level, between 0 and 1", 0, 0, 1);
	battery->setControllableFeedbackOnly(true);

	addChildControllableContainer(&flightCC);
	position = flightCC.addPoint3DParameter("Position", "Position of the drone");
	//position->setBounds(-10, 0, -10, 10, 5, 10);

	addChildControllableContainer(&lightCC);
	color = lightCC.addColorParameter("Color", "Color of the light deck", Colours::black);
	headlight = lightCC.addBoolParameter("Headlight", "Healight enabled or not", false);

	activeProvider = addTargetParameter("Active Block", "The current active block for this Drone");
	activeProvider->targetType = TargetParameter::CONTAINER;
	//activeProvider->customGetTargetContainerFunc = &MotionBlockModelLibrary::showProvidersAndGet;
	//activeProvider->hideInEditor = true;
}

Drone::~Drone()
{
	setBlockFromProvider(nullptr);
	signalThreadShouldExit();
	waitForThreadToExit(10000);
}

void Drone::clearItem()
{
	setBlockFromProvider(nullptr);
}

void Drone::setBlockFromProvider(MotionBlockDataProvider * model)
{
	if (currentBlock == nullptr && model == nullptr) return;
	if (model != nullptr && currentBlock != nullptr && currentBlock->provider == model) return;

	if (currentBlock != nullptr)
	{
		signalThreadShouldExit();
		waitForThreadToExit(100);

		removeChildControllableContainer(currentBlock);
		if (!currentBlock->provider.wasObjectDeleted())
		{
			currentBlock->provider->setHighlighted(false);

			linkedInspectables.removeAllInstancesOf(currentBlock->provider.get());
			currentBlock->provider->linkedInspectables.removeAllInstancesOf(this);
			currentBlock->provider->removeColorProviderListener(this);
			currentBlock->provider->removeInspectableListener(this);
		}

		//currentBlock->removeMotionBlockListener(this);
		currentBlock = nullptr;
	}

	if (model != nullptr) currentBlock = new MotionBlock(model);


	if (currentBlock != nullptr)
	{
		addChildControllableContainer(currentBlock);
		currentBlock->provider->addInspectableListener(this);
		currentBlock->provider->addColorProviderListener(this);

		linkedInspectables.addIfNotAlreadyThere(currentBlock->provider.get());
		currentBlock->provider->linkedInspectables.addIfNotAlreadyThere(this);

		startThread();
	}

	if (Engine::mainEngine != nullptr && !Engine::mainEngine->isClearing)
	{
		droneListeners.call(&DroneListener::droneBlockChanged, this);
		droneNotifier.addMessage(new DroneEvent(DroneEvent::BLOCK_CHANGED, this));
	}
}

void Drone::update()
{
	if (currentBlock == nullptr) return;
	if (Engine::mainEngine == nullptr || Engine::mainEngine->isClearing) return;

	double time = (Time::getMillisecondCounter() % (int)1e9) / 1000.0;
	motionData = currentBlock->getMotionData(this, time, var());

	
	bool forceUpdate = motionData.getProperty("forceUpdate", false);

	var posData = motionData.getProperty("position", var());
	if (posData.isArray())
	{
		position->setVector(posData[0], posData[1], posData[2]);
		if (forceUpdate) position->setValue(position->value, false, true);
	}

	var colorData = motionData.getProperty("color", var());
	if (colorData.isArray())
	{
		color->setColor(Colour((int)colorData[0], (int)colorData[1], (int)colorData[2]));
	}

	var headlightData = motionData.getProperty("headlight", var());
	if (!headlightData.isVoid()) headlight->setValue(headlightData);

	droneListeners.call(&DroneListener::dataUpdated, this);
	droneNotifier.addMessage(new DroneEvent(DroneEvent::DATA_UPDATED, this));
}

void Drone::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == activeProvider)
	{
		setBlockFromProvider(dynamic_cast<MotionBlockDataProvider *>(activeProvider->targetContainer.get()));
	}
	else if (p == enabled)
	{
		if (!enabled->boolValue())
		{
			motionData = var();
		}
	}
}

void Drone::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	if (c == globalID)
	{
		droneListeners.call(&DroneListener::droneGlobalIDChanged, this, previousID);
		previousID = globalID->intValue();
	}
	else if (c == position)
	{

	}
	else if (c == color)
	{

	}
	else if (c == headlight)
	{

	}
}

void Drone::inspectableDestroyed(Inspectable * i)
{
	if (currentBlock != nullptr && i == currentBlock->provider) setBlockFromProvider(nullptr);
}


var Drone::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (currentBlock != nullptr) data.getDynamicObject()->setProperty("block", currentBlock->getJSONData());
	return data;
}

void Drone::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	if (currentBlock != nullptr) currentBlock->loadJSONData(data.getProperty("block", var()));
}

void Drone::run()
{
	sleep(100);
	while (!threadShouldExit())
	{
		update();
		sleep(1000.0f / updateRate); //50fps
	}

}
