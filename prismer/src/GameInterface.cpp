#include <sstream>
#include <algorithm>

#include <Arya.h>

#include "GameInterface.h"

namespace Prismer {

using std::stringstream;

GameInterface::GameInterface()
{
    /* FPS COUNTER */
    using Arya::Material;
    auto matGray = Material::create(vec4(0.8f, 0.8f, 0.8f, 0.8f));

    using Arya::Font;
    auto font = make_shared<Font>();
    font->loadFromFile("DejaVuSansMono.ttf", 28);

    using Arya::View;
    _interfaceView = View::create();
    _interfaceView->setPosition(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f));
    _interfaceView->setSize(vec2(1.0f, 1.0f), vec2(0.0f, 0.0f));
    _interfaceView->addToRootView();

    using Arya::ImageView;
    _fpsView = ImageView::create();
    _fpsView->setMaterial(matGray);
    _fpsView->setPosition(vec2(-1.0f, 1.0f), vec2(65.0f, -25.0f));
    _fpsView->setSize(vec2(0.0f, 0.0f), vec2(120.0f, 40.0f));
    _interfaceView->add(_fpsView);

    using Arya::Label;
    _fpsCounterLabel = Label::create();
    _fpsCounterLabel->setPosition(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f));
    _fpsCounterLabel->setSize(vec2(0.0f, 0.0f), vec2(80.0f, 30.0f));
    if (font)
        _fpsCounterLabel->setFont(font);
    _fpsCounterLabel->setText("0 FPS");
    _fpsView->add(_fpsCounterLabel);
}

GameInterface::~GameInterface()
{
    // FIXME do we explicitely need to remove interface elements?
    // I think so
}

void GameInterface::update(float elapsedTime)
{
    for (int i = 99; i >= 1; --i)
        _elapsedTimes[i] = _elapsedTimes[i - 1];
    _elapsedTimes[0] = elapsedTime;

    auto sum_of_totals = std::accumulate(_elapsedTimes.begin(),
            _elapsedTimes.end(), 0.0f);
    auto average = sum_of_totals / 100.0f;

    auto fps = (int)(1.0f / average);
    stringstream ss;
    ss << fps << " FPS";
    _fpsCounterLabel->setText(ss.str());
}

void GameInterface::toggleFPS()
{
    _showFPS = !_showFPS;
    _fpsView->setVisible(_showFPS);
}

} // namespace Prismer
