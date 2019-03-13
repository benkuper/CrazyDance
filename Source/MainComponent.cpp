/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "MotionBlock/model/ui/MotionBlockModelLibraryUI.h"
#include "Drone/ui/DroneManagerUI.h"
#include "Timeline/TimelineEditor.h"
#include "Drone/Cluster/ui/DroneClusterGroupManagerUI.h"
#include "Viz/StageViz.h"
#include "Communication/ui/IOInterfaceManagerUI.h"

//==============================================================================
MainComponent::MainComponent()
{
}

MainComponent::~MainComponent()
{
}

void MainComponent::init()
{
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Drones", &DroneManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Library", &MotionBlockModelLibraryUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(TimelineEditor::getTypeStringStatic(), &TimelineEditor::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Clusters", &DroneClusterGroupManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Stage Viz", &StageViz::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Interfaces", &IOInterfaceManagerUI::create));

	ShapeShifterManager::getInstance()->setDefaultFileData(BinaryData::default_cdlayout);
	ShapeShifterManager::getInstance()->setLayoutInformations("cdlayout", "CrazyDance/layouts");

	OrganicMainContentComponent::init();
}