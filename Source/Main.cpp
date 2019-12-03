/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/
#include "JuceHeader.h"
#include "CDEngine.h"
#include "MainComponent.h"

//==============================================================================
class CrazyDanceApplication :
	public OrganicApplication
{
public:
	//==============================================================================
	CrazyDanceApplication() : OrganicApplication("CrazyDance") {}

	//==============================================================================
	void initialiseInternal(const String& commandLine) override
	{
		AppUpdater::getInstance()->setURLs(URL("http://benjamin.kuperberg.fr/crazydance/update.json"), "http://benjamin.kuperberg.fr/crazydance/download/", "CrazyDance");
		engine.reset(new CDEngine());
		mainComponent.reset(new MainComponent());
	}
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(CrazyDanceApplication)
