#include "Test.h"
#include "imgui/imgui.h"

namespace test{

	TestMenu::TestMenu()
	{
		m_CurrentTest = this;
	}
	TestMenu::~TestMenu()
	{
		if (m_CurrentTest != this)
		{
			delete m_CurrentTest;
		}
	}
	void TestMenu::Return()
	{
		if (m_CurrentTest != this)
		{
			delete m_CurrentTest;
			m_CurrentTest = this;
		}
	}
	void TestMenu::OnImGuiRender()
	{
		for (auto& test: m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
}