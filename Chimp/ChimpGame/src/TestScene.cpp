#include "TestScene.h"

TestScene::TestScene(Chimp::Engine& engine)
	: m_Engine(engine)
{

}

TestScene::~TestScene()
{
}

void TestScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
}

void TestScene::OnDeactivate()
{
}

void TestScene::OnUpdate()
{
	if (m_Engine.GetWindow().GetInputManager().IsKeyDown(Chimp::Keyboard::SPACE))
	{
		std::cout << "Space key is pressed" << std::endl;
	}

	if (m_Engine.GetWindow().GetInputManager().IsKeyPressed(Chimp::Keyboard::A))
	{
		std::cout << "A key is pressed" << std::endl;
	}

	if (m_Engine.GetWindow().GetInputManager().IsMouseButtonDown(Chimp::Mouse::LEFT))
	{
		std::cout << "Left mouse button is down" << std::endl;
		auto pos = m_Engine.GetWindow().GetInputManager().GetMousePosition();
		std::cout << "Mouse pos is " << pos.x << ", " << pos.y << std::endl;
	}

	if (m_Engine.GetWindow().GetInputManager().IsMouseButtonPressed(Chimp::Mouse::RIGHT))
	{
		std::cout << "Right mouse button is pressed" << std::endl;
	}
}

void TestScene::OnRender()
{

}
