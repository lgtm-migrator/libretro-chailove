#include "script.h"
#include "chaigame.h"
#include "../Application.h"
#include <SDL.h>

#ifdef __HAVE_CHAISCRIPT__
#include "chaiscript/extras/math.hpp"
using namespace chaiscript;
#endif

namespace chaigame {
	bool script::loadModule(const std::string& moduleName) {
		#ifdef __HAVE_CHAISCRIPT__
		Application* app = Application::getInstance();
		std::string contents = app->filesystem.read(moduleName);
		if (!contents.empty()) {
			contents = replaceString(contents, "\t", "  ");
			chai.eval(contents, Exception_Handler(), moduleName);
			return true;
		}
		else {
			printf("Module %s was empty.", moduleName.c_str());
		}
		#endif
		return false;
	}

	script::script() {
		#ifdef __HAVE_CHAISCRIPT__
		Application* app = Application::getInstance();

		// Add the Quad.
		chai.add(user_type<Quad>(), "Quad");
		chai.add(fun(&Quad::x), "x");
		chai.add(fun(&Quad::y), "y");
		chai.add(fun(&Quad::width), "width");
		chai.add(fun(&Quad::height), "height");
		chai.add(fun(&Quad::sw), "sw");
		chai.add(fun(&Quad::sh), "sh");

		// Add the Point.
		chai.add(user_type<Point>(), "Point");
		chai.add(fun(&Point::x), "x");
		chai.add(fun(&Point::y), "y");

		// Add ImageData.
		chai.add(user_type<ImageData>(), "ImageData");
		chai.add(fun(&ImageData::getWidth), "getWidth");
		chai.add(fun(&ImageData::getHeight), "getHeight");

		// Add the Config.
		chai.add(user_type<windowConfig>(), "windowConfig");
		chai.add(fun(&windowConfig::width), "width");
		chai.add(fun(&windowConfig::height), "height");
		chai.add(fun(&windowConfig::bbp), "bbp");
		chai.add(fun(&windowConfig::title), "title");
		chai.add(user_type<moduleConfig>(), "moduleConfig");
		chai.add(fun(&moduleConfig::sound), "sound");
		chai.add(user_type<Config>(), "Config");
		chai.add(fun(&Config::window), "window");
		chai.add(fun(&Config::modules), "modules");

		// Register the Graphics module.
		chai.add(fun(&graphics::rectangle), "rectangle");
		chai.add(fun(&graphics::newImage), "newImage");
		chai.add(fun(&graphics::print), "print");
		chai.add(fun(&graphics::point), "point");
		chai.add(fun(&graphics::arc), "arc");
		chai.add(fun(&graphics::ellipse), "ellipse");
		chai.add(fun(&graphics::getWidth), "getWidth");
		chai.add(fun(&graphics::getHeight), "getHeight");
		chai.add(fun(&graphics::circle), "circle");
		chai.add(fun(&graphics::line), "line");
		chai.add(fun(&graphics::newQuad), "newQuad");
		chai.add(fun<void, graphics, int, int, int, int>(&graphics::setColor), "setColor");
		chai.add(fun<void, graphics, int, int, int>(&graphics::setColor), "setColor");
		chai.add(fun<void, graphics, int, int, int, int>(&graphics::setBackgroundColor), "setBackgroundColor");
		chai.add(fun<void, graphics, int, int, int>(&graphics::setBackgroundColor), "setBackgroundColor");
		chai.add(fun<void, graphics, ImageData*, int, int>(&graphics::draw), "draw");
		chai.add(fun<void, graphics, ImageData*, Quad, int, int>(&graphics::draw), "draw");
		chai.add(fun<void, graphics, int, int, int, int>(&graphics::clear), "clear");
		chai.add(fun<void, graphics, int, int, int>(&graphics::clear), "clear");
		chai.add(fun<void, graphics>(&graphics::clear), "clear");
		chai.add_global(var(std::ref(app->graphics)), "graphics");

		// Register the Keyboard module.
		chai.add(fun(&keyboard::update), "update");
		chai.add(fun(&keyboard::isDown), "isDown");
		chai.add_global(var(std::ref(app->keyboard)), "keyboard");

		// Register the Event module.
		chai.add(fun(&event::quit), "quit");
		chai.add_global(var(std::ref(app->event)), "event");

		// Register the Image module.
		chai.add(fun(&image::newImageData), "newImageData");
		chai.add_global(var(std::ref(app->image)), "image");

		// Register the Filesystem module.
		chai.add(fun(&filesystem::read), "read");
		chai.add(fun(&filesystem::exists), "exists");
		chai.add(fun<int, filesystem, const std::string&>(&filesystem::getSize), "getSize");
		chai.add(fun(&filesystem::load), "load");
		chai.add_global(var(std::ref(app->filesystem)), "filesystem");

		// Register the System module.
		chai.add(fun(&system::getOS), "getOS");
		chai.add_global(var(std::ref(app->system)), "system");

		// Register the Mouse module.
		chai.add(fun(&mouse::setVisible), "setVisible");
		chai.add(fun(&mouse::isVisible), "isVisible");
		chai.add(fun(&mouse::setX), "setX");
		chai.add(fun(&mouse::setY), "setY");
		chai.add(fun(&mouse::getX), "getX");
		chai.add(fun(&mouse::getY), "getY");
		chai.add(fun(&mouse::getPosition), "getPosition");
		chai.add(fun(&mouse::isDown), "isDown");
		chai.add_global(var(std::ref(app->mouse)), "mouse");

		// Register the Sound module.
		chai.add(fun<SoundData*, sound, const std::string&, const std::string&>(&sound::newSoundData), "newSoundData");
		chai.add(fun<SoundData*, sound, const std::string&>(&sound::newSoundData), "newSoundData");
		chai.add_global(var(std::ref(app->sound)), "sound");

		// Register the Audio module.
		chai.add(fun(&audio::play), "play");
		chai.add(fun<SoundData*, audio, const std::string&, const std::string&>(&audio::newSource), "newSource");
		chai.add(fun<SoundData*, audio, const std::string&>(&audio::newSource), "newSource");
		chai.add_global(var(std::ref(app->audio)), "audio");

		// Register the Window module.
		chai.add(fun(&window::setTitle), "setTitle");
		chai.add(fun(&window::getTitle), "getTitle");
		chai.add_global(var(std::ref(app->window)), "window");

		// Register the Timer module.
		chai.add(fun(&timer::getDelta), "getDelta");
		chai.add(fun(&timer::step), "step");
		chai.add_global(var(std::ref(app->timer)), "timer");

		// Register the Joystick module.
		chai.add(fun(&joystick::getJoysticks), "getJoysticks");
		chai.add(fun(&joystick::getJoystickCount), "getJoystickCount");
		chai.add(fun<bool, joystick, int, const std::string&>(&joystick::isDown), "isDown");
		chai.add_global(var(std::ref(app->joystick)), "joystick");

		// Register the Math module.
		auto mathlib = chaiscript::extras::math::bootstrap();
		chai.add(mathlib);
		chai.add(fun(&math::pi), "pi");
		chai.add(fun(&math::e), "e");
		chai.add(fun<double, math>(&math::random), "random");
		chai.add(fun<int, math, int>(&math::random), "random");
		chai.add(fun<int, math, int, int>(&math::random), "random");
		chai.add(fun<void, math, int>(&math::setRandomSeed), "setRandomSeed");
		chai.add(fun<void, math, int, int>(&math::setRandomSeed), "setRandomSeed");
		chai.add_global(var(std::ref(app->math)), "math");

		// Load main.chai.
		loadModule("main.chai");

		// Find the game functions.
		try {
			chaiload = chai.eval<std::function<void ()> >("load");
		}
		catch (std::exception& e) {
			printf("Skipping getting load(): %s\n", e.what());
		}
		try {
			chaiupdate = chai.eval<std::function<void (float)> >("update");
		}
		catch (std::exception& e) {
			printf("Skipping getting update(delta): %s\n", e.what());
		}
		try {
			chaiconf = chai.eval<std::function<void (Config&)> >("conf");
		}
		catch (std::exception& e) {
			printf("Skipping getting conf(t): %s\n", e.what());
		}
		try {
			chaidraw = chai.eval<std::function<void ()> >("draw");
		}
		catch (std::exception& e) {
			printf("Skipping getting draw(): %s\n", e.what());
		}
		try {
			chaijoystickpressed = chai.eval<std::function<void (int, int)> >("joystickpressed");
		}
		catch (std::exception& e) {
			printf("Skipping getting joystickpressed(): %s\n", e.what());
		}
		#endif
	}

