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
	std::cout << "delta time: " << m_Engine.GetTimeManager().GetDeltaTime() * 1'000 << " ms." << std::endl;
}

void TestScene::OnRender()
{

}
