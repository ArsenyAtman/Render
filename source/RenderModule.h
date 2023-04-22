#pragma once

class Render;
class Device;
struct ApplicationSettings;

class RenderModule
{
public:

	RenderModule(Render* render, Device* device, const ApplicationSettings* settings);
	virtual ~RenderModule();

protected:

	Render* getRender() const { return render; }
	Device* getDevice() const { return device; }
	const ApplicationSettings* getSettings() const { return settings; }

private:

	Render* render = nullptr;
	Device* device = nullptr;
	const ApplicationSettings* settings = nullptr;
};