	void script::conf(Config& t) {
		#ifdef __HAVE_CHAISCRIPT__
		try {
			chaiconf(t);
		}
		catch (chaiscript::exception::dispatch_error& e) {
			printf("Skipping call to conf(t): %s\n", e.what());
		}
		catch (const chaiscript::exception::eval_error &e) {
			printf("Skipping call to conf(): %s\n", e.what());
		}
		catch (std::exception& e) {
			printf("Skipping conf(t): %s\n", e.what());
		}
		catch (...) {
			printf("Skipping conf(t)");
		}
		#endif
	}

	void script::load() {
		#ifdef __HAVE_CHAISCRIPT__
		try {
			chaiload();
		}
		catch (chaiscript::exception::dispatch_error& e) {
			printf("Skipping call to load(): %s\n", e.what());
		}
		catch (const chaiscript::exception::eval_error &e) {
			printf("Failed calling load(): %s\n", e.what());
		}
		catch (...) {
			printf("Skipping load()");
		}
		#endif
	}

	void script::update(float delta) {
		#ifdef __HAVE_CHAISCRIPT__
		try {
			if (hasUpdate) {
				chaiupdate(delta);
			}
		}
		catch (chaiscript::exception::dispatch_error& e) {
			hasUpdate = false;
			printf("Skipping call to update(): %s\n", e.what());
		}
		catch (const chaiscript::exception::eval_error &e) {
			hasUpdate = false;
			printf("Skipping call to update(): %s\n", e.what());
		}
		catch (std::exception& e) {
			printf("Failed to call update(t): %s\n", e.what());
		}
		catch (...) {
			printf("Unhandled exception in update()");
		}
		#endif
	}

	void script::draw() {
		#ifdef __HAVE_CHAISCRIPT__
		try {
			if (hasDraw) {
				chaidraw();
			}
		}
		catch (chaiscript::exception::dispatch_error& e) {
			hasDraw = false;
			printf("Skipping call to draw(): %s\n", e.what());
		}
		catch (const chaiscript::exception::eval_error &e) {
			hasDraw = false;
			printf("Skipping call to update(): %s\n", e.what());
		}
		catch (std::exception& e) {
			printf("Failed to call update(t): %s\n", e.what());
		}
		catch (...) {
			printf("Unhandled exception in draw()");
		}
		#endif
	}

	void script::joystickpressed(int joystick, int button) {
		#ifdef __HAVE_CHAISCRIPT__
		try {
			if (hasjoystickpressed) {
				chaijoystickpressed(joystick, button);
			}
		}
		catch (...) {
			hasjoystickpressed = false;
		}
		#endif
	}

	std::string script::replaceString(std::string subject, const std::string& search, const std::string& replace) {
	    size_t pos = 0;
	    while ((pos = subject.find(search, pos)) != std::string::npos) {
	         subject.replace(pos, search.length(), replace);
	         pos += replace.length();
	    }
	    return subject;
	}
}